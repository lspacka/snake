//  -lraylib -lgdi32 -lwinmm -Wall -std=c99 -I c:/raylib/raylib/src

//  - change speed to vector

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "raylib.h"
#include "raymath.h"

typedef  struct Snake {
    Vector2 pos;
    float xspeed;
    float yspeed;
    int capacity;
    Vector2* tail ;
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
    SetTargetFPS(13);

    int i;
    int scale = 20;
    int total = 0;
    // int capacity = 100;
    int speed = 1;
    Snake snake = {0};
    snake.capacity = (width/scale) * (height/scale);
    snake.tail = malloc(snake.capacity * sizeof(Vector2));
    Vector2 food = {0};
    // Color pinky = {233, 30, 99};

    snake.xspeed = speed;
    snake.yspeed = 0;

    PickLocation(&food, width, height, scale);

    while (!WindowShouldClose()) {
        if (Vector2Distance(snake.pos, food) < 1) {
            PickLocation(&food, width, height, scale);
            total++;
            // size_t new_size = total * sizeof(Vector2);
            // snake.tail = realloc(snake.tail, new_size);
            // if (total > snake.capacity) {
            //     snake.capacity *= 2;
            //     size_t new_size = snake.capacity * sizeof(Vector2);
            //     snake.tail = realloc(snake.tail, new_size);
            // }

            //  check tail array resizing
            // if (snake.tail == NULL) 
            //     printf("tail>0 NULL\n\n");
            // else
            //     printf("%zu\n", new_size);

            // init new tail segment to current head position
            if (total > 0) 
                snake.tail[total-1] = snake.pos;

            // PickLocation(&food, width, height, scale);
        }

        // growing logic
        // shift tail segments, excluding the last one
        for (i = 0; i < total-1; i++) {
            snake.tail[i] = snake.tail[i+1];
        }

        // update last tail segment with current head position
        if (total >= 1) {
            Vector2 tail_piece = {snake.pos.x, snake.pos.y};
            snake.tail[total-1] = tail_piece;
        }

        // for (i = 0; i < total; i ++) {
        //     DrawRectangle(snake.tail[i].x+1, snake.tail[i].y, scale, scale, RAYWHITE);
        // }

        // if snake dies, free(snake.tail)

        // update position
        snake.pos.x += snake.xspeed * scale;
        snake.pos.y += snake.yspeed * scale;

        snake.pos.x = Clamp(snake.pos.x, 0, width-scale);
        snake.pos.y = Clamp(snake.pos.y, 0, height-scale);        

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