/* Если клетка мертвая, а вокруг нее ровно три живые, то она становится в следующем поколении живой.
   Если она живая, и у нее есть 2 или 3 живые соседки, она остается живой, в противном случае умирает.
*/

//check [4][5]

#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#pragma warning (disable : 4996)

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Size of desk
#define NUM_CELL_X 5
#define NUM_CELL_Y 5


//SDL things
//Starts up SDL and creates window
int init();
//Frees media and shuts down SDL
void closer();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;


// Структура одной клетки с текущим и будущим состояниями: 0 - мертва, 1 - жива
struct {
    unsigned short state_1;
    unsigned short state_2;
} typedef cell;


int init()
{
    //Initialization flag
    int success = 1;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = 0;
    }
    else
    {
        //Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            printf("Warning: Linear texture filtering not enabled!");
        }
        //Create window
        gWindow = SDL_CreateWindow("Conway's game of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = 0;
        }
        else
        {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == NULL)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = 0;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            }
        }
    }

    return success;
}

void closer()
{
    //Destroy window	
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

void printDesk(cell** desk, unsigned short state) {

    cell* p_desk = (cell*)desk;

    //Clear screen
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);


    //Draw gray vertical lines
    SDL_SetRenderDrawColor(gRenderer, 0x80, 0x80, 0x80, 0xFF);

    for (int i = 1; i < 5; i++) {
        SDL_RenderDrawLine(gRenderer, i * (SCREEN_WIDTH / 5), 0, i * (SCREEN_WIDTH / 5), SCREEN_HEIGHT);
        SDL_RenderDrawLine(gRenderer, 0, i * (SCREEN_HEIGHT / 5), SCREEN_WIDTH, i * (SCREEN_HEIGHT / 5));
    }

    if (state == 1) {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (p_desk[j * NUM_CELL_Y + i].state_1 == 1) {
                    SDL_Rect fillRect = { i * (SCREEN_WIDTH / 5), j * (SCREEN_HEIGHT / 5), SCREEN_WIDTH / 5, SCREEN_HEIGHT / 5 };
                    SDL_SetRenderDrawColor(gRenderer, 0x80, 0x80, 0x80, 0xFF);
                    SDL_RenderFillRect(gRenderer, &fillRect);
                }
            }
        }
    }
    else if (state == 2) {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (p_desk[j * NUM_CELL_Y + i].state_2 == 1) {
                    SDL_Rect fillRect = { i * (SCREEN_WIDTH / 5), j * (SCREEN_HEIGHT / 5), SCREEN_WIDTH / 5, SCREEN_HEIGHT / 5 };
                    SDL_SetRenderDrawColor(gRenderer, 0x80, 0x80, 0x80, 0xFF);
                    SDL_RenderFillRect(gRenderer, &fillRect);
                }
            }
        }
    }
    else {
        printf("Conway error\n");
        exit(3);
    }

    //Update screen
    SDL_RenderPresent(gRenderer);
}


unsigned short Conway(cell** desk, unsigned int height, unsigned int width, unsigned short state){

    unsigned int i;
    unsigned int j;

    cell *p_desk = (cell *)desk;

    unsigned short is_cell_alive = 0;
    unsigned short neighbors_alive = 0;//количество живых соседей
    if(state == 1) {
        for (i = 0; i < height; ++i) {
            for (j = 0; j < width; ++j) {
                if(i >= 1 && j >= 1){
                    neighbors_alive += p_desk[(j - 1) * height + (i - 1)].state_1;
                }
                if(i >= 1){
                    neighbors_alive += p_desk[j * height + (i - 1)].state_1;
                }
                if(i >= 1 && j < (width - 1)){
                    neighbors_alive += p_desk[(j + 1) * height + (i - 1)].state_1;
                }
                if(j >= 1){
                    neighbors_alive += p_desk[(j - 1) * height + i].state_1;
                }
                if(j < (width - 1)){
                    neighbors_alive += p_desk[(j + 1) * height + i].state_1;
                }
                if(i < (height - 1) && j >= 1){
                    neighbors_alive += p_desk[(j - 1) * height + (i + 1)].state_1;
                }
                if(i < (height - 1)){
                    neighbors_alive += p_desk[j * height + (i + 1)].state_1;
                }
                if(i < (height - 1) && j < (width - 1)){
                    neighbors_alive += p_desk[(j + 1) * height + (i + 1)].state_1;
                }

                is_cell_alive = p_desk[j * height + i].state_1;
                if(is_cell_alive){
                    if(neighbors_alive == 2 || neighbors_alive == 3){
                        p_desk[j * height + i].state_2 = 1;
                    } else{
                        p_desk[j * height + i].state_2 = 0;
                    }
                } else{
                    if(neighbors_alive == 3){
                        p_desk[j * height + i].state_2 = 1;
                    } else{
                        p_desk[j * height + i].state_2 = 0;
                    }
                }
                neighbors_alive = 0;
            }
        }
        return 2; // Какое поколение стало новым, и которое нужно напечатать
    } else if(state == 2){
        for (i = 0; i < height; ++i) {
            for (j = 0; j < width; ++j) {
                if(i >= 1 && j >= 1){
                    neighbors_alive += p_desk[(j - 1) * height + (i - 1)].state_2;
                }
                if(i >= 1){
                    neighbors_alive += p_desk[j * height + (i - 1)].state_2;
                }
                if(i >= 1 && j < (width - 1)){
                    neighbors_alive += p_desk[(j + 1) * height + (i - 1)].state_2;
                }
                if(j >= 1){
                    neighbors_alive += p_desk[(j - 1) * height + i].state_2;
                }
                if(j < (width - 1)){
                    neighbors_alive += p_desk[(j + 1) * height + i].state_2;
                }
                if(i < (height - 1) && j >= 1){
                    neighbors_alive += p_desk[(j - 1) * height + (i + 1)].state_2;
                }
                if(i < (height - 1)){
                    neighbors_alive += p_desk[j * height + (i + 1)].state_2;
                }
                if(i < (height - 1) && j < (width - 1)){
                    neighbors_alive += p_desk[(j + 1) * height + (i + 1)].state_2;
                }

                is_cell_alive = p_desk[j * height + i].state_2;
                if(is_cell_alive){
                    if(neighbors_alive == 2 || neighbors_alive == 3){
                        p_desk[j * height + i].state_1 = 1;
                    } else{
                        p_desk[j * height + i].state_1 = 0;
                    }
                } else{
                    if(neighbors_alive == 3){
                        p_desk[j * height + i].state_1 = 1;
                    } else{
                        p_desk[j * height + i].state_1 = 0;
                    }
                }
                neighbors_alive = 0;
            }
        }
        return 1; // Какое поколение стало новым, и которое нужно напечатать
    } else{
        printf("Conway error\n");
        exit(3);
    }
}


//Рисовка начального состояния игрального поля
void startDraw(cell** desk){

    unsigned short finishStartPos = 0;
    unsigned short quit = 0;
    SDL_Event e;
    cell* p_desk = (cell*)desk;

    while (!finishStartPos && !quit) {

        //Handle events on queue
        while (SDL_PollEvent(&e) != 0){
            
            //User requests quit
            if (e.type == SDL_QUIT){
                quit = 1;
            }
            //User finished his start draw
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_RIGHT) {
                finishStartPos = 1;
            }
            //User clicked in cell
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                int x, y, cellX, cellY;
                SDL_GetMouseState(&x, &y);
                for (int i = 0; i < 5; i++) {
                    if (x < (i + 1) * SCREEN_WIDTH / 5) {
                        cellX = i;
                        break;
                    }
                }
                for (int i = 0; i < 5; i++) {
                    if (y < (i + 1) * SCREEN_HEIGHT / 5) {
                        cellY = i;
                        break;
                    }
                }
                if (p_desk[cellY * NUM_CELL_X + cellX].state_1 == 1) p_desk[cellY * NUM_CELL_X + cellX].state_1 = 0;
                else p_desk[cellY * NUM_CELL_X + cellX].state_1 = 1;
            }
        }
        printDesk((cell**)desk, 1);
    }

}

gameLoop(cell** desk) {

    cell* p_desk = (cell*)desk;
    unsigned short quit = 0;
    SDL_Event e;
    unsigned short state = 1;

    while (!quit) {

        //Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            //User requests quit
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
            if (quit != 1) {
                state = Conway((cell**)desk, NUM_CELL_X, NUM_CELL_Y, state);
                printDesk((cell**)desk, state);
                SDL_Delay(2000); //Could be not default
            }
        }
    }
    
}

fillingZeros(cell** desk) {

    cell* p_desk = (cell*)desk;

    for (int i = 0; i < NUM_CELL_X;i++) {
        for (int j = 0; j < NUM_CELL_Y; j++) {
            p_desk[j * NUM_CELL_Y + i].state_1 = 0;
            p_desk[j * NUM_CELL_Y + i].state_2 = 0;
        }
    }
}

// Меню
int main() {
    //Start up SDL and create window
    if (!init()){
        printf("Failed to initialize!\n");
    }
    else {
        //desk for play
        cell desk[NUM_CELL_X][NUM_CELL_Y];
        //fil with 0s
        //это приведение к неконстантному указателю?
        fillingZeros((cell**)desk);
        startDraw((cell**)desk);
        gameLoop((cell**)desk);
    }
    //Free resources and close SDL
    closer();
    return 0;
}