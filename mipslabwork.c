#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "mipslab.h" /* Declatations for these labs */
#include "drawingFunctions.h"

int mytime = 0x5957;
unsigned int tickValue = 0x1;
volatile int *trise = (volatile int *)0xbf886100;
volatile int *porte = (volatile int *)0xbf886110;
int ticks = 0;
int count = 0;
extern uint8_t screen_data[512];
extern char player_default[88];

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr(void)
{
    if (IFS(0))
  {
    IFSCLR(0) = 0x100;
    count++;
    if (count == 10)
    {
    count = 0;
    ticks++;
    tick(&mytime);
    display_update();
    }
  }
  return;
}

void start(void)
{
  T2CON = 0b111 << 4;                               // sätter prescalree till 256
  PR2 = (80000000 / 256) / 10;                      // sätter period
  TMR2 = 0;                                         // nollar timer 2
  IECSET(0) = 0x00000100;                           // sätter så interupt är enable på timer 2
  IPCSET(2) = 0b11111;                              // sätter prioritet priritet 3 och sub prio 1
  enable_interrupt();                               // sätter interupt global
  T2CONSET = 0x8000;                                // aktiverar timer 2

  *trise = 0x00;
  *porte = 0xff;

  TRISD |= 0x7f;
  count = 0;
  return;
}

void update(void)
{
  *porte = ticks & 0xff;

  char shouldUpdate = 0;

  int switches = getsw();
  int buttons = getbtns();
  if (buttons & 0x1)
  {
    mytime = (mytime & 0xff0f) | (switches << 4); //pretty self explanatory tbh
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

  /*if (IFS(0)) //& 0x080
  {
    IFSCLR(0) = 0xFFFFFFFF; //clear interupt måste cleara alla vet ej varför? ska bara vara en bit egentlien
    count++;
  }
  */
  
  clearScreen();
  drawGraphic(1, 5, 8, 11, player_default);
  display_image(0, screen_data);
/*
  if (count == 10)
  {
    count = 0;
    ticks++;
    tick(&mytime);

    display_update();
  }
  */
}