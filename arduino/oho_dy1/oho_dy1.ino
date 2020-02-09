/*
 * Usage example for 7-led segment 3-digits display
 * OHO Elektronik 
 * model OHO DY1, distributed by Trenz Electronics
 * see connection diagram on Github
 * (c)2020 Giovanni Bernardo (CyB3rn0id)
 * https://www.settorezero.com
 */

// Arduino pins attached to shift registers
#define SR_DAT  2
#define SR_CLK  3
#define SR_LAT  4

// display segments as attached on shift registers bits
// on display OHO_DY1
#define SEG_A 0b00010000  
#define SEG_B 0b00000010  
#define SEG_C 0b00001000  
#define SEG_D 0b01000000  
#define SEG_E 0b10000000  
#define SEG_F 0b00000001 
#define SEG_G 0b00100000  
#define SEG_P 0b00000100  // decimal point

// digits definition
// OHO_DY1 display is common anode: 1=segment OFF, 0=segment on that's why I've put ~
// segments to be turned on for display numbers and simbols
// I've put ~ because display used is common anode, so 1=segment off and 0=segment on
uint8_t digits[]=
  {                                                 //  index value=digit showed on display
  ~(SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F),           //  0=0
  ~(SEG_B|SEG_C),                                   //  1=1
  ~(SEG_A|SEG_B|SEG_D|SEG_E|SEG_G),                 //  2=2
  ~(SEG_A|SEG_B|SEG_C|SEG_D|SEG_G),                 //  3=3
  ~(SEG_B|SEG_C|SEG_F|SEG_G),                       //  4=4
  ~(SEG_A|SEG_C|SEG_D|SEG_F|SEG_G),                 //  5=5
  ~(SEG_A|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G),           //  6=6
  ~(SEG_A|SEG_B|SEG_C),                             //  7=7
  ~(SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G),     //  8=8
  ~(SEG_A|SEG_B|SEG_C|SEG_D|SEG_F|SEG_G),           //  9=9
  ~(SEG_A|SEG_B|SEG_C|SEG_E|SEG_F|SEG_G),           // 10=A
  ~(SEG_C|SEG_D|SEG_E|SEG_F|SEG_G),                 // 11=b
  ~(SEG_A|SEG_D|SEG_E|SEG_F),                       // 12=C
  ~(SEG_B|SEG_C|SEG_D|SEG_E|SEG_G),                 // 13=d
  ~(SEG_A|SEG_D|SEG_E|SEG_F|SEG_G),                 // 14=E
  ~(SEG_A|SEG_E|SEG_F|SEG_G),                       // 15=F
  ~(0)                                              // 16=all segments OFF
  };

// mnemonic to array index for non-numbers
#define DIG_A 10
#define DIG_B 11
#define DIG_C 12
#define DIG_D 13
#define DIG_E 14
#define DIG_F 15
#define DIG_OFF 16

// digits in ram
uint8_t display_values[3];

void setup() 
  { 
  pinMode(SR_DAT, OUTPUT);
  pinMode(SR_CLK, OUTPUT);
  pinMode(SR_LAT, OUTPUT);
  digitalWrite(SR_DAT, LOW);
  digitalWrite(SR_CLK, LOW);
  digitalWrite(SR_LAT, LOW);
  }


void loop() 
  {
  // Shows AbC
  display_set(DIG_A, DIG_B, DIG_C);
  delay(1000);
  
  // Shows dEF
  display_set(DIG_D, DIG_E, DIG_F);
  delay(1000);
  
  // cancel display
  display_reset();
  delay(1000);
  
  // shows numbers from 0 to 254
  for (uint8_t i=0; i<255; i++)
    {
     display_write(i);
     delay(100); 
    }
  }

// *****************************************************************************************
// Display functions
// *****************************************************************************************
// set single digits to passed values
// example:
// you want to show 1A9 on display
// you write: display_set(1, DIG_A, 9);
// or: display_set(1, 10, 9) since DIG_A=10 and the digits array index 10 shows the A letter
void display_set(uint8_t v1, uint8_t v2, uint8_t v3)
  {
  display_values[2]=digits[v1];
  display_values[1]=digits[v2];  
  display_values[0]=digits[v3];  
  display_update();
  }

// reset display
void display_reset(void)
  {
  display_set(DIG_OFF, DIG_OFF, DIG_OFF); 
  }
  
// writes an unsigned integer to display
void display_write(uint8_t num)
  {
  uint8_t digit1=DIG_OFF;
  uint8_t digit2=DIG_OFF;
  uint8_t digit3=DIG_OFF;
  
  if (num<10)
    {
    digit3=num;  
    }
  else if (num<100)
    {
    digit3=num%10;
    digit2=num/10;  
    }
  else
    {
    digit3=num%10;
    digit2=(num%100)/10;
    digit1=num/100;  
    }
  display_set(digit1, digit2, digit3);
  }

// transfer datas from array in ram to shift registers
void display_update(void)
  {
  // send data to shift registers from last number to first number
  for (int8_t i=2; i>=0; i--) 
    {
    shiftOut(SR_DAT, SR_CLK, MSBFIRST, display_values[i]);
    }
  // strobe signal on latch pin
  digitalWrite(SR_LAT, HIGH); 
  digitalWrite(SR_LAT, LOW); 
  }
