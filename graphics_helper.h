#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

// variabila care va retine handle carte GOP
EFI_GRAPHICS_OUTPUT_PROTOCOL *mGop;

// generic
EFI_STATUS LocateGop();

// graphics
EFI_STATUS FillRect(UINTN X, UINTN Y, UINTN W, UINTN H, EFI_GRAPHICS_OUTPUT_BLT_PIXEL Color);
VOID SetCursorPos(UINTN Col, UINTN Row);