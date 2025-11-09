#include <stdio.h>
#include <windows.h>
#include <string.h>

typedef struct {
    char name[30];
    char email[50];
    char contact[10];
    int tier;
    float fine;
    int curr_borrowed;

    
} User;

//global variables to store data of currently logged in user and if user is logged in rn or not
User user;
int isLoggedIn = 0;

int UCount=0, BCount=0, BorrCount=0;


void redPrint(char str[]);
void bluePrint(char str[]);
void greenPrint(char str[]);
void yellowPrint(char str[]);
void brightwhitePrint(char str[]);
void purplePrint(char str[]);
void LMSHeading();
void LoginInfo();

int SignUp();
int CreateStr(int id,char email[50],char password[30], char name[30], char contact[10], float fine, int curr_borr, int tier, char string[200]);
int Login();
int EmailExists();


int main(){
    char choice;

    // ascii art of LMS

    loadCounts();
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

void loadCounts() {
    FILE *fp = fopen("meta.txt", "r");
    if (fp == NULL) {
        // If file doesn’t exist, initialize with 0
        UCount = 0;
        BCount = 0;
        BorrCount = 0;
        return;
    }

    fscanf(fp, "%d %d %d", &UCount, &BCount, &BorrCount);
    fclose(fp);
}
void saveCounts() {
    FILE *fp = fopen("meta.txt", "w");
    if (fp == NULL) {
        printf("Error saving counters!\n");
        return;
    }

    fprintf(fp, "%d %d %d", UCount, BCount, BorrCount);
    fclose(fp);
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
        break;
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
        purplePrint("3 - Platinium\n");

        printf("Enter Tier: ");
        scanf("%d", &tier);
        getchar();
        
        // Function to write data to file in csv format
        // id,Email,Password,Name,Contact,fine,currently_borrowed,Tier (if error in this, return 0)
        char line[200];
        int x = CreateStr(UCount,email,password,name,contact,0.0,0,tier,line);
        if (x){
            x = AppendToFile("Users.txt", line);
            if (!x){
                redPrint("Error in writing to file\n");
                return 0;
            }
        } else{
            redPrint("An Error Occured while creating string in csv form\n");
            return 0;
        }

        // when user signs in, put its data in global struct
        strcpy(user.email, email);
        strcpy(user.name, name);
        strcpy(user.contact, contact);
        user.fine = 0.0;
        user.curr_borrowed=0;
        user.tier = tier;
        UCount++;
        saveCounts();

        

        //---------------------------

    return 1;
}


int CreateStr(int id, char email[50], char password[30], char name[30], char contact[10],
              float fine, int curr_borr, int tier, char string[200]) {

    // Create formatted string safely
    int written = sprintf(string, "%d,%s,%s,%s,%s,%.2f,%d,%d",
                          id, email, password, name, contact, fine, curr_borr, tier);

    // Check for errors (sprintf returns number of chars written or negative if failed)
    if (written < 0)
        return 0;
    else
        return 1;
}
int AppendToFile(const char *filename, const char *data) {
    FILE *fp = fopen(filename, "a"); // open in append mode
    if (fp == NULL)
        return 0; // failed to open file

    fprintf(fp, "%s\n", data); // write string + newline
    fclose(fp);
    return 1; // success
}
int Login(){

}