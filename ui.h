#ifndef UI_H
#define UI_H

#include <windows.h>

// Color functions
void ColorPrint(char str[], int color);
void redPrint(char str[]);
void bluePrint(char str[]);
void greenPrint(char str[]);
void yellowPrint(char str[]);
void brightwhitePrint(char str[]);
void purplePrint(char str[]);

// UI / Menu Functions
void LMSHeading();
void LoginMenu();
void MainMenu();
void AdminMenu();
void ExitMenu();

#endif
