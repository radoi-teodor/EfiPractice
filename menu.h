#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include "graphics_helper.h"

typedef struct tab {
    CHAR16* title;
    CHAR16* description;
} Tab;

int tabNumber;
Tab* tabs;

// functii
EFI_STATUS InitMenu();
VOID DestroyMenu();