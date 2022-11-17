#include "GameObject.h"

typedef struct Collision
{
  double xPosition;
  double yPosition;
  double height;
  double width;
  GameObject *objectOne;
  GameObject *objectTwo;
} Collision;