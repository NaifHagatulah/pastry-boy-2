#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

typedef struct GameObject
{
  double xPosition;
  double yPosition;
  double xVelocity;
  double yVelocity;
  char graphicIndex;
  char usePhysics;
  char disabled;
  char grounded;
  char is_mirrored;
  char type;
  char health;
  int invincibilityCounter;
  char forcedMovement;
  char hasBeenLoaded;
} GameObject;

#endif