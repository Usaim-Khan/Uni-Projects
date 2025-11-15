// User.txt: UserId,Email,Password,Name,Contact,fine,currently_borrowed,Tier (if error in this, return 0)
// Books.txt: BookId,BookName,Author,Quantity
// Borrows.txt: BorrowId,UserID,BookID,BorrowDate,ReturnDate,DueDate

#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "ui.h"


#define ADMIN_EMAIL "admin@admin.com"

typedef struct {
    int id;
    char name[30];
    char email[50];
    char contact[15];
    int tier;
    float fine;
    int curr_borrowed;
    int max_borrow;
    float fine_rate;
    
} User;

//global variables to store data of currently logged in user and if user is logged in rn or not
User user;
int isLoggedIn = 0;

int UCount=0, BCount=0, BorrCount=0;

// initialization functions
void loadCounts();
void saveCounts();

// Authentication functions
int SignUp();
int Login();
void Logout();

//Authentication helper functions
int CreateUserLine(int id,char email[50],char password[30], char name[30], char contact[15], float fine, int curr_borr, int tier, char string[200]);
int EmailExists(char email[50]);
void TierSpecifics(int tier);


// Main functionalities
void BorrowBook();
void ReturnBook();
void DisplayBooks();
void PayFine();

// Main functionalities helper functions
void UpdateBookQuantity(int bookID ,int flag);
int FindBookName(char title[30]);
void UpdateUser();
void CreateBorrowLine(int borrowId, int userId, int bookId, int borrowDay, int borrowMonth, int borrowYear,int dueDay, int dueMonth, int dueYear,char string[200]);
void addDays(int day, int month, int year, int daysToAdd,
             int *newDay, int *newMonth, int *newYear);

// Admin functionalities
void AddBook();
void RemoveUser();
void ViewAllBorrowedBooks();
void RemoveBook();

// Admin functionalities helper functions
int CreateBookLine(int id, char bookName[30], char author[30], int quantity, char string[200]);
int RemoveLineByID(const char *filename, int targetID);

// General helper functions
void getCurrentDate(int *day, int *month, int *year);
void getUserNamesArray(char userNames[100][30]);
void getBookNamesArray(char bookNames[100][30]);
void trimNewline(char *str);
int AppendToFile(const char *filename, const char *data);




int main(){
    char choice;

    loadCounts();

    // ascii art of LMS
    LMSHeading();

    while(1){
        // validating choice of account
        do{
            if (isLoggedIn){
                break;
            }
            LoginMenu();
            scanf("%c", &choice);
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
            }

        }while(choice<1 || choice >2 || !isLoggedIn);

        // we ensured that user is logged in here

        while (1){
            if (!isLoggedIn){
                break;
            }
            MainMenu();
            if(strcmp(user.email, ADMIN_EMAIL) == 0){
                AdminMenu();
            }
            ExitMenu();
            printf("Enter Your Choice: ");
            scanf("%c", &choice);
            getchar();

            switch (choice){
            case 1: BorrowBook(); break;
            case 2:ReturnBook();break;
            case 3: PayFine(); break;
            case 4: DisplayBooks(); break;
            case 5: AddBook();break;
            case 6: RemoveUser(); break;
            case 7: ViewAllBorrowedBooks(); break;
            case 8: RemoveBook(); break;
            case 9: Logout(); break;
            case 10: exit(0);
            default: redPrint("Invalid Choice\n"); break;
            }

        }
            
    }

    return 0;
}

// =====================================================================
// INITIALIZATION FUNCTIONS
// =====================================================================

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

// =====================================================================
// AUTHENTICATION FUNCTIONS
// =====================================================================

int SignUp(){
    char email[50], password[30], name[30], contact[15];
    int tier;

    while (1){
        printf("Enter Email: ");
        fgets(email, sizeof(email), stdin);
        trimNewline(email);

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
    brightwhitePrint("1 - Silver: 3 Borrows at a time and fine of Rs100 per day if nook not returned\n");
    yellowPrint("2 - Gold: 5 Borrows at a time and fine of Rs75 per day if nook not returned\n");
    purplePrint("3 - Platinium: 7 Borrows at a time and fine of Rs50 per day if nook not returned\n");

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
    TierSpecifics(tier);
    greenPrint("Sign Up Successful!\n");
    //---------------------------

    return 1;
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
            TierSpecifics(user.tier);
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

// =====================================================================
// AUTHENTICATION HELPER FUNCTIONS
// =====================================================================

void TierSpecifics(int tier){
    switch(tier){
        case 1:
            user.max_borrow = 3;
            user.fine_rate = 100;
            break;
        case 2:
            user.max_borrow = 5;
            user.fine_rate = 75;
            break;
        case 3:
            user.max_borrow = 7;
            user.fine_rate = 50;
            break;
        default:
            printf("Invalid tier. Setting to default values.\n");
            break;
    }
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



// =====================================================================
// MAIN FUNCTIONALITIES
// =====================================================================
void ReturnBook(){
    // go through borrows.txt
    // find records with user id = user.id and where return date is "NULL"
    // display those books with due dates
    // ask which book to return

    FILE *fp = fopen("Borrows.txt", "r");
    if (fp == NULL) {
        printf("Error: could not open Borrows.txt\n");
        return; // File not found
    }
    int borrowId, userId, bookId;
    char borrowDate[15], returnDate[15], dueDate[15];

    //an array to store book names at their ids index

    char bookNames[100][30];
    getBookNamesArray(bookNames);

    printf("Books You Have Borrowed:\n");
    printf("%-5s %-40s %-15s\n", "ID", "Title", "Due Date");
    printf("--------------------------------------------------------------\n");
    int foundAny = 0;
    // Read each line and extract fields separated by commas
    while (fscanf(fp, "%d,%d,%d,%[^,],%[^,],%[^,\n]\n",
                  &borrowId,
                  &userId,
                  &bookId,
                  borrowDate,
                  returnDate,
                  dueDate) == 6)
    {
        if (userId == user.id && strcmp(returnDate, "NULL") == 0){
            foundAny = 1;
            printf("%-5d %-40s %-15s\n", borrowId, bookNames[bookId], dueDate);
        }
    }
    printf("\n-----------------------------------------------------------------------------\n");  

    fclose(fp);
    if (!foundAny){
        yellowPrint("You have no borrowed books to return.\n");
        return;
    }
    printf("Enter Borrow ID of the book you want to return: ");
    int borrowIDToReturn;
    scanf("%d", &borrowIDToReturn);
    getchar();
    // Now, update the return date of this borrow record to current date
    fp = fopen("Borrows.txt", "r");
    if (fp == NULL) {
        printf("Error: could not open Borrows.txt\n");
        return; // File not found
    }
    // temp file to write updated data of borrows.txt
    FILE *temp = fopen("tempforborrow.txt", "w");
    if (temp == NULL) {
        fclose(fp);
        printf("Error: could not create temporary file\n");
        return;
    }
    int found = 0;
    int day, month, year;
    getCurrentDate(&day, &month, &year);
    char currentDateStr[15];
    sprintf(currentDateStr, "%02d-%02d-%04d", day, month, year);
    // Read each line and extract fields separated by commas

    while (fscanf(fp, "%d,%d,%d,%[^,],%[^,],%[^,\n]\n",
                  &borrowId,
                  &userId,
                  &bookId,
                  borrowDate,
                  returnDate,
                  dueDate) == 6)
    {
        if (borrowId == borrowIDToReturn && userId == user.id && strcmp(returnDate, "NULL") == 0){
            // Update return date to current date
            fprintf(temp, "%d,%d,%d,%s,%s,%s\n",
                    borrowId,
                    userId,
                    bookId,
                    borrowDate,
                    currentDateStr,
                    dueDate);
            found = 1;
            //check for fine
            int dueDay, dueMonth, dueYear;
            sscanf(dueDate, "%02d-%02d-%04d", &dueDay, &dueMonth, &dueYear);
            // compare current date with due date
            if (year > dueYear || (year == dueYear && month > dueMonth) || 
                (year == dueYear && month == dueMonth && day > dueDay)) {
                // book is returned late
                // calculate days late
                int daysLate = 0;
                if (year == dueYear && month == dueMonth) {
                    daysLate = day - dueDay;
                } else if (year == dueYear) {
                    // months are different
                    // assuming all months have 30 days for simplicity
                    daysLate = (month - dueMonth) * 30 + (day - dueDay);
                } else {
                    // years are different
                    daysLate = (year - dueYear) * 365 + (month - dueMonth) * 30 + (day - dueDay);
                }
                float this_fine = daysLate * user.fine_rate;
                user.fine += this_fine;
                UpdateUser();
                char fineMessage[200];
                sprintf(fineMessage, "You have returned the book late by %d days. Fine incurred: Rs%.2f\n", daysLate, this_fine);
                redPrint(fineMessage);
            }

            // Increment book quantity

            UpdateBookQuantity(bookId,1); // increment quantity
            user.curr_borrowed--;
            UpdateUser();
            greenPrint("Book Returned Successfully!\n");

        } else {
            // Write original line
            fprintf(temp, "%d,%d,%d,%s,%s,%s\n",
                    borrowId,
                    userId,
                    bookId,
                    borrowDate,
                    returnDate,
                    dueDate);
        }
    }

    if (!found){
        redPrint("No matching borrow record found for the given Borrow ID.\n");
    }
    fclose(fp);
    fclose(temp);
    // Replace original file with temp file
    remove("Borrows.txt");
    rename("tempforborrow.txt", "Borrows.txt");
}

void DisplayBooks(){
    // open Books.txt
    FILE *fp = fopen("Books.txt", "r");
    if (fp == NULL) {
        printf("Error: could not open Books.txt\n");
        return; // File not found
    }
    int id, quantity;
    char title[30], author[30];
    printf("Books Available in Library:\n");
    printf("%-5s %-40s %-25s %-5s\n", "ID", "Title", "Author", "Qty");
    printf("-----------------------------------------------------------------------------\n");
    // Read each line and extract fields separated by commas
    while (fscanf(fp, "%d,%[^,],%[^,],%d\n",
                  &id,
                  title,
                  author,
                  &quantity) == 4)
    {
        printf("%-5d %-40s %-25s %-5d\n", id, title, author, quantity);
    }
    fclose(fp);
}

void PayFine(){
    float amount;
    printf("Your current fine is: %.2f\n", user.fine);
    printf("Enter amount to pay: ");
    scanf("%f", &amount);
    getchar();

    if (amount <= 0){
        redPrint("Invalid amount. Try Again.\n");
        return;
    }
    if (amount > user.fine){
        redPrint("You cannot pay more than your current fine. Try Again.\n");
        return;
    }

    user.fine -= amount;
    UpdateUser();
    greenPrint("Fine Paid Successfully!\n");
}

void BorrowBook(){
    char bookTitle[30];

    printf("Enter a Book Title to Borrow: ");
    fgets(bookTitle, sizeof(bookTitle), stdin);
    trimNewline(bookTitle);
    int id = FindBookName(bookTitle);
    if (id == -1){
        return;
    } else{
        int d;
        do
        {
            printf("Enter number of days to borrow: ");
            scanf("%d", &d);
            getchar();
            if (d <= 0){
                redPrint("Invalid number of days. Try Again.\n");
            }
            if (d > 7){
                redPrint("You can borrow a book for a maximum of 7 days only. Try Again.\n");
            }
            //max borrow check

        } while (d <= 0 || d > 7);
            
        UpdateBookQuantity(id, 0); // decrement quantity
        user.curr_borrowed++;
        UpdateUser();  

        //add record to borrows.txt
        char line[200];
        int day, month, year;
        int newDay, newMonth, newYear;
        getCurrentDate(&day, &month, &year);
        addDays(day, month, year, d, &newDay, &newMonth, &newYear);
        CreateBorrowLine(BorrCount, user.id, id,
                         day, month, year,
                         newDay, newMonth, newYear,
                         line);

        int x = AppendToFile("Borrows.txt", line);
        if (!x){
            redPrint("Error in writing to file\n");
            return;
        }
        BorrCount++;
        saveCounts();

        //create a string message displaying user name, book name, and due date
        char message[200];
        sprintf(message, "Book '%s' successfully borrowed by %s. Due date: %02d-%02d-%04d\n",
                bookTitle, user.name, newDay, newMonth, newYear);
        greenPrint(message);
    }


}

// =====================================================================
// MAIN FUNCTIONALITIES HELPER FUNCTIONS
// =====================================================================

void addDays(int day, int month, int year, int daysToAdd,
             int *newDay, int *newMonth, int *newYear) {
    
    struct tm date = {0};
    date.tm_mday = day;
    date.tm_mon = month - 1;       // struct tm months: 0-11
    date.tm_year = year - 1900;    // struct tm years: since 1900

    date.tm_mday += daysToAdd;     // add days

    mktime(&date);                 // normalize the date

    *newDay = date.tm_mday;
    *newMonth = date.tm_mon + 1;
    *newYear = date.tm_year + 1900;
}


void UpdateBookQuantity(int bookID,int flag){
    // if flag is 1, increment quantity instead of decrementing
    // if flag is 0, decrement quantity
    FILE *fp = fopen("Books.txt", "r");
    if (fp == NULL) {
        printf("Error: could not open Books.txt\n");
        return; // File not found
    }
    FILE *temp = fopen("tempforbook.txt", "w");
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
            if (flag == 0){
                quantity--; // Decrement quantity
            } else{
                quantity++; // Increment quantity
            }
        }
        // Write updated line to temp file
        fprintf(temp, "%d,%s,%s,%d\n", id, title, author, quantity);
    }
    fclose(fp);
    fclose(temp);
    // Replace original file with temp file
    remove("Books.txt");
    rename("tempforbook.txt", "Books.txt");
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
        }
        
    }
    redPrint("Book not available(Does not exists or quantity is zero)\n");

    fclose(fp);
    return -1; // Book not found
}

void CreateBorrowLine(int borrowId, int userId, int bookId, 
                      int borrowDay, int borrowMonth, int borrowYear,
                      int dueDay, int dueMonth, int dueYear,
                      char string[200]){
    // Borrows.txt: BorrowId,UserID,BookID,BorrowDate,ReturnDate,DueDate

    sprintf(string, "%d,%d,%d,%02d-%02d-%04d,NULL,%02d-%02d-%04d",
            borrowId, userId, bookId,
            borrowDay, borrowMonth, borrowYear,
            dueDay, dueMonth, dueYear);
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
    FILE *temp = fopen("tempforuser.txt", "w");
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

    fclose(fp);
    fclose(temp);
    // Replace original file with temp file
    remove("Users.txt");
    rename("tempforuser.txt", "Users.txt");
}

// =====================================================================
// ADMIN FUNCTIONALITIES
// =====================================================================


void ViewAllBorrowedBooks(){
    if (strcmp(user.email, ADMIN_EMAIL) != 0){
        redPrint("Only Admins can view all borrowed books!\n");
        return;
    }
    FILE *fp = fopen("Borrows.txt", "r");
    if (fp == NULL) {
        printf("Error: could not open Borrows.txt\n");
        return; // File not found
    }
    int borrowId, userId, bookId;
    char borrowDate[15], returnDate[15], dueDate[15];

    char userNames[100][30];
    char bookNames[100][30];
    getUserNamesArray(userNames);
    getBookNamesArray(bookNames);

    printf("All Borrowed Books:\n");
    printf("%-5s %-20s %-40s %-15s %-15s\n", "ID", "User", "Book Title", "Borrow Date", "Due Date");
    printf("---------------------------------------------------------------------------------------------------------------\n");
    // Read each line and extract fields separated by commas
    while (fscanf(fp, "%d,%d,%d,%[^,],%[^,],%[^,\n]\n",
                  &borrowId,
                  &userId,
                  &bookId,
                  borrowDate,
                  returnDate,
                  dueDate) == 6)
    {
        printf("%-5d %-20s %-40s %-15s %-15s\n", borrowId, userNames[userId], bookNames[bookId], borrowDate, dueDate);
    }
    printf("---------------------------------------------------------------------------------------------------------------\n\n");
    fclose(fp);

}

void RemoveBook(){
    if (strcmp(user.email, ADMIN_EMAIL) != 0){
        redPrint("Only Admins can remove books!\n");
        return;
    }
    int id;
    printf("Enter Book ID to Remove: ");
    scanf("%d", &id);
    getchar();
    int x = RemoveLineByID("Books.txt", id);
    if (x){
        greenPrint("Book Removed Successfully!\n");
    } else{
        redPrint("Book ID not found!\n");
    }

}

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

// =====================================================================
// ADMIN FUNCTIONALITIES HELPER FUNCTIONS
// =====================================================================

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


int CreateBookLine(int id, char bookName[30], char author[30], int quantity, char string[200]){
    int written;
    trimNewline(bookName);
    trimNewline(author);

    written = sprintf(string, "%d,%s,%s,%d",
                      id, bookName, author, quantity);
    // Check for errors (sprintf returns number of chars written or negative if failed)
    if (written < 0)
        return 0;
    else
        return 1;

}

// =====================================================================
// GENERAL HELPER FUNCTIONS
// =====================================================================

void getCurrentDate(int *day, int *month, int *year) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    *day = tm.tm_mday;
    *month = tm.tm_mon + 1;   // months are 0-11
    *year = tm.tm_year + 1900; // years since 1900
}

void getUserNamesArray(char userNames[100][30]){
    FILE *fp = fopen("Users.txt", "r");
    if (fp == NULL) {
        printf("Error: could not open Users.txt\n");
        return; // File not found
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
        strcpy(userNames[id], name);
    }

    fclose(fp);
}

void getBookNamesArray(char bookNames[100][30]){
    FILE *fp = fopen("Books.txt", "r");
    if (fp == NULL) {
        printf("Error: could not open Books.txt\n");
        return; // File not found
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
        strcpy(bookNames[id], title);
    }

    fclose(fp);
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
