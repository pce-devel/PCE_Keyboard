/*
 * PCEKeybd - Adapts a USB keyboard for use with the PC Engine (as Tsuushin Keyboard)
 *            For Raspberry Pi Pico or other RP2040 MCU
 *            In particular, I like the Adafruit KB2040 board
 *
 * This code is based on the TinyUSB Host HID example from pico-SDK v1.5.0
 *
 * Modifications for PCEKeybd
 * Copyright (c) 2021, 2022, 2023 David Shadoff
 *
 * ------------------------------------
 *
 * The MIT License (MIT)
 *
 * Original TinyUSB example
 * Copyright (c) 2021, Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "bsp/board.h"
#include "tusb.h"

//--------------------------------------------------------------------+
// MACRO TYPEDEF CONSTANT ENUM DECLARATION
//--------------------------------------------------------------------+

// If your host terminal support ansi escape code such as TeraTerm
// it can be use to simulate mouse cursor movement within terminal
#define USE_ANSI_ESCAPE   0

#define MAX_REPORT  4


// Core functionality
// ------------------
static uint8_t const keycode2ascii[128][2] =  { HID_KEYCODE_TO_ASCII };

static uint8_t const keycode2PCE[141][2] = {
{ 0, 0x00}, //   0 - first byte is offset from PCE array, second byte is bitmask
{ 0, 0x00}, //   1
{ 0, 0x00}, //   2
{ 0, 0x00}, //   3
{ 3, 0x02}, //   4 = A
{ 3, 0x04}, //   5 = B
{ 3, 0x08}, //   6 = C
{ 3, 0x10}, //   7 = D
{ 3, 0x20}, //   8 = E
{ 3, 0x40}, //   9 = F
{12, 0x40}, //  10 = G
{ 4, 0x01}, //  11 = H
{ 4, 0x02}, //  12 = I
{ 4, 0x04}, //  13 = J
{ 4, 0x08}, //  14 = K
{ 4, 0x10}, //  15 = L
{ 4, 0x20}, //  16 = M
{ 4, 0x40}, //  17 = N
{12, 0x10}, //  18 = O
{ 5, 0x01}, //  19 = P
{ 5, 0x02}, //  20 = Q
{ 5, 0x04}, //  21 = R
{ 5, 0x08}, //  22 = S
{ 5, 0x10}, //  23 = T
{ 5, 0x20}, //  24 = U
{ 5, 0x40}, //  25 = V
{14, 0x40}, //  26 = W
{ 6, 0x01}, //  27 = X
{ 6, 0x02}, //  28 = Y
{ 6, 0x04}, //  29 = Z
{ 7, 0x02}, //  30 = 1
{ 7, 0x04}, //  31 = 2
{ 7, 0x08}, //  32 = 3
{ 7, 0x10}, //  33 = 4
{ 7, 0x20}, //  34 = 5
{ 7, 0x40}, //  35 = 6
{16, 0x40}, //  36 = 7
{ 8, 0x01}, //  37 = 8
{ 8, 0x02}, //  38 = 9
{ 7, 0x01}, //  39 = 0
{15, 0x01}, //  40 = Return
{15, 0x40}, //  41 = Escape
{13, 0x20}, //  42 = Backspace
{11, 0x01}, //  43 = Tab
{10, 0x40}, //  44 = Space
{16, 0x20}, //  45 = -
{ 6, 0x40}, //  46 = ^
{ 3, 0x01}, //  47 = @
{ 6, 0x08}, //  48 = [
{ 0, 0x00}, //  49 =    (backslash on western keyboards, not on Japanese)
{ 6, 0x20}, //  50 = ]
{ 8, 0x08}, //  51 = ;
{ 8, 0x04}, //  52 = :
{14, 0x08}, //  53 = hankaku
{ 8, 0x10}, //  54 = ,
{ 8, 0x20}, //  55 = .
{ 8, 0x40}, //  56 = /
{15, 0x10}, //  57 = CapsLock
{10, 0x02}, //  58 = F1
{10, 0x04}, //  59 = F2
{10, 0x08}, //  60 = F3
{10, 0x10}, //  61 = F4
{10, 0x20}, //  62 = F5
{13, 0x01}, //  63 = F6
{13, 0x02}, //  64 = F7
{13, 0x04}, //  65 = F8
{13, 0x08}, //  66 = F9
{13, 0x10}, //  67 = F10
{ 0, 0x00}, //  68 = 
{ 0, 0x00}, //  69 = 
{11, 0x08}, //  70 = Print Screen (COPY)
{ 0, 0x00}, //  71 = Scroll Lock (GRPH)
{10, 0x01}, //  72 = Pause/Break (STOP)
{13, 0x40}, //  73 = Insert
{ 9, 0x01}, //  74 = Home (CLR)
{12, 0x02}, //  75 = RollUp
{15, 0x20}, //  76 = Del
{11, 0x10}, //  77 = End (HELP)
{12, 0x01}, //  78 = RollDn
{ 9, 0x04}, //  79 = Right
{11, 0x04}, //  80 = Left
{11, 0x02}, //  81 = Down
{ 9, 0x02}, //  82 = Up
{ 0, 0x00}, //  83 = 
{11, 0x40}, //  84 = Keypd /
{ 2, 0x04}, //  85 = Keypd *
{11, 0x20}, //  86 = Keypd -
{ 2, 0x08}, //  87 = Keypd +
{15, 0x02}, //  88 = Keypd Enter
{ 1, 0x02}, //  89 = Keypd 1
{ 1, 0x04}, //  90 = Keypd 2
{ 1, 0x08}, //  91 = Keypd 3
{ 1, 0x10}, //  92 = Keypd 4
{ 1, 0x20}, //  93 = Keypd 5
{ 1, 0x40}, //  94 = Keypd 6
{14, 0x20}, //  95 = Keypd 7
{ 2, 0x01}, //  96 = Keypd 8
{ 2, 0x02}, //  97 = Keypd 9
{ 1, 0x01}, //  98 = Keypd 0
{ 2, 0x40}, //  99 = Keypd .
{ 0, 0x00}, // 100 = 
{ 0, 0x00}, // 101 = 
{ 0, 0x00}, // 102 = 
{ 0, 0x00}, // 103 = 
{ 0, 0x00}, // 104 = 
{ 0, 0x00}, // 105 = 
{ 0, 0x00}, // 106 = 
{ 0, 0x00}, // 107 = 
{ 0, 0x00}, // 108 = 
{ 0, 0x00}, // 109 = 
{ 0, 0x00}, // 110 = 
{ 0, 0x00}, // 111 = 
{ 0, 0x00}, // 112 = 
{ 0, 0x00}, // 113 = 
{ 0, 0x00}, // 114 = 
{ 0, 0x00}, // 115 = 
{ 0, 0x00}, // 116 = 
{ 0, 0x00}, // 117 = 
{ 0, 0x00}, // 118 = 
{ 0, 0x00}, // 119 = 
{ 0, 0x00}, // 120 = 
{ 0, 0x00}, // 121 = 
{ 0, 0x00}, // 122 = 
{ 0, 0x00}, // 123 = 
{ 0, 0x00}, // 124 = 
{ 0, 0x00}, // 125 = 
{ 0, 0x00}, // 126 = 
{ 0, 0x00}, // 127 = 
{ 0, 0x00}, // 128 = 
{ 0, 0x00}, // 129 = 
{ 0, 0x00}, // 130 = 
{ 0, 0x00}, // 131 = 
{ 0, 0x00}, // 132 = 
{ 0, 0x00}, // 133 = 
{ 0, 0x00}, // 134 = 
{12, 0x20}, // 135 = \ (underscore)
{ 9, 0x20}, // 136 = Kana
{ 6, 0x10}, // 137 = yen
{14, 0x01}, // 138 = henkou
{14, 0x02}, // 139 = muhenkou
{ 0, 0x00}  // 140 = 

};

// uint8_t key_latch[18];  // latched from key_array when latch signal comes

volatile uint8_t key_array[18] = {
  0x02,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0x02
};

//  0:
//     start = 0x02
//
//  Note that the following keys are '1' when not pressed, and '0' when pressed
//
//  1:
//    bit 7   bit 6   bit 5   bit 4   bit 3   bit 2   bit 1   bit 0
//   (Val 0)  Kypd 6  Kypd 5  Kypd 4  Kypd 3  Kypd 2  Kypd 1  Kypd 0
//             94      93      92      91      90      89      98
//
//  2:
//    bit 7   bit 6   bit 5   bit 4   bit 3   bit 2   bit 1   bit 0
//   (Val 0)  Kypd .  Kypd ,  Kypd =  Kypd +  Kypd *  Kypd 9  Kypd 8
//             99      ??      ??      87      85      97      96
//
//  3:
//    bit 7   bit 6   bit 5   bit 4   bit 3   bit 2   bit 1   bit 0
//   (Val 0)    f       e       d       c       b       a       @
//              9       8       7       6       5       4      47
//
//  4:
//    bit 7   bit 6   bit 5   bit 4   bit 3   bit 2   bit 1   bit 0
//   (Val 0)    n       m       l       k       j       i       h
//             17      16      15      14      13      12      11
//
//  5:
//    bit 7   bit 6   bit 5   bit 4   bit 3   bit 2   bit 1   bit 0
//   (Val 0)    v       u       t       s       r       q       p
//             25      24      23      22      21      20      19
//
//  6:
//    bit 7   bit 6   bit 5   bit 4   bit 3   bit 2   bit 1   bit 0
//   (Val 0)    ^       ]      yen      [       z       y       x
//             46      50      137     48      29      28      27
//
//  7:
//    bit 7   bit 6   bit 5   bit 4   bit 3   bit 2   bit 1   bit 0
//   (Val 0)    6       5       4       3       2       1       0
//             35      34      33      32      31      30      39
//
//  8:
//    bit 7   bit 6   bit 5   bit 4   bit 3   bit 2   bit 1   bit 0
//   (Val 0)    /       .       ,       ;       :       9       8
//             56      55      54      51      52      38      37
//
//  9:
//    bit 7   bit 6   bit 5   bit 4   bit 3   bit 2   bit 1   bit 0
//   (Val 0) (Val 0)  KANA    GRPH   (Val 0)  right    up     clear
//            shift?                 BKSPC?
//          (special) 136      **      42      79      82      **
//
// 10:
//    bit 7   bit 6   bit 5   bit 4   bit 3   bit 2   bit 1   bit 0
//   (Val 0)  space    F5      F4      F3      F2      F1     stop
//             44      62      61      60      59      58      **
//
// 11:
//    bit 7   bit 6   bit 5   bit 4   bit 3   bit 2   bit 1   bit 0
//   (Val 0)  Kypd /  Kypd -  copy    help    left    down     tab
//             84      86      **      **      80      81      43
//
// 12:
//    bit 7   bit 6   bit 5   bit 4   bit 3   bit 2   bit 1   bit 0
//   (Val 0)    g       _       o    (Val 0) (Val 0) rollup  rolldn
//             10      **      18                      75      78
//
// 13:
//    bit 7   bit 6   bit 5   bit 4   bit 3   bit 2   bit 1   bit 0
//   (Val 0)   ins    bkspc    F10     F9      F8      F7      F6
//             73      42      67      66      65      64      63
//
// 14:
//    bit 7   bit 6   bit 5   bit 4   bit 3   bit 2   bit 1   bit 0
//   (Val 0)    w     Kypd 7  ctrl   hankaku   PC     muhen   henkou
//             26      95      **      53     136     139     138
//
// 15:
//    bit 7   bit 6   bit 5   bit 4   bit 3   bit 2   bit 1   bit 0
//   (Val 0)   Esc     Del    CapsLk  R_Shft L_Shft  Kypd En  Return
//             41      76      57      **      **      88      40
//
// 16:
//    bit 7   bit 6   bit 5   bit 4   bit 3   bit 2   bit 1   bit 0
//   (Val 0)    7       -    (Val 0) (Val 0) (Val 0) (Val 0) (Val 0)
//             36      45
//
// 17: 
//     end  = 0x02

// Each HID instance can has multiple reports
static struct
{
  uint8_t report_count;
  tuh_hid_report_info_t report_info[MAX_REPORT];
}hid_info[CFG_TUH_HID];

static void process_kbd_report(hid_keyboard_report_t const *report);
static void process_mouse_report(hid_mouse_report_t const * report);
static void process_generic_report(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len);

void hid_app_task(void)
{
  // nothing to do
}

//--------------------------------------------------------------------+
// TinyUSB Callbacks
//--------------------------------------------------------------------+

// Invoked when device with hid interface is mounted
// Report descriptor is also available for use. tuh_hid_parse_report_descriptor()
// can be used to parse common/simple enough descriptor.
// Note: if report descriptor length > CFG_TUH_ENUMERATION_BUFSIZE, it will be skipped
// therefore report_desc = NULL, desc_len = 0
void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* desc_report, uint16_t desc_len)
{
  printf("HID device address = %d, instance = %d is mounted\r\n", dev_addr, instance);

  // Interface protocol (hid_interface_protocol_enum_t)
  const char* protocol_str[] = { "None", "Keyboard", "Mouse" };
  uint8_t const itf_protocol = tuh_hid_interface_protocol(dev_addr, instance);

  printf("HID Interface Protocol = %s\r\n", protocol_str[itf_protocol]);

  // By default host stack will use activate boot protocol on supported interface.
  // Therefore for this simple example, we only need to parse generic report descriptor (with built-in parser)
  if ( itf_protocol == HID_ITF_PROTOCOL_NONE )
  {
    hid_info[instance].report_count = tuh_hid_parse_report_descriptor(hid_info[instance].report_info, MAX_REPORT, desc_report, desc_len);
    printf("HID has %u reports \r\n", hid_info[instance].report_count);
  }

  // request to receive report
  // tuh_hid_report_received_cb() will be invoked when report is available
  if ( !tuh_hid_receive_report(dev_addr, instance) )
  {
    printf("Error: cannot request to receive report\r\n");
  }
}

// Invoked when device with hid interface is un-mounted
void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance)
{
  printf("HID device address = %d, instance = %d is unmounted\r\n", dev_addr, instance);
}

// Invoked when received report from device via interrupt endpoint
void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len)
{
  uint8_t const itf_protocol = tuh_hid_interface_protocol(dev_addr, instance);

  switch (itf_protocol)
  {
    case HID_ITF_PROTOCOL_KEYBOARD:
      TU_LOG2("HID receive boot keyboard report\r\n");
      process_kbd_report( (hid_keyboard_report_t const*) report );
    break;

    case HID_ITF_PROTOCOL_MOUSE:
      TU_LOG2("HID receive boot mouse report\r\n");
      process_mouse_report( (hid_mouse_report_t const*) report );
    break;

    default:
      // Generic report requires matching ReportID and contents with previous parsed report info
      process_generic_report(dev_addr, instance, report, len);
    break;
  }

  // continue to request to receive report
  if ( !tuh_hid_receive_report(dev_addr, instance) )
  {
    printf("Error: cannot request to receive report\r\n");
  }
}

//--------------------------------------------------------------------+
// Keyboard
//--------------------------------------------------------------------+

// look up new key in previous keys
static inline bool find_key_in_report(hid_keyboard_report_t const *report, uint8_t keycode)
{
  for(uint8_t i=0; i<6; i++)
  {
    if (report->keycode[i] == keycode)  return true;
  }

  return false;
}

static void process_kbd_report(hid_keyboard_report_t const *report)
{
  static hid_keyboard_report_t prev_report = { 0, 0, {0} }; // previous report to check key released

uint32_t i,j;
uint8_t temp_key;
bool fnd;
uint8_t array_ptr;
uint8_t bitfld;
//
// scan prev_report.keycode[] keycode array (up to 6 entries) to
// check whether they are in the report->keycode[] array
// (i.e. still pressed)
// If not in new array, key is deemed to have been released
//
  for(i=0; i<6; i++)
  {
    if (prev_report.keycode[i] > 3) {
      fnd = false;
      temp_key = prev_report.keycode[i];
      for (j=0; j<6; j++) {
        if (report->keycode[j] == temp_key) fnd = true;
      }
      if (fnd == false) {
        printf("release %d\n", temp_key);
        array_ptr = keycode2PCE[temp_key][0];
        bitfld = (keycode2PCE[temp_key][1]);
        if (array_ptr > 0) {
          key_array[array_ptr] |= bitfld;
        }
      }
    }
  }

//
// scan report->keycode[] keycode array (up to 6 entries) to
// check whether they are in the prev_report.keycode[] array
// (i.e. still pressed)
// If not in new array, key is deemed to have been released
//
  for(i=0; i<6; i++)
  {
    if (report->keycode[i] > 3) {
      fnd = false;
      temp_key = report->keycode[i];
      for (j=0; j<6; j++) {
        if (prev_report.keycode[j] == temp_key) fnd = true;
      }
      if (fnd == false) {
        printf("press %d\n", temp_key);
        array_ptr = keycode2PCE[temp_key][0];
        bitfld = (keycode2PCE[temp_key][1] ^ 0xFF);
        if (array_ptr > 0) {
          key_array[array_ptr] &= bitfld;
        }
      }
    }
  }

  if (report->modifier & KEYBOARD_MODIFIER_LEFTSHIFT) {
    key_array[15] &= 0xFB;
  } else {
    key_array[15] |= 0x04;
  }

  if (report->modifier & KEYBOARD_MODIFIER_RIGHTSHIFT) {
    key_array[15] &= 0xF7;
  } else {
    key_array[15] |= 0x08;
  }

  if (report->modifier & (KEYBOARD_MODIFIER_LEFTCTRL | KEYBOARD_MODIFIER_RIGHTCTRL)) {
    key_array[14] &= 0xEF;
  } else {
    key_array[14] |= 0x10;
  }

  if (report->modifier & KEYBOARD_MODIFIER_LEFTALT) {   // GRPH
    key_array[9] &= 0xEF;
  } else {
    key_array[9] |= 0x10;
  }

  if (report->modifier & KEYBOARD_MODIFIER_RIGHTALT) {   // PC
    key_array[14] &= 0xFB;
  } else {
    key_array[14] |= 0x04;
  }


  //------------- example code ignore control (non-printable) key affects -------------//
//  for(uint8_t i=0; i<6; i++)
//  {
//    if ( report->keycode[i] )
//    {
//      if ( find_key_in_report(&prev_report, report->keycode[i]) )
//      {
//        // exist in previous report means the current key is holding
//      }else
//      {
//        // not existed in previous report means the current key is pressed
//        bool const is_shift = report->modifier & (KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_RIGHTSHIFT);
//        uint8_t ch = keycode2ascii[report->keycode[i]][is_shift ? 1 : 0];
//        //putchar(ch);
//        printf("%4.4X %d\n", report->modifier, report->keycode[i]);
//
//        //if ( ch == '\r' ) putchar('\n'); // added new line for enter key
//
//        fflush(stdout); // flush right away, else nanolib will wait for newline
//      }
//    }
//    // TODO example skips key released
//  }

  prev_report = *report;
}

//--------------------------------------------------------------------+
// Mouse
//--------------------------------------------------------------------+

static void process_mouse_report(hid_mouse_report_t const * report)
{
  static hid_mouse_report_t prev_report = { 0 };

}

//--------------------------------------------------------------------+
// Generic Report
//--------------------------------------------------------------------+
static void process_generic_report(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len)
{
  (void) dev_addr;

  uint8_t const rpt_count = hid_info[instance].report_count;
  tuh_hid_report_info_t* rpt_info_arr = hid_info[instance].report_info;
  tuh_hid_report_info_t* rpt_info = NULL;

  if ( rpt_count == 1 && rpt_info_arr[0].report_id == 0)
  {
    // Simple report without report ID as 1st byte
    rpt_info = &rpt_info_arr[0];
  }else
  {
    // Composite report, 1st byte is report ID, data starts from 2nd byte
    uint8_t const rpt_id = report[0];

    // Find report id in the arrray
    for(uint8_t i=0; i<rpt_count; i++)
    {
      if (rpt_id == rpt_info_arr[i].report_id )
      {
        rpt_info = &rpt_info_arr[i];
        break;
      }
    }

    report++;
    len--;
  }

  if (!rpt_info)
  {
    printf("Couldn't find the report info for this report !\r\n");
    return;
  }

  // For complete list of Usage Page & Usage checkout src/class/hid/hid.h. For examples:
  // - Keyboard                     : Desktop, Keyboard
  // - Mouse                        : Desktop, Mouse
  // - Gamepad                      : Desktop, Gamepad
  // - Consumer Control (Media Key) : Consumer, Consumer Control
  // - System Control (Power key)   : Desktop, System Control
  // - Generic (vendor)             : 0xFFxx, xx
  if ( rpt_info->usage_page == HID_USAGE_PAGE_DESKTOP )
  {
    switch (rpt_info->usage)
    {
      case HID_USAGE_DESKTOP_KEYBOARD:
        TU_LOG1("HID receive keyboard report\r\n");
        // Assume keyboard follow boot report layout
        process_kbd_report( (hid_keyboard_report_t const*) report );
      break;

      case HID_USAGE_DESKTOP_MOUSE:
        TU_LOG1("HID receive mouse report\r\n");
        // Assume mouse follow boot report layout
        process_mouse_report( (hid_mouse_report_t const*) report );
      break;

      default: break;
    }
  }
}
