#include <stdint.h>
#include "GameObject.h"

extern uint8_t screen_data[512];
extern uint8_t imageSizes[][2];
extern char* images[];

struct GameObjectStruct
{
  double xPosition;
  double yPosition;
  double xVelocity;
  double yVelocity;
  int graphicIndex;
};

typedef struct GameObjectStruct GameObject;

struct card
{
    int face; // define pointer face
}; // end structure card


typedef struct card Card ;

void passByReference(Card *c) ;

void face(car)


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
  for (i = 0; i < 512; i++)
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

void drawGameObject(GameObject *gameObject)
{
  drawGraphic((int)gameObject->xPosition, (int)gameObject->yPosition, imageSizes[gameObject->graphicIndex][0], imageSizes[gameObject->graphicIndex][1], images[gameObject->graphicIndex]);  
}