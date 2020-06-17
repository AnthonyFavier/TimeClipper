#ifndef DEF_CONSTANTS
#define DEF_CONSTANTS

// 30x30 tiles
#define TILE_SIZE_PIXEL 50
#define NB_X_TILE 16
#define NB_Y_TILE 10
const int WINDOW_WIDTH = TILE_SIZE_PIXEL * NB_X_TILE;
const int WINDOW_HEIGHT = TILE_SIZE_PIXEL * NB_Y_TILE;
#define FPS 60
#define DROP_TILE_THRESHOLD 2.f
#define GRAVITY 3000
#define MAX_FALLING_SPEED 600
#define MIN_JUMP_SPEED 300
#define CHAR_JUMP_SPEED -470
#define CHAR_WALK_SPEED 300

#define NB_KEY_CHARACTER 6
enum KeyInputChar{GoLeft=0,GoRight,GoDown,GoJump,SizeUp,SizeDown};

#endif
