#include <stdint.h>
#include "GameObject.h"

extern uint8_t screen_data[512];
extern uint8_t screen_background[512];
extern uint8_t imageSizes[][2];
extern char* images[2];
extern char* characters[];

uint8_t *canvases[] = { screen_data, screen_background };

void setPixel(int x, int y, char value, int canvasIndex)
{
  if(x < 0 || x > 127 || y < 0 || y > 31)
    return;

  y = 32 - y;
	int blockPosition = x + ((y / 8) * 128);
	char block = canvases[canvasIndex][blockPosition];

	//block &= ~(1 << (y % 8));

  if(value > 0)
  {
    block |= 1 << (y % 8);
  }
  
	canvases[canvasIndex][blockPosition] = block;
}

void clear_background()
{
  int i;
  for (i = 0; i < 512; i++)
  {
    screen_background[i] = 0;
  }
}

void clear_screen()
{
  int i;
  for (i = 0; i < 512; i++)
  {
    screen_data[i] = screen_background[i];
  }
}

void drawGraphic(int x, int y, int width, int height, char* graphic, char mirrored, char transparent, int canvasIndex)
{
  int i;
  int j;
  for (i = 0; i < width; i++)
  {
    int drawX;
    
    if(mirrored == 0)
        drawX = x + i;
      else
        drawX = (x + width - i);
        
    for (j = 0; j < height; j++)
    {
      char newPixelValue = graphic[i + j * (width)];

      if(transparent && j % 2 == 0)
        transparent = 0;

      setPixel(drawX, (y + height) - j, newPixelValue, canvasIndex);
    }  
  } 
}

void draw_character(int x, int y, char character, int canvasIndex)
{
  char charIndex = 0;

  if(character >= 48 && character <= 57)
    charIndex = character - 48; //numbers
  else if(character >= 65 && character <= 90)
    charIndex = character - 65 + 12; //capital letters
  else if(character == 32)
    charIndex = 11; //white space
  else if(character == 94) // ^ to write the menu arrow
    charIndex = 38;
  else if(character == 91) // [ to write the button icon
    charIndex = 39;
  else if(character == 92) // ] to write the switch icon
    charIndex = 40;
  else
    charIndex = 10; //unsupported character

  drawGraphic(x, y, 5, 5, characters[charIndex], 0, 0, canvasIndex);
}

void draw_string(int x, int y, char* string, int length, int canvasIndex)
{
  int currentOffset = 0;
  int i;
  for (i = 0; i < length; i++)
  {
    draw_character(x + currentOffset, y, string[i], canvasIndex);
    currentOffset += 6;
  }
}

void draw_game_object(GameObject *gameObject, int canvasIndex)
{
  char transparent = 0;
  if(gameObject->invincibilityCounter > 0)
    transparent = 1;

  drawGraphic((int)gameObject->xPosition, (int)gameObject->yPosition, imageSizes[gameObject->graphicIndex][0], imageSizes[gameObject->graphicIndex][1], images[gameObject->graphicIndex], gameObject->is_mirrored, transparent, canvasIndex);  
}

int get_game_object_width(GameObject *gameObject)
{
  return imageSizes[gameObject->graphicIndex][0];
}

int get_game_object_height(GameObject *gameObject)
{
  return imageSizes[gameObject->graphicIndex][1];
}