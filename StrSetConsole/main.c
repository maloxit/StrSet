#include "../StrSetLib/StrSetLib.h"
#include "StrSetConsole.h"
#include <stdio.h>
#include <windows.h>

#pragma warning(disable:4996)


void PrintOutCodeInfo(OUT_CODE oCode)
{
  switch (oCode)
  {
  case OC_OK:
    break;
  case OC_NO:
    printf("?NO?\n");
    break;
  case OC_YES:
    printf("?YES?\n");
    break;
  case E_INCORRECT_COMMAND_FORMAT:
    printf("Incorrect command format, see 'help' for reference.\n");
    break;
  case E_MEMALLOCATION_ERROR:
    printf("Out of memory.\n");
    break;
  case E_NULL_REF:
    printf("Programm error 'NULL_REF'.\n");
    break;
  case E_UNCLASSIFIED:
    printf("Programm error 'UNCLASSIFIED'.\n");
    break;
  default:
    printf("Something terrible has happened.\n");
    break;
  }
}

int main(void)
{
  OUT_CODE oCode;
  BOOL isExit = FALSE, isCommandEnd = FALSE;
  int commandId, len;
  SetList_t* setList;
  oCode = SetListGet(&setList);
  if (oCode != OC_OK)
  {
    return -1;
  }
  SetConsoleOutputCP(1251);
  SetConsoleCP(1251);
  printf("Type command 'help' for instructions.\n\n");
  do
  {
    commandId = ScanCommand();
    if (commandId == 0)
    {
      isExit = TRUE;
      continue;
    }
    else
    {
      printf("\n");
      oCode = ExecuteCommand(commandId, setList, &isCommandEnd);
      PrintOutCodeInfo(oCode);
    }
    if (!isCommandEnd && getchar() != '\n') {
      len = scanf("%*[^\n]");
    }
    printf("\n");
  } while (!isExit);
  SetListFree(setList);
  return 0;
}