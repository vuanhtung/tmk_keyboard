#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include "host.h"
#include "eeconfig.h"
#include "action_layer.h"
#include "keymap.h"
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
    SET_QWERTY,
    SET_DVORAK,
    SET_COLEMAK,
    TOGGLE_NKRO,
};

#define QWERTY 0
#define ARROWS 1
#define INV_COL 2
#define DVORAK 2
#define COLEMAK 3
#define SYMB 4
#define FUNC 5
#define MEDIA 6

// If defined
#define INVERSE_COLEMAK

#define AC_FN1            ACTION_LAYER_ONESHOT(SYMB)
#define AC_FN2            ACTION_LAYER_ONESHOT(FUNC)
#define AC_FN3            ACTION_LAYER_ONESHOT(MEDIA)
#define AC_SSFT           ACTION_MODS_ONESHOT(MOD_LSFT)
#define AC_SFT_ENT        ACTION_MODS_TAP_KEY(MOD_LSFT, KC_ENT)

#define AC_FN0            ACTION_FUNCTION(RESET_LAYER_STATE)
#define AC_QWERTY         ACTION_DEFAULT_LAYER_SET(QWERTY)
#define AC_DVORAK         ACTION_DEFAULT_LAYER_SET(DVORAK)
#define AC_COLEMAK        ACTION_DEFAULT_LAYER_SET(COLEMAK)
#define AC_ARROWS         ACTION_DEFAULT_LAYER_SET(ARROWS)

#define AC_PROMICRO_PROGRAM  ACTION_FUNCTION_TAP(PROMICRO_PROGRAM)
#define AC_PROMICRO_RESET    ACTION_FUNCTION_TAP(PROMICRO_RESET)
#define AC_SET_QWERTY        ACTION_FUNCTION_TAP(SET_QWERTY)
#define AC_SET_DVORAK        ACTION_FUNCTION_TAP(SET_DVORAK)
#define AC_SET_INV_COLEMAK   AC_SET_DVORAK
#define AC_SET_COLEMAK       ACTION_FUNCTION_TAP(SET_COLEMAK)
#define AC_TOGGLE_NKRO       ACTION_FUNCTION_TAP(TOGGLE_NKRO)


const uint16_t PROGMEM actionmaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* qwerty */
    [QWERTY] = ACTIONMAP(
        /* left hand */
            TAB     , Q   , W    , E   , R   , T ,
            SFT_ENT , A   , S    , D   , F   , G ,
            LALT    , Z   , X    , C   , V   , B ,
            FN0     , FN3 , LCTL , SPC , FN2 ,
        /* right hand */
            Y    , U    , I    , O    , P    , MINS ,
            H    , J    , K    , L    , SCLN , QUOT ,
            N    , M    , COMM , DOT  , SLSH , GRV ,
            BSPC , SSFT , FN1  , LGUI , FN0
    ),
    [ARROWS] = ACTIONMAP(
        /* left hand */
            TRNS  , TRNS  , TRNS  , TRNS  , TRNS  , TRNS  ,
            TRNS  , TRNS  , TRNS  , TRNS  , TRNS  , TRNS  ,
            TRNS  , TRNS  , TRNS  , TRNS  , TRNS  , TRNS  ,
            TRNS  , TRNS  , TRNS  , TRNS  , TRNS  ,
        /* right hand */
            TRNS  , TRNS  , UP    , TRNS  , TRNS  , TRNS  ,
            TRNS  , LEFT  , DOWN  , RIGHT  , TRNS  , TRNS  ,
            TRNS  , TRNS  , TRNS  , TRNS  , TRNS  , TRNS  ,
            TRNS  , TRNS  , TRNS  , TRNS  , TRNS
    ),
#ifdef INVERSE_COLEMAK
    /* inverse colemak */
    [INV_COL] = ACTIONMAP(
        /* left hand */
            TRNS , Q    , W    , K    , S    , F ,
            TRNS , A    , D    , G    , E    , T ,
            TRNS , Z    , X    , C    , V    , B ,
            TRNS , TRNS , TRNS , TRNS , TRNS ,
        /* right hand */
            O    , I    , L    , SCLN , R    , MINS ,
            H    , Y    , N    , U    , P    , QUOT ,
            J    , M    , COMM , DOT  , SLSH , GRV ,
            TRNS , TRNS , TRNS , TRNS , TRNS
    ),
#else
    /* dvorak */
    [DVORAK] = ACTIONMAP(
        /* left hand */
            TRNS , QUOT , COMM , DOT  , P    , Y ,
            TRNS , A    , O    , E    , U    , I ,
            TRNS , SCLN , Q    , J    , K    , X ,
            TRNS , TRNS , TRNS , TRNS , TRNS ,
        /* right hand */
            F    , G    , C    , R    , L    , SLSH ,
            D    , H    , T    , N    , S    , MINS ,
            B    , M    , W    , V    , Z    , GRV  ,
            TRNS , TRNS , TRNS , TRNS , TRNS
    ),
#endif
    /* colemak */
    [COLEMAK] = ACTIONMAP(
        /* left hand */
            TRNS , Q    , W    , F    , P    , G ,
            TRNS , A    , R    , S    , T    , D ,
            TRNS , Z    , X    , C    , V    , B ,
            TRNS , TRNS , TRNS , TRNS , TRNS ,
        /* right hand */
            J    , L    , U    , Y    , SCLN , MINS ,
            H    , N    , E    , I    , O    , QUOT ,
            K    , M    , COMM , DOT  , SLSH , GRV ,
            TRNS , TRNS , TRNS , TRNS , TRNS
   ),
   /* symbol */
   [SYMB] = ACTIONMAP(
       /* left hand */
            TRNS , s(BSLS) , 2    , 3    , 4    , 5       ,
            TRNS , 1       , BSLS , s(9) , LBRC , s(LBRC) ,
            TRNS , s(1)    , s(2) , s(3) , s(4) , s(5)    ,
            TRNS , TRNS    , TRNS , TRNS , TRNS ,
        /* right hand */
            6       , 7    , 8    , 9    , s(EQL) , MINS   ,
            s(RBRC) , RBRC , s(0) , EQL  , 0      , s(8)   ,
            s(6)    , s(7) , COMM , DOT  , SLSH   , s(GRV) ,
            TRNS    , TRNS , TRNS , TRNS , TRNS
    ),
    /* fn */
    [FUNC] = ACTIONMAP(
        /* left hand */
            INS  , DEL  , HOME , UP   , END  , PGUP ,
            TRNS , ESC  , LEFT , DOWN , RGHT , PGDN ,
            TRNS , F1   , F2   , F3   , F4   , F5   ,
            TRNS , TRNS , TRNS , TRNS , TRNS ,
        /* right hand */
            c(PGUP) , c(HOME) , c(UP)   , c(END)  , c(DEL)  , F12 ,
            c(PGDN) , c(LEFT) , c(DOWN) , c(RGHT) , c(BSPC) , F11 ,
            F6      , F7      , F8      , F9      , F10     , F13 ,
            TRNS    , TRNS    , TRNS    , TRNS    , TRNS
        ),
    /* media */
    [MEDIA] = ACTIONMAP(
        /* left hand */
            NO   , PSCR , MPLY , VOLU , MSTP   , F11     ,
            BSPC , MUTE , MPRV , VOLD , MNXT   , APP     ,
            LGUI , c(Y) , c(B) , c(I) , c(DOT) , c(SCLN) ,
            TRNS , TRNS , TRNS , TRNS , TRNS   ,
        /* right hand */
            WH_U , BTN1   , MS_U   , BTN2    , NO   , PROMICRO_PROGRAM  ,
            WH_D , MS_L   , MS_D   , MS_R    , BTN3 , PROMICRO_RESET  ,
            /* ARROWS   , QWERTY , DVORAK , COLEMAK , NO   , FN0 , */
            ARROWS   , SET_QWERTY , SET_DVORAK , SET_COLEMAK , TOGGLE_NKRO , FN0 ,
            TRNS , TRNS   , TRNS   , TRNS    , TRNS
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
    keymap_config_t keymap_config;
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
        case SET_QWERTY:
            default_layer_state = (1<<QWERTY);
            eeprom_update_byte(EECONFIG_DEFAULT_LAYER, (uint8_t)default_layer_state);
            break;
        case SET_DVORAK:
            default_layer_state = (1<<DVORAK);
            eeprom_update_byte(EECONFIG_DEFAULT_LAYER, (uint8_t)default_layer_state);
            break;
        case SET_COLEMAK:
            default_layer_state = (1<<COLEMAK);
            eeprom_update_byte(EECONFIG_DEFAULT_LAYER, (uint8_t)default_layer_state);
            break;
        case TOGGLE_NKRO:
            if (record->event.pressed) {
                keymap_config.raw = eeprom_read_byte(EECONFIG_KEYMAP);
                keymap_config.nkro = !keymap_config.nkro;
                eeprom_update_byte(EECONFIG_KEYMAP, keymap_config.raw);
                keyboard_nkro = keymap_config.nkro;
            }
            break;
        default:
            break;
    }
}

// vim:sw=4:
