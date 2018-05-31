/* Copyright 2017 REPLACE_WITH_YOUR_NAME
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "actg.h"
#include "tetris_text.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[0] = KEYMAP( /* Base */
  KC_T, KC_A, \
  KC_C, KC_G
)
};

const uint16_t PROGMEM fn_actions[] = {
};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
  // MACRODOWN only works in this function
      switch(id) {
        case 0:
          if (record->event.pressed) {
            register_code(KC_RSFT);
          } else {
            unregister_code(KC_RSFT);
          }
        break;
      }
    return MACRO_NONE;
};


void matrix_init_user(void) {

}

void led_set_user(uint8_t usb_led) {

}

static uint8_t daarrows_ascii = 0;
static uint8_t daarrows_i = 0;

uint16_t ascii_to_keycode(uint8_t ascii) {
  switch (ascii) {
  case 0x08: return KC_BSPACE;
  case 0x09: return KC_TAB;
  case 0x0A: return KC_ENTER;
  case 0x1B: return KC_ESCAPE;
  case 0x20: return KC_SPACE;
  case '0': return KC_0;
  case '1': return KC_1;
  case '2': return KC_2;
  case '3': return KC_3;
  case '4': return KC_4;
  case '5': return KC_5;
  case '6': return KC_6;
  case '7': return KC_7;
  case '8': return KC_8;
  case '9': return KC_9;
  case 'q': case 'Q': return KC_Q;
  case 'w': case 'W': return KC_W;
  case 'e': case 'E': return KC_E;
  case 'r': case 'R': return KC_R;
  case 't': case 'T': return KC_T;
  case 'y': case 'Y': return KC_Y;
  case 'u': case 'U': return KC_U;
  case 'i': case 'I': return KC_I;
  case 'o': case 'O': return KC_O;
  case 'p': case 'P': return KC_P;
  case 'a': case 'A': return KC_A;
  case 's': case 'S': return KC_S;
  case 'd': case 'D': return KC_D;
  case 'f': case 'F': return KC_F;
  case 'g': case 'G': return KC_G;
  case 'h': case 'H': return KC_H;
  case 'j': case 'J': return KC_J;
  case 'k': case 'K': return KC_K;
  case 'l': case 'L': return KC_L;
  case 'z': case 'Z': return KC_Z;
  case 'x': case 'X': return KC_X;
  case 'c': case 'C': return KC_C;
  case 'v': case 'V': return KC_V;
  case 'b': case 'B': return KC_B;
  case 'n': case 'N': return KC_N;
  case 'm': case 'M': return KC_M;
  }
  return KC_DOT;
}

static uint8_t tetris_key_presses = 0;
static uint16_t tetris_timer = 0;
static uint8_t tetris_running = 0;
static int tetris_keypress = 0;

void action_function(keyrecord_t *record, uint8_t id, uint8_t opt) {
}

void matrix_scan_user(void) {
  if (tetris_running) {
    tetris_timer++;
    if (tetris_timer > 1000) {
      // every 1000 times this is run is about 100 ms.
      if (!tetris_tick(100)) {
        // game over
        tetris_running = 0;
      }
      tetris_timer = 0;
    }
  }      
}

void send_keycode(uint16_t keycode) {
  register_code(keycode);
  unregister_code(keycode);
}

void send_keycode_shift(uint16_t keycode) {
  register_code(KC_LSFT);
  register_code(keycode);
  unregister_code(keycode);
  unregister_code(KC_LSFT);
}

static uint8_t presses = 0;
static uint8_t pressed = 0;

static uint8_t aminoacid_keys = 0;
static uint8_t aminoacid_count = 0;
static uint16_t aminoacid[3];

uint16_t base(void) {
    switch (pressed) {
    case 0b1000: return KC_A;
    case 0b0100: return KC_T;
    case 0b0010: return KC_C;
    case 0b0001: return KC_G;
    case 0b1001: return KC_R;
    case 0b0110: return KC_Y;
    case 0b0101: return KC_K;
    case 0b1010: return KC_M;
    case 0b0011: return KC_S;
    case 0b1100: return KC_W;
    case 0b0111: return KC_B;
    case 0b1101: return KC_D;
    case 0b1110: return KC_H;
    case 0b1011: return KC_V;
    case 0b1111: return KC_N;
    }
    return KC_SPACE;
}

uint8_t is_aminoacid_mode(void) {
    return aminoacid_keys == 3;
}

void aminoacid_mode(uint16_t key) {
    // D: A T G
    // N: A T C C
    // A: A
    if ((aminoacid_keys == 0 && key == KC_D)
        || (aminoacid_keys == 1 && key == KC_N)
        || (aminoacid_keys == 2 && key == KC_A)) {
        aminoacid_keys++;
    } else {
        aminoacid_keys = 0;
    }
}

uint16_t get_aminoacid(uint16_t base1, uint16_t base2, uint16_t base3) {
    if (base1 == KC_A) {
        if (base2 == KC_A) {
            if (base3 == KC_A || base3 == KC_G) {
                return KC_K;
            } else {
                return KC_N;
            }
        }
        else if (base2 == KC_T) {
            if (base3 == KC_G) {
                return KC_M;
            } else {
                return KC_I;
            }
        }
        else if (base2 == KC_C) {
            return KC_T;
        }
        else if (base2 == KC_G) {
            if (base3 == KC_A || base3 == KC_G) {
                return KC_R;
            } else {
                return KC_S;
            }
        }
    }
    else if (base1 == KC_T) {
        if (base2 == KC_A) {
            if (base3 == KC_T || base3 == KC_C) {
                return KC_Y;
            } else if (base3 == KC_A) {
                return KC_1;
            } else {
                return KC_1;
            }
        }
        else if (base2 == KC_T) {
            if (base3 == KC_T || base3 == KC_C) {
                return KC_F;
            } else {
                return KC_L;
            }
        }
        else if (base2 == KC_C) {
            return KC_S;
        }
        else if (base2 == KC_G) {
            if (base3 == KC_T || base3 == KC_C) {
                return KC_C;
            } else if (base3 == KC_A) {
                return KC_1;
            } else {
                return KC_W;
            }
        }
    }
    else if (base1 == KC_C) {
        if (base2 == KC_A) {
            if (base3 == KC_A || base3 == KC_G) {
                return KC_Q;
            } else {
                return KC_H;
            }
        }
        else if (base2 == KC_T) {
            return KC_L;
        }
        else if (base2 == KC_C) {
            return KC_P;
        }
        else if (base2 == KC_G) {
            return KC_R;
        }
    }
    else if (base1 == KC_G) {
        if (base2 == KC_A) {
            if (base3 == KC_A || base3 == KC_G) {
                return KC_E;
            } else {
                return KC_D;
            }
        }
        else if (base2 == KC_T) {
            return KC_V;
        }
        else if (base2 == KC_C) {
            return KC_A;
        }
        else if (base2 == KC_G) {
            return KC_G;
        }
    }
    return KC_SPACE;
}

uint8_t tetris_keys = 0;
uint8_t is_tetris_mode(void) {
    return tetris_keys == 6;
}
void tetris_mode(uint16_t key) {
    // T: A C C
    // E: G A A
    // T: A C C
    // R: A G G
    // I: A U U
    // S: U C C
    if ((tetris_keys == 0 && key == KC_T)
        || (tetris_keys == 1 && key == KC_E)
        || (tetris_keys == 2 && key == KC_T)
        || (tetris_keys == 3 && key == KC_R)
        || (tetris_keys == 4 && key == KC_I)
        || (tetris_keys == 5 && key == KC_S)) {
        tetris_keys++;
    } else {
        tetris_keys = 0;
    }
}

uint8_t ascii_keys = 0;
uint8_t is_ascii_mode(void) {
    return ascii_keys == 5;
}
void ascii_mode(uint16_t key) {
    // A: G C C
    // S: U C C
    // C: U G U
    // I: A U U
    // I: A U U
    if ((ascii_keys == 0 && key == KC_A)
        || (ascii_keys == 1 && key == KC_S)
        || (ascii_keys == 2 && key == KC_C)
        || (ascii_keys == 3 && key == KC_I)
        || (ascii_keys == 4 && key == KC_I)) {
        ascii_keys++;
    } else {
        ascii_keys = 0;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    if (tetris_running) {
        if (record->event.pressed) {
            tetris_keypress = 0;
            switch (keycode) {
            case KC_A: tetris_keypress = 1; break;
            case KC_C: tetris_keypress = 2; break;
            case KC_T: tetris_keypress = 3; break;
            case KC_G: tetris_keypress = 4; break;
            }
        }
        return false;
    }


    if (is_ascii_mode()) {
        if (record->event.pressed) {
            if (keycode == KC_G) {
                ascii_mode(0);
                return false;
            }

            daarrows_ascii = (daarrows_ascii << 1) + (keycode == KC_A);
            daarrows_i++;
            if (daarrows_i >= 8) {
                uint16_t k = ascii_to_keycode(daarrows_ascii);
                if (k != 0) {
                    send_keycode(k);
                }
                daarrows_ascii = 0;
                daarrows_i = 0;
            }
        }
        return false;
    }
                        
    if (record->event.pressed) {
        tetris_key_presses++;
        presses++;
      
        switch (keycode) {
        case KC_A: pressed += 8; break;
        case KC_T: pressed += 4; break;
        case KC_C: pressed += 2; break;
        case KC_G: pressed += 1; break;
        }
    } else { // released
        if (is_aminoacid_mode()) {
            aminoacid[aminoacid_count] = keycode;
            aminoacid_count++;

            if (aminoacid_count == 3) {
                aminoacid_count = 0;
                uint16_t key = get_aminoacid(aminoacid[0], aminoacid[1], aminoacid[2]);
                send_keycode_shift(key);

                tetris_mode(key);
                if (is_tetris_mode()) {
                    tetris_mode(0); // exit tetris mode so it works when tetris quits
                    send_keycode(KC_ENTER);
                    tetris_running = 1;
                    tetris_keypress = 0;
                    tetris_timer = 0;
                    // set randomness using number of keys pressed
                    tetris_start(tetris_key_presses);
                }

                ascii_mode(key);
                if (is_ascii_mode()) {
                    send_keycode(KC_ENTER);
                }
            }
        } else {
            presses--;
            if (presses == 0) {
                uint16_t key = base();
                send_keycode_shift(key);
                aminoacid_mode(key);
                if (is_aminoacid_mode()) {
                    send_keycode(KC_ENTER);
                }
                pressed = 0;
            }
        }
    }
    return false;

  if (!record->event.pressed) {
      send_keycode_shift(keycode);
      return false;
  }
  return false;
  

  if (tetris_running && record->event.pressed) {
    tetris_keypress = 0;
    switch (keycode) {
    case KC_UP: tetris_keypress = 1; break;
    case KC_LEFT: tetris_keypress = 2; break;
    case KC_DOWN: tetris_keypress = 3; break;
    case KC_RIGHT: tetris_keypress = 4; break;
    case KC_ESC: tetris_running = 0; layer_off(4); return false; // ESC quits tetris
    }
    if (tetris_keypress != 0) {
      return false;
    }
  }

  return true;
}

void tetris_send_up(void) {
  send_keycode(KC_UP);
}
void tetris_send_left(void) {
  send_keycode(KC_LEFT);
}
void tetris_send_down(void) {
  send_keycode(KC_DOWN);
}
void tetris_send_right(void) {
  send_keycode(KC_RGHT);
}

void tetris_send_backspace(void) {
  send_keycode(KC_BSPC);
}
void tetris_send_delete(void) {
  send_keycode(KC_DEL);
}

void tetris_send_string(const char *s) {
  for (int i = 0; s[i] != 0; i++) {
    if (s[i] >= 'a' && s[i] <= 'z') {
      send_keycode(KC_A + (s[i] - 'a'));
    } else if (s[i] >= 'A' && s[i] <= 'Z') {
      send_keycode_shift(KC_A + (s[i] - 'A'));
    } else if (s[i] >= '1' && s[i] <= '9') {
      send_keycode(KC_1 + (s[i] - '1'));
    } else {
      switch (s[i]) {
      case ' ': send_keycode(KC_SPACE); break;
      case '.': send_keycode(KC_DOT); break;
      case '0': send_keycode(KC_0); break;
      }
    }
  }
}

void tetris_send_newline(void) {
  send_keycode(KC_ENT);
}

int tetris_get_keypress(void) {
  int out = tetris_keypress;
  tetris_keypress = 0;
  return out;
}


