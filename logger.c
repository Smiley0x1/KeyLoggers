#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>

int main() {
    FILE *logfile;
    char key;
    char filename[30]; // Adjust the size based on your needs

    // Open a file for writing
    time_t t;
    struct tm* current_time;
    t = time(NULL);
    current_time = localtime(&t);

    // Extract day, month, and year
    int day = current_time->tm_mday;
    int month = current_time->tm_mon + 1;  // Months are 0-indexed
    int year = current_time->tm_year + 1900;  // Years since 1900

    char date[12];
    sprintf(date, "%02d-%02d-%04d", day, month, year);

    sprintf(filename, "key_history_%s.log", date);

    logfile = fopen(filename, "w");

    if (logfile == NULL) {
        fprintf(stderr, "Error opening log file.\n");
        return 1;
    }

    printf("Keylogger started. Press 'ESC' to exit.\n");

    // Disable terminal buffering and echoing
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while (1) {
        key = getchar(); // Get a keypress

        // Exit the program if the 'ESC' key is pressed
        if (key == 27) {
            break;
        }

        // Log the key to the file
        fprintf(logfile, "%c", key);
    }

    // Restore terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    // Close the log file
    fprintf(logfile, "\n");
    fclose(logfile);

    printf("Keylogger stopped. Keystrokes logged to '%s'.\n", filename);

    return 0;
}

