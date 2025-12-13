/*Write a Breakout-style game with the following specifications:
-inside a window, a paddle moves left and right at the bottom, controlled by the arrow keys;
-above the paddle there are several bricks;
-a ball moves automatically, bouncing off the paddle, walls, and bricks;
-when the ball hits a brick, the brick disappears and the score increases;
-the current score is continuously displayed and updated in a corner of the window;
-if the ball reaches the bottom of the window, it disappears and the game is lost;
-the game starts with the ball placed on the paddle and begins when a key is pressed;
-the game ends when the ball is lost, when all bricks are destroyed, or when a dedicated key is pressed.
*/
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define PADDLE_WIDTH 10
#define PADDLE_HEIGHT 1
#define BALL_CHAR 'O'
#define PADDLE_CHAR '='
#define BRICK_CHAR '#'
#define BRICK_ROWS 5
#define BRICK_COLS 10
#define BRICK_WIDTH 6
#define BRICK_HEIGHT 2
#define DELAY 50000

typedef struct {
    int x, y;
    int dx, dy;
    int active;
} Ball;

typedef struct {
    int x, y;
    int width;
} Paddle;

typedef struct {
    int x, y;
    int width, height;
    int active;
} Brick;

typedef struct {
    Ball ball;
    Paddle paddle;
    Brick *bricks;
    int num_bricks;
    int score;
    int max_y, max_x;
    int game_started;
    int game_over;
} Game;

// Game initialization
Game* init_game() {
    Game *game = (Game*)malloc(sizeof(Game));
    if (game == NULL) {
        perror("Error allocating memory for the game");
        return NULL;
    }

    // Get screen dimensions
    getmaxyx(stdscr, game->max_y, game->max_x);

    // Check minimum window size
    if (game->max_y < 20 || game->max_x < 40) {
        free(game);
        fprintf(stderr, "Window too small! Minimum size is 40x20\n");
        return NULL;
    }

    // Paddle initialization
    game->paddle.width = PADDLE_WIDTH;
    game->paddle.x = game->max_x / 2 - PADDLE_WIDTH / 2;
    game->paddle.y = game->max_y - 3;

    // Ball initialization (on the paddle)
    game->ball.x = game->paddle.x + PADDLE_WIDTH / 2;
    game->ball.y = game->paddle.y - 1;
    game->ball.dx = 1;
    game->ball.dy = -1;
    game->ball.active = 0;

    // Brick initialization
    game->num_bricks = BRICK_ROWS * BRICK_COLS;
    game->bricks = (Brick*)malloc(sizeof(Brick) * game->num_bricks);
    if (game->bricks == NULL) {
        perror("Error allocating memory for bricks");
        free(game);
        return NULL;
    }

    int idx = 0;
    int start_x = (game->max_x - (BRICK_COLS * BRICK_WIDTH)) / 2;
    int start_y = 3;

    for (int i = 0; i < BRICK_ROWS; i++) {
        for (int j = 0; j < BRICK_COLS; j++) {
            game->bricks[idx].x = start_x + j * BRICK_WIDTH;
            game->bricks[idx].y = start_y + i * BRICK_HEIGHT;
            game->bricks[idx].width = BRICK_WIDTH - 1;
            game->bricks[idx].height = BRICK_HEIGHT - 1;
            game->bricks[idx].active = 1;
            idx++;
        }
    }

    game->score = 0;
    game->game_started = 0;
    game->game_over = 0;

    return game;
}

// Draw paddle
void draw_paddle(Paddle *paddle) {
    if (paddle == NULL) {
        return;
    }

    attron(COLOR_PAIR(1));
    for (int i = 0; i < paddle->width; i++) {
        mvaddch(paddle->y, paddle->x + i, PADDLE_CHAR);
    }
    attroff(COLOR_PAIR(1));
}

// Draw ball
void draw_ball(Ball *ball) {
    if (ball == NULL || !ball->active) {
        return;
    }

    attron(COLOR_PAIR(2));
    mvaddch(ball->y, ball->x, BALL_CHAR);
    attroff(COLOR_PAIR(2));
}

// Draw bricks
void draw_bricks(Brick *bricks, int num_bricks) {
    if (bricks == NULL) {
        return;
    }

    attron(COLOR_PAIR(3));
    for (int i = 0; i < num_bricks; i++) {
        if (bricks[i].active) {
            for (int y = 0; y < bricks[i].height; y++) {
                for (int x = 0; x < bricks[i].width; x++) {
                    mvaddch(bricks[i].y + y, bricks[i].x + x, BRICK_CHAR);
                }
            }
        }
    }
    attroff(COLOR_PAIR(3));
}

// Check collision with a brick
int check_brick_collision(Ball *ball, Brick *bricks, int num_bricks, int *score) {
    if (ball == NULL || bricks == NULL || score == NULL) {
        return 0;
    }

    for (int i = 0; i < num_bricks; i++) {
        if (bricks[i].active) {
            if (ball->x >= bricks[i].x &&
                ball->x < bricks[i].x + bricks[i].width &&
                ball->y >= bricks[i].y &&
                ball->y < bricks[i].y + bricks[i].height) {

                bricks[i].active = 0;
                (*score) += 10;

                int prev_x = ball->x - ball->dx;
                int prev_y = ball->y - ball->dy;

                if (prev_x < bricks[i].x || prev_x >= bricks[i].x + bricks[i].width) {
                    ball->dx = -ball->dx;
                }
                if (prev_y < bricks[i].y || prev_y >= bricks[i].y + bricks[i].height) {
                    ball->dy = -ball->dy;
                }

                return 1;
            }
        }
    }
    return 0;
}

// Check collision with the paddle
void check_paddle_collision(Ball *ball, Paddle *paddle) {
    if (ball == NULL || paddle == NULL) {
        return;
    }

    if (ball->y == paddle->y - 1 &&
        ball->x >= paddle->x &&
        ball->x < paddle->x + paddle->width) {

        ball->dy = -ball->dy;

        // Change horizontal direction based on hit position
        int hit_pos = ball->x - paddle->x;
        if (hit_pos < paddle->width / 3) {
            ball->dx = -1;
        } else if (hit_pos > 2 * paddle->width / 3) {
            ball->dx = 1;
        }
    }
}

// Update ball position and state
void update_ball(Game *game) {
    if (game == NULL || !game->ball.active) {
        return;
    }

    Ball *ball = &game->ball;

    // Move the ball
    ball->x += ball->dx;
    ball->y += ball->dy;

    // Collision with side walls
    if (ball->x <= 0 || ball->x >= game->max_x - 1) {
        ball->dx = -ball->dx;
    }

    // Collision with ceiling
    if (ball->y <= 0) {
        ball->dy = -ball->dy;
        ball->y = 1;
    }

    // Check if the ball hits the bottom
    if (ball->y >= game->max_y - 1) {
        game->game_over = 1;
        ball->active = 0;
        return;
    }

    // Check collisions
    check_paddle_collision(ball, &game->paddle);
    check_brick_collision(ball, game->bricks, game->num_bricks, &game->score);
}

// Check if all bricks are destroyed
int all_bricks_destroyed(Brick *bricks, int num_bricks) {
    for (int i = 0; i < num_bricks; i++) {
        if (bricks[i].active) {
            return 0;
        }
    }
    return 1;
}

// Draw frame
void draw_frame(Game *game) {
    erase();

    // Draw borders
    for (int i = 0; i < game->max_x; i++) {
        mvaddch(0, i, '-');
        mvaddch(game->max_y - 1, i, '-');
    }
    for (int i = 0; i < game->max_y; i++) {
        mvaddch(i, 0, '|');
        mvaddch(i, game->max_x - 1, '|');
    }

    // Display score
    attron(A_BOLD);
    mvprintw(1, 2, "SCORE: %d", game->score);
    attroff(A_BOLD);

    // Draw game elements
    draw_bricks(game->bricks, game->num_bricks);
    draw_paddle(&game->paddle);
    draw_ball(&game->ball);

    // Messages
    if (!game->game_started) {
        mvprintw(game->max_y / 2, game->max_x / 2 - 15, "Press SPACE to start");
    }

    if (game->game_over) {
        mvprintw(game->max_y / 2, game->max_x / 2 - 10, "GAME OVER!");
        mvprintw(game->max_y / 2 + 1, game->max_x / 2 - 12, "Press 'q' to quit");
    }

    if (all_bricks_destroyed(game->bricks, game->num_bricks)) {
        game->game_over = 1;
        mvprintw(game->max_y / 2, game->max_x / 2 - 8, "YOU WIN!");
        mvprintw(game->max_y / 2 + 1, game->max_x / 2 - 12, "Press 'q' to quit");
    }

    refresh();
}

// Main function
int main() {
    // Initialize ncurses
    initscr();

    if (!has_colors()) {
        endwin();
        fprintf(stderr, "Terminal does not support colors\n");
        return 1;
    }

    start_color();

    // Initialize color pairs
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);

    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    // Initialize game
    Game *game = init_game();
    if (game == NULL) {
        endwin();
        return 1;
    }

    srand(time(NULL));

    // Main game loop
    int ch;
    while (1) {
        ch = getch();

        if (ch == 'q' || ch == 'Q') {
            break;
        }

        if (ch == ' ' && !game->game_started) {
            game->game_started = 1;
            game->ball.active = 1;
        }

        if (game->game_started && !game->game_over) {
            if (ch == KEY_LEFT) {
                game->paddle.x -= 5;
                if (game->paddle.x < 1) {
                    game->paddle.x = 1;
                }
            }
            if (ch == KEY_RIGHT) {
                game->paddle.x += 5;
                if (game->paddle.x > game->max_x - game->paddle.width - 1) {
                    game->paddle.x = game->max_x - game->paddle.width - 1;
                }
            }
        }

        if (game->game_started && !game->game_over) {
            update_ball(game);
        }

        draw_frame(game);
        usleep(DELAY);
    }

    // Cleanup
    free(game->bricks);
    free(game);
    endwin();

    return 0;
}
