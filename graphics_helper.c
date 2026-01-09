#include "graphics_helper.h"

// variabila care va retine handle carte GOP
extern EFI_GRAPHICS_OUTPUT_PROTOCOL *mGop = NULL;

// generic

// functie care va obtine handle catre protocolul GOP
// locate protocol este folosit fiindca GOP e global
EFI_STATUS LocateGop()
{
  return gBS->LocateProtocol(
      &gEfiGraphicsOutputProtocolGuid,
      NULL,
      (VOID **)&mGop
  );
}

// graphics

// functie care va crea un dreptunchi la o coordonata
// cu latime si lungime data de parametrii
EFI_STATUS FillRect(UINTN X, UINTN Y, UINTN W, UINTN H, EFI_GRAPHICS_OUTPUT_BLT_PIXEL Color)
{
  if (mGop == NULL) return EFI_NOT_READY;

  return mGop->Blt(
      mGop,
      &Color,
      EfiBltVideoFill,
      0, 0,
      X, Y,
      W, H,
      0
  );
}

VOID SetCursorPos(UINTN Col, UINTN Row)
{
  gST->ConOut->SetCursorPosition(gST->ConOut, Col, Row);
}