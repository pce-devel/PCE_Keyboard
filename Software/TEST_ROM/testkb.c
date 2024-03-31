/*
 *
 * testkb - 
 *
 */


#include <huc.h>

#incbin(kana_font, "kana_font.bin");

#define TITLE_LINE       1
#define HEX_LINE         6


char pad;
char mask;

char i;
char j;
char retcode;

char binary[16];
char keyinput[24];
char keyinput_prev[24];

const char scan0[]  = { " 0   0   0   0   0   0   1   0  " }; 
const char scan1[]  = { " 1  Pd6 Pd5 Pd4 Pd3 Pd2 Pd1 Pd0 " };  
const char scan2[]  = { " 1  Pd. Pd, Pd= Pd+ Pd* Pd9 Pd8 " };  
const char scan3[]  = { " 1   F   E   D   C   B   A   @  " };  
const char scan4[]  = { " 1   N   M   L   K   J   I   H  " };  
const char scan5[]  = { " 1   V   U   T   S   R   Q   P  " };  
const char scan6[]  = { " 1   ^   ]  Yen  [   Z   Y   X  " };  
const char scan7[]  = { " 1   6   5   4   3   2   1   0  " };  
const char scan8[]  = { " 1   /   .   ,   ;   :   9   8  " };  
const char scan9[]  = { " 1  N/A KANAGRPHN/A Rt  Up  CLR " };  
const char scan10[] = { " 1  Spc F5  F4  F3  F2  F1  STOP" };  
const char scan11[] = { " 1  Pd/ Pd- HELPCOPYLft Dn  Tab " };  
const char scan12[] = { " 1   G   _   O  N/A N/A RUp RDn " };  
const char scan13[] = { " 1  Ins BkspF10 F9  F8  F7  F6  " };  
const char scan14[] = { " 1   W  Kp7 CtrlWid PC  NCvtCvt " };  
const char scan15[] = { " 1  Esc Del CpLkRSh LSh PdEnRet " };  
const char scan16[] = { " 1   7   -  N/A N/A N/A N/A N/A " };  

char LineText[580];

char CurrLine[50];
char CurrLabel[6];


//
// Read the keyboard is implemented in HuC, but only searches for
// regular joypads.
//
// In order to read a Tsushin Keyboard, it needs to read 18
// consecutive controllers in the same way
//
// In this way, the joypad read routine needed to be replaced
//
//
key_scan()
{
#asm
	lda	#$01
	sta	joyport
	lda	#$03
	sta	joyport
	cly
.rdlp:	lda	#$01
	sta	joyport

	;; delay
	lda	_keyinput,Y	; time-waste, just like in Tsushin ROM
	sta	_keyinput,Y	; time-waste, just like in Tsushin ROM
	;; end delay

	lda	joyport
	stz	joyport
	asl a                   ; this is a necessary function, but also serves as
	asl a                   ; time-waste between write to port and read from port
	asl a
	asl a
	sta	_keyinput,Y
	lda	joyport
	and	#$0F
	ora	_keyinput,Y
	sta	_keyinput,Y
	iny
	cpy	#18
	bcc	.rdlp

#endasm
}

main()
{
   /* set up palette */
   /* Font uses sub-palette #1 for FG, #2 for BG */

   /* palette #0 is default - light green background, bright white foreground */
   set_color_rgb(0, 0,2,0);
   set_color_rgb(1, 7,7,7);
   set_color_rgb(2, 0,2,0);
   
   /* palette #1 is selection/inverse - bright white background, light green foreground */
   set_color_rgb(16, 7,7,7);
   set_color_rgb(17, 0,2,0);
   set_color_rgb(18, 7,7,7);

   /* palette #2 is disabled/dimmed - light green background, dimmed white foreground */
   set_color_rgb(32, 0,2,0);
   set_color_rgb(33, 4,4,4);
   set_color_rgb(34, 0,2,0);

   /* palette #3 is error/red - light green background, bright red foreground */
   set_color_rgb(48, 0,2,0);
   set_color_rgb(49, 7,0,0);
   set_color_rgb(50, 0,2,0);

   /* palette #4 is highlight/yellow - light green background, bright yellow foreground */
   set_color_rgb(64, 0,2,0);
   set_color_rgb(65, 7,7,0);
   set_color_rgb(66, 0,2,0);

   /* palette #5 is highlight/blue-green - light green background, blue-green foreground */
   set_color_rgb(80, 0,2,0);
   set_color_rgb(81, 0,6,7);
   set_color_rgb(82, 0,2,0);

   /* need to reload font with non-zero background color */
   /* (it is preloaded as BG=0 by the bootup process, and 0 is transparent in most palettes) */
   set_font_color(1,2);
   set_font_pal(0);

   /* kana isn't in HuC by default - need to explicitly load */
   load_font(kana_font,128,0xE00);

   /* reload the basic font, same location but different colors */
   set_font_addr(0x800);
   load_default_font();


   set_xres(320);


   /* intialize */

   strcpy(LineText, scan0);
   strcpy(&LineText[32], scan1);
   strcpy(&LineText[64], scan2);
   strcpy(&LineText[96], scan3);
   strcpy(&LineText[128], scan4);
   strcpy(&LineText[160], scan5);
   strcpy(&LineText[192], scan6);
   strcpy(&LineText[224], scan7);
   strcpy(&LineText[256], scan8);
   strcpy(&LineText[288], scan9);
   strcpy(&LineText[320], scan10);
   strcpy(&LineText[352], scan11);
   strcpy(&LineText[384], scan12);
   strcpy(&LineText[416], scan13);
   strcpy(&LineText[448], scan14);
   strcpy(&LineText[480], scan15);
   strcpy(&LineText[512], scan16);
   strcpy(&LineText[544], scan0);

   binary[8] = 0x00;

   /* Display Headings */

   set_font_pal(4);
   put_string("PCE Key/Joy Test", 12, TITLE_LINE);
   put_string("v0.1", 33, TITLE_LINE);
   set_font_pal(0);

   set_font_pal(5);
   put_string("Scan", 1, HEX_LINE - 2);

   for (i = 0; i < 18; i++) {
      put_number(i, 2, 2, HEX_LINE + i);
   }

   put_string("Bit", 20, HEX_LINE - 2);
   for (i = 0; i < 8; i++) {
      put_number((7 - i), 1, 7 + (i * 4), HEX_LINE - 1);
   }

   set_font_pal(0);

   // Main loop
   //
   while (1) {

      for (i = 0; i < 18; i++)
      {
         pad = keyinput[i];

         strncpy(CurrLine, &LineText[(i*32)], 33);

         mask = 0x80;

         for (j = 0; j < 8; j++)
         {
            if ((pad & mask) != 0) {
               set_font_pal(0);
               binary[j] = '1';
            }
            else {
               // highlight pressed keys; disregard zeroes on first
	       // and last lines if Keyboard identifier is found
	       // (i.e. keyinput[0] == 0x02)
               if ((keyinput[0] == 0x02) && ((i == 0) || (i == 17)))
                  set_font_pal(0);
               else
                  set_font_pal(1);
               binary[j] = '0';
            }

            strncpy(CurrLabel, &CurrLine[j*4], 4);
	    CurrLabel[4] = 0x00;

            put_string(CurrLabel, 6 + (j*4), HEX_LINE + i);
            mask = mask >> 1;
         }

      }

      vsync(0);
      key_scan();
   }

   return;
}
