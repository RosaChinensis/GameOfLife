/* Если клетка мертвая, а вокруг нее ровно три живые, то она становится в следующем поколении живой.
 * Если она живая, и у нее есть 2 или 3 живые соседки, она остается живой, в противном случае умирает.
*/

#include <stdio.h>
#include <stdlib.h>
//#define _CRT_SECURE_NO_WARNINGS

// Структура одной клетки с текущим и будущим состояниями: 0 - мертва, 1 - жива
struct {
    unsigned short state_1;
    unsigned short state_2;
} typedef cell;

// Процедура, печатающая доску в состоянии state = 1 - это state_1, state = 2 - это state_2
void PrintDesk(cell **desk, unsigned int height, unsigned int width, unsigned short state){
    unsigned int i;
    unsigned int j;

    // https://webhamster.ru/mytetrashare/index/mtb0/4028
    cell *p_desk = (cell *)desk;

    if(state == 1) {
        for (i = 0; i < height; ++i) {
            for (j = 0; j < width; ++j) {
                printf("%d ", p_desk[j * height + i].state_1);
            }
            putchar('\n');
        }
        putchar('\n');
    } else if(state == 2){
        for (i = 0; i < height; ++i) {
            for (j = 0; j < width; ++j) {
                printf("%d ", p_desk[j * height + i].state_2);
            }
            putchar('\n');
        }
        putchar('\n');
    } else{
        printf("PrintDesk error\n");
        exit(2);
    }
}

// Алгоритм, принимающий доску в состоянии state_n -- предки,
// и заполняющий на его основе state_k -- потомки. Возвращает переменную state, в которой номер k
// полученных потомков - 1 или 2 (для печати)
unsigned short Conway(cell **desk, unsigned int height, unsigned int width, unsigned short state){
    unsigned int i;
    unsigned int j;

    cell *p_desk = (cell *)desk;

    unsigned short is_cell_alive = 0;
    unsigned short neighbors_alive = 0;
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

// Загрузка начального состояния игрального поля
void Start(){
    const unsigned int height = 4;
    const unsigned int width = 5;

    cell desk[4][5];

    // Начальные условия -- рандомное заполнение клеток
    unsigned int i;
    unsigned int j;
    for(i = 0; i < height; ++i){
        for(j = 0; j < width; ++j){
            desk[i][j].state_1 = rand() % 2;
        }
    }

    // Печать начального состояния поля
    PrintDesk((cell **) desk, height, width, 1);

    // Игра началась -- закочится через 10 поколений
    unsigned short state = 1;
    for(i = 1; i < 10; ++i){
        state = Conway((cell **) desk, height, width, state);
        PrintDesk((cell **) desk, height, width, state);
    }

}

// Меню
int main() {
    Start();
    return 0;
}
