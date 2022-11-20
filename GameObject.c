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
  char type; // 1 = player, 2 = small block, 3 = dog
} GameObject;