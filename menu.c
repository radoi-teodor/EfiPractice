#include "menu.h"

extern int tabNumber = 2;
extern Tab* tabs = NULL;

EFI_STATUS InitMenu(){
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
            sizeof(CHAR16) * 255,
            &tabs[i].title
        );

        // alocam pentru descriere
        gBS->AllocatePool(
            EfiLoaderData,
            sizeof(CHAR16) * 1024,
            &tabs[i].description
        );

    }

    return status;
}

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