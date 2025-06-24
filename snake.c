//  -lraylib -lgdi32 -lwinmm -Wall -std=c99 -I c:/raylib/raylib/src

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "raylib.h"
#include "raymath.h"

typedef  struct Snake {
    Vector2 pos;
    Vector2 speed;
    Vector2* tail;
    int capacity;
} Snake;

void SnakeDir(Snake* snake, float speed);
void PickLocation(Vector2* food, int width, int height, int scale);

int main()
{
    srand(time(NULL));
    int width = 700;
    int height = 700;

    SetTraceLogLevel(LOG_WARNING);
    InitWindow(width, height, "Snake");
    SetTargetFPS(12);

    int i;
    int scale = 20;
    int total = 0;
    int speed = 1;
    Snake snake = {0};
    snake.capacity = (width/scale) * (height/scale);
    snake.tail = malloc(snake.capacity * sizeof(Vector2));
    Vector2 food = {0};
    Vector2 tail_piece;
    snake.speed = (Vector2){speed, 0};

    PickLocation(&food, width, height, scale);

    while (!WindowShouldClose()) {
        if (Vector2Distance(snake.pos, food) < 1) {
            total++;

            // init new tail segment to current head position
            if (total > 0) 
                snake.tail[total-1] = snake.pos;

            PickLocation(&food, width, height, scale);
            // printf("total: %d\n", total);
        }

        // growing logic
        // shift tail segments, excluding the last one
        for (i = 0; i < total-1; i++) {
            snake.tail[i] = snake.tail[i+1];
        }

        // update last tail segment with current head position
        if (total >= 1) {
            tail_piece = (Vector2){snake.pos.x, snake.pos.y};
            snake.tail[total-1] = tail_piece;
        }

        // update position
        snake.pos.x += snake.speed.x * scale;
        snake.pos.y += snake.speed.y * scale;

        snake.pos.x = Clamp(snake.pos.x, 0, width-scale);
        snake.pos.y = Clamp(snake.pos.y, 0, height-scale);  

        // game reset
        for (i = 0; i < total; i++) {
            Vector2 tail_pos = snake.tail[i];
            // if (Vector2Distance(snake.pos, tail_pos) < 1) {
            if (snake.pos.x == tail_pos.x && snake.pos.y == tail_pos.y) {
                total = 0;
                PickLocation(&food, width, height, scale);
                break;
            }
        }

        SnakeDir(&snake, speed);

        BeginDrawing();
        ClearBackground(BLACK);

        for (i = 0; i < total; i ++) 
            DrawRectangle(snake.tail[i].x, snake.tail[i].y, scale, scale, RAYWHITE);

        DrawRectangle(snake.pos.x, snake.pos.y, scale, scale, RAYWHITE);
        DrawRectangle(food.x, food.y, scale, scale, GREEN);

        EndDrawing();
    }

    free(snake.tail);
    CloseWindow();

    return 0;
}

void SnakeDir(Snake* snake, float speed)
{
    if (IsKeyPressed(KEY_DOWN)) 
        snake->speed = (Vector2){0, speed};
    else if (IsKeyPressed(KEY_UP)) 
        snake->speed = (Vector2){0, -speed};
    else if (IsKeyPressed(KEY_LEFT)) 
        snake->speed = (Vector2){-speed, 0};
    else if (IsKeyPressed(KEY_RIGHT)) 
        snake->speed = (Vector2){speed, 0};
}

void PickLocation(Vector2* food, int width, int height, int scale)
{
    int cols = width / scale;
    int rows = height / scale;

    food->x = (rand() % cols) * scale;
    food->y = (rand() % rows) * scale;
}
