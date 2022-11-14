#include <stdint.h>
#include "GameObject.h"

extern int** levels[];

int GetLevelSceneLength(int level, int scene)
{
    return (sizeof(levels[level][scene]) / sizeof(int)) / 3; //each game object is represented by 3 values, the prefab number, and x and y positions
}

void LoadLevelScene(GameObject sceneArray[], int level, int scene)
{
    int length = GetLevelSceneLength(level, scene);
    
    int index;
    for(index = 0; index <= 16; index++)
    {
        sceneArray[index].xPosition = levels[level][scene][index * 3 + 1];
        sceneArray[index].yPosition = levels[level][scene][index * 3 + 2];
        
        //if(sceneArray[index +1 ] == NULL){}

        int number = levels[level][scene][index * 3];

        if(index > 0)
            number = 2;

        if(number == 1)
        {
            sceneArray[index].graphicIndex = 0; // 0 is player default
        }
        else if(number == 2)
        {
            sceneArray[index].graphicIndex = 1;
        }
        else if(number == 3)
        {
            sceneArray[index].graphicIndex = 2;
        };
    }
}
