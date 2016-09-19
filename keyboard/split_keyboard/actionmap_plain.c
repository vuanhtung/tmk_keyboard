#include <avr/pgmspace.h>
#include "actionmap.h"
#include "actionmap_common.h"
#include "action_util.h"
#include "action_code.h"
#include "action_layer.h"
#include "wait.h"

#include <avr/wdt.h>

/* id for user defined functions */
enum function_id {
    RESET_LAYER_STATE,
    PROMICRO_RESET,
    PROMICRO_PROGRAM,
};

#define AC_FN1            ACTION_LAYER_ONESHOT(1)

#define AC_PROMICRO_PROGRAM  ACTION_FUNCTION_TAP(PROMICRO_PROGRAM)
#define AC_PROMICRO_RESET    ACTION_FUNCTION_TAP(PROMICRO_RESET)

const uint16_t PROGMEM actionmaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* qwerty */
    [0] = ACTIONMAP(
        /* left hand */
            ESC  , 1    , 2    , 3    , 4   , 5     ,  6  ,
            TAB  , Q    , W    , E    , R   , T     ,  Y  ,
            LCTL , A    , S    , D    , F   , G     ,  H  ,
            LSFT , Z    , X    , C    , V   , B     ,  N  ,
            CAPS , FN1  ,        BSPC ,       SPC   , CAPS,
        /* right hand */
            7    , 8    , 9    , 0    , MINS , EQL  , BSPC,
            U    , I    , O    , P    , LBRC , RBRC , BSLS,
            J    , K    , L    , SCLN , QUOT ,  UP  , ENT ,
            M    , COMM , DOT  , SLSH , LEFT , DOWN , RGHT,
            SPC  , RCTL ,        ENT  ,        RALT , RGUI
    ),

    /* fn */
    [1] = ACTIONMAP(
        /* left hand */
            GRV  , F1  , F2  , F3  , F4  , F5  , F6  ,
            TRNS ,TRNS ,VOLU ,TRNS ,TRNS ,TRNS ,TRNS ,
            TRNS ,TRNS ,VOLD ,TRNS ,TRNS ,TRNS ,TRNS ,
            TRNS ,TRNS ,TRNS ,TRNS ,TRNS ,TRNS ,TRNS ,
            TRNS ,      TRNS ,      TRNS ,TRNS ,TRNS ,
        /* right hand */
            F7   ,F8   ,F9   ,F10  ,F11  ,F12  ,INS ,
            TRNS ,TRNS ,TRNS ,TRNS ,TRNS ,TRNS ,DEL ,
            TRNS ,HOME ,PGUP ,TRNS ,TRNS ,PGUP ,TRNS,
            TRNS ,END  ,PGDN ,TRNS ,TRNS ,PGDN ,TRNS,
            TRNS ,      TRNS ,      TRNS ,TRNS ,TRNS
        ),
    /* media */
    [2] = ACTIONMAP(
        /* left hand */
            TRNS ,TRNS ,TRNS ,TRNS ,TRNS ,TRNS ,PROMICRO_PROGRAM,
            TRNS ,TRNS ,TRNS ,TRNS ,TRNS ,TRNS ,PROMICRO_RESET,
            TRNS ,TRNS ,TRNS ,TRNS ,TRNS ,TRNS ,TRNS ,
            TRNS ,TRNS ,TRNS ,TRNS ,TRNS ,TRNS ,TRNS ,
            TRNS ,      TRNS ,      TRNS ,TRNS ,TRNS ,
        /* right hand */
            TRNS ,TRNS ,TRNS ,TRNS ,TRNS ,TRNS ,PROMICRO_PROGRAM,
            TRNS ,TRNS ,TRNS ,TRNS ,TRNS ,TRNS ,PROMICRO_RESET,
            TRNS ,TRNS ,TRNS ,TRNS ,TRNS ,TRNS ,TRNS ,
            TRNS ,TRNS ,TRNS ,TRNS ,TRNS ,TRNS ,TRNS ,
            TRNS ,      TRNS ,      TRNS ,TRNS ,TRNS

    ),
};

void promicro_bootloader_jmp(bool program) {
    uint16_t *const bootKeyPtr = (uint16_t *)0x0800;

    // Value used by Caterina bootloader use to determine whether to run the
    // sketch or the bootloader programmer.
    uint16_t bootKey = program ? 0x7777 : 0;

    *bootKeyPtr = bootKey;

    // setup watchdog timeout
    wdt_enable(WDTO_60MS);

    while(1) {} // wait for watchdog timer to trigger
}

/*
 * user defined action function
 */
void action_function(keyrecord_t *record, uint8_t id, uint8_t opt)
{
    uint8_t tap_count = record->tap.count;
    switch (id) {
        case RESET_LAYER_STATE:
            if (record->event.pressed) {
                if (!get_oneshot_locked_mods() && !get_oneshot_layer_state()) {
                    register_code(KC_ESC);
                }
            } else {
                if (!get_oneshot_locked_mods() && !get_oneshot_layer_state()) {
                    unregister_code(KC_ESC);
                } else {
                    reset_oneshot_layer();
                    clear_oneshot_locked_mods();
                    layer_clear();
                    clear_keyboard();
                }
            }
            break;
        case PROMICRO_RESET:
            if(tap_count == 5) {
                promicro_bootloader_jmp(false);
            }
            break;
        case PROMICRO_PROGRAM:
            if(tap_count == 5) {
                promicro_bootloader_jmp(true);
            }
            break;
        default:
            break;
    }
}

// vim:sw=4:
