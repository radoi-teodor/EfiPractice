#include "menu.h"

int tabNumber = 2;
int selectedTabIdx = -1;
Tab* tabs = NULL;

// variabile neexterne
EFI_GRAPHICS_OUTPUT_BLT_PIXEL butColor   = { 0x6e, 0x45, 0x13, 0x00 };
EFI_GRAPHICS_OUTPUT_BLT_PIXEL selectedButColor   = { 0xc9, 0x7e, 0x22, 0x00 };

// functia va initializa meniul cu un numar de elemente
EFI_STATUS InitMenu(int targetTabNumber){
    tabNumber = targetTabNumber;

    EFI_STATUS status = gBS->AllocatePool(
        EfiLoaderData,
        sizeof(Tab) * tabNumber,
        &tabs
    );

    if (EFI_ERROR (status)) {
        Print(L"[!] Error creating menu\n");
        return status;
    }

    for(int i = 0; i < tabNumber; i++){

        // alocam pentru titlu
        gBS->AllocatePool(
            EfiLoaderData,
            sizeof(char) * (TITLE_MAX_LENGTH+1),
            &tabs[i].title
        );

        // alocam pentru descriere
        gBS->AllocatePool(
            EfiLoaderData,
            sizeof(char) * (DESCRIPTION_MAX_LENGTH+1),
            &tabs[i].description
        );

    }


    // definim by default tabul selectat cu primul item
    if(targetTabNumber>0){
        selectedTabIdx = 0;
    }

    return status;
}

BOOLEAN InitMenuItem(int targetTabIdx, char* title, char* description){
    if(targetTabIdx >= tabNumber){
        return FALSE;
    }

    if(title == NULL || description == NULL){
        return FALSE;
    }

    // verificam marimea bufferelor (titlu si descriere)
    if(AsciiStrLen(title) > TITLE_MAX_LENGTH){
        return FALSE;
    }
    if(AsciiStrLen(description) > TITLE_MAX_LENGTH){
        return FALSE;
    }

    AsciiStrCpyS(tabs[targetTabIdx].title, TITLE_MAX_LENGTH + 1, title);
    AsciiStrCpyS(tabs[targetTabIdx].description, DESCRIPTION_MAX_LENGTH + 1, description);

    return TRUE;
}

// functie care afiseaza meniul
VOID DisplayMenu(UINT32 screenW, UINT32 screenH){
    int upperItemUnit = screenW/tabNumber;

    for(int i = 0; i < tabNumber; i++){
        if(i == selectedTabIdx){
            FillRect(i*upperItemUnit, 0, upperItemUnit, 50, selectedButColor);
        }else{
            FillRect(i*upperItemUnit, 0, upperItemUnit, 50, butColor);
        }
    }
}

// va elibera memoria meniului
VOID DestroyMenu(){
    if(tabs == NULL){
        return; // nu s-a alocat de la inceput meniul
    }

    // incercam sa eliberam string-urile
    for(int i = 0; i < tabNumber; i++){
        gBS->FreePool(tabs[i].title);
        gBS->FreePool(tabs[i].description);
    }

    EFI_STATUS status = gBS->FreePool(tabs);

    if (EFI_ERROR (status)) {
        Print(L"[!] Error destroying menu\n");
    }
}