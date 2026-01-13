#include <Uefi.h>
#include <Library/UefiLib.h> // pentru AsciiPrint
#include <Library/UefiBootServicesTableLib.h>

// variabila care va retine handle carte GOP
extern EFI_GRAPHICS_OUTPUT_PROTOCOL *mGop;
extern int colNumber;
extern int rowNumber;
extern int dispX; // rezolutia X
extern int dispY; // rezolutia Y

// generic
EFI_STATUS LocateGop();

// graphics
EFI_STATUS FillRect(UINTN X, UINTN Y, UINTN W, UINTN H, EFI_GRAPHICS_OUTPUT_BLT_PIXEL Color);
VOID SetCursorPos(UINTN Col, UINTN Row);
