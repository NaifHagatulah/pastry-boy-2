/* mipslabwork.c
   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall
   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):
   This file modified 2022-09-28 by Adam Tovatt
   For copyright and licensing, see file COPYING */

#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "mipslab.h" /* Declatations for these labs */

int mytime = 0x5957;
unsigned int tickValue = 0x1;
volatile int *trise = (volatile int *)0xbf886100;
volatile int *porte = (volatile int *)0xbf886110;
int ticks = 0;
int count = 0;

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr(void)
{
  return;
}

/* Lab-specific initialization goes here */
void labinit(void)
{
  T2CON = 0b111 << 4;     // sätter prescale till 256
  PR2 = 31250;            // sätter period
  TMR2 = 0;               // nollar timer 2
  IECSET(0) = 0x00000100; // sätter så interupt är enable på timer 2
  IPCSET(0) = 0b11111;    // sätter prioritet
  T2CONSET = 0x8000;      // aktiverar timer 2

  *trise = 0x00;
  *porte = 0xff;

  TRISD |= 0x7f;

  return;
}

uint8_t screen_data[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
};

char playerDefault[] = 
{ 
    0, 0, 0, 1, 1, 0, 0, 0,
    0, 0, 1, 0, 0, 1, 0, 0,
    0, 0, 1, 0, 0, 1, 0, 0,
    0, 0, 0, 1, 1, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 1, 1, 1, 1, 0, 1,
    1, 0, 1, 1, 1, 1, 0, 1,
    0, 0, 1, 1, 1, 1, 0, 0,
    0, 0, 1, 0, 0, 1, 0, 0,
    0, 0, 1, 0, 0, 1, 0, 0,
    0, 0, 1, 1, 0, 1, 1, 0, 
};

void setPixel(int x, int y, char value)
{
  if(x < 0 || x > 127 || y < 0 || y > 31)
    return;

  y = 31 - y;
	int blockPosition = x + ((y / 8) * 128);
	char block = screen_data[blockPosition];

	block &= ~(1 << (y % 8));

  if(value > 0)
  {
    block |= 1 << (y % 8);
  }

	screen_data[blockPosition] = block;
}

void clearScreen()
{
  int i;
  for (i = 0; i < 4096; i++)
  {
    screen_data[i] = 0;
  }
}

void drawGraphic(int x, int y, int width, int height, char* graphic)
{
  int i;
  int j;
  for (i = 0; i < width; i++)
  {
    for (j = 0; j < height; j++)
    {
      setPixel(x + i, (y + height) - j, graphic[i + j * (width)]);
    }  
  } 
}

/* This function is called repetitively from the main program */
void labwork(void)
{
  // delay(1000);
  time2string(textstring, mytime);
  // tick(&mytime);

  *porte = ticks & 0xff;

  char shouldUpdate = 0;

  int switches = getsw();
  int buttons = getbtns();
  if (buttons & 0x1)
  {
    mytime = (mytime & 0xff0f) | (switches << 4); // pretty self explanatory tbh
    shouldUpdate = 1;
  }

  if (buttons & 0x2)
  {
    mytime = (mytime & 0xf0ff) | (switches << 8);
    shouldUpdate = 1;
  }

  if (buttons & 0x4)
  {
    mytime = (mytime & 0x0fff) | (switches << 12);
    shouldUpdate = 1;
  }

  if (IFS(0)) //& 0x080
  {
    IFSCLR(0) = 0xFFFFFFFF; // clear interupt måste cleara alla vet ej varför? ska bara vara en bit egentlien
    count++;
  }
  
  clearScreen();
  drawGraphic(ticks, 5, 8, 11, playerDefault);
  display_image(0, screen_data);

  if (count == 10)
  {
    count = 0;
    ticks++;
    tick(&mytime);

    display_update();
  }
}