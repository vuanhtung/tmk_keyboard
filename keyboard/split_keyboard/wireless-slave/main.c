#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <avr/wdt.h>

#include "../config.h"
#include "../wireless/nrf.h"
#include "../wireless/nRF24L01.h"
#include "../wireless/crypto.h"
#include "./matrix.h"
#include "./clock.h"

// How long the keyboard can be inactive before it goes to sleep. The keyboard
// is considered active if any key is down.
#ifndef INACTIVITY_TIMEOUT
#define INACTIVITY_TIMEOUT 0 // 0-255 seconds
#endif

// If no keys are either presses or released in this time, the keyboard
// will go to sleep. If something gets left on the keyboard, this value will
// let it know when it is safe to assume nobody is using it, but it also
// limits how long you can hold down a key for.
#ifndef UNCHANGED_TIMEOUT
#define UNCHANGED_TIMEOUT 30 // 0-255 seconds
#endif

// If no keys are either presses or released in this time, the keyboard
// will go to sleep. If something gets left on the keyboard, this value will
// let it know when it is safe to assume nobody is using it, but it also
// limits how long you can hold down a key for.
#ifndef ERROR_LIMIT
#define ERROR_LIMIT 7
#endif

// how many iterations the main loop can do in a second
// WARN: this is value was measured for the current clock speeds and does
// not control the given scan rate. It is used as a rough estimate of how
// long a second is in terms of matrix scans.
#define SCAN_RATE 300

aes_ctx_t aes_ctx = { 0 };
ecb_state_t ecb_state = { {0} };
device_settings_t settings = { 0 };

void disable_unused_hardware(void) {
  wdt_disable();

  ADCSRA = 0; // disable ADC
  power_adc_disable();

  power_usart0_disable();
  power_timer0_disable();
  power_timer1_disable();
  power_timer2_disable();
  power_twi_disable();
}

void disable_hardware(void) {
  nrf_power_set(0);
  power_spi_disable();
}

void enable_hardware(void) {
  power_spi_enable();
}

void reset_hardware(void) {
  // default state, output low
  DDRB = 0xff;
  DDRC = 0xff;
  DDRD = 0xff;
  PORTB = 0x00;
  PORTC = 0x00;
  PORTD = 0x00;

  spi_setup();
  nrf_setup(&settings);
  matrix_init();
}

void slave_sleep(void) {
  // want to be fast on wakeup
  clock_fast();

  disable_hardware();

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  cli();
  sleep_enable();

  // set the matrix to fire interrupt on pin change
  matrix_interrupt_mode();

  // Turn off brown out detection during sleep. Saves about 35μA in sleep mode.
  // Brown-out detection will be automatically reenabled on wake up.
  // WARN: This is a timed sequence. Have 4 clock cycles, from this point
  // to go to sleep, or otherwise BODS will be reenabled.
  MCUCR = (1<<BODS) | (1<<BODSE); // special write sequence, see datasheet
  MCUCR = (1<<BODS);
  sei();
  sleep_cpu();
  cli();

  enable_hardware();
  reset_hardware();
  sei();
}

volatile static uint16_t timer2_counter = 0;

ISR(TIMER2_OVF_vect) {
  timer2_counter++;
}

void timer2_init(void) {
  timer2_counter = 0;
  /* TCCR1B |= (1 << CS10); */
  TCNT2 = 0;
  TCCR2A = 0;
  TCCR2B = (0b110<<CS20);
  TIMSK2 = (1<<TOIE2);
}

/* void slave_disconnect_pause(void) { */
/*   /1* 255/(8000000 / 128 / 1024) = 4.17792000 *1/ */
/*   /1* 255/(8000000 / 128 / 512) = 2.08896000 *1/ */
/* sleep_again: */
/*   set_sleep_mode(SLEEP_MODE_PWR_SAVE); */
/*   cli(); */
/*   sleep_enable(); */

/*   // use slow clock for longer sleep */
/*   clock_slow(); */

/*   power_timer2_enable(); */
/*   timer2_init(); */
/*   asm volatile("nop"); */
/*   asm volatile("nop"); */
/*   asm volatile("nop"); */
/*   asm volatile("nop"); */

/*   sei(); */
/*   sleep_cpu(); */
/*   cli(); */

/*   /1* if (timer2_counter < 10) { *1/ */
/*   /1*   goto sleep_again; *1/ */
/*   /1* } *1/ */


/*   power_timer2_disable(); */
/*   TIMSK2 = 0; */

/*   sei(); */
/* } */

void slave_disable(void) {
  enable_hardware();
}

void setup(void) {
  clock_fast();

  disable_unused_hardware();
  load_eeprom_settings(&settings);
  _delay_ms(100); // nrf takes about this long to start from no power
  reset_hardware();

  crypto_init(&aes_ctx);
  ecb_init(&ecb_state, DEVICE_ID);

  sei();
}

// get vcc voltage
uint16_t vcc_ref(void) {
  // From the data sheet:
  //   ADC = V_in * 1024 / V_ref
  // 
  // Here we mearsue the 1.1 internal V_ref, with AVCC. So:
  //    V_in  = V_bg
  //    V_ref = AVCC 
  // 
  // So:
  //   AVCC = V_ref = (1.1 * 1024) / ADC
  // Or with 8 bit resolution:
  //   AVCC = V_ref = (1.1 * 256) /  ADC
  uint8_t adc;
  power_adc_enable();
  //ADCSRA = (1 << ADEN) | 0x7;
  ADCSRA = (1 << ADEN) | 0x7;
  ADMUX = (0b01 << REFS0) | (1 << ADLAR) | (0b1110 << MUX0);
  _delay_us(200); // 200us
  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC)); // ~ (25 * CLK / 128) @ 8MHZ => 800us
  adc = ADCH;
  ADCSRA = 0;
  power_adc_disable();
  return (uint16_t)(1.1 * 256 * 100) / adc;
}

int main(void) {
  matrix_scan_t scan = {0};
  uint8_t nrf_status = 0;
  uint8_t inactive_time = 0;
  uint8_t unchanged_time = 0;
#if SCAN_RATE <= 255
  uint8_t scan_rate_counter = 0;
#else
  uint16_t scan_rate_counter = 0;
#endif
  /* uint8_t error_rate = 0; */

  setup();
  clock_slow();

  while(1) {
    // WARN: the debounce of this scan is effectively the rate at which this
    // loop runs.
    matrix_scan_slow(&scan);

    nrf_status = nrf_get_status();

    /* if(nrf_status & (1<<TX_DS)) { // data sent, recieved ack */
    /*     error_rate=0; */
    /* } */

    if (nrf_status & (1<<MAX_RT) || nrf_status & (1<<TX_FULL)) {
      clock_fast();
      spi_command(FLUSH_TX);
      nrf_clear_flags();
      clock_slow();
    }


    const uint8_t should_ping = scan_rate_counter == 0;
    // only send the scan result if something changed
    if (scan.changed || should_ping) {
      clock_fast();

      /* uint8_t checksum = 0; */
      /* for (int i = 0; i < ROWS_PER_HAND; ++i) { */
      /*   aes_state.data[i] = matrix_get_row(i); */
      /*   checksum += aes_state.data[i]; */
      /* } */

      /* aes_state.data[ROWS_PER_HAND] = checksum; */
      /* aes_state.data[ROWS_PER_HAND+1] = checksum; */

      /* encrypt(&aes_state, &aes_ctx); */

      for (int i = 0; i < ROWS_PER_HAND; ++i) {
        ecb_state.payload[i] = matrix_get_row(i);
      }



      /* uint16_t vcc = vcc_ref(); */
      /* /1* ecb_state.payload[ROWS_PER_HAND] = (&vcc)[0]; *1/ */
      /* /1* ecb_state.payload[ROWS_PER_HAND+1] = (&vcc)[1]; *1/ */
      /* ecb_state.payload[ROWS_PER_HAND] = (&vcc)[0]; */
      /* ecb_state.payload[ROWS_PER_HAND+1] = (&vcc)[1]; */

#ifndef NO_ENCRYPT
      uint8_t encrypted_data[AES_BUF_LEN];
      ecb_encrypt(&ecb_state, &aes_ctx, encrypted_data);

      nrf_load_tx_fifo(encrypted_data, RF_BUFFER_LEN);
#else
      nrf_load_tx_fifo(&ecb_state, RF_BUFFER_LEN);
#endif
      nrf_send_one();

      if(scan.changed) {
        unchanged_time = 0;
      }

      if (scan.active) {
        inactive_time = 0;
      }

      clock_slow();
    }

    scan_rate_counter++;
    if (scan_rate_counter == SCAN_RATE) {
      scan_rate_counter = 0;

      if ( (inactive_time > INACTIVITY_TIMEOUT) ||
          (unchanged_time > UNCHANGED_TIMEOUT)) {
        inactive_time = 0;
        unchanged_time = 0;
        slave_sleep();
      }

      inactive_time++;
      unchanged_time++;
    }
  }
}
