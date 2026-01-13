#include <Uefi.h>
#include <Library/PrintLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include "graphics_helper.h"

// definim numarul de caractere maxim din titlu (fara null character)
#ifndef TITLE_MAX_LENGTH
#define TITLE_MAX_LENGTH 255
#endif

// definim numarul de caractere maxim din descriere (fara null character)
#ifndef DESCRIPTION_MAX_LENGTH
#define DESCRIPTION_MAX_LENGTH 1023
#endif

typedef struct tab {
    char* title;
    char* description;
} Tab;

extern int tabNumber;
extern Tab* tabs;
extern int selectedTabIdx;

// functii
EFI_STATUS InitMenu(int targetTabNumber);
// FOARTE IMPORTANT: Folosim BOOLEAN, nu bool/BOOL
BOOLEAN InitMenuItem(int targetTabIdx, char* title, char* description);

// functie care afiseaza meniul
VOID DisplayMenu(UINT32 screenW, UINT32 screenH);

VOID DestroyMenu();