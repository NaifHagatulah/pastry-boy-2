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
  char type; // 1 = player, 2 = small block, 3 = dog, 4 = door, 5 = key
  char health;
  int invincibilityCounter;
  char forcedMovement;
  char hasBeenLoaded;
} GameObject;