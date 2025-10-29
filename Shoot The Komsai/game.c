#include <stdio.h>
#include <emscripten/emscripten.h>

int get_screen_width();

int player_x_movement = 0;
int object_speed = 2;
int object_y_position = 0;
int screen_width = 0;
int player_x_COORDINATES = 1920 / 2;

EMSCRIPTEN_KEEPALIVE
void update() {
  screen_width = get_screen_width();

  //Player Movement
  player_x_COORDINATES += player_x_movement;
  if (player_x_COORDINATES < 0 || player_x_COORDINATES > screen_width - 100) {
    player_x_movement = 0;
    player_x_COORDINATES = (player_x_COORDINATES < 0) ? 0 : screen_width - 100;
  }

  //Falling Objects Movement
  object_y_position += object_speed;
  if (object_y_position > 600) {
    object_y_position = 0;
  }
}

int get_screen_width() {
  int screen_width = EM_ASM_INT({
    return window.innerWidth;
  });
  return screen_width;
}

EMSCRIPTEN_KEEPALIVE
int get_x() { return player_x_COORDINATES; }

EMSCRIPTEN_KEEPALIVE
int right_movement() {
  player_x_movement = 5;
  return player_x_movement;
}

EMSCRIPTEN_KEEPALIVE
int left_movement() {
  player_x_movement = -5;
  return player_x_movement;
}
