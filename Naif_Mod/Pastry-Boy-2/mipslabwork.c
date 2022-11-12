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
  if (IFS(0))
  {
    IFSCLR(0) = 0x100;
    counter++;
   
    if (counter == 10)
    {
       display_update();
      *p_led = *p_led + 0x1;
      setPixel(*p_led,*p_led,0);
      display_image(0, icon);

      counter = 0;
    }
  }
  return;
}

/* Lab-specific initialization goes here */
void labinit(void)
{

  T2CON = 0b111 << 4;                               // sätter prescalree till 256
  PR2 = (80000000 / 256) / 10;                      // sätter period
  TMR2 = 0;                                         // nollar timer 2
  IECSET(0) = 0x00000100;                           // sätter så interupt är enable på timer 2
  IPCSET(2) = 0b11111;                              // sätter prioritet priritet 3 och sub prio 1
  enable_interrupt();                               // sätter interupt global
  T2CONSET = 0x8000;                                // aktiverar timer 2
  volatile int *trise = (volatile int *)0xbf886100; // make a pointer to adress were led are initilized 0 for output we need zero
  *trise = 0;                                       // make sure that the leds are set as output (0)

  TRISDSET = 0x0ff0; // make sure that btns/sw are input leave the others same

  counter = 0;
  return;
}
void setPixel(int x, int y, char value)
{
    int blockPosition = x + ((y / 8) * 128);
    char block = icon[blockPosition];
    block |= 1 << (y % 8);
    icon[blockPosition] = block;
}
/* This function is called repetitively from the main program */
void labwork(void)
{
  int i = 0;
  while (i<512)
  {
    
    icon[i] = 0;
    i++;
  }
 
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
    setPixel(1,1,0);
    mytime |= (switches) << 12;
  }
  
     // lägger till 1 dit led pekar dvs ökar så en till lampa lyser index 0-7 är lampor
  
  
}
