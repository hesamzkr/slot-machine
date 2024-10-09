#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/select.h>

#define NUM_SLOTS 3
#define MAXIMUM_NUMBER 2
#define ANIMATION_DURATION 3

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define ORANGE  "\033[38;5;208m"
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
    int slotStopped[num_slots];
    int slotValues[num_slots];
    int slotStopTime[num_slots];
    int elapsedTimeMs = 0;

    for (int i = 0; i < num_slots; i++) {
        slotStopped[i] = 0;
        slotValues[i] = 0;
        slotStopTime[i] = (i + 1) * (ANIMATION_DURATION * 1000) / num_slots;
    }

    printf("Spinning...\n\n");

    while (1) {
        for (int i = 0; i < num_slots; i++) {
            if (!slotStopped[i]) {
                if (elapsedTimeMs >= slotStopTime[i]) {
                    slotStopped[i] = 1;
                    slotValues[i] = finalSlots[i];
                } else {
                    slotValues[i] = rand() % MAXIMUM_NUMBER + 1;
                }
            }
        }

        // Display the slots
        printf("\r | ");
        for (int i = 0; i < num_slots; i++) {
            printf("%s%d%s | ", getColor(slotValues[i]), slotValues[i], RESET);
        }
        fflush(stdout);

        // Check if all slots have stopped
        bool allSlotsStopped = true;
        for (int i = 0; i < num_slots; i++) {
            if (!slotStopped[i]) {
                allSlotsStopped = false;
                break;
            }
        }
        if (allSlotsStopped) {
            break;
        }

        usleep(150000);
        elapsedTimeMs += 150;
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

void waitForEnter() {
    int colorToggle = 0;
    const char* colors[] = {YELLOW, ORANGE};

    printf("\n");

    while (1) {
        printf("\r%s[ENTER]%s", colors[colorToggle], RESET);
        fflush(stdout);

        fd_set set;
        struct timeval tv;

        FD_ZERO(&set);
        FD_SET(STDIN_FILENO, &set);

        tv.tv_sec = 0;
        tv.tv_usec = 500000; // Wait up to 0.5 seconds

        int res = select(STDIN_FILENO + 1, &set, NULL, NULL, &tv);
        if (res > 0 && FD_ISSET(STDIN_FILENO, &set)) {
            // Read until newline
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
            break;
        }

        colorToggle = 1 - colorToggle;
    }

    printf("\r         \r"); // Clear the line
}

int main() {
    int slots[NUM_SLOTS];
    bool firstTime = true;

    srand(time(0));

    while (1) {
        if (firstTime) {
            waitForEnter();
            firstTime = false;
        }

        spinSlotMachine(slots, NUM_SLOTS);

        animateSlots(slots, NUM_SLOTS);

        if (checkWin(slots, NUM_SLOTS)) {
            animateMessage("You won!", GREEN);
        } else {
            animateMessage("You lost!", RED);
        }

        waitForEnter();

        printf("\n-----------------------------------\n\n");
    }

    return 0;
}
