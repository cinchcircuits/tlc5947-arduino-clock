// Nixie_num_show
// By: Michael Gregg, Mike Pechner
// Email: mgregg@michaelgregg.com
// Email: mikey@mikey.com
// 
//
// Designed for use with tlc5947's controlling nixie tubes. 
//
#include "Tlc5947.h"

#define DAYVAL 3000 // max on brightness of tubes.
#define NIGHTVAL 800 // Optional value that the digits run at over night.
#define MAXDIGIT 4 // the number of digits in this clock

int MAXVAL = 3500; // current max brightness

int digit,val;
char buff[100];
byte digitArray[MAXDIGIT];
byte newDigitArray[MAXDIGIT];
int x;

void setup () 
{
  Serial.begin(57600);
  Serial.print("num_tlcs is ");
  Serial.println(NUM_TLCS);
  Tlc.init(); // start the led controllers
  for ( x=0;x<MAXDIGIT;x++){
    digitArray[x] = 0xff; // 0xff means off
  }
}
void StartupSeq()
{
  int digit,val;
  for (val=0; val<=11; val++)
  {
    Tlc.clear();
    int showNum = 0;
    for (digit=0;digit<MAXDIGIT;digit++)
    {
      showNum = val + digit;
      if ( showNum >11) showNum = showNum - 11;
      sprintf(buff,"setting digit %d to %d", digit, showNum);
      Serial.println(buff);
      
      onNum(showNum,digit,NIGHTVAL);
    }
    Tlc.update();
    delay(2000);
  }
}
void loop()
{
  Tlc.clear();  
 
 StartupSeq();
 delay(2000);
}

// Set a new digit on the tlc5940 array to MAXVAL
// be sure to use TLC.update() after this to enable the outputs.
void onNum(int inval, int digit, int brightness)
{

    Tlc.set((12*digit)+inval, brightness);
  
//  digitArray[digit] = inval; // set the digitArray value to whatever it should be for this digit. 
}
// transistions the numbers to the new states if they have changed.
// transition the current on digits to 0, and new new on digits to MAXVAL
void goNums()
{
  int x,y;
  int newD=0;
  for (x=0;x<MAXDIGIT;x++)
  {
    if ( newDigitArray[x] != digitArray[x] )
    { newD=1;break; } // if there is a difference, note it and break
  }
  if ( newD == 0 ) { return; } // If nothing has changed, return to top
    
  // Now, transition to the new digit
  for (y=MAXVAL;y>=0;y=y-3)
  {
    Tlc.clear();
    for (x=0;x<MAXDIGIT;x++)
    {
      if ( newDigitArray[x] == digitArray[x] )
      {
        onNum(digitArray[x],x,MAXVAL); // If the digits have not changed, leave the uncanged digit on
      } else {
        onNum(digitArray[x],x,y); // Otherwise set the new digit down a bit.
      }
    }
    
    Tlc.update();    
  }
    
  for (y=0;y<=MAXVAL;y=y+3)
  {
    Tlc.clear();
    for (x=0;x<MAXDIGIT;x++)
    {
      if ( newDigitArray[x] == digitArray[x] )
      {
        if ( newDigitArray[x] != 0xff ) // 0xff means the digit should be off.
        {
          onNum(digitArray[x],x,MAXVAL); // If the digits have not changed, leave the uncanged digit on
        }
      } else {
                if ( newDigitArray[x] != 0xff ) // 0xff means the digit should be off.
        {
          onNum(newDigitArray[x],x,y); // Otherwise set the new digit up a bit.
        }
      }
    }
    Tlc.update();    
  }
  for (x=0;x<MAXDIGIT;x++)
  {
    digitArray[x] = newDigitArray[x]; // Update the current digits to match the set digits.
  }
}






