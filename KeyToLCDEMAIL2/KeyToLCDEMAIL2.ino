/* Aquarius Telecom V1.0BETA by COVID-19 */
// Coder writer: Aberope <abel@abelromero.com>.
// Inspired by Malkov.
// In colaboration with HackMadrid and Hacker Village.
// This software is public domain but contains sublicensed software. Pay attention to it.
// Please, read the manual before sending any issue or help question... and never give up.
// Finally, enjoy: this product is a hard effort of one life working for the common good... I've used so many nicknames, posted or asked in so many places but, this is the result.
//
//
//
// CHANGELOG:
// Lun 15:57 26 Oct 2020 -- Added support to PS/2 and able to send/receive commands & responses from SIM800L... waiting for inspiration and sparse time to be able to send an email over GPRS.
//
//
// GREETINGS:
// Thanks mom, you made this possible :-) also... ty dad, you made this also.
// Finally, thanks bro and family... I'LL NEVER GIVE UP!

/* international keyboard layout example to serial port

  Test for keyboard with additional keyboard mapping for UTF-8 and LiquidCrystal
  Only uses ONE key mapping not changeable at run time only at compile time

  Requires PS2KeyAdvanced and PS2KeyMap libraries to work

  IMPORTANT WARNING
 
    If using a DUE or similar board with 3V3 I/O you MUST put a level translator 
    like a Texas Instruments TXS0102 or FET circuit as the signals are 
    Bi-directional (signals transmitted from both ends on same wire).
 
    Failure to do so may damage your Arduino Due or similar board.

  Test History
    September 2014 Uno and Mega 2560 September 2014 using Arduino V1.6.0
    January 2016   Uno, Mega 2560 and Due using Arduino 1.6.7 and Due Board 
                    Manager V1.6.6
    March 2020  Simplify example

  This sketch displays text received from PS2 keyboard to LCD
  on same Arduino. Importantly it maps the keyboard to UTF-8
  codes and allows changes of which keyboard map.

  Allows the cursor movement keys as follows -

    Enter move to beginning of next line and wraps to top
    PGUP and PGDN go to top and bottom row in same column of LCD
    Arrow keys move one line or character and wrap round rows
    Backspace does backspace space backspace sequence
    Home goes to top left
    End goes to bottom right
    Keys TAB DEL ESC and F1 to F12 are displayed as strings

  All typing and some cursor movements wrap to appropriate
  line forwards and backwards

  Example works on any size text LCD, set for 16 x 2
  Change MAX_COLS and MAX_ROWS to match your LCD

  Also example of strings in flash memory

  Map to the keyboard you want in setup with

    keymap.setmap( "UK" );

  or similar currently support for US (default), UK/GB, DE and FR.

  Displayable characters passed through (depends on LCD character set)

 The circuit:
  * LCD RS pin to digital pin 12
  * LCD Enable pin to digital pin 11
  * LCD D4 pin to digital pin 10
  * LCD D5 pin to digital pin 9
  * LCD D6 pin to digital pin 8
  * LCD D7 pin to digital pin 7
  * LCD R/W pin to ground
  * 10K variable resistor (trimmer or potentiometer):
      - ends to +5V and ground
      - wiper to LCD VO pin (pin 3)
  * KBD Clock (PS2 pin 1) to an interrupt pin on Arduino ( this example pin 3 )
  * KBD Data (PS2 pin 5) to a data pin ( this example pin 4 )
  * +5V from Arduino to PS2 pin 1
  * GND from Arduino to PS2 pin 3

  The connector to mate with PS2 keyboard is a 6 pin Female Mini-Din connector
  PS2 Pins to signal
    1       KBD Data
    3       GND
    4       +5V
    5       KBD Clock

   Keyboard has 5V and GND connected see plenty of examples and
   photos around on Arduino site and other sites about the PS2 Connector.

 Interrupts

   Clock pin from PS2 keyboard MUST be connected to an interrupt
   pin, these vary with the different types of Arduino

   For PS2KeyAdvanced you pass this info into begin()

     keyboard.begin( DATAPIN, IRQPIN );

  Valid irq pins:
     Arduino Uno:  2, 3
     Arduino Due:  All pins, except 13 (LED)
     Arduino Mega: 2, 3, 18, 19, 20, 21
     Teensy 2.0:   All pins, except 13 (LED)
     Teensy 2.0:   5, 6, 7, 8
     Teensy 1.0:   0, 1, 2, 3, 4, 6, 7, 16
     Teensy++ 2.0: 0, 1, 2, 3, 18, 19, 36, 37
     Teensy++ 1.0: 0, 1, 2, 3, 18, 19, 36, 37
     Sanguino:     2, 10, 11

  Like the Original library and example this is under LGPL license.

  Written by Paul Carpenter, PC Services <sales@pcserviceselectronics.co.uk>
*/

// include the library code:
#include <PS2KeyAdvanced.h>
#include <PS2KeyMap.h>
#include <LiquidCrystal.h>

/* Keyboard constants  Change to suit your Arduino
   define pins used for data and clock from keyboard */
#define DATAPIN 22
#define IRQPIN  3

/* LCD pins definitions to match your LCD 4 bit mode */
#define RS  8
#define ENA 9
#define RW  10
#define D7   7
#define D6   6
#define D5   5
#define D4   4

/* LCD Constants to match your display  16 x 2 */
/* Columns in display */
#define MAX_COL 16
/* Rows in display */
#define MAX_ROW  2

/* LCD Constants to match your display 20 x 4 */
/* Columns in display */
//#define MAX_COL 20
/* Rows in display */
//#define MAX_ROW 4

/* current cursor position */
signed char cols = 0;
signed char rows = 0;

/*  messages constants */
/* Key codes and strings for keys producing a string */
/* three arrays in same order ( keycode, string to display, length of string ) */
uint8_t codes[] = { PS2_KEY_SPACE, PS2_KEY_TAB, PS2_KEY_ESC,
                          PS2_KEY_DELETE, PS2_KEY_F1, PS2_KEY_F2, PS2_KEY_F3,
                          PS2_KEY_F4, PS2_KEY_F5, PS2_KEY_F6, PS2_KEY_F7,
                          PS2_KEY_F8, PS2_KEY_F9, PS2_KEY_F10, PS2_KEY_F11,
                          PS2_KEY_F12 };
const char *const keys[]  =  { " ", "[Tab]", "[ESC]", "[Del]", "[F1]", "[F2]", "[F3]",
                               "[F4]", "[F5]", "[F6]", "[F7]", "[F8]",
                               "[F9]", "[F10]", "[F11]", "[F12]" };
int8_t sizes[]  = { 1, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5 };


// Class initialisation/instantiation
// keyboard library 
PS2KeyAdvanced keyboard;
// Initialise the keyboard remapping to UTF-8
PS2KeyMap keymap;

// initialize the LCD library with the numbers of the interface pins
// For 4 bit interface first with RW pin grounded on LCD
//LiquidCrystal lcd( RS, ENA, D4, D5, D6, D7 );
LiquidCrystal lcd( RS, RW, ENA, D4, D5, D6, D7 );

void setup()
{
  Serial.begin(9600);
  // set up the LCD's number of columns and rows
  lcd.begin( MAX_COL, MAX_ROW);
  lcd.clear();                      // clear the screen
  //lcd.cursor();                     // Enable Cursor
  lcd.blink();                      // Blinking cursor
  keyboard.begin( DATAPIN, IRQPIN );// Setup keyboard pins
  keyboard.setNoBreak( 1 );         // No break codes for keys (when key released)
  keyboard.setNoRepeat( 1 );        // Don't repeat shift ctrl etc
  keymap.selectMap( (char *)"ES" ); // set which type of keyboard we have
  // Display type of keyboard mapped
  //lcd.setCursor( 13,0 );
  //lcd.print( keymap.getMap( ) );    // display keyboard setting
  //lcd.setCursor( 12,0 );
  //cols = 12;                        // update cursor position
  //rows = 0;
  lcdPrompt();
  Serial1.begin(9600);
  //Serial1.println("ATE0");
}

void check_cursor()
{
if( cols >= MAX_COL )
  {
  cols = 0;
  rows++;
  if( rows >= MAX_ROW )
    rows = 0;
  lcd.setCursor( cols, rows );
  }
}

void lcdPrompt() {
  lcd.clear();
  for (int i = 0; i < MAX_ROW; i++) {
    for (int j = 0; j < MAX_COL; j++) {
      lcd.setCursor(j, i);
      lcd.write('\0');
    }
  }
  cols = rows = 0;
  lcd.setCursor( 0, 0 );
}

void lcdNl() {
  cols = 0;
  rows++;
  if( rows >= MAX_ROW )
     rows = 0;
  lcd.setCursor( cols, rows );
}

void lcdPrint(byte base) {
  check_cursor( );
  cols++;
  lcd.write( base );
  check_cursor( );
}

void loop()
{
  /* mode = 0 echo character
     mode = 1 print string
     mode = 2 cursor movement NO other echo
     mode = 4 ignore key no echo */
  byte mode;
  byte idx;
  byte base;
  int c;
                  
  if( keyboard.available() )
    {
    // read the next key
    if( ( c = keyboard.read() ) )
      {
      // check for some of the special keys
      mode = 2;
      base = c & 0xFF;
      switch( base )          /* Cursor movements */
        {
        case PS2_KEY_ENTER:           /* Cursor to beginning of next line or start */
        case PS2_KEY_KP_ENTER:
                  if (c & PS2_SHIFT) {
                    //Serial.println(c);
                    for(rows = 0; rows < MAX_ROW; rows++) {
                      for (cols = 0; cols < MAX_COL; cols++) {
                        //Serial.println(rows, DEC);
                        //Serial.println(cols, DEC);
                        lcd.setCursor(cols, rows);
                        char c2 = lcd.read();
                        if (c2 != 0) {
                          Serial1.write(c2);
                          //Serial.println(c2, HEX);
                        }
                      }
                    }
                    Serial1.println();
                    //Serial.println();
                  }
                  lcdNl();
                  break;
        case PS2_KEY_PGDN:            /* Cursor to top row current column */
                  rows = MAX_ROW - 1;
                  break;
        case PS2_KEY_PGUP:            /* Cursor to bottom row current column */
                  rows = 0;
                  break;
        case PS2_KEY_L_ARROW:         /* Cursor left or end of previous line */
                  cols--;
                  if( cols < 0 )
                    {
                    cols = MAX_COL - 1;
                    rows--;
                    if( rows < 0 )
                      rows = MAX_ROW - 1;
                    }
                  break;
        case PS2_KEY_R_ARROW:         /* Cursor right or start of next line */
                  cols++;
                  if( cols >= MAX_COL )
                    {
                    cols = 0;
                    rows++;
                    if( rows >= MAX_COL )
                      rows = 0;
                    }
                  break;
        case PS2_KEY_UP_ARROW:    /* Cursor up one line no wrap */
                  rows--;
                  if( rows < 0 )
                     rows = 0;
                  break;
        case PS2_KEY_DN_ARROW:    /* Cursor down one line no wrap */
                  rows++;
                  if( rows >= MAX_ROW )
                     rows = MAX_ROW - 1;
                  break;
        case PS2_KEY_BS:      /* Move cursor back write space move cursor back */
                  cols--;
                  if( cols < 0 )
                    {
                    cols = MAX_COL - 1;
                    rows--;
                    if( rows < 0 )
                       rows = MAX_ROW - 1;
                    }
                  lcd.setCursor( cols, rows );
                  lcd.write( '\0' );
                  break;
        case PS2_KEY_HOME:        /* Cursor to top left */
                  cols = 0;
                  rows = 0;
                  break;
        case PS2_KEY_END:         /* Cursor to max position */
                  cols = MAX_COL - 1;
                  rows = MAX_ROW - 1;
                  break;
        case PS2_KEY_ESC:
                lcdPrompt();
                break;
        default:  /* Not cursor movement */
                  mode = 0;
        }
        /* if was cursor movement do last movement */
        if( mode == 2 )
          lcd.setCursor( cols, rows );
        else
          {
          /* Check for strings or single character to display */
          if( base != PS2_KEY_EUROPE2 && ( base < PS2_KEY_KP0 || base >= PS2_KEY_F1 ) )
            {  // Non printable sort which ones we can print
            for( idx = 0; idx < sizeof( codes ); idx++ )
              if( base == codes[ idx ] )
                {
                /* String outputs */
                mode = 1;
                c = sizes[ idx ];
                cols += c - 1;
                check_cursor( );
                /* when cursor reset keep track */
                if( cols == 0 )
                  cols = c;
                lcd.print( keys[ idx ] );
                cols++;
                check_cursor( );
                break;
                }
            /* if not found a string ignore key cant do anything */
            }
          else
            {  /* Supported key */
            if( ( base = keymap.remapKey( c ) ) > 0 )
              {
              lcdPrint(base);
              }
            }
          }
        }
    delay( 100 );
    }

    if (Serial1.available()) {
      base = Serial1.read();
      Serial.write(base);
      if (base != '\r') {
        if (base == '\n') {
          //Serial.println("NL");
          lcdNl();
        } else {
          lcdPrint(base);
        }
      }
    }
}
