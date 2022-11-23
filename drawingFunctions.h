#include "GameObject.h"

void set_pixel(int x, int y, char value);
void clear_screen();
void draw_graphic(int x, int y, int width, int height, char* graphic);
void draw_game_object(GameObject *gameObject, int canvasIndex);
int get_game_object_width(GameObject *gameObject);
int get_game_object_height(GameObject *gameObject);
void clear_background();
void draw_character(int x, int y, char character);
void draw_string(int x, int y, char* string, int length);