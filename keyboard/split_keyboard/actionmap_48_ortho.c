#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include "host.h"
#include "eeconfig.h"
#include "action_layer.h"
#include "keymap.h"
#include "actionmap.h"
#include "actionmap_48_ortho_common.h"
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

#define RESET_TAP_COUNT 1

#define QWERTY 0
#define DVORAK 1
#define COLEMAK 2
#define RAISE 3
#define LOWER 4
#define ADJUST 5

/* #define AC_RAISE            ACTION_LAYER_ONESHOT(RAISE) */
/* #define AC_LOWER            ACTION_LAYER_ONESHOT(LOWER) */
/* #define AC_ADJUST           ACTION_LAYER_ONESHOT(ADJUST) */
#define AC_RAISE             ACTION_LAYER_MOMENTARY(RAISE)
#define AC_LOWER             ACTION_LAYER_MOMENTARY(LOWER)
#define AC_ADJUST            ACTION_LAYER_MOMENTARY(ADJUST)

#define AC_QWERTY            ACTION_DEFAULT_LAYER_SET(QWERTY)
#define AC_DVORAK            ACTION_DEFAULT_LAYER_SET(DVORAK)
#define AC_COLEMAK           ACTION_DEFAULT_LAYER_SET(COLEMAK)

#define AC_PROGRAM           ACTION_FUNCTION_TAP(PROMICRO_PROGRAM)
#define AC_RESET             ACTION_FUNCTION_TAP(PROMICRO_RESET)
#define AC_SET_QWERTY        ACTION_FUNCTION_TAP(SET_QWERTY)
#define AC_SET_DVORAK        ACTION_FUNCTION_TAP(SET_DVORAK)
#define AC_SET_COLEMAK       ACTION_FUNCTION_TAP(SET_COLEMAK)
#define AC_TOGGLE_NKRO       ACTION_FUNCTION_TAP(TOGGLE_NKRO)

const uint16_t PROGMEM actionmaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* qwerty */
    [QWERTY] = ACTIONMAP(
        /* left hand */
            TAB  , Q    , W    , E    , R     , T   ,
            ESC  , A    , S    , D    , F     , G   ,
            LSFT , Z    , X    , C    , V     , B   ,
            NO   , LCTL , LALT , LGUI , LOWER , SPC ,
        /* right hand */
            Y   , U     , I    , O    , P    , BSPC  ,
            H   , J     , K    , L    , SCLN , QUOT  ,
            N   , M     , COMM , DOT  , SLSH , ENT   ,
            SPC , RAISE , LEFT , DOWN , UP   , RIGHT
    ),

    /* dvorak */
    [DVORAK] = ACTIONMAP(
        /* left hand */
            TAB  , QUOT , COMM , DOT  , P     , Y   ,
            ESC  , A    , O    , E    , U     , I   ,
            LSFT , SCLN , Q    , J    , K     , X   ,
            NO   , LCTL , LALT , LGUI , LOWER , SPC ,
        /* right hand */
            F   , G     , C    , R    , L  , BSPC  ,
            D   , H     , T    , N    , S  , SLSH  ,
            B   , M     , W    , V    , Z  , ENT   ,
            SPC , RAISE , LEFT , DOWN , UP , RIGHT
    ),

    /* colemak */
    [COLEMAK] = ACTIONMAP(
        /* left hand */
            TAB  , Q    , W    , F    , P     , G   ,
            ESC  , A    , R    , S    , T     , D   ,
            LSFT , Z    , X    , C    , V     , B   ,
            NO   , LCTL , LALT , LGUI , LOWER , SPC ,
        /* right hand */
            J   , L     , U    , Y    , SCLN , BSPC  ,
            H   , N     , E    , I    , O    , QUOT  ,
            K   , M     , COMM , DOT  , SLSH , ENT   ,
            SPC , RAISE , LEFT , DOWN , UP   , RIGHT
   ),

   /* lower */
   [LOWER] = ACTIONMAP(
       /* left hand */
        GRV  , s(1) , s(2) , s(3) , s(4) , s(5) ,
        DEL  , F1   , F2   , F3   , F4   , F5   ,
        ____ , F7   , F8   , F9   , F10  , F11  ,
        ____ , ____ , ____ , ____ , ____ , ____ ,
        /* right hand */
        s(6) , s(7) , s(8) , s(9) , s(0) , BSPC ,
        F6   , MINS , EQL  , LBRC , RBRC , BSLS ,
        F12  , NUHS , NUBS , ____ , ____ , ____ ,
        ____ , ADJUST , MNXT , VOLD , VOLU , MPLY
    )        ,

    /* raise */
    [RAISE] = ACTIONMAP(
        /* left hand */
            TILD , 1    , 2    , 3    , 4    , 5    ,
            DEL  , F1   , F2   , F3   , F4   , F5   ,
            ____ , F7   , F8   , F9   , F10  , F11  ,
            ____ , ____ , ____ , ____ , ADJUST , ____ ,
        /* right hand */
            6    , 7          , 8          , 9    , 0    , BSPC ,
            F6   , UNDS       , PLUS       , LCBR , RCBR , PIPE ,
            F12  , s(NUHS) , s(NUBS) , ____ , ____ , ____ ,
            ____ , ____       , MNXT       , VOLD , VOLU , MPLY
    ),
    /* adjust */
    [ADJUST] = ACTIONMAP(
        /* left hand */
            ____ , PROGRAM , RESET , ____ , ____ , ____ ,
            ____ , ____ , ____ , ____ , ____ , ____ ,
            ____ , ____ , ____ , ____ , ____ , ____ ,
            ____ , ____ , ____ , ____ , ____ , ____ ,
        /* right hand */
            ____ , SET_QWERTY , SET_DVORAK , SET_COLEMAK , TOGGLE_NKRO , ____ ,
            ____ , ____       , ____       , ____        , ____        , ____ ,
            ____ , ____       , ____       , ____        , ____        , ____ ,
            ____ , ____       , ____       , ____        , ____        , ____
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
            if (tap_count == RESET_TAP_COUNT) {
                promicro_bootloader_jmp(false);
            }
            break;
        case PROMICRO_PROGRAM:
            if(tap_count == RESET_TAP_COUNT) {
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
