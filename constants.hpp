#ifndef DEF_CONSTANTS
#define DEF_CONSTANTS

// 30x30 tiles
//#define TILE_SIZE_PIXEL 16
#define TILE_SIZE_PIXEL 50
#define WINDOW_WIDTH 400 //480
#define WINDOW_HEIGHT 400
#define FPS 60
#define NB_KEY_CHARACTER 4
#define GRAVITY 400
#define MAX_FALLING_SPEED 250
#define MIN_JUMP_SPEED 300
#define CHAR_JUMP_SPEED -300
#define CHAR_WALK_SPEED 100

enum KeyInputChar{GoLeft=0,GoRight,GoDown,GoJump};

#endif
