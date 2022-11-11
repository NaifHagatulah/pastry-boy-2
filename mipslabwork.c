/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog

   For copyright and licensing, see file COPYING */

#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "mipslab.h"
volatile int *p_led = (volatile int *)0xbf886110;
int counter;
int mytime = 0x5957;
 extern  uint8_t  icon[512];

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr(void)
{
  return;
}

/* Lab-specific initialization goes here */
void labinit(void)
{
  /*
  3 stycken funktioner för timer finns för varje timer.
  funktion 3
  TMRx  hold current count
  PRx period register när den ska rulla över
  txcon   kontrollerar hur den är Prescaller
  count = TMR2
  PR2 = 20000; ska bli noll efter 20000 räkingar! vi ska ha 4 000 000 för få 1 per sekund
  Timer 1 prescaler: 1,8,64,256

  */

  // PR2 = 9000; // de blir 1 sek om de körs 16 miljoner ggr

  T2CON = 0b111 << 4;          // sätter prescalree till 256
  PR2 = (80000000 / 256) / 10; // sätter period
  TMR2 = 0;                    // nollar timer 2
  IECSET(0) = 0x00000100;      // sätter så interupt är enable på timer 2
  IPCSET(0) = 0b11111;         // sätter prioritet
  T2CONSET = 0x8000;           // aktiverar timer 2

  volatile int *trise = (volatile int *)0xbf886100; // make a pointer to adress were led are initilized 0 for output we need zero
  *trise = 0;                                       // make sure that the leds are set as output (0)

  TRISDSET = 0x0ff0; // make sure that btns/sw are input leave the others same

  counter = 0;
  return;
}

/* This function is called repetitively from the main program */
void labwork(void)
{
  int switches = getsw();
  int button = getbtns();
  if (*p_led & 0xffff00) // if we are abow index 7 we dont know what happens if we switch between 0-1
  {
    *p_led = 0x00; // reset incase we come abow index 7
  }

  if (button & 0x02) // kontrollerar om någon av de tre knapparna trycks
  {
    mytime &= 0xff0f;          // nollar minuterna
    mytime |= (switches) << 4; // sätter in getsw() in i minuterna allt annat förblir densamma då de bara är nollor
  }
  if (button & 0x04) // kontrollerar om någon av de tre knapparna trycks
  {
    mytime &= 0xf0ff;          // reset the number connected to the btn
    mytime |= (switches) << 8; // insert switch value to time
  }
  if (button & 0x08) // kontrollerar om någon av de tre knapparna trycks
  {
    mytime &= 0x0fff;
    mytime |= (switches) << 12;
  }
  display_image(0, icon);

  if (IFS(0) & 0xFFFFFFFF)
  {
    IFSCLR(0) = 0xFFFFFFFF; // clear interupt måste cleara alla vet ej varför? ska bara vara en bit egentlien
    counter++;
  }
  if (counter == 10)
  {
    counter = 0;
    time2string(textstring, mytime);
    //display_string(3, textstring);
    display_update();
    tick(&mytime);
    *p_led = *p_led + 0x1; // lägger till 1 dit led pekar dvs ökar så en till lampa lyser index 0-7 är lampor
  }
}
