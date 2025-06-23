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
    Color color;
} Node;

Node* grid[ROWS][COLS];
Node* startNode = NULL;
Node* endNode = NULL;

Node* stack[ROWS * COLS];
int stackTop = -1;
bool searchComplete = false;

bool isStackEmpty()
{
    return stackTop < 0;
}

void push(Node* node)
{
    stack[++stackTop] = node;
}

Node* pop()
{
    if(isStackEmpty()) return NULL;
    return stack[stackTop--];
}

bool isInBounds(int x, int y)
{
    return x >= 0 && y >= 0 && x < ROWS && y < COLS;
}

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
            grid[i][j]->color = grid[i][j]->wall ? BROWN : GRAY;
        }
    }

    startNode = grid[2][1];
    endNode = grid[ROWS - 2][COLS - 1];
    startNode->wall = false;
    endNode->wall = false;
    startNode->color = BLUE;
    endNode->color = RED;

    push(startNode);
}

void drawMaze()
{
    for (int i = 0; i < ROWS; i++)
    {
        for(int j = 0; j < COLS; j++)
        {
            DrawRectangle(i * GRID_SIZE, j * GRID_SIZE, GRID_SIZE - 1, GRID_SIZE - 1, grid[i][j]->color);
        }
    }
}

void depthFirstSearch()
{
    if(searchComplete || isStackEmpty()) return;

    Node* current = pop();
    if(!current || current->visited || current->wall) return;
    
    current->visited = true;

    if(current == endNode)
    {
        searchComplete = true;
        return;
    }
    
    int dx[] = {0, 1, 0, -1};
    int dy[] = {-1, 0, 1, 0};

    for(int i = 0; i < 4; i++)
    {
        int nx = current->x + dx[i];
        int ny = current->y + dy[i];

        if(!isInBounds(nx, ny)) return;

        Node* neighbour = grid[nx][ny];

        if(!neighbour->visited && !neighbour->wall)
        {
            neighbour->parent = current;
            push(neighbour);
            neighbour->color = YELLOW;
        }
    }
}

void drawPath(Node* node)
{
    Node* current = node;
    while(current != NULL && current != startNode)
    {
        DrawRectangle(current->x * GRID_SIZE, current->y * GRID_SIZE, GRID_SIZE - 1 , GRID_SIZE - 1, GREEN);
        current = current->parent;
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
        
        depthFirstSearch();
        drawMaze();

        if(searchComplete)
        {
            drawPath(endNode);
        }
        
        EndDrawing();

        usleep(1000000);
    }

    return EXIT_SUCCESS;
}
