#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 80
#define HEIGHT 25
#define PADDLE_SIZE 3
#define WINNING_SCORE 21

typedef struct {
    int x, y;
    int dx, dy;
} Ball;

typedef struct {
    int y;
    int score;
} Paddle;

typedef struct {
    Ball ball;
    Paddle left_paddle;
    Paddle right_paddle;
    int game_over;
} GameState;

void initialize_game(GameState *game) {
    // Инициализация мяча
    game->ball.x = WIDTH / 2;
    game->ball.y = HEIGHT / 2;
    game->ball.dx = 1;
    game->ball.dy = 1;
    
    // Инициализация ракеток
    game->left_paddle.y = HEIGHT / 2 - PADDLE_SIZE / 2;
    game->right_paddle.y = HEIGHT / 2 - PADDLE_SIZE / 2;
    
    game->left_paddle.score = 0;
    game->right_paddle.score = 0;
    
    game->game_over = 0;
}

void draw_game(const GameState *game) {
    system("cls"); // Очистка экрана 
    
    // Верхняя граница
    for (int i = 0; i < WIDTH + 2; i++) {
        printf("#");
    }
    printf("\n");
    
    // Игровое поле
    for (int y = 0; y < HEIGHT; y++) {
        printf("#"); // Левая граница
        
        for (int x = 0; x < WIDTH; x++) {
            // Отрисовка левой ракетки
            if (x == 1 && y >= game->left_paddle.y && y < game->left_paddle.y + PADDLE_SIZE) {
                printf("|");
            }
            // Отрисовка правой ракетки
            else if (x == WIDTH - 2 && y >= game->right_paddle.y && y < game->right_paddle.y + PADDLE_SIZE) {
                printf("|");
            }
            // Отрисовка мяча
            else if (x == game->ball.x && y == game->ball.y) {
                printf("O");
            }
            // Отрисовка сетки (центральная линия)
            else if (x == WIDTH / 2) {
                printf(".");
            }
            // Пустое пространство
            else {
                printf(" ");
            }
        }
        
        printf("#\n"); // Правая граница
    }
    
    // Нижняя граница
    for (int i = 0; i < WIDTH + 2; i++) {
        printf("#");
    }
    printf("\n");
    
    // Счет
    printf("Player 1: %d\tPlayer 2: %d\n", game->left_paddle.score, game->right_paddle.score);
    printf("Controls: A/Z - Player 1, K/M - Player 2, Space - Skip\n");
}

void update_ball(GameState *game) {
    // Движение мяча
    game->ball.x += game->ball.dx;
    game->ball.y += game->ball.dy;
    
    // Отскок от верхней и нижней границ
    if (game->ball.y <= 0 || game->ball.y >= HEIGHT - 1) {
        game->ball.dy = -game->ball.dy;
    }
    
    // Проверка столкновения с левой ракеткой
    if (game->ball.x == 2 && 
        game->ball.y >= game->left_paddle.y && 
        game->ball.y < game->left_paddle.y + PADDLE_SIZE) {
        game->ball.dx = 1;
    }
    
    // Проверка столкновения с правой ракеткой
    if (game->ball.x == WIDTH - 3 && 
        game->ball.y >= game->right_paddle.y && 
        game->ball.y < game->right_paddle.y + PADDLE_SIZE) {
        game->ball.dx = -1;
    }
    
    // Проверка забития гола
    if (game->ball.x <= 0) {
        game->right_paddle.score++;
        // Сброс мяча
        game->ball.x = WIDTH / 2;
        game->ball.y = HEIGHT / 2;
        game->ball.dx = 1;
        game->ball.dy = 1;
    }
    
    if (game->ball.x >= WIDTH - 1) {
        game->left_paddle.score++;
        // Сброс мяча
        game->ball.x = WIDTH / 2;
        game->ball.y = HEIGHT / 2;
        game->ball.dx = -1;
        game->ball.dy = 1;
    }
    
    // Проверка условия победы
    if (game->left_paddle.score >= WINNING_SCORE || game->right_paddle.score >= WINNING_SCORE) {
        game->game_over = 1;
    }
}

void process_input(GameState *game) {
    char input;
    int valid_input = 0;
    
    while (!valid_input) {
        printf("Enter command: ");
        input = getchar();
        
        // Очистка буфера ввода
        while (getchar() != '\n');
        
        switch (input) {
            case 'a': case 'A': // Движение левой ракетки вверх
                if (game->left_paddle.y > 0) {
                    game->left_paddle.y--;
                    valid_input = 1;
                }
                break;
                
            case 'z': case 'Z': // Движение левой ракетки вниз
                if (game->left_paddle.y < HEIGHT - PADDLE_SIZE) {
                    game->left_paddle.y++;
                    valid_input = 1;
                }
                break;
                
            case 'k': case 'K': // Движение правой ракетки вверх
                if (game->right_paddle.y > 0) {
                    game->right_paddle.y--;
                    valid_input = 1;
                }
                break;
                
            case 'm': case 'M': // Движение правой ракетки вниз
                if (game->right_paddle.y < HEIGHT - PADDLE_SIZE) {
                    game->right_paddle.y++;
                    valid_input = 1;
                }
                break;
                
            case ' ': // Пропуск хода
                valid_input = 1;
                break;
                
            default:
                printf("Invalid input! Use A/Z for Player 1, K/M for Player 2, Space to skip.\n");
                break;
        }
    }
}

void display_winner(const GameState *game) {
    system("cls");
    printf("########################################\n");
    printf("#             GAME OVER               #\n");
    printf("########################################\n\n");
    
    if (game->left_paddle.score >= WINNING_SCORE) {
        printf("Congratulations Player 1! You won with score %d:%d!\n", 
               game->left_paddle.score, game->right_paddle.score);
    } else {
        printf("Congratulations Player 2! You won with score %d:%d!\n", 
               game->left_paddle.score, game->right_paddle.score);
    }
    
    printf("\nThanks for playing!\n");
}

int main() {
    GameState game;
    
    initialize_game(&game);
    
    printf("Welcome to ASCII Pong!\n");
    printf("First player to reach %d points wins!\n", WINNING_SCORE);
    printf("Press Enter to start...");
    getchar();
    
    while (!game.game_over) {
        draw_game(&game);
        process_input(&game);
        update_ball(&game);
    }
    
    display_winner(&game);
    
    return 0;
}
