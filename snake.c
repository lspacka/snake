//  -lraylib -lgdi32 -lwinmm -Wall -std=c99 -I c:/raylib/raylib/src

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "raylib.h"
#include "raymath.h"

typedef  struct Snake {
    Vector2 pos;
    float xspeed;
    float yspeed;
    // Snake* tail;
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
    SetTargetFPS(8);

    int scale = 20;
    int total = 1;
    float speed = 1.0f;
    Snake snake = {0};
    // Snake* snake = malloc(total * sizeof(Vector2));
    Snake* tail = malloc(total * sizeof(Vector2));
    Vector2 food = {0};
    // Color pinky = {233, 30, 99};

    snake.xspeed = speed;
    snake.yspeed = 0;

    PickLocation(&food, width, height, scale);

    while (!WindowShouldClose()) {
        snake.pos.x += snake.xspeed * scale;
        snake.pos.y += snake.yspeed * scale;

        snake.pos.x = Clamp(snake.pos.x, 0, width-scale);
        snake.pos.y = Clamp(snake.pos.y, 0, height-scale);        

        SnakeDir(&snake, speed);

        if (Vector2Distance(snake.pos, food) < 1) {
            total++;
            size_t new_size = total * sizeof(Vector2);
            tail = realloc(tail, new_size);
            // if (tail == NULL) 
            //     printf("tail>0 NULL\n\n");
            // else
            //     printf("%zu\n", new_size);

            PickLocation(&food, width, height, scale);
        }

        // update tail pos

        BeginDrawing();
        ClearBackground(BLACK);

        DrawRectangle(snake.pos.x, snake.pos.y, scale, scale, RAYWHITE);
        DrawRectangle(food.x, food.y, scale, scale, GREEN);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

void SnakeDir(Snake* snake, float speed)
{
    if (IsKeyPressed(KEY_DOWN)) {
        snake->xspeed = 0;
        snake->yspeed = speed;
    } else if (IsKeyPressed(KEY_UP)) {
        snake->xspeed = 0;
        snake->yspeed = -speed;
    } else if (IsKeyPressed(KEY_LEFT)) {
        snake->xspeed = -speed;
        snake->yspeed = 0;
    } else if (IsKeyPressed(KEY_RIGHT)) {
        snake->xspeed = speed;
        snake->yspeed = 0;
    }
}

void PickLocation(Vector2* food, int width, int height, int scale)
{
    int cols = width / scale;
    int rows = height / scale;

    food->x = (rand() % cols) * scale;
    food->y = (rand() % rows) * scale;
}