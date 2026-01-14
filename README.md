# Library Management System (LMS)

A comprehensive C-based Library Management System designed for managing books, users, borrowing records, and fines. This system implements a tiered membership approach with role-based access control for administrators.

## Features

### User Management
- **User Registration & Authentication**: Secure sign-up and login system with email and password validation
- **Tiered Membership System**:
  - **Silver Tier**: 3 concurrent borrows, Rs100/day late fine
  - **Gold Tier**: 5 concurrent borrows, Rs75/day late fine
  - **Platinum Tier**: 7 concurrent borrows, Rs50/day late fine
- **Fine Management**: Track and pay overdue fines on borrowed books

### Book Management
- View all available books in the library with details (title, author, quantity)
- Book catalog with real-time quantity tracking
- Support for multiple copies of the same book

### Borrowing System
- Borrow books for 1-7 days
- Automatic due date calculation
- Return books with automatic late fine calculation
- Track currently borrowed books per user
- Enforce maximum borrow limits based on tier

### Administrator Features
- Add new books to the library
- Remove books from inventory
- Remove users from the system
- View all borrowed books across the system
- Full system access with admin-level controls

## Technical Details

### Technology Stack
- **Language**: C
- **Platform**: Windows (uses Windows API for colored console output)
- **File Format**: CSV-based data storage
- **Standard Libraries**: stdio.h, stdlib.h, string.h, time.h, ctype.h, windows.h

### Data Structure

**User Struct**:
```c
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
```

### Data Files

1. **Users.txt**: Stores user information
   - Format: `UserId,Email,Password,Name,Contact,Fine,CurrentlyBorrowed,Tier`

2. **Books.txt**: Stores book inventory
   - Format: `BookId,BookName,Author,Quantity`

3. **Borrows.txt**: Stores borrowing records
   - Format: `BorrowId,UserID,BookID,BorrowDate,ReturnDate,DueDate`

4. **meta.txt**: Stores counter information for IDs
   - Format: `UCount BCount BorrCount`

## How to Use

### Compilation
```bash
gcc main.c -o lms
```

### Running the Application
```bash
./lms
```

### User Workflow

1. **Sign Up**
   - Enter valid email (e.g., user@domain.com)
   - Set password (minimum 6 characters, must contain at least 1 number)
   - Enter full name (letters and spaces only)
   - Enter contact number (10-15 digits)
   - Select membership tier (1-3)

2. **Login**
   - Enter registered email
   - Enter password
   - Access member dashboard

3. **Borrow Books**
   - View available books
   - Enter book title to borrow
   - Specify borrowing duration (1-7 days)
   - System calculates and displays due date

4. **Return Books**
   - View currently borrowed books
   - Enter borrow ID to return
   - System automatically calculates any late fines
   - Fine amount deducted from account

5. **Pay Fine**
   - Check current outstanding fine
   - Pay partial or full fine amount
   - Fine updated in system

### Administrator Access
- Login with admin credentials: `admin@gmail.com`
- Access additional menu options for book and user management
- Add books, remove users, view all borrowed books, remove books

## Input Validation

The system includes comprehensive validation for:
- Email format (RFC-compliant email validation)
- Password strength (minimum 6 characters, at least 1 number)
- User names (letters and spaces only)
- Contact numbers (10-15 digits)
- Book quantities (positive integers)
- Borrowing duration (1-7 days)
- Payment amounts (positive values)

## Key Functions

### Authentication
- `SignUp()`: User registration with validation
- `Login()`: User authentication
- `Logout()`: Secure logout
- `EmailExists()`: Check for duplicate emails

### Core Operations
- `BorrowBook()`: Handle book borrowing
- `ReturnBook()`: Handle book returns with fine calculation
- `DisplayBooks()`: Show available books
- `PayFine()`: Process fine payments
- `AddBook()`: Add new books (admin only)
- `RemoveUser()`: Remove users (admin only)
- `RemoveBook()`: Remove books (admin only)

### Utilities
- `getCurrentDate()`: Get system date
- `addDays()`: Add days to a date for due date calculation
- `UpdateUser()`: Update user information in file
- `UpdateBookQuantity()`: Update book availability
- `ValidateEmail()`, `ValidatePassword()`, etc.: Input validation functions
- `ColorPrint()`: Display colored console output

## File Operations

The system uses file-based storage with CSV format:
- Read/write operations for persistent data storage
- Temporary files for safe updates (prevent data loss)
- Atomic operations using file rename
- Proper file handle management

## Error Handling

- File not found checks for all file operations
- Invalid input validation with user-friendly error messages
- Duplicate email prevention
- Book availability checks before borrowing
- Fine calculation with proper date comparisons

## Author

Muhammad Usaim Khan (Student ID: 25K-xxxx)

## Course

PF Lab - First Semester Programming Fundamentals Laboratory

## Notes

- This is a console-based application with Windows-specific features
- All dates are stored in DD-MM-YYYY format
- The system maintains referential integrity between users, books, and borrow records
- User fines are automatically calculated based on tier-specific rates and days late

## Future Enhancements

- Multi-platform support (remove Windows API dependency)
- Database integration for better data management
- GUI interface
- Email notifications for due dates
- Book reservation system
- Advanced reporting features
