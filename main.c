#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <time.h>


#define ADMIN_EMAIL "admin@admin.com"

typedef struct {
    int id;
    char name[30];
    char email[50];
    char contact[15];
    int tier;
    float fine;
    int curr_borrowed;

    
} User;

//global variables to store data of currently logged in user and if user is logged in rn or not
User user;
int isLoggedIn = 0;

int UCount=0, BCount=0, BorrCount=0;

//printng functions
void redPrint(char str[]);
void bluePrint(char str[]);
void greenPrint(char str[]);
void yellowPrint(char str[]);
void brightwhitePrint(char str[]);
void purplePrint(char str[]);
void LMSHeading();
void LoginMenu();
void MainMenu();
void AdminMenu();


// initialization functions
void loadCounts();
void saveCounts();

//signup functions
int SignUp();
int CreateUserLine(int id,char email[50],char password[30], char name[30], char contact[15], float fine, int curr_borr, int tier, char string[200]);
void trimNewline(char *str);
int AppendToFile(const char *filename, const char *data);

// login functions
int Login();
int EmailExists(char email[50]);
void Logout();

// main functionalities
void BorrowBook();
void ReturnBook();
void DisplayBooks();
void PayFine();

// admin functionalities
void AddBook();
void RemoveUser();
void ViewAllBorrowedBooks();
void ListBadMembers();


int main(){
    char choice;
    int x;

    // ascii art of LMS

    loadCounts();
    LMSHeading();
    // yellowPrint("USAIM\n");
    // brightwhitePrint("USAIM\n");
    // purplePrint("USAIM\n");

    // validating choice of account
    do{
        LoginMenu();
        scanf("%d", &choice);
        getchar();
        switch(choice){
            case 1:
                SignUp(); break;
            case 2:
                Login(); break;
            default:
                redPrint("Invalid Choice. Try Again\n");
        }
        if (!isLoggedIn){
            yellowPrint("You need to be logged in to continue\n");
            continue;
        }

    }while(choice<1 || choice >2);

    // we ensured that user is logged in here


    MainMenu();
    if(strcmp(user.email, ADMIN_EMAIL) == 0){
        AdminMenu();
    }
    printf("Enter Your Choice: ");
    scanf("%d", &choice);
    getchar();


    switch (choice)
    {
    case 1:
        BorrowBook();
        break;
    case 2:
        ReturnBook();
        break;
    case 3:
        PayFine();
        break;
    case 4:
        DisplayBooks();
        break;
    case 5:
        Logout();
        break;
    case 6:
        AddBook();
        break;
    case 7:
        RemoveUser();
        break;
    case 8:
        ViewAllBorrowedBooks();
        break;
    case 9:
        ListBadMembers();
        break;
    
    default:
    redPrint("Invalid Choice\n");
        break;
    }

    return 0;
}

void ColorPrint(char str[],int color) {

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, color);
    printf("%s", str);
    
    SetConsoleTextAttribute(hConsole, 7);
}
void redPrint(char str[]){
    ColorPrint(str,12);
}
void bluePrint(char str[]){
    ColorPrint(str,9);
}
void greenPrint(char str[]){
    ColorPrint(str,10);
}
void yellowPrint(char str[]) {
    ColorPrint(str,6);
}
void brightwhitePrint(char str[]) {
    ColorPrint(str,15);
}
void purplePrint(char str[]) {
    ColorPrint(str,13);
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
void LoginMenu(){
    printf("1 - Sign Up\n");
    printf("2 - Login\n");
    printf("Enter Your Choice: ");
}
void MainMenu(){
    printf("1 - Borrow A Book\n");
    printf("2 - Return A Book\n");
    printf("3 - Pay Fine\n");
    printf("4 - View Books in Library\n");
    printf("5 - Logout\n");

}
void AdminMenu(){
    printf("6 - Add Book\n");
    printf("7 - Remove A User\n");
    printf("8 - View All Borrowed Books \n");
    printf("9 - List Bad Members\n");

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
    char email[50], password[30], name[30], contact[15];
    int tier;

    while (1){
        printf("Enter Email: ");
        fgets(email, sizeof(email), stdin);

        if (EmailExists(email)){
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
    int x = CreateUserLine(UCount,email,password,name,contact,0.0,0,tier,line);
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
    user.id = UCount;
    user.fine = 0.0;
    user.curr_borrowed=0;
    user.tier = tier;
    UCount++;
    saveCounts();
    isLoggedIn = 1;
    greenPrint("Sign Up Successful!\n");
    //---------------------------

    return 1;
}
int EmailExists(char email[50]) {
    FILE *fp = fopen("Users.txt", "r");
    if (fp == NULL) {
        printf("Error: could not open Users.txt\n");
        return 0; // If file not found, treat as empty (no duplicate)
    }

    int id, currentlyBorrowed;
    float fine;
    char storedEmail[50], password[30], name[50], contact[15], tier[15];

    // Read each line and extract fields separated by commas
    while (fscanf(fp, "%d,%[^,],%[^,],%[^,],%[^,],%f,%d,%s\n",
                  &id,
                  storedEmail,
                  password,
                  name,
                  contact,
                  &fine,
                  &currentlyBorrowed,
                  tier) == 8)
    {
        if (strcmp(storedEmail, email) == 0) {
            fclose(fp);
            return 1; // Email already exists
        }
    }

    fclose(fp);
    return 0; // Email not found
}
int CreateUserLine(int id, char email[50], char password[30], char name[30], char contact[15],
              float fine, int curr_borr, int tier, char string[200]) {

    // Create formatted string safely
    trimNewline(email);
    trimNewline(password);
    trimNewline(name);
    trimNewline(contact);

    int written = sprintf(string, "%d,%s,%s,%s,%s,%.2f,%d,%d",
                          id, email, password, name, contact, fine, curr_borr, tier);

    // Check for errors (sprintf returns number of chars written or negative if failed)
    if (written < 0)
        return 0;
    else
        return 1;
}
void trimNewline(char *str){
    int len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
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
    char email[50], password[30];

    printf("Enter Email: ");
    fgets(email, sizeof(email), stdin);
    printf("Enter Password: ");
    fgets(password, sizeof(password), stdin);

    trimNewline(email);
    trimNewline(password);

    FILE *fp = fopen("Users.txt", "r");
    if (fp == NULL) {
        redPrint("Error: could not open Users.txt\n");
        return 0; // File not found
    }

    int id, currentlyBorrowed;
    float fine;
    char storedEmail[50], storedPassword[30], name[50], contact[15], tier[15];

    // Read each line and extract fields separated by commas
    while (fscanf(fp, "%d,%[^,],%[^,],%[^,],%[^,],%f,%d,%s\n",
                  &id,
                  storedEmail,
                  storedPassword,
                  name,
                  contact,
                  &fine,
                  &currentlyBorrowed,
                  tier) == 8)
    {
        if (strcmp(storedEmail, email) == 0 && strcmp(storedPassword, password) == 0) {
            // Successful login
            strcpy(user.email, storedEmail);
            strcpy(user.name, name);
            strcpy(user.contact, contact);
            user.id = id;
            user.fine = fine;
            user.curr_borrowed = currentlyBorrowed;
            user.tier = atoi(tier);
            isLoggedIn = 1;
            fclose(fp);
            greenPrint("Login Successful!\n");
            return 1;
        }
    }

    fclose(fp);
    redPrint("Invalid email or password.\n");
    return 0; // Login failed


}

void Logout(){
    isLoggedIn = 0;


    user.id = -1;
    strcpy(user.email, "");
    strcpy(user.name, "");
    strcpy(user.contact, "");
    user.fine = 0.0;
    user.curr_borrowed = 0;
    user.tier = 0;
    greenPrint("Logged Out Successfully!\n");
}


void BorrowBook(){
    char bookTitle[30];

    printf("Enter a Book Title to Borrow: ");
    fgets(bookTitle, sizeof(bookTitle), stdin);
    int id = FindBookName(bookTitle);
    if (id == -1){
        return;
    } else{
        int days;
        do
        {
            printf("Enter number of days to borrow: ");
            scanf("%d", &days);
            getchar();
            if (days <= 0){
                redPrint("Invalid number of days. Try Again.\n");
            }
            if (days > 7){
                redPrint("You can borrow a book for a maximum of 7 days only. Try Again.\n");
            }

        } while (days <= 0 || days > 7);
            
        DecrementBookQuantity(id);
        user.curr_borrowed++;
        UpdateUser();  

        //add record to borrows.txt

        

            
        

        greenPrint("Book Borrowed Successfully!\n");
    }




}

void CreateBorrowLine(){}
void DecrementBookQuantity(int bookID){
    FILE *fp = fopen("Books.txt", "r");
    if (fp == NULL) {
        printf("Error: could not open Books.txt\n");
        return; // File not found
    }

    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        fclose(fp);
        printf("Error: could not create temporary file\n");
        return;
    }

    int id, quantity;
    char title[30], author[30];
    // Read each line and extract fields separated by commas
    while (fscanf(fp, "%d,%[^,],%[^,],%d\n",
                  &id,
                  title,
                  author,
                  &quantity) == 4)
    {
        if (id == bookID) {
            quantity--; // Decrement quantity
        }
        fprintf(temp, "%d,%s,%s,%d\n", id, title, author, quantity);
    }

    fclose(fp);
    fclose(temp);

    // Replace original file with temp file
    remove("Books.txt");
    rename("temp.txt", "Books.txt");
}
int FindBookName(char title[30]){
    FILE *fp = fopen("Books.txt", "r");
    if (fp == NULL) {
        printf("Error: could not open Books.txt\n");
        return -1; // File not found
    }

    int id, quantity;
    char storedTitle[30], author[30];

    // Read each line and extract fields separated by commas
    while (fscanf(fp, "%d,%[^,],%[^,],%d\n",
                  &id,
                  storedTitle,
                  author,
                  &quantity) == 4)
    {
        if (strcmp(storedTitle, title) == 0 && quantity > 0) {
            fclose(fp);
            return id;// Book found
        }else{
            redPrint("Book not available(Does not exists or quantity is zero)\n");
            return -1;
        }
    }

    fclose(fp);
    return 0; // Book not found
}
void UpdateUser(){
    // open users.txt
    // read line by line
    // if id matches, update the line with new data from user struct
    // write to temp file
    // replace users.txt with temp file
    FILE *fp = fopen("Users.txt", "r");
    if (fp == NULL) {
        printf("Error: could not open Users.txt\n");
        return; // File not found
    }
    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        fclose(fp);
        printf("Error: could not create temporary file\n");
        return;
    }
    int id, currentlyBorrowed;
    float fine;
    char storedEmail[50], password[30], name[50], contact[15], tier[15];

    // Read each line and extract fields separated by commas
    while (fscanf(fp, "%d,%[^,],%[^,],%[^,],%[^,],%f,%d,%s\n",
                  &id,
                  storedEmail,
                  password,
                  name,
                  contact,
                  &fine,
                  &currentlyBorrowed,
                  tier) == 8)
    {
        if (id == user.id) {
            // Update line with new data from user struct
            fprintf(temp, "%d,%s,%s,%s,%s,%.2f,%d,%d\n",
                    user.id,
                    user.email,
                    password, // assuming password remains unchanged
                    user.name,
                    user.contact,
                    user.fine,
                    user.curr_borrowed,
                    user.tier);
        } else {
            // Write original line
            fprintf(temp, "%d,%s,%s,%s,%s,%.2f,%d,%s\n",
                    id,
                    storedEmail,
                    password,
                    name,
                    contact,
                    fine,
                    currentlyBorrowed,
                    tier);
        }
    }

}
void ReturnBook(){}
void DisplayBooks(){}
void PayFine(){}


void AddBook(){
    char bookName[30], author[30];
    int quantity;

    if (strcmp(user.email, ADMIN_EMAIL) != 0){
        redPrint("Only Admins can add books!\n");
        return;
    }
    printf("Enter Book Name: ");
    fgets(bookName, sizeof(bookName), stdin);
    printf("Enter Author Name: ");
    fgets(author, sizeof(author), stdin);
    printf("Enter Quantity: ");
    scanf("%d", &quantity);
    getchar();

    char line[200];
    CreateBookLine(BCount, bookName, author, quantity, line);

    int x = AppendToFile("Books.txt", line);
    if (!x){
        redPrint("Error in writing to file\n");
        return;
    }
    BCount++;
    saveCounts();
    greenPrint("Book Added Successfully!\n");


}

int CreateBookLine(int id, char bookName[30], char author[30], int quantity, char string[200]){
    int written;
    trimNewline(bookName);
    trimNewline(author);

    sprintf(string, "%d,%s,%s,%d", id, bookName, author, quantity);

        // Check for errors (sprintf returns number of chars written or negative if failed)
    if (written < 0)
        return 0;
    else
        return 1;

}
void RemoveUser(){
    if (strcmp(user.email, ADMIN_EMAIL) != 0){
        redPrint("Only Admins can remove users!\n");
        return;
    }
    int id;
    printf("Enter User ID to Remove: ");
    scanf("%d", &id);
    getchar();

    int x = RemoveLineByID("Users.txt", id);
    if (x){
        greenPrint("User Removed Successfully!\n");
    } else{
        redPrint("User ID not found!\n");
    }

}

int RemoveLineByID(const char *filename, int targetID) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) return 0; // File open error

    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        fclose(fp);
        return 0;
    }

    char line[512];
    int id;
    int removed = 0;

    while (fgets(line, sizeof(line), fp)) {
        // Extract ID before first comma
        if (sscanf(line, "%d", &id) == 1) {
            if (id == targetID) {
                removed = 1; // skip this line
                continue;
            }
        }
        fputs(line, temp); // keep other lines
    }

    fclose(fp);
    fclose(temp);

    // Replace original file with temp file
    remove(filename);
    rename("temp.txt", filename);

    return removed; // 1 if removed, 0 if not found
}

void ViewAllBorrowedBooks(){

}
void ListBadMembers(){}





void getCurrentDate(int *day, int *month, int *year) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    *day = tm.tm_mday;
    *month = tm.tm_mon + 1;   // months are 0-11
    *year = tm.tm_year + 1900; // years since 1900
}
