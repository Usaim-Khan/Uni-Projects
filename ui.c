#include <stdio.h>
#include <windows.h>
#include "ui.h"

// COLOR FUNCTIONS
void ColorPrint(char str[], int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
    printf("%s", str);
    SetConsoleTextAttribute(hConsole, 7);
}

void redPrint(char str[]) { ColorPrint(str, 12); }
void bluePrint(char str[]) { ColorPrint(str, 9); }
void greenPrint(char str[]) { ColorPrint(str, 10); }
void yellowPrint(char str[]) { ColorPrint(str, 6); }
void brightwhitePrint(char str[]) { ColorPrint(str, 15); }
void purplePrint(char str[]) { ColorPrint(str, 13); }


// UI / MENU FUNCTIONS

void LMSHeading() {
    printf(" _     _ _                            __  __                                      \n");
    printf("| |   (_) |__  _ __ __ _ _ __ _   _  |  \\/  | __ _ _ __   __ _  __ _  ___         \n");
    printf("| |   | | '_ \\| '__/ _` | '__| | | | | |\\/| |/ _` | '_ \\ / _` |/ _` |/ _ \\  _____ \n");
    printf("| |___| | |_) | | | (_| | |  | |_| | | |  | | (_| | | | | (_| | (_| |  __/ |_____| \n");
    printf("|_____|_|_.__/|_|  \\__,_|_|   \\__, | |_|  |_|\\__,_|_| |_|\\__,_|\\__, |\\___|        \n");
    printf("                      _     __|___/         _                  |___/               \n");
    printf(" _ __ ___   ___ _ __ | |_  / ___| _   _ ___| |_ ___ _ __ ___                      \n");
    printf("| '_ ` _ \\ / _ \\ '_ \\| __| \\___ \\| | | / __| __/ _ \\ '_ ` _ \\                     \n");
    printf("| | | | | |  __/ | | | |_   ___) | |_| \\__ \\ ||  __/ | | | | |                    \n");
    printf("|_| |_| |_|\\___|_| |_|\\__| |____/ \\__, |___/\\__\\___|_| |_| |_|                    \n");
    printf("                                  |___/                                           \n");
}

void LoginMenu() {
    printf("1 - Sign Up\n");
    printf("2 - Login\n");
    printf("Enter Your Choice: ");
}

void MainMenu() {
    printf("1 - Borrow A Book\n");
    printf("2 - Return A Book\n");
    printf("3 - Pay Fine\n");
    printf("4 - View Books in Library\n");

}

void AdminMenu() {
    // remove book option to add
    printf("5 - Add A Book\n");
    printf("6 - Remove A User\n");
    printf("7 - View All Borrowed Books\n");
    printf("8 - Remove A Book\n");
}

void ExitMenu() {
    printf("9 - Logout\n");
    printf("10 - Exit\n");
}


