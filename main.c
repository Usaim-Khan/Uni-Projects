#include <stdio.h>
#include <windows.h>
#include <string.h>

typedef struct {
    char name[30];
    char email[50];
    char password[30];
    char contact[10];
    int tier;
    
} User;


void redPrint(char str[]);
void bluePrint(char str[]);
void greenPrint(char str[]);
void yellowPrint(char str[]);
void brightwhitePrint(char str[]);
void purplePrint(char str[]);
void LMSHeading();
void LoginInfo();

int SignUp();
int Login();
int EmailExists();


int main(){
    char choice;

    // ascii art of LMS
    LMSHeading();
    yellowPrint("USAIM\n");
    brightwhitePrint("USAIM\n");
    purplePrint("USAIM\n");

    // validating choice of account
    do{
        LoginInfo();
        scanf("%d", &choice);
        getchar();
        switch(choice){
            case 1:
                SignUp();
                break;
            case 2:
                Login();
                break;
            default:
                redPrint("Invalid Choice. Try Again\n");
        }

    }while(choice<1 || choice >2);

    


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
void yellowPrint(char str[]) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, 6); // Bright Yellow text
    printf("%s", str);
    
    SetConsoleTextAttribute(hConsole, 7);  // Reset to default
}
void brightwhitePrint(char str[]) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, 15); // Gray text
    printf("%s", str);

    
    SetConsoleTextAttribute(hConsole, 7); // Reset to default
}
void purplePrint(char str[]) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, 13); // Light Magenta (Purple) text
    printf("%s", str);
    
    SetConsoleTextAttribute(hConsole, 7);  // Reset to default
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
void LoginInfo(){
    printf("1 - Sign Up\n");
    printf("2 - Login\n");
    printf("Enter Your Choice: ");
}

int SignUp(){
    char email[50], password[30], name[30], contact[10];
    int tier;

    while (1){
        printf("Enter Email: ");
        fgets(email, sizeof(email), stdin);
        if (EmailExists){
            redPrint("This email already exists in system. Use another email\n");
            continue;
        }
        printf("Enter Password: ");
        fgets(password, sizeof(password), stdin);
        printf("Enter Name: ");
        fgets(name, sizeof(name), stdin);
        printf("Enter Contact Num: ");
        fgets(contact, sizeof(contact), stdin);
        printf("TIERS\n");
        brightwhitePrint("1 - Silver\n");
        yellowPrint("2 - Gold\n");
        purplePrint("3 - Amethyst\n");

        printf("Enter Tier: ");
        scanf("%d", &tier);
        getchar();
        
        // Function to write data to file in csv format
        // Email,Password,Name,Contact,Tier (if error in this, return 0)

        break;

    }

    return 1;
}
int Login(){

}