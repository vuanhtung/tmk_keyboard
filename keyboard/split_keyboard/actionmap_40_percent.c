#define _QWERTY 0
#define _COLEMAK 1
#define _DVORAK 2
#define _LOWER 3
#define _RAISE 4
#define _PLOVER 5
#define _ADJUST 16

enum planck_keycodes {
  QWERTY = SAFE_RANGE,
  COLEMAK,
  DVORAK,
  PLOVER,
  LOWER,
  RAISE,
  BACKLIT,
  EXT_PLV
};

// Fillers to make layering more clear
#define _______ KC_TRNS
#define XXXXXXX KC_NO

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Qwerty
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Esc  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  "   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Brite| Ctrl | Alt  | GUI  |Lower |    Space    |Raise | Left | Down |  Up  |Right |
 * `-----------------------------------------------------------------------------------'
 */
[_QWERTY] = {
  {KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC},
  {KC_ESC,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT},
  {KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_ENT },
  {BACKLIT, KC_LCTL, KC_LALT, KC_LGUI, LOWER,   KC_SPC,  KC_SPC,  RAISE,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT}
},

/* Colemak
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |   Q  |   W  |   F  |   P  |   G  |   J  |   L  |   U  |   Y  |   ;  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Esc  |   A  |   R  |   S  |   T  |   D  |   H  |   N  |   E  |   I  |   O  |  "   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   K  |   M  |   ,  |   .  |   /  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Brite| Ctrl | Alt  | GUI  |Lower |    Space    |Raise | Left | Down |  Up  |Right |
 * `-----------------------------------------------------------------------------------'
 */
[_COLEMAK] = {
  {KC_TAB,  KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,    KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN, KC_BSPC},
  {KC_ESC,  KC_A,    KC_R,    KC_S,    KC_T,    KC_D,    KC_H,    KC_N,    KC_E,    KC_I,    KC_O,    KC_QUOT},
  {KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_K,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_ENT },
  {BACKLIT, KC_LCTL, KC_LALT, KC_LGUI, LOWER,   KC_SPC,  KC_SPC,  RAISE,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT}
},

/* Dvorak
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |   "  |   ,  |   .  |   P  |   Y  |   F  |   G  |   C  |   R  |   L  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Esc  |   A  |   O  |   E  |   U  |   I  |   D  |   H  |   T  |   N  |   S  |  /   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Shift|   ;  |   Q  |   J  |   K  |   X  |   B  |   M  |   W  |   V  |   Z  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Brite| Ctrl | Alt  | GUI  |Lower |    Space    |Raise | Left | Down |  Up  |Right |
 * `-----------------------------------------------------------------------------------'
 */
[_DVORAK] = {
  {KC_TAB,  KC_QUOT, KC_COMM, KC_DOT,  KC_P,    KC_Y,    KC_F,    KC_G,    KC_C,    KC_R,    KC_L,    KC_BSPC},
  {KC_ESC,  KC_A,    KC_O,    KC_E,    KC_U,    KC_I,    KC_D,    KC_H,    KC_T,    KC_N,    KC_S,    KC_SLSH},
  {KC_LSFT, KC_SCLN, KC_Q,    KC_J,    KC_K,    KC_X,    KC_B,    KC_M,    KC_W,    KC_V,    KC_Z,    KC_ENT },
  {BACKLIT, KC_LCTL, KC_LALT, KC_LGUI, LOWER,   KC_SPC,  KC_SPC,  RAISE,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT}
},

/* Lower
 * ,-----------------------------------------------------------------------------------.
 * |   ~  |   !  |   @  |   #  |   $  |   %  |   ^  |   &  |   *  |   (  |   )  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Del  |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |   _  |   +  |   {  |   }  |  |   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |ISO ~ |ISO | |      |      |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      | Next | Vol- | Vol+ | Play |
 * `-----------------------------------------------------------------------------------'
 */
[_LOWER] = {
  {KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_BSPC},
  {KC_DEL,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE},
  {_______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,S(KC_NUHS),S(KC_NUBS),_______, _______, _______},
  {_______, _______, _______, _______, _______, _______, _______, _______, KC_MNXT, KC_VOLD, KC_VOLU, KC_MPLY}
},

/* Raise
 * ,-----------------------------------------------------------------------------------.
 * |   `  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Del  |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |   -  |   =  |   [  |   ]  |  \   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |ISO # |ISO / |      |      |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      | Next | Vol- | Vol+ | Play |
 * `-----------------------------------------------------------------------------------'
 */
[_RAISE] = {
  {KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC},
  {KC_DEL,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS},
  {_______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_NUHS, KC_NUBS, _______, _______, _______},
  {_______, _______, _______, _______, _______, _______, _______, _______, KC_MNXT, KC_VOLD, KC_VOLU, KC_MPLY}
},

/* Plover layer (http://opensteno.org)
 * ,-----------------------------------------------------------------------------------.
 * |   #  |   #  |   #  |   #  |   #  |   #  |   #  |   #  |   #  |   #  |   #  |   #  |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |      |   S  |   T  |   P  |   H  |   *  |   *  |   F  |   P  |   L  |   T  |   D  |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |TogOut|   S  |   K  |   W  |   R  |   *  |   *  |   R  |   B  |   G  |   S  |   Z  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Exit |      |      |   A  |   O  |             |   E  |   U  |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */

[_PLOVER] = {
  {KC_1,    KC_1,    KC_1,    KC_1,    KC_1,    KC_1,    KC_1,    KC_1,    KC_1,    KC_1,    KC_1,    KC_1   },
  {XXXXXXX, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC},
  {XXXXXXX, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT},
  {EXT_PLV, XXXXXXX, XXXXXXX, KC_C,    KC_V,    XXXXXXX, XXXXXXX, KC_N,    KC_M,    XXXXXXX, XXXXXXX, XXXXXXX}
},

/* Adjust (Lower + Raise)
 * ,-----------------------------------------------------------------------------------.
 * |      | Reset|      |      |      |      |      |      |      |      |      |  Del |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |      |      |      |Aud on|Audoff|AGnorm|AGswap|Qwerty|Colemk|Dvorak|Plover|      |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |Voice-|Voice+|Mus on|Musoff|MIDIon|MIDIof|      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_ADJUST] = {
  {_______, RESET,   _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_DEL},
  {_______, _______, _______, AU_ON,   AU_OFF,  AG_NORM, AG_SWAP, QWERTY,  COLEMAK, DVORAK,  PLOVER,  _______},
  {_______, MUV_DE,  MUV_IN,  MU_ON,   MU_OFF,  MI_ON,   MI_OFF,  _______, _______, _______, _______, _______},
  {_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______}
}


};

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

#define QWERTY 0
#define ARROWS 1
#define INV_COL 2
#define DVORAK 2
#define COLEMAK 3
#define SYMB 4
#define FUNC 5
#define MEDIA 6

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

#define INVERSE_COLEMAK

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
            ARROWS   , QWERTY , DVORAK , COLEMAK , NO   , FN0 ,
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

#define KPRINT_DELAY 1
void kput_char(char c) {
    uint8_t code = 0;
    uint8_t mods = 0;
    if ('a' <= c && c <= 'z') {
        code = (c - 'a') + KC_A;
    } else if ('A' <= c && c <= 'Z') {
        code = (c - 'A') + KC_A;
        mods = MOD_BIT(KC_LSHIFT);
    } else if ('0' <= c && c <= '9') {
        code = (c == '0') ? KC_0 : (c - '1') + KC_1;
    } else {
        switch (c) {
            case ' ': code = KC_SPACE; break;
            case '\n': code = KC_ENTER; break;
            case '\t': code = KC_TAB; break;
            case ';': code = KC_SCOLON; break;
            case ',': code = KC_COMMA; break;
            case '.': code = KC_DOT; break;
            case '/': code = KC_SLASH; break;
            case '\\': code = KC_BSLASH; break;
            case '[': code = KC_LBRACKET; break;
            case ']': code = KC_RBRACKET; break;
            case '-': code = KC_MINUS; break;
            case '=': code = KC_EQUAL; break;
            case '`': code = KC_GRAVE; break;
            case '\'': code = KC_QUOTE; break;
        }
        if (!code) {
            switch (c) {
                case ':': code = KC_SCOLON; break;
                case '<': code = KC_COMMA; break;
                case '>': code = KC_DOT; break;
                case '?': code = KC_SLASH; break;
                case '|': code = KC_BSLASH; break;
                case '{': code = KC_LBRACKET; break;
                case '}': code = KC_RBRACKET; break;
                case '_': code = KC_MINUS; break;
                case '+': code = KC_EQUAL; break;
                case '~': code = KC_GRAVE; break;
                case '"': code = KC_QUOTE; break;

                case '!': code = KC_1; break;
                case '@': code = KC_2; break;
                case '#': code = KC_3; break;
                case '$': code = KC_4; break;
                case '%': code = KC_5; break;
                case '^': code = KC_6; break;
                case '&': code = KC_7; break;
                case '*': code = KC_8; break;
                case '(': code = KC_9; break;
                case ')': code = KC_0; break;
            }
            mods = code ? MOD_BIT(KC_LSHIFT) : 0;
        }
    }
    if (!code) {
        code = KC_SLASH;
        mods = MOD_BIT(KC_LSHIFT);
    }

    // key down
    add_weak_mods(mods);
    register_code(code);

    wait_ms(KPRINT_DELAY);

    // key up
    del_weak_mods(mods);
    unregister_code(code);
    /* send_keyboard_report(); */
}

void kprint(char *s) {
    while(*s) {
        kput_char(*s);
        s++;
    }
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
