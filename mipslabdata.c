/* mipslabdata.c
   This file compiled 2015 by F Lundevall
   from original code written by Axel Isaksson

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

char textbuffer[4][16];

uint8_t screen_background[] = 
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
};

uint8_t screen_data[] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 

};

//level 01 VVV
char scene_01_01[] =
{
	1, 20, 10,
	2, 0, 0,
	2, 8, 0,
	2, 16, 0,
	2, 16, 4,
	2, 24, 0,
	2, 32, 0,
	2, 40, 0,
	2, 48, 0,
	2, 56, 0,
	2, 64, 0,
	2, 64, 4,
	2, 72, 4,
	2, 72, 8,
	2, 72, 0,
	2, 80, 0,
	4, 94, 4,
	2, 88, 0,
	2, 96, 0,
	2, 104, 0,
	2, 112, 0,
	2, 120, 0,
	2, 128, 0,
};

char scene_01_02[] =
{
	1, 7, 8, //player needs to be first 
	2, 0, 0,
	2, 8, 0,
	2, 16, 0,
	2, 16, 4,
	2, 24, 0,
	2, 32, 0,
	2, 40, 0,
	2, 48, 0,
	3, 50, 8, // dog 
	2, 56, 0,
	2, 64, 0,
	2, 64, 4,
	2, 72, 4,
	2, 72, 8,
	2, 72, 0,
	2, 80, 0,
	2, 88, 0,
	2, 96, 0,
	2, 104, 0,
	5, 104, 9, // key
	2, 112, 0,
	2, 120, 0,
	2, 128, 0,
	
};

char* level_01[] = 
{
	scene_01_01,
	scene_01_02,
};

char level_01_sizes[] = 
{
	23,
	24,
};
//level 01 ^^

//level 02 VVV
char scene_02_01[] =
{
	1, 5, 6,
	2, 0, 0,
	2, 8, 0,
	2, 16, 0,
	2, 24, 0,
	2, 32, 0,
	2, 40, 0,
	2, 48, 4,
	2, 48, 0,
	2, 56, 8,
	2, 56, 4,
	2, 56, 0,
	2, 88, 0,
	2, 96, 0,
	2, 104, 0,
	2, 112, 0,
	2, 120, 0,
};

char scene_02_02[] =
{
	1, 5, 6,
	2, 0, 0,
	2, 8, 0,
	2, 16, 0,
	2, 23, 26,
	2, 48, 5,
	2, 56, 5,
	5, 108, 8,
	2, 96, 0,
	2, 104, 0,
	2, 112, 0,
	2, 120, 0,
};

char scene_02_03[] = 
{
	1, 5, 6,
	2, 0, 0,
	2, 8, 0,
	2, 29, 0,
	2, 50, 5,
	2, 73, 8,
	2, 96, 0,
	2, 104, 0,
	2, 112, 0,
	2, 120, 0,
	4, 104, 4,
};

char* level_02[] = 
{
	scene_02_01,
	scene_02_02,
	scene_02_03,
};

char level_02_sizes[] = 
{
	17,
	11,
	11
};
//level 02 ^^

//level 03 VVV
char scene_03_01[] =
{
	1, 5, 6,
	2, 0, 0,
	2, 8, 0,
	2, 14, 4,
	2, 16, 0,
	2, 24, 0,
	3, 25, 5,
	3, 40, 5,
	2, 32, 0,
	2, 40, 0,
	2, 48, 0,
	2, 56, 0,
	2, 64, 0,
	2, 72, 0,
	2, 80, 0,
	2, 88, 0,
	2, 91, 4,
	2, 96, 0,
	2, 99, 4,
	2, 104, 0,
	2, 120, 0,
};

char scene_03_02[] =
{
	1, 5, 6,
	2, 0, 0,
	2, 8, 0,
	2, 16, 0,
	2, 24, 0,
	2, 32, 0,
	2, 40, 0,
	2, 48, 0,
	2, 56, 4,
	2, 56, 0,
	2, 64, 8,
	2, 64, 4,
	2, 64, 0,
	2, 72, 8,
};

char* level_03[] = 
{
	scene_03_01,
	scene_03_02,
};

char level_03_sizes[] = 
{
	21,
	14,
};
//level 03 ^^


char** levels[] =
{
	level_01,
	level_02,
	level_03,
};

char* level_scene_sizes[] =
{
	level_01_sizes,
	level_02_sizes,
	level_03_sizes,
};

char number_of_scenes[3] = 
{
	2,
	3,
	2,
};

uint8_t imageSizes[][2] =
{
	{ 8, 11 },
	{ 8, 4 },
	{ 8, 6},
	{ 9, 11 },
	{ 17, 18},
	{ 28, 27 },
	{ 4, 9 },
};

char player_default[] = 
{ 
    0, 0, 0, 1, 1, 0, 0, 0,
    0, 0, 1, 0, 0, 1, 0, 0,
    0, 0, 1, 0, 0, 1, 0, 0,
    0, 0, 0, 1, 1, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 1, 1, 1, 1, 0, 1,
    1, 0, 1, 1, 1, 1, 0, 1,
    0, 0, 1, 1, 1, 1, 0, 0,
    0, 0, 1, 0, 0, 1, 0, 0,
    0, 0, 1, 0, 0, 1, 0, 0,
    0, 0, 1, 1, 0, 1, 1, 0, 
};

char baby_block_01[] =
{
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 0, 0, 0, 1, 0, 1,
	1, 0, 0, 1, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
};

char small_dog[] =
{
	0, 0, 1, 0, 0, 0, 0, 0,
	1, 1, 1, 0, 0, 0, 0, 1,
	0, 1, 1, 1, 1, 1, 1, 0,
	1, 1, 1, 1, 1, 1, 1, 0,
	0, 0, 0, 1, 0, 0, 1, 0,
	0, 0, 1, 1, 0, 1, 1, 0,
};

char player_kick[] = 
{
	0, 0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 1, 0, 0, 1, 0, 0, 0,
	0, 0, 1, 0, 0, 1, 0, 0, 0,
	0, 0, 0, 1, 1, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 0,
	1, 0, 1, 1, 1, 1, 0, 1, 0,
	1, 0, 1, 1, 1, 1, 0, 0, 1,
	0, 0, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 1, 0, 0, 0, 0, 0, 0,
	0, 0, 1, 0, 0, 0, 0, 0, 0,
	0, 0, 1, 1, 0, 0, 0, 0, 0,
};

char door[] = 
{
	0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0,
	0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0,
	0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0,
	1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1,
	1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
	1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
	1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1,
	1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
	1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
	1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1,
	1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
	1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
	1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1,
	1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
	1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
	1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,
};

char door_speach[] = 
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0,
	0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0,
	0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0,
	0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

char key[] = 
{
	0, 1, 1, 0,
	1, 0, 0, 1,
	1, 0, 0, 1,
	0, 1, 1, 0,
	0, 1, 0, 0,
	0, 1, 0, 0,
	0, 1, 1, 0,
	0, 1, 0, 0,
	0, 1, 1, 0,
};

char* images[] = { player_default, baby_block_01, small_dog, player_kick, door, door_speach, key };

char number_0[] = 
{
	0, 1, 1, 0, 0,
	1, 0, 0, 1, 0,
	1, 0, 0, 1, 0,
	1, 0, 0, 1, 0,
	0, 1, 1, 0, 0,
};

char number_1[] = 
{
	0, 1, 0, 0, 0,
	1, 1, 0, 0, 0,
	0, 1, 0, 0, 0,
	0, 1, 0, 0, 0,
	1, 1, 1, 0, 0,
};

char number_2[] = 
{
	1, 1, 1, 0, 0,
	0, 0, 0, 1, 0,
	0, 1, 1, 0, 0,
	1, 0, 0, 0, 0,
	1, 1, 1, 1, 0,
};

char number_3[] = 
{
	1, 1, 1, 0, 0,
	0, 0, 0, 1, 0,
	0, 1, 1, 0, 0,
	0, 0, 0, 1, 0,
	1, 1, 1, 0, 0,
};

char number_4[] = 
{
	1, 0, 0, 1, 0,
	1, 0, 0, 1, 0,
	1, 1, 1, 1, 0,
	0, 0, 0, 1, 0,
	0, 0, 0, 1, 0,
};

char number_5[] = 
{
	1, 1, 1, 1, 0,
	1, 0, 0, 0, 0,
	1, 1, 1, 0, 0,
	0, 0, 0, 1, 0,
	1, 1, 1, 0, 0,
};

char number_6[] = 
{
	0, 1, 1, 0, 0,
	1, 0, 0, 0, 0,
	1, 1, 1, 0, 0,
	1, 0, 0, 1, 0,
	0, 1, 1, 0, 0,
};

char number_7[] = 
{
	1, 1, 1, 1, 0,
	0, 0, 0, 1, 0,
	0, 0, 1, 0, 0,
	0, 1, 0, 0, 0,
	0, 1, 0, 0, 0,
};

char number_8[] = 
{
	0, 1, 1, 0, 0,
	1, 0, 0, 1, 0,
	0, 1, 1, 0, 0,
	1, 0, 0, 1, 0,
	0, 1, 1, 0, 0,
};

char number_9[] = 
{
	0, 1, 1, 0, 0,
	1, 0, 0, 1, 0,
	0, 1, 1, 1, 0,
	0, 0, 0, 1, 0,
	0, 1, 1, 0, 0,
};

char whiteSpace[] = 
{
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
};

char unsupported[] = 
{
	0, 1, 0, 1, 0,
	1, 1, 1, 1, 1,
	0, 1, 0, 1, 0,
	1, 1, 1, 1, 1,
	0, 1, 0, 1, 0,
};

char letter_a[] = 
{
	0, 1, 1, 0, 0,
	1, 0, 0, 1, 0,
	1, 1, 1, 1, 0,
	1, 0, 0, 1, 0,
	1, 0, 0, 1, 0,
};

char letter_b[] = 
{
	1, 1, 1, 0, 0,
	1, 0, 0, 1, 0,
	1, 1, 1, 0, 0,
	1, 0, 0, 1, 0,
	1, 1, 1, 0, 0,
};

char letter_c[] = 
{
	0, 1, 1, 1, 0,
	1, 0, 0, 0, 0,
	1, 0, 0, 0, 0,
	1, 0, 0, 0, 0,
	0, 1, 1, 1, 0,
};

char letter_d[] = 
{
	1, 1, 1, 0, 0,
	1, 0, 0, 1, 0,
	1, 0, 0, 1, 0,
	1, 0, 0, 1, 0,
	1, 1, 1, 0, 0,
};

char letter_e[] = 
{
	1, 1, 1, 1, 0,
	1, 0, 0, 0, 0,
	1, 1, 1, 0, 0,
	1, 0, 0, 0, 0,
	1, 1, 1, 1, 0,
};

char letter_f[] = 
{
	1, 1, 1, 1, 0,
	1, 0, 0, 0, 0,
	1, 1, 1, 0, 0,
	1, 0, 0, 0, 0,
	1, 0, 0, 0, 0,
};

char letter_g[] = 
{
	0, 1, 1, 1, 0,
	1, 0, 0, 0, 0,
	1, 0, 1, 1, 0,
	1, 0, 0, 1, 0,
	0, 1, 1, 1, 0,
};

char letter_h[] = 
{
	1, 0, 0, 1, 0,
	1, 0, 0, 1, 0,
	1, 1, 1, 1, 0,
	1, 0, 0, 1, 0,
	1, 0, 0, 1, 0,
};

char letter_i[] = 
{
	1, 1, 1, 0, 0,
	0, 1, 0, 0, 0,
	0, 1, 0, 0, 0,
	0, 1, 0, 0, 0,
	1, 1, 1, 0, 0,
};

char letter_j[] = 
{
	1, 1, 1, 1, 0,
	0, 0, 0, 1, 0,
	0, 0, 0, 1, 0,
	1, 0, 0, 1, 0,
	0, 1, 1, 0, 0,
};

char letter_k[] = 
{
	1, 0, 0, 1, 0,
	1, 0, 1, 0, 0,
	1, 1, 0, 0, 0,
	1, 0, 1, 0, 0,
	1, 0, 0, 1, 0,
};

char letter_l[] = 
{
	1, 0, 0, 0, 0,
	1, 0, 0, 0, 0,
	1, 0, 0, 0, 0,
	1, 0, 0, 0, 0,
	1, 1, 1, 1, 0,
};

char letter_m[] = 
{
	1, 0, 0, 0, 1,
	1, 1, 0, 1, 1,
	1, 0, 1, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
};

char letter_n[] = 
{
	1, 0, 0, 1, 0,
	1, 1, 0, 1, 0,
	1, 0, 1, 1, 0,
	1, 0, 0, 1, 0,
	1, 0, 0, 1, 0,
};

char letter_o[] = 
{
	0, 1, 1, 0, 0,
	1, 0, 0, 1, 0,
	1, 0, 0, 1, 0,
	1, 0, 0, 1, 0,
	0, 1, 1, 0, 0,
};

char letter_p[] = 
{
	1, 1, 1, 0, 0,
	1, 0, 0, 1, 0,
	1, 1, 1, 0, 0,
	1, 0, 0, 0, 0,
	1, 0, 0, 0, 0,
};

char letter_q[] = 
{
	0, 1, 1, 1, 0,
	1, 0, 0, 0, 1,
	1, 0, 0, 0, 1,
	0, 1, 1, 1, 0,
	0, 0, 0, 0, 1,
};

char letter_r[] = 
{
	1, 1, 1, 0, 0,
	1, 0, 0, 1, 0,
	1, 1, 1, 0, 0,
	1, 0, 1, 0, 0,
	1, 0, 0, 1, 0,
};

char letter_s[] = 
{
	0, 1, 1, 1, 0,
	1, 0, 0, 0, 0,
	0, 1, 1, 0, 0,
	0, 0, 0, 1, 0,
	1, 1, 1, 0, 0,
};

char letter_t[] = 
{
	1, 1, 1, 0, 0,
	0, 1, 0, 0, 0,
	0, 1, 0, 0, 0,
	0, 1, 0, 0, 0,
	0, 1, 0, 0, 0,
};

char letter_u[] = 
{
	1, 0, 0, 1, 0,
	1, 0, 0, 1, 0,
	1, 0, 0, 1, 0,
	1, 0, 0, 1, 0,
	0, 1, 1, 0, 0,
};

char letter_v[] = 
{
	1, 0, 1, 0, 0,
	1, 0, 1, 0, 0,
	1, 0, 1, 0, 0,
	1, 0, 1, 0, 0,
	0, 1, 0, 0, 0,
};

char letter_w[] = 
{
	1, 0, 0, 0, 1,
	1, 0, 1, 0, 1,
	1, 0, 1, 0, 1,
	1, 0, 1, 0, 1,
	0, 1, 0, 1, 0,
};

char letter_x[] = 
{
	1, 0, 0, 1, 0,
	1, 0, 0, 1, 0,
	0, 1, 1, 0, 0,
	1, 0, 0, 1, 0,
	1, 0, 0, 1, 0,
};

char letter_y[] = 
{
	1, 0, 1, 0, 0,
	1, 0, 1, 0, 0,
	0, 1, 0, 0, 0,
	0, 1, 0, 0, 0,
	0, 1, 0, 0, 0,
};

char letter_z[] = 
{
	1, 1, 1, 1, 1,
	0, 0, 0, 0, 1,
	0, 1, 1, 1, 0,
	1, 0, 0, 0, 0,
	1, 1, 1, 1, 1,
};

char letter_arrow[] = 
{
	1, 1, 0, 0, 0,
	1, 0, 1, 0, 0,
	1, 0, 0, 1, 0,
	1, 0, 1, 0, 0,
	1, 1, 0, 0, 0,
};

char letter_button[] = 
{
	0, 1, 1, 1, 0,
	1, 1, 1, 1, 1,
	1, 1, 1, 1, 1,
	1, 1, 1, 1, 1,
	0, 1, 1, 1, 0,
};

char letter_switch[] =
{
	1, 0, 0, 1, 0,
	1, 0, 1, 0, 0,
	1, 1, 0, 0, 0,
	1, 0, 0, 0, 0,
	1, 0, 0, 0, 0,
};

char letter_question[] = 
{
	0, 1, 1, 1, 0,
	0, 0, 0, 0, 1,
	0, 0, 1, 1, 0,
	0, 0, 0, 0, 0,
	0, 0, 1, 0, 0,
};

char letter_arrow_up[] =
{
	0, 0, 1, 0, 0,
	0, 1, 0, 1, 0,
	1, 0, 0, 0, 1,
	1, 1, 1, 1, 1,
	0, 0, 0, 0, 0,
};

char letter_line[] = 
{
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 1, 1, 1, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
};

char* characters[] = 
{
	number_0,
	number_1,
	number_2,
	number_3,
	number_4,
	number_5,
	number_6,
	number_7,
	number_8,
	number_9,
	unsupported,
	whiteSpace,
	letter_a,
	letter_b,
	letter_c,
	letter_d,
	letter_e,
	letter_f,
	letter_g,
	letter_h,
	letter_i,
	letter_j,
	letter_k,
	letter_l,
	letter_m,
	letter_n,
	letter_o,
	letter_p,
	letter_q,
	letter_r,
	letter_s,
	letter_t,
	letter_u,
	letter_v,
	letter_w,
	letter_x,
	letter_y,
	letter_z,
	letter_arrow,
	letter_button,
	letter_switch,
	letter_question,
	letter_arrow_up,
	letter_line,
};