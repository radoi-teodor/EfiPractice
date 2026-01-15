#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include "graphics_helper.h"
#include "menu.h"

// culoare background - format: Blue, Green, Red
EFI_GRAPHICS_OUTPUT_BLT_PIXEL bg   = { 0x10, 0x10, 0x10, 0x00 };

EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  // preluam GOP
  EFI_STATUS status = LocateGop();

  // facem error handling
  if(EFI_ERROR(status)){
    Print(L"[!] Could not get GOP handle\n");
    return status;
  }

  // preluam rezolutia ecranului
  // preluam latimea in pixeli
  UINT32 ScreenW = mGop->Mode->Info->HorizontalResolution;
  dispX = ScreenW;
  // repluam lungimea in pixeli
  UINT32 ScreenH = mGop->Mode->Info->VerticalResolution;
  dispY = ScreenH;

  // preluam cate coloane avem
  colNumber = 0; // preluam cate coloane are ecranul
  rowNumber = 0;   // preluam cate randuri are ecranul
  status = gST->ConOut->QueryMode(gST->ConOut, gST->ConOut->Mode->Mode, &colNumber, &rowNumber);
  if(EFI_ERROR(status)){
    Print(L"[!] Could not get rows and columns\n");
    return status;
  }

  /*
  vom pregati aplicatia pentru 60 FPS
  60 FPS: 1/60 sec = 16.666 ms => 166666 * 100ns
  -------------------------
  asta inseamna ca odata la 166666 * 100 nanosecunde avem un cadru
  */

  // variabila care va stoca un event gol
  EFI_EVENT timerEvent;

  // creem un event care va fi apelat odata la fiecare frame
  status = gBS->CreateEvent(
      EVT_TIMER,    // tipul event-ului; EVT_TIMER e necesar pentru a folosi eventul cu SetTimer
      TPL_CALLBACK, // nivel de prioritate (ori `TPL_CALLBACK` ori `TPL_NOTIFY`)
      NULL,         // functie de callback (nula); definitie functie callback `VOID EFIAPI MyTimerCallback (IN EFI_EVENT Event, IN VOID *Context);`
      NULL,         // pointer catre parametrul transmis functiei de callback prin argumentul `Context`
      &timerEvent   // argument de output catre variabila care retine timer-ul
  );

  // facem error handling
  if (EFI_ERROR(status)) {
    Print(L"[!] CreateEvent failed with status: %r\n", status);
    return status;
  }

  // creem un timer de 166666 * 100 nanosecunde
  // BootServices->SetTimer functioneaza cu unitati de 100 nanosecundeEFI_EVENT TimerEvent;
  // evenimentul se va declansa de fiecare data cand timpul expira
  status = gBS->SetTimer(
    timerEvent,     // ce event se declanseaza
    TimerPeriodic,  // tipul de timer; TimerPeriodic - eventul se repeta
    166666          // perioada, in unitati de 100 nanosecunde
  );
  // facem error handling
  if (EFI_ERROR(status)) {
    Print(L"[!] SetTimer failed with status: %r\n", status);
    gBS->CloseEvent(timerEvent);
    return status;
  }


  // initializam meniul
  status = InitMenu(3);

  // setal meniul - item 1
  InitMenuItem(0, "Title 1", "Desc 1");
  InitMenuItem(1, "Title 2", "Desc 2");
  InitMenuItem(2, "Title 3", "Desc 3");

  // error handle
  if (EFI_ERROR(status)) {
    return status;
  }


  // va trebui sa definim un array in care sa tinem doua evenimente care pot fi declansate odata pe frame:
  // - ori primim input de la tastatura (event cu prioritate)
  // - ori este declansat `TimerEvent` - un event gol care semnaleaza ca nu avem niciun input
  EFI_EVENT waitList[2];
  waitList[0] = gST->ConIn->WaitForKey; // eveniment tastatură
  waitList[1] = timerEvent;            // eveniment frame
  // prioritatea eventurilor este data de indecsii din array

  // definim un boolean care ne va spune daca aplicatia ruleaza
  BOOLEAN running = TRUE;

  while (running) {
    UINTN index;
    status = gBS->WaitForEvent(2, waitList, &index); // preluam indexul event-ului care s-a declansat

    // gestionam inputul
    if (index == 0) {
      // avem event de tastatura
      EFI_INPUT_KEY key;
      status = gST->ConIn->ReadKeyStroke(gST->ConIn, &key); // preluam key-ul apasat de user

      if (!EFI_ERROR(status)) {
        if (key.ScanCode == SCAN_ESC) { // daca user-ul a apasat ESC,
          running = FALSE;              // oprim executia
        }

        if (key.ScanCode == SCAN_LEFT){
          if(selectedTabIdx==0){
            selectedTabIdx = 2;
          }else{
            selectedTabIdx -=1;
          }
        }

        if (key.ScanCode == SCAN_RIGHT){
          selectedTabIdx = (selectedTabIdx + 1) % tabNumber;
        }
      }
    }
    
    // daca avem vreo eroare, iesim din loop
    if (EFI_ERROR(status)) break;

    // curatam ecranul
    FillRect(0, 0, ScreenW, ScreenH, bg);

    // desenam
    DisplayMenu(ScreenW, ScreenH);

  }

  DestroyMenu();

  return EFI_SUCCESS;
}