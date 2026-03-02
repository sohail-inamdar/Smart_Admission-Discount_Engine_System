#include <stdio.h>
#include <stdlib.h>
#include "ui.h"

// ANSI Colors
#define RESET   "\033[0m"
#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define CYAN    "\033[36m"
#define YELLOW  "\033[33m"

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void printLine() {
    printf("====================================================\n");
}

void printHeader(char title[]) {
    printLine();
    printf(CYAN "   %s\n" RESET, title);
    printLine();
}

void printSuccess(char msg[]) {
    printf(GREEN "[SUCCESS] %s\n" RESET, msg);
}

void printError(char msg[]) {
    printf(RED "[ERROR] %s\n" RESET, msg);
}

void pressEnter() {
    printf(YELLOW "\nPress Enter to continue..." RESET);
    getchar();
    getchar();
}