#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define ROWS 5
#define COLS 5
#define MAX_MOVIES 3
#define MAX_SEATS (ROWS * COLS)
#define MAX_TICKETS 20

typedef struct {
    char name[50];
    int price;
    char slots[3][20];
    int seats[3][ROWS][COLS];
} Movie;

// Visualising the seating arrangement
void displaytheSeats(int seats[ROWS][COLS]) {
    printf("Seats (0 = empty, X = booked):\n   ");
    for (char col = 'A'; col < 'A' + COLS; col++)
        printf(" %c ", col);

        printf("\n");
    for (int i = 0; i < ROWS; i++)
    {
        printf("%2d ", i + 1);
        for (int j = 0; j < COLS; j++)
        printf(" %c ", seats[i][j] ? 'X' : '0');

        printf("\n");
    }
}


void saveSeats(Movie movies[], int nMovies) {
    FILE *file = fopen("seats_data.txt", "w");
    if (!file)
        { printf("Error: Could not save seat data.\n");

         return;
        }
    for (int m = 0; m < nMovies; m++)
        for (int s = 0; s < 3; s++)
            for (int r = 0; r < ROWS; r++)
                for (int c = 0; c < COLS; c++)
                    fprintf(file, "%d ", movies[m].seats[s][r][c]);
    fclose(file);
}

void loadselectedSeats(Movie movies[], int nMovies) {
    FILE *file = fopen("seats_data.txt", "r");
    if (!file)
        {
            printf("No previous seat data found. Using default seat arrangement.\n");

            return;
        }
    for (int m = 0; m < nMovies; m++)
        for (int s = 0; s < 3; s++)
            for (int r = 0; r < ROWS; r++)
                for (int c = 0; c < COLS; c++)
                    fscanf(file, "%d", &movies[m].seats[s][r][c]);
    fclose(file);
}


int getValidInput(int min, int max) {
    int input;
    while (1) {
        if (scanf("%d", &input) == 1 && input >= min && input <= max)

            return input;
        printf("Whoopsie! Wrong input. Try entering a number between %d and %d: ", min, max);
        while (getchar() != '\n'); // Clear buffer
    }
}


void bookselectedSeats(int seats[ROWS][COLS], int numSeats, char ticketNames[][4]) {
    for (int i = 0; i < numSeats; i++) {
        int row, colIndex;
        char col;
        while (1) {
            printf("Enter row (1-%d) and column (A-%c) for ticket %d: ", ROWS, 'A' + COLS - 1, i + 1);
            if (scanf("%d %c", &row, &col) != 2 || row < 1 || row > ROWS ||
                (colIndex = toupper(col) - 'A') < 0 || colIndex >= COLS || seats[row - 1][colIndex]) {
                printf("Invalid input or occupied seat. Try again.\n");
                while (getchar() != '\n'); // Clear buffer
            } else {
                seats[row - 1][colIndex] = 1;
                sprintf(ticketNames[i], "%d%c", row, col);
                printf("Seat %s booked successfully.\n", ticketNames[i]);
                break;
            }
        }
    }
}

// Clear screen
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    printf("\nPress Enter to continue...");
    getchar();
}

int main() {
    Movie movies[MAX_MOVIES] = {
        {"BEDER MEYE JOSNA", 600, {"11 AM", "4 PM", "8 PM"}, {0}},
        {"BOSTIR MEYE RADHA", 750, {"10 AM", "3 PM", "7 PM"}, {0}},
        {"JOKER 2", 500, {"9 AM", "2 PM", "6 PM"}, {0}}
    };

    int movieChoice, slotChoice, nTickets;
    char ticketNames[MAX_TICKETS][4];
    loadselectedSeats(movies, MAX_MOVIES);

    // this part is for Selecting the movie
    while (1) {
        clearScreen();
        printf("Movies:\n");
        for (int i = 0; i < MAX_MOVIES; i++) printf("%d. %s\n", i + 1, movies[i].name);
        printf("Enter your choice (1-%d): ", MAX_MOVIES);
        movieChoice = getValidInput(1, MAX_MOVIES) - 1;
        break;
    }

    // this part is for Selecting time slot
    while (1) {
        clearScreen();
        printf("Slots for %s:\n", movies[movieChoice].name);
        for (int i = 0; i < 3; i++)
            printf("%d. %s\n", i + 1, movies[movieChoice].slots[i]);
        printf("Enter your choice (1-3): ");
        slotChoice = getValidInput(1, 3) - 1;
        break;
    }

    //This part is for Selecting tickets
    while (1) {
        clearScreen();
        printf("Enter the number of tickets you want to buy (1-%d): ", MAX_TICKETS);
        nTickets = getValidInput(1, MAX_TICKETS);
        break;
    }

    //This part is for booking tickets
    clearScreen();
    printf("Booking for %s at %s:\n", movies[movieChoice].name, movies[movieChoice].slots[slotChoice]);
    displaytheSeats(movies[movieChoice].seats[slotChoice]);
    bookselectedSeats(movies[movieChoice].seats[slotChoice], nTickets, ticketNames);

    //this part shows the booking confirmation
    clearScreen();
    printf("Congratulations! The tickets have been booked.\nMovie: %s\nTime: %s\nTickets: %d\nSeats: ",
           movies[movieChoice].name, movies[movieChoice].slots[slotChoice], nTickets);
    for (int i = 0; i < nTickets; i++)
        printf("%s ", ticketNames[i]);

    printf("\nTotal: %d\n", nTickets * movies[movieChoice].price);
    printf("Thanks for purchasing the tickets! Hope you will enjoy the experience.\n");

    saveSeats(movies, MAX_MOVIES);
    return 0;
}