#include <raylib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define ROWS 20
#define COLS 20
#define GRID_SIZE 40

#define WIDTH (COLS * GRID_SIZE)
#define HEIGHT (ROWS * GRID_SIZE)

typedef struct Node
{
    int x, y;
    bool wall;
    bool visited;
    struct Node* parent;
} Node;

Node* grid[ROWS][COLS];
Node* startNode = NULL;
Node* endNode = NULL;

void generateMaze()
{
    for(int i = 0; i < ROWS; i++)
    {
        for(int j = 0; j < COLS; j++)
        {
            grid[i][j] = (Node *)malloc(sizeof(Node));
            grid[i][j]->x = i;
            grid[i][j]->y = j;
            grid[i][j]->wall = (rand() % 5 == 0);
            grid[i][j]->visited = false;
            grid[i][j]->parent = NULL;
        }
    }

    startNode = grid[2][1];
    endNode = grid[ROWS - 2][COLS - 1];
    startNode->wall = false;
    endNode->wall = false;
}

void drawMaze()
{
    for (int i = 0; i < ROWS; i++)
    {
        for(int j = 0; j < COLS; j++)
        {
            Color color = GRAY;

            if(grid[i][j]->wall)
            {
                color = BROWN;
            }
            else if(grid[i][j] == startNode)
            {
                color = BLUE;
            }
            else if(grid[i][j] == endNode)
            {
                color = RED;
            }


            DrawRectangle(i * GRID_SIZE, j * GRID_SIZE, GRID_SIZE - 1, GRID_SIZE - 1, color);
        }
    }
}

int main(int argc, char *argv[])
{
    InitWindow(WIDTH, HEIGHT, "Maze Solver");
    SetTargetFPS(60);

    generateMaze();

    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        drawMaze();
        EndDrawing();
    }

    return EXIT_SUCCESS;
}
