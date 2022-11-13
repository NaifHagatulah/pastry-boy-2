#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "mipslab.h" /* Declatations for these labs */
#include "drawingFunctions.h"
#include "GameObject.h"

unsigned int tickValue = 0x1;
volatile int *trise = (volatile int *)0xbf886100;
volatile int *porte = (volatile int *)0xbf886110;
int ticks = 0;
int displayUpdateCounter = 0;
extern uint8_t screen_data[512];
extern char player_default[88];
GameObject player;

GameObject gameObjects[1];

void user_isr(void)
{
  /*if (IFS(0))
  {
    IFSCLR(0) = 0x100;
    count++;
    if (count == 10)
    {
    count = 0;
    ticks++;
    tick(&mytime);
    }
    
  }*/
  return;
}

void start(void)
{
  T2CON = 0b111 << 4;                               // sätter prescale till 256
  PR2 = (80000000 / 256) / 100;                     // sätter period
  TMR2 = 0;                                         // nollar timer 2
  IECSET(0) = 0x00000100;                           // sätter så interupt är enable på timer 2
  IPCSET(0) = 0b11111;                              // sätter prioritet priritet 3 och sub prio 1
  enable_interrupt();                               // sätter interupt global
  T2CONSET = 0x8000;                                // aktiverar timer 2

  *trise = 0x00;
  *porte = 0xff;

  TRISD |= 0x7f;
  TRISF |= 0x2;

  player.xPosition = 5;
  player.yPosition = 5;
  player.graphicIndex = 0;

  //gameObjects[0] = player;
}

void game_update(void) //will run every time the timer ticks
{
  int buttons = getbtns();
  int btn1 = getbtn1();

  if(btn1 & 0x2)
  {
    player.xPosition += 0.7;
  }

  if (buttons & 0x1) //button 1 is pressed
  {
    player.xPosition += 0.02;   
  }

  if (buttons & 0x2) //button 2 is pressed
  {
    player.xPosition -= 0.02;   
  }

  if (buttons & 0x4) //button 3 is pressed
  {
    
  }
}

void draw_update(void) //will run every 100th time timer ticks
{
  displayUpdateCounter = 0;
  display_image(0, screen_data); //draw image to screen
}

void master_update(void)
{
  if(!IFS(0)) //only update on timer
    return;

  displayUpdateCounter++;
  ticks++;
  game_update();

  IFSCLR(0) = 0x100;
  
  clearScreen();
  drawGameObject(player);
  //drawGraphic((int)player.xPosition, 5, 8, 11, player_default);  

  if (displayUpdateCounter == 100)
  {
    draw_update();
  }
}