#define MATRIX_WIDTH 10
#define MATRIX_HEIGHT 20
#define TILE_SIZE 36

#define WINDOW_WIDTH 760
#define WINDOW_HEIGHT 720

#define WINDOW_WIDTH_MENU 450
#define WINDOW_HEIGHT_MENU 600

#define TETRIS_MAP_WIDTH 360
#define TETRIS_MAP_HEIGHT 720

#define TETRIS_MAP_INIT_X 200
#define TETRIS_MAP_INIT_Y 0

#define TETRIS_ENEMY_MAP_INIT_X 760

#define TETRIS_MAP_MARGIN 36
#define TETRIMINO_QUEUE_MARGIN 120

#define WINDOW_WIDTH_MULTIPLAYER 1200
#define WINDOW_HEIGHT_MULTIPLAYER 800

#define LOCK_DOWN_TIME 500 // in ms
#define VIRTUAL_BAG                       \
    {                                     \
        'I', 'O', 'T', 'L', 'J', 'Z', 'S' \
    }

#define ANIMATION_PROGRESS 0.1

/* For multiplayer */
#define SERVER_PORT 8080
#define CODE_ROOM_UNAVAILABLE 0
#define CODE_ROOM_SIZE 1
#define CODE_START_GAME 2
#define CODE_PLAYER_MAP 3
#define CODE_PLAYER_LINES 4
#define CODE_PLAYER_DEAD 5
#define CODE_GAME_OVER 6
#define CODE_PLAYER_DISCONNECT 7
