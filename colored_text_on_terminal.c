#include <stdio.h>
#include <windows.h>

int main() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, 12); // Red text
    printf("This is red text\n");

    SetConsoleTextAttribute(hConsole, 10); // Green text
    printf("This is green text\n");

    SetConsoleTextAttribute(hConsole, 7);  // Reset (white/gray)
    printf("Back to normal color\n");

    return 0;
}
