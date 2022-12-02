#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

typedef struct GameObject
{
  double xPosition;
  double yPosition;
  double xVelocity;
  double yVelocity;
  int graphicIndex;
  char usePhysics;
  char disabled;
  char grounded;
  char is_mirrored;
  char type;
  int health;
  int invincibilityCounter;
  char forcedMovement;
} GameObject;

#endif