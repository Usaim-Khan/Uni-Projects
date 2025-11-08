#include <stdio.h>
#include <windows.h>

void redPrint(char str[]);
void bluePrint(char str[]);
void greenPrint(char str[]);
void LMSHeading();

int main() {

    LMSHeading();
    bluePrint("Usaim\n");
    redPrint("Usaim\n");
    greenPrint("Usaim\n");

    return 0;
}

void redPrint(char str[]){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, 12); // Red text
    printf("%s", str);
    
    SetConsoleTextAttribute(hConsole, 7);
}
void bluePrint(char str[]){
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


    SetConsoleTextAttribute(hConsole, 3); //blue
    printf("%s", str);
    // for (int i =0; i<16; i++){
    //     SetConsoleTextAttribute(hConsole, i);
    //     printf("%s", str);
    // }
    
    SetConsoleTextAttribute(hConsole, 7);
}
void greenPrint(char str[]){

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, 10); // green
    printf("%s", str);
    
    SetConsoleTextAttribute(hConsole, 7);
}
void LMSHeading(){
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


