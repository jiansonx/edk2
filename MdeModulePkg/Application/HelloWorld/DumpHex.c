/** @file
  This sample application bases on HelloWorld PCD setting
  to print "UEFI Hello World!" to the UEFI Console.

  Copyright (c) 2006 - 2018, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/
 #pragma optimize( "", off )
#include <Uefi.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Guid/GlobalVariable.h>
#include <Library/MemoryAllocationLib.h>
#include <stdarg.h>

#define CHECK3(...) { Print(__VA_ARGS__); }

//
// String token ID of help message text.
// Shell supports to find help message in the resource section of an application image if
// .MAN file is not found. This global variable is added to make build tool recognizes
// that the help string is consumed by user and then build tool will add the string into
// the resource section. Thus the application can use '-?' option to show help message in
// Shell.
//
GLOBAL_REMOVE_IF_UNREFERENCED EFI_STRING_ID mStringHelpTokenId = STRING_TOKEN (STR_HELLO_WORLD_HELP_INFORMATION);

STATIC CONST CHAR8 Hex[] = {
  '0',
  '1',
  '2',
  '3',
  '4',
  '5',
  '6',
  '7',
  '8',
  '9',
  'A',
  'B',
  'C',
  'D',
  'E',
  'F'
};

#if 0
#define HEX_DUMP1(Indent, Offset, DataSize, UserData, format, ...) \
	Print( __VA_ARGS__)
#define CHECK3(...) { Print(__VA_ARGS__); }

#define HEX_DUMP(Indent, Offset, DataSize, UserData, format, ...)                                           \
{                                                                                             \
  UINT8 *Data;                                                                                \
                                                                                              \
  CHAR8 Val[50];                                                                              \
                                                                                              \
  CHAR8 Str[20];                                                                              \
                                                                                              \
  UINT8 TempByte;                                                                             \
  UINTN Size;                                                                                 \
  UINTN Index;                                                                                \
                                                                                              \
  Data = (UINT8 *)(UserData);                                                                 \
  while (DataSize != 0) {                                                                     \
    Size = 16;                                                                                \
    if (Size > DataSize) {                                                                    \
      Size = DataSize;                                                                        \
    }                                                                                         \
                                                                                              \
    for (Index = 0; Index < Size; Index += 1) {                                               \
      TempByte            = Data[Index];                                                      \
      Val[Index * 3 + 0]  = Hex[TempByte >> 4];                                               \
      Val[Index * 3 + 1]  = Hex[TempByte & 0xF];                                              \
      Val[Index * 3 + 2]  = (CHAR8) ((Index == 7) ? '-' : ' ');                               \
      Str[Index]          = (CHAR8) ((TempByte < ' ' || TempByte > '~') ? '.' : TempByte);    \
    }                                                                                         \
       Print(__VA_ARGS__)  ;                                                                   \
    Val[Index * 3]  = 0;                                                                      \
    Str[Index]      = 0;                                                                      \
    va_list args;                                                                             \
    va_start(args, format);                                                                  \
                                                                                              \
    if (*format == 'c') {                                                                     \
     /*  int c = va_arg(args, int); */                                                             \
    }                                                                                         \
  /*  Print( L"%*a%08X: %-48a *%a*\r\n", Indent, "", Offset, Val, Str);  */                       \
       Indent = 0;                                                                                       \
    Data += Size;                                                                             \
    Offset += Size;                                                                           \
    DataSize -= Size;                                                                         \
  }                                                                                           \
} 

#endif

#define HEX_DUMPP(Indent, Offset, DataSize, UserData, fmt, ...)                           \
{                                                                                            \
  UINT8 *Data;                                                                               \
  CHAR8 Val[50];                                                                             \
  CHAR8 Str[20];                                                                              \
   UINT8 TempByte;                                                                             \
  UINTN Size;                                                                                  \
  UINTN Index;                                                                                \
  VA_LIST args;                                                                              \
  char *format = (char *)(fmt);                                                              \
  VA_START(args, format);                                                                          \
  Data = (UINT8 *)(UserData);                                                                    \
  while (DataSize != 0) {                                                                         \
    Size = 16;                                                                                    \
    if (Size > DataSize) {                                                                       \
      Size = DataSize;                                                                              \
    }                                                                                                \
                                                                                              \
    for (Index = 0; Index < Size; Index += 1) {                                                       \
      TempByte            = Data[Index];                                                                 \
      Val[Index * 3 + 0]  = Hex[TempByte >> 4];                                                         \
      Val[Index * 3 + 1]  = Hex[TempByte & 0xF];                                                            \
      Val[Index * 3 + 2]  = (CHAR8) ((Index == 7) ? '-' : ' ');                                                 \
      Str[Index]          = (CHAR8) ((TempByte < ' ' || TempByte > '~') ? '.' : TempByte);                \
    }                                                                                                    \
                                                                                                        \
    Val[Index * 3]  = 0;                                                                             \
    Str[Index]      = 0;                                                                                  \
    if (*format == 'c') {                                                                                  \
       char* p = (char *) va_arg(args, void *);                                                                \
      Print(L"%s", (char*)p);                                                                             \
    }                                                                                                  \
    Print( L"%*a%08X: %-48a *%a*\r\n", Indent, " ", Offset, Val, Str);                                \
    Data += Size;                                                                                     \
    Offset += Size;                                                                                     \
    DataSize -= Size;                                                                              \
    ++format;                                                                                     \
  }                                                                                                    \
  VA_END(args);                                                                                      \
}                                                                                                   \                                                                        





#define HEX_DUMPPp(Indent, Offset, DataSize, UserData, fmt, ...)                     \
{                                                                                    \
  UINT8 *Data;                                                                         \
  CHAR8 Val[50];                                                                  \
  CHAR8 Str[20]; \
   UINT8 TempByte;                                                                 \
  UINTN Size;                                                                       \
  UINTN Index;                                                                      \
  VA_LIST args;                                                                    \
  char *format = (char *)(fmt);                                                   \
  VA_START(args, format);                                                            \
  Data = (UINT8 *)(UserData);                                                        \
  while (DataSize != 0) {                                                         \
    Size = 16;                                                                   \
    if (Size > DataSize) {                                                       \
      Size = DataSize;                                                          \
    }                                                                                               \
                                                                                               \
    for (Index = 0; Index < Size; Index += 1) {                                                  \
      TempByte            = Data[Index];                                                                 \
      Val[Index * 3 + 0]  = Hex[TempByte >> 4];                                                          \
      Val[Index * 3 + 1]  = Hex[TempByte & 0xF];                                                             \
      Val[Index * 3 + 2]  = (CHAR8) ((Index == 7) ? '-' : ' ');                                                \
      Str[Index]          = (CHAR8) ((TempByte < ' ' || TempByte > '~') ? '.' : TempByte);                 \
    }                                                                                                     \
                                                                                                         \
    Val[Index * 3]  = 0;                                                                              \
    Str[Index]      = 0;                                                                               \
    if (*format == 'c') {                                                                                \
       char* p = (char *) va_arg(args, void *);                                                            \
      Print(L"%s", (char*)p);                                                                               \
    }                                                                                                \
   /* Print( L"%*a%08X: %-48a *%a* r n", Indent, " ", Offset, Val, Str); */                            \
    Data += Size;                                                                           \
    Offset += Size;                                                                     \
    DataSize -= Size;                                                                    \
    ++format;                                                                            \
  }                                                                                    \
  VA_END(args);                                                                      \
}           \

/**
  Dump some hexadecimal data to the screen.

  @param[in] Indent     How many spaces to indent the output.
  @param[in] Offset     The offset of the printing.
  @param[in] DataSize   The size in bytes of UserData.
  @param[in] UserData   The data to print out.
**/
VOID
EFIAPI
DumpHex (
  IN UINTN        Indent,
  IN UINTN        Offset,
  IN UINTN        DataSize,
  IN VOID         *UserData,
  char              *format,
  ...
  )
{
  UINT8 *Data;
  CHAR8 Val[50];
  CHAR8 Str[20];

  UINT8 TempByte;
  UINTN Size;
  UINTN Index;
  VA_LIST args;
  VA_START(args, format);
  Data = UserData;
  while (DataSize != 0) {
    Size = 16;
    if (Size > DataSize) {
      Size = DataSize;
    }

    for (Index = 0; Index < Size; Index += 1) {
      TempByte            = Data[Index];
      Val[Index * 3 + 0]  = Hex[TempByte >> 4];
      Val[Index * 3 + 1]  = Hex[TempByte & 0xF];
      Val[Index * 3 + 2]  = (CHAR8) ((Index == 7) ? '-' : ' ');
      Str[Index]          = (CHAR8) ((TempByte < ' ' || TempByte > '~') ? '.' : TempByte);
    }

    Val[Index * 3]  = 0;
    Str[Index]      = 0;
    if (*format == 'c') {     
       char* p = (char *) va_arg(args, void *);
      Print(L"%s", (char*)p);
    }
    Print( L"%*a%08X: %-48a *%a*\r\n", Indent, " ", Offset, Val, Str);
    Data += Size;
    Offset += Size;
    DataSize -= Size;
    ++format;
  }
  VA_END(args);
}
                                                                                            

/**
  The user Entry Point for Application. The user code starts with this function
  as the real entry point for the application.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  UINTN                   Length;
  Length = 32;
  UINT8                     *Buffer;
  Buffer  = AllocatePool (18);
  Buffer[0] = 9;
  Buffer[1] = 1;
  Buffer[2] = 2;
  Buffer[3] = 5;
  Buffer[4] = 8;
  Buffer[18] = 9;
  Buffer[19] = 10;

  //  DumpHex (0, 0, Length, Buffer , "ccc", L"aa", L"cc", 'b' );
//  CHECK3(L"here %s %s %s", "are", "some", "varargs3(5)\n");
 // UINTN aa  = 3;
  UINTN bb = 0;
//  HEX_DUMP (bb, aa, Length, Buffer);
   // DumpHex (0, 0, Length, Buffer , "%s%s%c", L"aa", "cc", 'b' );
 //  HEX_DUMP (aa, bb, Length, Buffer , L"here %s %s %s", L"are", L"some", L"varargs3(5)\n");
    HEX_DUMPPp(0, bb, Length, Buffer , "ccc", L"aa", L"cc", L'b' );
 // __asm int 3;
 //HEX_DUMP1(0, 0, Length, Buffer , L"%s%s%c", L"aa", L"cc", L'b' );
 // CHECK3(L"here %s %s %s", L"are", L"some", L"varargs3(5)\n");
  return EFI_SUCCESS;
}
