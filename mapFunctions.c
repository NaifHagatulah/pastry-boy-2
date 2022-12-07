#include <stdint.h>
#include "GameObject.h"

extern int** levels[];
extern int* level_scene_sizes[];

int get_level_scene_length(int level, int scene)
{
    return level_scene_sizes[level][scene]; //each game object is represented by 3 values, the prefab number, and x and y positions
}

void load_level_scene(GameObject sceneArray[], int level, int scene)
{
    int length = get_level_scene_length(level, scene);
    int index;
    for(index = 0; index < length; index++)
    {
        sceneArray[index].xPosition = levels[level][scene][index * 3 + 1];
        sceneArray[index].yPosition = levels[level][scene][index * 3 + 2];

        int number = levels[level][scene][index * 3];

        sceneArray[index].type = number;
        
        if(number == 1) //player
        {
            sceneArray[index].graphicIndex = 0; // 0 is player default
            sceneArray[index].usePhysics = 1;
            sceneArray[index].health = 10;
        }
        else if(number == 2) //small block
        {
            sceneArray[index].graphicIndex = 1;
        }
        else if(number == 3) //dog
        {
            sceneArray[index].graphicIndex = 2;
            sceneArray[index].usePhysics = 1;
            sceneArray[index].xVelocity = -0.15;
            sceneArray[index].health = 3;
        }
        else if(number == 4) //door
        {
            sceneArray[index].graphicIndex = 4;
            sceneArray[index].usePhysics = 0;
        }
        else if(number == 5) //key
        {
            sceneArray[index].graphicIndex = 6;
            sceneArray[index].usePhysics = 1;
        }
    }
}
