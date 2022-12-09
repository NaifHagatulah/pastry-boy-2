#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include <stdlib.h>
#include "mipslab.h" /* Declatations for these labs */
#include "drawingFunctions.h"
#include "Collision.h"
#include "mapFunctions.h"
#include  "playerScore.h"
#include <string.h> // Memset, initalize and change elements in array

const double JUMP_FORCE = 0.4;
const double GRAVITY_FORCE = 0.008;
const double MOVE_SPEED = 0.3;
const int INVINCIBILIY_TIME = 50;

unsigned int tickValue = 0x1;
volatile int *trise = (volatile int *)0xbf886100;
volatile int *porte = (volatile int *)0xbf886110;

char menuOptionChangeCooldown = 0;
char menuOptionSelected = 0;
char menuScreen = 0;
int ticks = 0;
int gameCounter = 1;
int displayUpdateCounter = 0;
double timeScale = 1;
char switchStartState = 0;
char switchStartState2 = 0;
char keys = 0;
int gravityDirection = 1;
char maxMenuOption = 3;
char currentScore = 0;

extern uint8_t screen_data[512];
extern char player_default[88];
extern char baby_block_01[32];
extern char* images[2];
extern uint8_t imageSizes[][2];
extern char number_of_scenes[2];

char currentNameInput[3];
char currentNameInputIndex;

PlayerScore scores[3];
GameObject *player;
GameObject levelSceneObjects[32*4];
GameObject *gameObjects;
char gameObjectsLength;
char currentScene;
char currentLevel;

Collision collisions[100];
char collisionsLength;

int graphicChangeTime = 0;
GameObject *graphicChangeTarget;
char graphicChangeNewIndex;

int kickCooldown = 0;
int kickTimeCounter = 0;
char playerIsOnDoor = 0;
GameObject *door;

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
void queue_graphic_change(GameObject *gameObject, int newGraphicIndex, int delay);
void perform_queued_graphic_change();
void set_object_graphic(GameObject *gameObject, int graphicIndex);
void master_game_update();
void main_menu_draw();
void menu_logic_update();
void invert_binary_value(char *value);
void view_scores_draw();
void death_screen_draw();
void go_to_main_menu();
void draw_enter_name_screen();
void go_to_enter_name_screen();
void save_name();
void sort_scores();

void user_isr(void)
{ 
  if (IFS(0) & 0x100) //master updates
  {
    displayUpdateCounter++;

    IFSCLR(0) = 0x100;
    if (displayUpdateCounter % 20 == 0)
    {
      if(menuScreen == 0) // no menu screen, in game
        master_game_update();
      else if(menuScreen == 1) // main menu
      {
        menu_logic_update();
        main_menu_draw();
      }
      else if(menuScreen == 2) // high score screen
      {
        menu_logic_update();
        view_scores_draw();
      }
      else if(menuScreen == 3) // deathscreen option to save score or not
      {
        menu_logic_update();
        death_screen_draw();
      }
      else if(menuScreen == 4) // choose name screen
      {
        menu_logic_update();
        draw_enter_name_screen();
      }
    }
  }
  return;
}

void main_menu_draw()
{
  if(menuOptionSelected == 0)
  {
    clear_background();
    clear_screen();
    return;
  }

  clear_screen(); //clear screen to make it ready for drawing

  draw_string(54, 25, "PASTRY BOY 2", 12, 0);
  draw_string(10, 14, "PLAY", 4, 0);
  draw_string(10, 8, "VIEW SCORES", 11, 0);
  draw_string(10, 1, "ABOUT", 5, 0);
  draw_string(1, (menuOptionSelected * -6) + 20, "^", 1, 0);
  
  display_image(0, screen_data); //display the image on the screen
}

void draw_enter_name_screen()
{
  clear_screen(); //clear screen to make it ready for drawing

  draw_string(5, 25, "CHOOSE YOUR NAME", 16, 0);        //alfabete is from <= 65 <= 90
  draw_string(10, 14, currentNameInput, 3, 0);
  draw_string(10 + currentNameInputIndex * 5, 8, "*", 1, 0);
  draw_string(100, 2, "[4 OK", 5, 0);

  display_image(0, screen_data); //display the image on the screen
}

void death_screen_draw()
{
  clear_screen(); //clear screen to make it ready for drawing

  draw_string(20, 25, "MR PASTRY DIED", 14, 0);
  draw_string(10, 15, "SAVE SCORE?", 11, 0);
  draw_string(10, 8, "YES", 3, 0);
  draw_string(10, 2, "NO", 2, 0);
  draw_string(1, (menuOptionSelected * -6) + 14, "^", 1, 0);
  
  display_image(0, screen_data); //display the image on the screen
}

void view_scores_draw()
{
  clear_screen(); //clear screen to make it ready for drawing

  char i;
  for(i = 0; i < 3; i++)
  {
    char scoreMemset[5];
    memset(scoreMemset, 0, 5*sizeof(scoreMemset[0])); //set all value to 0
    
    char height = 18 - i * 6;
    char scoreString[5] = {0};

    if(scores[i].hasValue)
      get_int_as_string(scores[i].score, scoreString, sizeof(scoreString) / sizeof(scoreString[0]) );

    draw_string(25, height, scoreString, 5, 0);
    draw_string(2, height, scores[i].name, 3, 0);
  }

  //draw_string(1, (menuOptionSelected * -6) + 20, "^", 1, 0);
  
  display_image(0, screen_data); //display the image on the screen
}

void level_menu()
{
  clear_screen();
}

void creators_point_of_view()
{
  draw_string(1, 1, "WE WORK HARD", 13, 1);
  draw_string(1, 7, "WE WORK HARD", 13, 1);
  draw_string(1, 15, "WE WORK HARD", 13, 1);
}

void master_game_update()
{
  apply_gravity();
  find_collisions();
  handle_collisions();
  game_update();
  apply_velocities();

  clear_screen(); //clear screen to make it ready for drawing
  draw_objects(); //draw gameobjects

  // char string[10] = {0};
  // get_int_as_string(currentScene, string, sizeof(string));
  // draw_string(1, 10, string, 10, 0);

  // char string2[10] = {0};
  // get_int_as_string(number_of_scenes[currentLevel], string2, sizeof(string2));
  // draw_string(1, 16, string2, 10, 0);

  display_image(0, screen_data); //display the image on the screen
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

void write_scene_specific_texts(int level, int scene)
{
  if(level == 0 && scene == 0)
  {
    //draw_string(1, 26, "BUTTONS [1[2[3[4", 16, 1);
  }
  else if(level == 0 && scene == 1)
  {
    //draw_string(1, 26, "[4 ATTACK", 9, 1);
  }
}

void load_level(int level)
{
  currentLevel = level;
  currentScene = 0;
  
  int i;
  for(i = 0; i < 32 * 4; i++)
  {
    //reset all objects
    levelSceneObjects[i].disabled = 0;
    levelSceneObjects[i].hasBeenLoaded = 0;
    levelSceneObjects[i].usePhysics = 0;
    levelSceneObjects[i].xVelocity = 0;
    levelSceneObjects[i].yVelocity = 0;
    levelSceneObjects[i].is_mirrored = 0;
  }
}

void load_scene(int scene)
{ 
  clear_background();
  currentScene = scene;
  gameObjects = &levelSceneObjects[32 * currentScene];

  write_scene_specific_texts(currentLevel, scene);

  gameObjectsLength = get_level_scene_length(currentLevel, scene);

  switchStartState = getsw() & 0x1;
  switchStartState2 = getsw() & 0x01;

  load_level_scene(gameObjects, currentLevel, scene);
  player = &gameObjects[0];

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

  enable_interrupt();

  T2CONSET = 0x8000; // aktiverar timer 2

  *trise = 0x00;

  TRISD |= 0x7f;

  go_to_main_menu();
}

void go_to_main_menu()
{
  clear_background();
  maxMenuOption = 3;
  menuScreen = 1;
  menuOptionSelected = 1;
}

void game_update() //will run every time the timer ticks
{
  *porte = number_of_scenes[currentLevel];
  gameCounter++;
  kickCooldown--;
  kickTimeCounter--;

  int buttons = getbtns();
  int btn1 = getbtn1();
  int sw = getsw();

  if((sw & 0x1) != switchStartState)
  {
    go_to_main_menu();
  }

  if((sw & 0x01) != switchStartState2)
  {
    gravityDirection = -1;
  }
  else
  {
    gravityDirection = 1;
  }
  
  if(btn1 & 0x2) //button 4 is pressed
  {
    if(player->graphicIndex != 3 && kickCooldown <= 0)
    {
      set_object_graphic(player, 3);
      kickCooldown = 80;
      kickTimeCounter = 60;

      if(playerIsOnDoor)
      {
        if(keys == 0)
        {
           set_object_graphic(door, 5);
           queue_graphic_change(door, 4, 300);
        }
        else
        {
          keys = 0;
          playerIsOnDoor = 0;
          load_level(currentLevel + 1);
          load_scene(currentScene);
        }
      }
    }
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
    player->xPosition += MOVE_SPEED * timeScale; 
    player->is_mirrored = 0;
  }

  if (buttons & 0x4) //button 1 is pressed
  {
    player->xPosition -= MOVE_SPEED * timeScale;   
    player->is_mirrored = 1;
  }

  if(player->yPosition < -8)
  {
    player->xPosition = 2;
    player->yPosition = 5;
    player->yVelocity = 0;
    player->health--;
    if(player->health <= 0 )
    {
      die();
    }
  }

  if(currentScene == 0 && player->xPosition <= 0)
  {
    player->xPosition = 0;
  }

  if(currentScene == number_of_scenes[currentLevel] - 1 && player->xPosition >= 120) //128 - player width (8) = 120
  {
    player->xPosition = 120;
  }

  if(player->xPosition > 128)
  {
    go_to_next_scene();
  }
  else if(player->xPosition < -8)
  {
    go_to_previous_scene();
  }

  if(graphicChangeTime != 0 && gameCounter > graphicChangeTime) //check if we should perform a queued graphic change
  {
    perform_queued_graphic_change();
  }

  if(player->graphicIndex == 3 && kickTimeCounter <= 0)
  {
    set_object_graphic(player, 0);
  }
  
  int i;
  for(i = 0; i < gameObjectsLength; i++)
  {
    gameObjects[i].invincibilityCounter -= 1;

    if(gameObjects[i].health > 0)
      gameObjects[i].forcedMovement = 0;

    if(gameObjects[i].yPosition < -10)
      gameObjects[i].disabled = 1;
  }
}

void copy_current_name(char target[], char origin[])
{
  target[0] = origin[0];
  target[1] = origin[1];
  target[2] = origin[2];
}

void insert_score(char index)
{
  scores[index].hasValue = true;
  scores[index].score = currentScore;
  copy_current_name(scores[index].name, currentNameInput);
}

void save_name()
{
  if(scores[2].hasValue)
  {
    if(currentScore > scores[2].score)
    {
      insert_score(2);
    }
  }
  else
  {
    insert_score(2);
  }

  sort_scores();
}

void sort_scores()
{
  char temp_score;
  char temp_name[3];
  char i; 
  char j;
  char k;
  for (i = 0; i < 3; i++)
  {
    for(j = i; j < 3; j++)
    {
      if(scores[i].score < scores[j].score)
      {
        temp_score = scores[i].score;
        copy_current_name(temp_name, scores[i].name);
        copy_current_name(scores[i].name, scores[j].name);
        scores[i].score = scores[j].score;
        copy_current_name(scores[j].name, temp_name);
        scores[j].score = temp_score;
      }
    }
  }
}

void menu_logic_update()
{
  menuOptionChangeCooldown--;

  int buttons = getbtns();
  int btn1 = getbtn1();

  if(btn1 & 0x2) //button 4 is pressed
  {
    if(menuScreen == 4)
    {
      save_name();
      go_to_main_menu();
    }
  }

  if (buttons & 0x1) //button 3 is pressed
  {
    if(menuOptionChangeCooldown <= 0)
    {
      if(menuScreen == 1)
      {
        if(menuOptionSelected == 1)
        {
          clear_background();
          menuScreen = 0;
          load_level(0);
          load_scene(0);
        }
        if(menuOptionSelected == 2)
        {
          clear_background();
          menuScreen = 2;
        }
      }
      else if(menuScreen == 3)
      {
        if(menuOptionSelected == 1)
        {
          go_to_enter_name_screen();
        }
        if(menuOptionSelected == 2)
        {
          
        }
      }
      else if(menuScreen == 4)
      {
        currentNameInputIndex++;

        if(currentNameInputIndex > 2)
          currentNameInputIndex = 0;
      }
    }

    menuOptionChangeCooldown = 30;
  }

  if (buttons & 0x2) //button 2 is pressed
  {
    if(menuOptionChangeCooldown < 0)
    {
      if(menuScreen == 4) //is entering name
      {
        currentNameInput[currentNameInputIndex] = currentNameInput[currentNameInputIndex] + 1;
        if(currentNameInput[currentNameInputIndex] > 90)
        {
          currentNameInput[currentNameInputIndex] = 65;
        }  

        menuOptionChangeCooldown = 30;
      }
      else if(menuOptionSelected > 1) //is not entering name, limit menu option to not be less than 1
      {
        menuOptionSelected--;
      }

      menuOptionChangeCooldown = 30;
    }
  }

  if (buttons & 0x4) //button 1 is pressed
  {
    if(menuOptionChangeCooldown < 0)
    {
      if(menuScreen == 4) //is entering name
      {
        currentNameInput[currentNameInputIndex] = currentNameInput[currentNameInputIndex] - 1;;
        if(currentNameInput[currentNameInputIndex] < 65)
        {
          currentNameInput[currentNameInputIndex] = 90;
        }
        
        menuOptionChangeCooldown = 30;
      }
      else if(menuOptionSelected < maxMenuOption) //is not entering name, check so we are not at max menu option
      {
        menuOptionSelected++;
      }

      menuOptionChangeCooldown = 30;
    }
  }
}

void go_to_enter_name_screen()
{
  currentNameInput[0] = 65;
  currentNameInput[1] = 65;
  currentNameInput[2] = 65;
  currentNameInputIndex = 0;
  menuScreen = 4;
  menuOptionSelected = 1;
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

void master_update(void) //user_isr is used instead
{
}

void apply_gravity()
{
  int index = 0;
  for(index = 0; index < gameObjectsLength; index++)
  {
    if(gameObjects[index].usePhysics == 1)
    {
      gameObjects[index].grounded = 0; //reset grounded every frame
      gameObjects[index].yVelocity -= GRAVITY_FORCE * timeScale * gravityDirection; //apply gravity
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
      gameObjects[index].xPosition += gameObjects[index].xVelocity * timeScale; //move object with x velocity
      gameObjects[index].yPosition += gameObjects[index].yVelocity * timeScale; //move object with y velocity
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
  if(currentScene < number_of_scenes[currentLevel] - 1)
  {
    int yPosition = player->yPosition;
    int yVelocity = player->yVelocity;
    load_scene(currentScene + 1); //needs to be fixed, should not be hardcoded
    player->yPosition = yPosition;
    player->xPosition = 2;
    player->yVelocity = yVelocity;
  }
}

void go_to_previous_scene()
{
  if(currentScene > 0)
  {
    int yPosition = player->yPosition;
    int yVelocity = player->yVelocity;
    load_scene(currentScene - 1);
    player->yPosition = yPosition;
    player->xPosition = 110;
    player->yVelocity = yVelocity;
  }
}

void die()
{
  menuOptionSelected = 1;
  menuScreen = 3;
  maxMenuOption = 2;
  clear_background();
}

int get_collision(GameObject *object1, GameObject *object2, Collision *collision)
{
  double thisLeft = object1->xPosition;
  double otherLeft = object2->xPosition;

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
  if(collision->objectTwo->type == 1) //player
  {
    if(collision->objectTwo->graphicIndex == 3 && collision->objectOne->invincibilityCounter <= 0) // graphicIndex 3 = kick
    { //player is kicking the dog
      collision->objectOne->health -= 1;

      collision->objectOne->xVelocity *= -1;
      collision->objectOne->yPosition += 0.5;
      collision->objectOne->yVelocity = 0.5;
      collision->objectOne->forcedMovement = 1;
      invert_binary_value(&collision->objectOne->is_mirrored);

      collision->objectOne->invincibilityCounter = INVINCIBILIY_TIME;
      
      if(collision->objectOne->health <= 0)
      {
        collision->objectOne->disabled = 1;
      }
    }
    else if (collision->objectTwo->graphicIndex != 3 && collision->objectOne->invincibilityCounter <= 0)
    {
      collision->objectTwo->health = collision->objectTwo->health - 1;

      //player
      collision->objectTwo->yPosition += 0.3;
      collision->objectTwo->yVelocity += 0.5;
      collision->objectTwo->forcedMovement = 1;
      collision->objectOne->invincibilityCounter = INVINCIBILIY_TIME;

      //dog
      collision->objectOne->xVelocity *= -1;
      collision->objectOne->yPosition += 0.2;
      collision->objectOne->yVelocity = 0.2;
      invert_binary_value(&collision->objectOne->is_mirrored);
      collision->objectOne->forcedMovement = 1;

      if(collision->objectTwo->health == 0)
      {
        collision->objectTwo->disabled = 1;
        die();
      }
    }
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
  playerIsOnDoor = 0;

  int i = 0;
  for (i = 0; i < collisionsLength; i++)
  {
    if(collisions[i].objectOne->type == 4 || collisions[i].objectTwo->type == 4) //one obect is door
    {
      if(collisions[i].objectOne->type == 1 || collisions[i].objectTwo->type == 1) //one object is player
      {
        playerIsOnDoor = 1;
        if(collisions[i].objectOne->type == 4) //object one is the door
        {
          door = collisions[i].objectOne;
        }
        else //object two is the door
        {
          door = collisions[i].objectTwo;
        }
        continue;
      }
    }
    
    if(collisions[i].objectOne->type == 5 || collisions[i].objectTwo->type == 5) //one object is key
    {
      if(collisions[i].objectOne->type == 1 || collisions[i].objectTwo->type == 1)//one object is player
      {
        keys = keys + 1;
        
        if(collisions[i].objectOne->type == 5)
        {
          collisions[i].objectOne->disabled = 1;
        }
        else
        {
          collisions->objectTwo->disabled = 1;
        }

        continue;
      }
    }

    if(collisions[i].objectOne->usePhysics == 1 && collisions[i].objectOne->forcedMovement == 0)
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

void set_object_graphic(GameObject *gameObject, int graphicIndex)
{
  gameObject->graphicIndex = graphicIndex;
}

void queue_graphic_change(GameObject *gameObject, int newGraphicIndex, int delay)
{
  if(graphicChangeTime != 0)
    perform_queued_graphic_change();

  graphicChangeTarget = gameObject;
  graphicChangeTime = gameCounter + delay;
  graphicChangeNewIndex = newGraphicIndex;
}

void perform_queued_graphic_change()
{
  set_object_graphic(graphicChangeTarget, graphicChangeNewIndex);
  graphicChangeTime = 0;
}

void invert_binary_value(char *value)
{
  if(*value == 1)
  {
    *value = 0;
    return;
  }
  *value = 1;
}