/* Если клетка мертвая, а вокруг нее ровно три живые, то она становится в следующем поколении живой.
   Если она живая, и у нее есть 2 или 3 живые соседки, она остается живой, в противном случае умирает.
*/
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#pragma warning (disable : 4996)

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


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

void printDesk(cell* desk, unsigned short height, unsigned short width, unsigned short state) {

    //Clear screen
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);


    //Draw gray vertical & horisontal lines
    SDL_SetRenderDrawColor(gRenderer, 0x80, 0x80, 0x80, 0xFF);

    for (int i = 1; i <= height + 1 ; i++) {
        SDL_RenderDrawLine(gRenderer, i * (SCREEN_WIDTH / width), 0, i * (SCREEN_WIDTH / width), SCREEN_HEIGHT);
    }

    for (int i = 1; i <= width + 1; i++) {
        SDL_RenderDrawLine(gRenderer, 0, i * (SCREEN_HEIGHT / height), SCREEN_WIDTH, i * (SCREEN_HEIGHT / height));
    }


    if (state == 1) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (desk[i * width + j].state_1 == 1) {
                    SDL_Rect fillRect = { j * (SCREEN_WIDTH / width), i * (SCREEN_HEIGHT / height), SCREEN_WIDTH / width, SCREEN_HEIGHT / height };
                    SDL_SetRenderDrawColor(gRenderer, 0x80, 0x80, 0x80, 0xFF);
                    SDL_RenderFillRect(gRenderer, &fillRect);
                }
            }
        }
    }
    else if (state == 2) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (desk[i * width+ j].state_2 == 1) {
                    SDL_Rect fillRect = { j * (SCREEN_WIDTH / width), i * (SCREEN_HEIGHT / height), SCREEN_WIDTH / width, SCREEN_HEIGHT / height };
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


// Алгоритм, принимающий доску в состоянии state_n -- предки,
// и заполняющий на его основе state_k -- потомки. Возвращает переменную state, в которой номер k
// полученных потомков - 1 или 2 (для печати)
unsigned short Conway(cell* desk, unsigned short height, unsigned short width, unsigned short state) {
    unsigned int i;
    unsigned int j;

    unsigned short is_cell_alive = 0;
    unsigned short neighbors_alive = 0;
    if (state == 1) {
        for (i = 0; i < height; ++i) {
            for (j = 0; j < width; ++j) {

                //North West cell
                if (i >= 1 && j >= 1) {
                    neighbors_alive += desk[(i - 1) * width + (j - 1)].state_1;
                }
                else if (i == 0 && j == 0) {
                    neighbors_alive += desk[(height - 1) * width + (width - 1)].state_1;
                }
                else if (i == 0 && j >= 1) {
                    neighbors_alive += desk[(height - 1) * width + (j - 1)].state_1;
                }
                else if (i >= 1 && j == 0) {
                    neighbors_alive += desk[(i - 1) * width + (width - 1)].state_1;
                }

                //North
                if (i >= 1) {
                    neighbors_alive += desk[(i - 1) * width + j].state_1;
                }
                else if (i == 0) {
                    neighbors_alive += desk[(height - 1) * width + j].state_1;
                }

                //North East
                if (i >= 1 && j < (width - 1)) {
                    neighbors_alive += desk[(i - 1) * width + (j + 1)].state_1;
                }
                else if (i == 0 && j == (width - 1)) {
                    neighbors_alive += desk[(height - 1) * width].state_1;
                }
                else if (i == 0 && j < (width - 1)) {
                    neighbors_alive += desk[(height - 1) * width + (j + 1)].state_1;
                }
                else if (i >= 1 && j == (width - 1)) {
                    neighbors_alive += desk[(i - 1) * width].state_1;
                }

                //West
                if (j >= 1) {
                    neighbors_alive += desk[i * width + (j - 1)].state_1;
                }
                else if (j == 0) {
                    neighbors_alive += desk[i * width + (width - 1)].state_1;
                }

                //East
                if (j < (width - 1)) {
                    neighbors_alive += desk[i * width + (j + 1)].state_1;
                }
                else if (j == (width - 1)) {
                    neighbors_alive += desk[i * width].state_1;
                }

                //South West
                if (i < (height - 1) && j >= 1) {
                    neighbors_alive += desk[(i + 1) * width + (j - 1)].state_1;
                }
                else if (i == (height - 1) && j == 0) {
                    neighbors_alive += desk[width - 1].state_1;
                }
                else if (i == (height - 1) && j >= 1) {
                    neighbors_alive += desk[j - 1].state_1;
                }
                else if (i < (height - 1) && j == 0) {
                    neighbors_alive += desk[(i + 1) * width + (width - 1)].state_1;
                }

                //South
                if (i < (height - 1)) {
                    neighbors_alive += desk[(i + 1) * width + j].state_1;
                }
                else if (i == (height - 1)) {
                    neighbors_alive += desk[j].state_1;
                }

                //South East
                if (i < (height - 1) && j < (width - 1)) {
                    neighbors_alive += desk[(i + 1) * width + (j + 1)].state_1;
                }
                else if (i == (height - 1) && j == (width - 1)) {
                    neighbors_alive += desk[0].state_1;
                }
                else if (i == (height - 1) && j < (width - 1)) {
                    neighbors_alive += desk[j + 1].state_1;
                }
                else if (i < (height - 1) && j == (width - 1)) {
                    neighbors_alive += desk[(i + 1) * width].state_1;
                }


                is_cell_alive = desk[i * width + j].state_1;
                if (is_cell_alive) {
                    if (neighbors_alive == 2 || neighbors_alive == 3) {
                        desk[i * width + j].state_2 = 1;
                    }
                    else {
                        desk[i * width + j].state_2 = 0;
                    }
                }
                else {
                    if (neighbors_alive == 3) {
                        desk[i * width + j].state_2 = 1;
                    }
                    else {
                        desk[i * width + j].state_2 = 0;
                    }
                }
                neighbors_alive = 0;
            }
        }
        return 2; // Какое поколение стало новым, и которое нужно напечатать
    }
    else if (state == 2) {
        for (i = 0; i < height; ++i) {
            for (j = 0; j < width; ++j) {

                //North West cell
                if (i >= 1 && j >= 1) {
                    neighbors_alive += desk[(i - 1) * width + (j - 1)].state_2;
                }
                else if (i == 0 && j == 0) {
                    neighbors_alive += desk[(height - 1) * width + (width - 1)].state_2;
                }
                else if (i == 0 && j >= 1) {
                    neighbors_alive += desk[(height - 1) * width + (j - 1)].state_2;
                }
                else if (i >= 1 && j == 0) {
                    neighbors_alive += desk[(i - 1) * width + (width - 1)].state_2;
                }

                //North
                if (i >= 1) {
                    neighbors_alive += desk[(i - 1) * width + j].state_2;
                }
                else if (i == 0) {
                    neighbors_alive += desk[(height - 1) * width + j].state_2;
                }

                //North East
                if (i >= 1 && j < (width - 1)) {
                    neighbors_alive += desk[(i - 1) * width + (j + 1)].state_2;
                }
                else if (i == 0 && j == (width - 1)) {
                    neighbors_alive += desk[(height - 1) * width].state_2;
                }
                else if (i == 0 && j < (width - 1)) {
                    neighbors_alive += desk[(height - 1) * width + (j + 1)].state_2;
                }
                else if (i >= 1 && j == (width - 1)) {
                    neighbors_alive += desk[(i - 1) * width].state_2;
                }

                //West
                if (j >= 1) {
                    neighbors_alive += desk[i * width + (j - 1)].state_2;
                }
                else if (j == 0) {
                    neighbors_alive += desk[i * width + (width - 1)].state_2;
                }

                //East
                if (j < (width - 1)) {
                    neighbors_alive += desk[i * width + (j + 1)].state_2;
                }
                else if (j == (width - 1)) {
                    neighbors_alive += desk[i * width].state_2;
                }

                //South West
                if (i < (height - 1) && j >= 1) {
                    neighbors_alive += desk[(i + 1) * width + (j - 1)].state_2;
                }
                else if (i == (height - 1) && j == 0) {
                    neighbors_alive += desk[width - 1].state_2;
                }
                else if (i == (height - 1) && j >= 1) {
                    neighbors_alive += desk[j - 1].state_2;
                }
                else if (i < (height - 1) && j == 0) {
                    neighbors_alive += desk[(i + 1) * width + (width - 1)].state_2;
                }

                //South
                if (i < (height - 1)) {
                    neighbors_alive += desk[(i + 1) * width + j].state_2;
                }
                else if (i == (height - 1)) {
                    neighbors_alive += desk[j].state_2;
                }

                //South East
                if (i < (height - 1) && j < (width - 1)) {
                    neighbors_alive += desk[(i + 1) * width + (j + 1)].state_2;
                }
                else if (i == (height - 1) && j == (width - 1)) {
                    neighbors_alive += desk[0].state_2;
                }
                else if (i == (height - 1) && j < (width - 1)) {
                    neighbors_alive += desk[j + 1].state_2;
                }
                else if (i < (height - 1) && j == (width - 1)) {
                    neighbors_alive += desk[(i + 1) * width].state_2;
                }


                is_cell_alive = desk[i * width + j].state_2;
                if (is_cell_alive) {
                    if (neighbors_alive == 2 || neighbors_alive == 3) {
                        desk[i * width + j].state_1 = 1;
                    }
                    else {
                        desk[i * width + j].state_1 = 0;
                    }
                }
                else {
                    if (neighbors_alive == 3) {
                        desk[i * width + j].state_1 = 1;
                    }
                    else {
                        desk[i * width + j].state_1 = 0;
                    }
                }
                neighbors_alive = 0;
            }
        }
        return 1; // Какое поколение стало новым, и которое нужно напечатать
    }
    else {
        printf("Conway error\n");
        exit(3);
    }
}



//Рисовка начального состояния игрального поля
void startDraw(cell* desk, unsigned short height, unsigned short width){

    unsigned short finishStartPos = 0;
    unsigned short quit = 0;
    SDL_Event e;

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
                for (int i = 0; i < width; i++) {
                    if (x < (i + 1) * SCREEN_WIDTH / width) {
                        cellX = i;
                        break;
                    }
                }
                for (int i = 0; i < height; i++) {
                    if (y < (i + 1) * SCREEN_HEIGHT / height) {
                        cellY = i;
                        break;
                    }
                }
                if (desk[cellY * width + cellX].state_1 == 1) desk[cellY * width + cellX].state_1 = 0;
                else desk[cellY * width + cellX].state_1 = 1;
            }
        }
        printDesk(desk, height, width, 1);
    }

}

void gameLoop(cell* desk, unsigned short height, unsigned short width, unsigned short delay) {

    unsigned short quit = 0;
    SDL_Event e;
    unsigned short state = 1;

    while (!quit) {

        //Handle events on queue
        while (1) {

            SDL_PollEvent(&e);
            //User requests quit
            if (e.type == SDL_QUIT) {
                quit = 1;
                break;
            }
            if (quit != 1){
                state = Conway(desk, height, width, state);
                printDesk(desk, height, width, state);
                SDL_Delay(delay);
            }
        }
    }
}

void fillingZeros(cell* desk, unsigned short height, unsigned short width) {

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            desk[i * width + j].state_1 = 0;
            desk[i * width + j].state_2 = 0;
        }
    }
}

// Загрузка начального состояния игрального поля
void Start() {
    unsigned short height;
    unsigned short width;
    unsigned short delay;

    printf("Height: ");
    scanf("%hd", &height);
    printf("Width: ");
    scanf("%hd", &width);
    printf("Delay: ");
    scanf("%hd", &delay);


    cell* desk = NULL;
    desk = (cell*)malloc(height * width * sizeof(cell));

    fillingZeros(desk, height, width);

    startDraw(desk, height, width);
    gameLoop(desk, height, width, delay*1000);



    free(desk);
    desk = NULL;
}




// Меню
int main() {
    //Start up SDL and create window
    if (!init()){
        printf("Failed to initialize!\n");
    }
    else {
        Start();
    }
    //Free resources and close SDL
    closer();
    return 0;
}