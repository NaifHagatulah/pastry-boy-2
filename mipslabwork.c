#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "mipslab.h" /* Declatations for these labs */
#include "drawingFunctions.h"
#include "Collision.h"
#include "mapFunctions.h"

const double JUMP_FORCE = 0.3;
const double GRAVITY_FORCE = 0.008;

unsigned int tickValue = 0x1;
volatile int *trise = (volatile int *)0xbf886100;
volatile int *porte = (volatile int *)0xbf886110;

int ticks = 0;
int gameCounter = 1;
int displayUpdateCounter = 0;
double timeScale = 1;
int lastGameCounter = 0;
int lastDisplay = 0;
double deltaTime = 1;

extern uint8_t screen_data[512];
extern char player_default[88];
extern char baby_block_01[32];
extern char* images[2];
extern uint8_t imageSizes[][2];

GameObject *player;
GameObject gameObjects[32];
int gameObjectsLength;
int currentScene;
int currentLevel;

Collision collisions[100];
int collisionsLength;

void apply_velocities();
void find_collisions();
void handle_collisions();
void apply_gravity();
char get_collision_side(Collision *collision);
void die();
void go_to_next_scene();
void go_to_previous_scene();
void game_update();
void draw_objects();
void get_int_as_string(int value, char string[], int length);

void user_isr(void)
{
  if (IFS(0) & 0x00010000) //clock
  {
    displayUpdateCounter++;

    IFSCLR(0) = 0x00010000;
    if (displayUpdateCounter % 20 == 0)
    {
      int deltaGameCounter = gameCounter - lastGameCounter;
      timeScale = deltaGameCounter / 150.0;
      lastGameCounter = gameCounter;
      
      int test = (int)(deltaGameCounter>>4);

      apply_gravity();
      find_collisions();
      handle_collisions();
      game_update();
      apply_velocities();

      clear_screen(); //clear screen to make it ready for drawing
      draw_objects(); //draw gameobjects

      char string[10] = {0};
      get_int_as_string((int)(timeScale * 100.0), string, sizeof(string));
      draw_string(1, 26, string, 10);

      char string2[10] = {0};
      get_int_as_string(gameCounter, string2, sizeof(string2));
      draw_string(1, 21, string2, 10);

      char string3[10] = {0};
      get_int_as_string(displayUpdateCounter, string3, sizeof(string3));
      draw_string(1, 14, string3, 10);

      display_image(0, screen_data); //display the image on the screen
    }
  }
  
  if (IFS(0) & 0x100) //master updates
  {
    IFSCLR(0) = 0x100;
    gameCounter++;
  }

  return;
}

void get_int_as_string(int value, char string[], int length)
{
  int i = 0;
  int input = value;
  while(input)
  {
    string[length - 1 - i] = '0' + input % 10;
    input /= 10;
    i++;

    if(i > length)
      break;
  }  
}

void load_scene(int level, int scene)
{ 
  currentLevel = level;
  currentScene = scene;

  gameObjectsLength = get_level_scene_length(level, scene);

  load_level_scene(gameObjects, level, scene);
  player = &gameObjects[0];

  clear_background();
  int i;
  for (i = 0; i < gameObjectsLength; i++) //render to background
  {
    if(i != 0 && gameObjects[i].graphicIndex == 0) break;
    
    if(gameObjects[i].graphicIndex == 1)
      draw_game_object(&gameObjects[i], 1);
  }
}

void start(void)
{
  T2CON = 0b111 << 4;     // sätter prescale till 256
  PR2 = 30;               //(8000000 / 256) / 1000;            // sätter period
  TMR2 = 0;               // nollar timer 2
  IECSET(0) = 0x00000100;      // sätter så interupt är enable på timer 2
  IPCSET(2) = 0x0000000C;    // sätter prioritet

  //T4CON = 0x0; // Stop 16-bit Timer4 and clear control register
  //T5CON = 0x0; // Stop 16-bit Timer5 and clear control register
  //T4CONSET = 0x0038; // Enable 32-bit mode, prescaler at 1:8

  T4CON = 0b111 << 4;     // sätter prescale till 256
  TMR4 = 0; // Clear contents of the TMR4 and TMR5
  PR4 = 30; // Load PR4 and PR5 registers with 32-bit value
  IPCSET(5) = 0x00000004; // Set priority level=1 and
  IPCSET(5) = 0x00000001; // Set sub-priority level=1
  // Could have also done this in single
  // operation by assigning IPC5SET = 0x00000005
  IFSCLR(0) = 0x00010000; // Clear the Timer5 interrupt status flag
  IECSET(0) = 0x00010000; // Enable Timer5 interrupts

  enable_interrupt();

  T4CONSET = 0x8000; // Start Timer
  T2CONSET = 0x8000; // aktiverar timer 2

  *trise = 0x00;

  TRISD |= 0x7f;

  load_scene(currentLevel, currentScene);
}

void game_update() //will run every time the timer ticks
{
  int buttons = getbtns();
  int btn1 = getbtn1();

  if(btn1 & 0x2) //button 4
  {
    //jump,
    load_scene(0,1);
  }

  if (buttons & 0x1) //button 3 is pressed
  {
    if(player->grounded == 1)
    {
      player->yPosition += 1;
      player->yVelocity += JUMP_FORCE;
    }
  }

  if (buttons & 0x2) //button 2 is pressed
  {
    player->xPosition += 0.1; // timeScale; 
    player->is_mirrored = 0;
  }

  if (buttons & 0x4) //button 1 is pressed
  {
    player->xPosition -= 0.1; // timeScale;   
    player->is_mirrored = 1;
  }

  if(player->yPosition < -8)
  {
    die();
  }

  //*porte = currentScene + 1;

  if(currentScene == 0 && player->xPosition <= 0)
  {
    player->xPosition = 0;
  }

  if(player->xPosition > 128)
  {
    go_to_next_scene();
  }
  else if(player->xPosition < -8)
  {
    go_to_previous_scene();
  }
}

void draw_objects(void) //will run every 100th time timer ticks
{
  int i;
  for (i = 0; i < gameObjectsLength; i++)
  {
    if(i != 0 && gameObjects[i].graphicIndex == 0) break;
    
    if(gameObjects[i].type != 2) //don't draw static blocks
    {
      if(gameObjects[i].disabled == 0)
        draw_game_object(&gameObjects[i], 0);
    }
  }

  if(displayUpdateCounter == 2000000)
  {
    displayUpdateCounter = 0; 
  }
}

void master_update(void)
{
  /*
  if(IFS(0)) //only update on timer
    return;

  if(((IFS(0)>>2) && 1) == 1)
  {
    displayUpdateCounter++;
    ticks++;
  }

  
  if ((displayUpdateCounter % 100) == 0 && ((IFS(0)>>2) && 1) == 1)
  {
    IFSCLR(0) = 0x00000100;
    IECSET(0) = 0x00000100;
    apply_gravity();
    find_collisions();
    handle_collisions();
    game_update();
    apply_velocities();
    draw_update();
  }*/
}

void apply_gravity()
{
  int index = 0;
  for(index = 0; index < gameObjectsLength; index++)
  {
    if(gameObjects[index].usePhysics == 1)
    {
      gameObjects[index].grounded = 0; //reset grounded every frame
      gameObjects[index].yVelocity -= GRAVITY_FORCE;// / timeScale; //apply gravity
    }
  }
}

void apply_velocities()
{
  int index = 0;
  for(index = 0; index < gameObjectsLength; index++)
  {
    if(gameObjects[index].usePhysics == 1 && gameObjects[index].disabled == 0)
    {
      gameObjects[index].xPosition += gameObjects[index].xVelocity;// / timeScale; //move object with x velocity
      gameObjects[index].yPosition += gameObjects[index].yVelocity;// / timeScale; //move object with y velocity
    }
  }
}

void find_collisions()
{
  collisionsLength = 0;
  
  int i = 0;
  for(i = 0; i < gameObjectsLength; i++)
  {
    if(gameObjects[i].usePhysics == 1 && gameObjects[i].disabled == 0)
    {    
      int j = 0;
      for(j = 0; j < gameObjectsLength; j++)
      {
        if(i != j && gameObjects[j].disabled == 0)
          collisionsLength += get_collision(&gameObjects[i], &gameObjects[j], &collisions[collisionsLength]);
      }
    }
  }
}

void go_to_next_scene()
{
  int yPosition = player->yPosition;
  int yVelocity = player->yVelocity;
  load_scene(0, 1);
  player->yPosition = yPosition;
  player->xPosition = 2;
  player->yVelocity = yVelocity;
}

void go_to_previous_scene()
{
  int yPosition = player->yPosition;
  int yVelocity = player->yVelocity;
  load_scene(0, 0);
  player->yPosition = yPosition;
  player->xPosition = 110;
  player->yVelocity = yVelocity;
}

void die()
{
  player->xPosition = 2;
  player->yPosition = 5;
  player->yVelocity = 0;
}

double abs(double value)
{
  if(value < 0)
    return -value;
}

int get_collision(GameObject *object1, GameObject *object2, Collision *collision)
{
  double thisLeft = object1->xPosition;
  double otherLeft = object2->xPosition;
  
  //double difference = thisLeft - otherLeft;
  //if(abs(difference) > 10)
  //  return 0;

  double thisRight = object1->xPosition + get_game_object_width(object1);
  double thisTop = object1->yPosition + get_game_object_height(object1);
  double thisBottom = object1->yPosition;

  double otherRight = object2->xPosition + get_game_object_width(object2);
  double otherTop = object2->yPosition + get_game_object_height(object2);
  double otherBottom = object2->yPosition;
  
  if (otherLeft >= thisLeft && otherLeft <= thisRight && otherRight >= thisRight)
  {
      double xOverlap = thisRight - otherLeft;
      if (otherBottom >= thisBottom && otherBottom <= thisTop)
      {
          double yOverlap = thisTop - otherBottom;
          collision->xPosition = otherLeft;
          collision->yPosition = otherBottom;
          collision->width = xOverlap;
          collision->height = yOverlap;
          collision->objectOne = object1;
          collision->objectTwo = object2;
          return 1;
      }
      else if (otherTop >= thisBottom && otherTop <= thisTop)
      {
          double yOverlap = otherTop - thisBottom;
          collision->xPosition = otherLeft;
          collision->yPosition = thisBottom;
          collision->width = xOverlap;
          collision->height = yOverlap;
          collision->objectOne = object1;
          collision->objectTwo = object2;
          return 1;
      }
  }
  else if (otherRight >= thisLeft && otherRight <= thisRight)
  {
      double xOverlap = otherRight - thisLeft;
      if (otherTop >= thisBottom && otherTop <= thisTop)
      {
          double yOverlap = otherTop - thisBottom;
          collision->xPosition = thisLeft;
          collision->yPosition = thisBottom;
          collision->width = xOverlap;
          collision->height = yOverlap;
          collision->objectOne = object1;
          collision->objectTwo = object2;
          return 1;
      }
      else if (otherBottom >= thisBottom && otherBottom <= thisTop)
      {
          double yOverlap = thisTop - otherBottom;
          collision->xPosition = thisLeft;
          collision->yPosition = otherBottom;
          collision->width = xOverlap;
          collision->height = yOverlap;
          collision->objectOne = object1;
          collision->objectTwo = object2;
          return 1;
      }
  }
  else if (thisLeft >= otherLeft && thisRight <= otherRight)
  {
      if (thisTop <= otherTop)
      {
          if(thisBottom >= otherBottom) 
          {
            collision->xPosition = thisLeft;
            collision->yPosition = otherBottom;
            collision->width = 3;
            collision->height = 2;
            collision->objectOne = object1;
            collision->objectTwo = object2;
            return 1;
          }
      }
  }
  else if (otherLeft >= thisLeft  && otherRight <= thisRight)
  {
      if (otherTop <= thisTop)
      {
          if (otherBottom >= thisBottom)
          {
            collision->xPosition = thisLeft;
            collision->yPosition = otherBottom;
            collision->width = 3;
            collision->height = 2;
            collision->objectOne = object1;
            collision->objectTwo = object2;
            return 1;
          }
      }
  }
  
  return 0;
}

void handle_dog_side_collision(Collision *collision)
{
  if(collision->objectTwo->type == 1)
  {
    collision->objectTwo->xPosition = 5;
    collision->objectTwo->yPosition = 8;
    collision->objectOne->disabled = 1;
  }
  else
  {
    collision->objectOne->xVelocity = -collision->objectOne->xVelocity;

    if(collision->objectOne->xVelocity < 0)
      collision->objectOne->is_mirrored = 0;
    else
      collision->objectOne->is_mirrored = 1;
  }
}

void handle_collisions()
{
  int i = 0;
  for (i = 0; i < collisionsLength; i++)
  {
    if(collisions[i].objectOne->usePhysics == 1)
    {
      char side = get_collision_side(&collisions[i]);

      if(side == 4)
      {
        collisions[i].objectOne->yVelocity = 0;
        collisions[i].objectOne->grounded = 1;
        collisions[i].objectOne->yPosition = collisions[i].objectTwo->yPosition + 4;
      }
      else if(side == 1)
      {
        collisions[i].objectOne->xPosition = collisions[i].objectTwo->xPosition - get_game_object_width(collisions[i].objectOne);

        if(collisions[i].objectOne->type == 3) //is dog
        {
          handle_dog_side_collision(&collisions[i]);
        }
      }
      else if(side == 2)
      {
        if(collisions[i].objectOne->type == 1 && collisions[i].objectOne->yVelocity > 0) //is player && is going upwards
        {
          collisions[i].objectOne->yVelocity = 0;
        }
      }
      else if(side == 3)
      {
        collisions[i].objectOne->xPosition = collisions[i].objectTwo->xPosition + get_game_object_width(collisions[i].objectOne);
        
        if(collisions[i].objectOne->type == 3) //is dog
        {
          handle_dog_side_collision(&collisions[i]);
        }
      }
    }
  }
}

char get_collision_side(Collision *collision)
{
  double object_center_x =  collision->objectOne->xPosition + (get_game_object_width(collision->objectOne) / 2);
  double object_center_y =  collision->objectOne->yPosition + (get_game_object_height(collision->objectOne) / 2);
  double center_collision_overlap_x = collision->xPosition + (collision->width / 2); 
  double center_collision_overlap_y = collision->yPosition + (collision->height / 2);
  double x_diff = center_collision_overlap_x - object_center_x;
  double y_diff = center_collision_overlap_y - object_center_y;
    if (collision->height > collision->width)
  {
    if (x_diff > 0) //Rightside collision on the side
      return 1;
    else
      return 3;
  }
  else
  {
    if (y_diff > 0)
      return 2;
    else
      return 4;
  }
}