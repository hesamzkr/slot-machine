#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

#define NUM_SLOTS 3
#define MAXIMUM_NUMBER 6
#define ANIMATION_DURATION 3

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define RESET   "\033[0m"

const char* getColor(int number) {
    switch(number) {
        case 1: return RED;
        case 2: return GREEN;
        case 3: return YELLOW;
        case 4: return BLUE;
        case 5: return MAGENTA;
        case 6: return CYAN;
        default: return RESET;
    }
}

void spinSlotMachine(int slots[], int num_slots) {
    for (int i = 0; i < num_slots; i++) {
        slots[i] = rand() % MAXIMUM_NUMBER + 1;
    }
}

void animateSlots(int finalSlots[], int num_slots) {
    int fakeSlots[num_slots];
    time_t startTime = time(0);

    printf("Spinning...\n\n");

    while (time(0) - startTime < ANIMATION_DURATION) {
        for (int i = 0; i < num_slots; i++) {
            fakeSlots[i] = rand() % MAXIMUM_NUMBER + 1;
        }

        printf("\r | ");
        for (int i = 0; i < num_slots; i++) {
            printf("%s%d%s | ", getColor(fakeSlots[i]), fakeSlots[i], RESET);
        }
        fflush(stdout);
        usleep(150000);
    }

    printf("\r | ");
    for (int i = 0; i < num_slots; i++) {
        printf("%s%d%s | ", getColor(finalSlots[i]), finalSlots[i], RESET);
    }
    printf("\n");
}

void animateMessage(const char* message, const char* color) {
    for (int i = 0; i < 3; i++) {
        printf("\r%s%s%s   ", color, message, RESET);
        fflush(stdout);
        usleep(500000);
        printf("\r              ");
        fflush(stdout);
        usleep(500000);
    }
    printf("\r%s%s%s\n", color, message, RESET);
}

int checkWin(int slots[], int num_slots) {
    for (int i = 1; i < num_slots; i++) {
        if (slots[i] != slots[0]) {
            return 0;
        }
    }
    return 1;
}

int main() {
    int slots[NUM_SLOTS];
    bool firstTime = true;

    srand(time(0));

    while (1) {
        if (firstTime) {
            printf("Press Enter to spin the slot machine...");
            getchar();
            firstTime = false;
        }

        spinSlotMachine(slots, NUM_SLOTS);

        animateSlots(slots, NUM_SLOTS);

        if (checkWin(slots, NUM_SLOTS)) {
            animateMessage("You won!", GREEN);
        } else {
            animateMessage("You lost!", RED);
        }

        printf("Press Enter to play again...");
        getchar();

        printf("\n-----------------------------------\n\n");
    }

    return 0;
}
