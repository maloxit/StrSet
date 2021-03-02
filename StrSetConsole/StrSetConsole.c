#include "StrSetConsole.h"
#include "../StrSetLib/StrSetLib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#pragma warning(disable:4996)

#define COMMAND_COUNT 18

static OUT_CODE ScanSetName(char* name)
{
  int len;
  len = scanf("%*[\t ]");
  len = scanf("%80[^\n\"\t ]", name);
  if (len == 0)
  {
    return E_INCORRECT_COMMAND_FORMAT;
  }
  return OC_OK;
}

static OUT_CODE ScanItemStr(char* str, BOOL* isCommandEnd)
{
  int len;
  char chTry;
  len = scanf("%*[\t ]");
  if ((chTry = getchar()) != '\"') {
    if (chTry == '\n')
    {
      *isCommandEnd = TRUE;
    }
    return E_INCORRECT_COMMAND_FORMAT;
  }
  len = scanf("%80[^\n\"]", str);
  if (len == 0)
  {
    return E_INCORRECT_COMMAND_FORMAT;
  }
  if ((chTry = getchar()) != '\"') {
    if (chTry == '\n')
    {
      *isCommandEnd = TRUE;
    }
    return E_INCORRECT_COMMAND_FORMAT;
  }
  return OC_OK;
}

typedef OUT_CODE CmdMethod_t(SetList_t* setList, BOOL* isCommandEnd);

static OUT_CODE CmdPrintSetList(SetList_t* setList, BOOL* isCommandEnd)
{
  OUT_CODE oCode;
  oCode = SetListPrint(setList, stdout, "%s;\n");
  switch (oCode)
  {
  case OC_YES:
    return OC_OK;
    break;
  case OC_NO:
    printf("Set list is empty.\n");
    return OC_OK;
    break;
  default:
    return oCode;
    break;
  }
}

static OUT_CODE CmdPrintSetListAll(SetList_t* setList, BOOL* isCommandEnd)
{
  OUT_CODE oCode;
  oCode = SetListPrintAll(setList, stdout, "%s:\n", "{\n", "\t\"%s\",\n", "};\n");
  switch (oCode)
  {
  case OC_YES:
    return OC_OK;
    break;
  case OC_NO:
    printf("Set list is empty.\n");
    return OC_OK;
    break;
  default:
    return oCode;
    break;
  }
}

static OUT_CODE CmdSetNew(SetList_t* setList, BOOL* isCommandEnd)
{
  OUT_CODE oCode;
  char name[81];
  Set_t* newSet;
  oCode = ScanSetName(name);
  if (oCode != OC_OK)
  {
    return oCode;
  }
  oCode = SetGet(&newSet, name);
  if (oCode != OC_OK)
  {
    return oCode;
  }
  oCode = SetListAdd(setList, newSet);
  switch (oCode)
  {
  case OC_YES:
    printf("Set '%s' is added to list.\n", name);
    return OC_OK;
    break;
  case OC_NO:
    SetFree(newSet);
    printf("Set '%s' is already in list.\n", name);
    return OC_OK;
    break;
  default:
    return oCode;
    break;
  }
}

static OUT_CODE CmdSetDelete(SetList_t* setList, BOOL* isCommandEnd)
{
  OUT_CODE oCode;
  char name[81];
  Set_t* deletedSet;
  oCode = ScanSetName(name);
  if (oCode != OC_OK)
  {
    return oCode;
  }
  oCode = SetListRemove(setList, &deletedSet, name);
  switch (oCode)
  {
  case OC_YES:
    oCode = SetFree(deletedSet);
    if (oCode == OC_OK)
    {
      printf("Set '%s' is deleted from list.\n", name);
    }
    return oCode;
    break;
  case OC_NO:
    printf("Set '%s' is not in list.\n", name);
    return OC_OK;
    break;
  default:
    return oCode;
    break;
  }
}

static OUT_CODE CmdSetAddItem(SetList_t* setList, BOOL* isCommandEnd)
{
  OUT_CODE oCode;
  char name[81];
  char str[81];
  Set_t* set;
  oCode = ScanSetName(name);
  if (oCode != OC_OK)
  {
    return oCode;
  }
  oCode = SetListFind(setList, &set, name);
  if (oCode != OC_YES)
  {
    if (oCode == OC_NO)
    {
      printf("Set '%s' is not in list.\n", name);
      return OC_OK;
    }
    return oCode;
  }
  oCode = ScanItemStr(str, isCommandEnd);
  if (oCode != OC_OK)
  {
    return oCode;
  }
  oCode = SetAddItem(set, str);
  switch (oCode)
  {
  case OC_YES:
    printf("Item \"%s\" is added to set '%s'.\n", str, name);
    return OC_OK;
    break;
  case OC_NO:
    printf("Item \"%s\" is already in set '%s'.\n", str, name);
    return OC_OK;
    break;
  default:
    return oCode;
    break;
  }
}

static OUT_CODE CmdSetRemoveItem(SetList_t* setList, BOOL* isCommandEnd)
{
  OUT_CODE oCode;
  char name[81];
  char str[81];
  Set_t* set;
  oCode = ScanSetName(name);
  if (oCode != OC_OK)
  {
    return oCode;
  }
  oCode = SetListFind(setList, &set, name);
  if (oCode != OC_YES)
  {
    if (oCode == OC_NO)
    {
      printf("Set '%s' is not in list.\n", name);
      return OC_OK;
    }
    return oCode;
  }
  oCode = ScanItemStr(str, isCommandEnd);
  if (oCode != OC_OK)
  {
    return oCode;
  }
  oCode = SetRemoveItem(set, str);
  switch (oCode)
  {
  case OC_YES:
    printf("Item \"%s\" is deleted from set '%s'.\n", str, name);
    return OC_OK;
    break;
  case OC_NO:
    printf("Item \"%s\" is not in set '%s'.\n", str, name);
    return OC_OK;
    break;
  default:
    return oCode;
    break;
  }
}

static OUT_CODE CmdSetHasItem(SetList_t* setList, BOOL* isCommandEnd)
{
  OUT_CODE oCode;
  char name[81];
  char str[81];
  Set_t* set;
  oCode = ScanSetName(name);
  if (oCode != OC_OK)
  {
    return oCode;
  }
  oCode = SetListFind(setList, &set, name);
  if (oCode != OC_YES)
  {
    if (oCode == OC_NO)
    {
      printf("Set '%s' is not in list.\n", name);
      return OC_OK;
    }
    return oCode;
  }
  oCode = ScanItemStr(str, isCommandEnd);
  if (oCode != OC_OK)
  {
    return oCode;
  }
  oCode = SetHasItem(set, str);
  switch (oCode)
  {
  case OC_YES:
    printf("Yes: Item \"%s\" is in set '%s'.\n", str, name);
    return OC_OK;
    break;
  case OC_NO:
    printf("No : Item \"%s\" is not in set '%s'.\n", str, name);
    return OC_OK;
    break;
  default:
    return oCode;
    break;
  }
}

static OUT_CODE CmdSetCardinality(SetList_t* setList, BOOL* isCommandEnd)
{
  OUT_CODE oCode;
  char name[81];
  Set_t* set;
  int cardinality;
  oCode = ScanSetName(name);
  if (oCode != OC_OK)
  {
    return oCode;
  }
  oCode = SetListFind(setList, &set, name);
  if (oCode != OC_YES)
  {
    if (oCode == OC_NO)
    {
      printf("Set '%s' is not in list.\n", name);
      return OC_OK;
    }
    return oCode;
  }
  oCode = SetCardinality(set, &cardinality);
  if (oCode == OC_OK)
  {
    printf("Cardinality of set '%s' is %i.\n", name, cardinality);
  }
  return oCode;
}

static OUT_CODE CmdSetPrint(SetList_t* setList, BOOL* isCommandEnd)
{
  OUT_CODE oCode;
  char name[81];
  Set_t* set;
  oCode = ScanSetName(name);
  if (oCode != OC_OK)
  {
    return oCode;
  }
  oCode = SetListFind(setList, &set, name);
  if (oCode != OC_YES)
  {
    if (oCode == OC_NO)
    {
      printf("Set '%s' is not in list.\n", name);
      return OC_OK;
    }
    return oCode;
  }
  oCode = SetPrintItemList(set, stdout, "%s:\n", "{\n", "\t\"%s\",\n", "};\n");
  return oCode;
}

static OUT_CODE CmdSetCopyTo(SetList_t* setList, BOOL* isCommandEnd)
{
  OUT_CODE oCode;
  char name1[81];
  char name2[81];
  Set_t* set1;
  Set_t* set2;
  oCode = ScanSetName(name1);
  if (oCode != OC_OK)
  {
    return oCode;
  }
  oCode = ScanSetName(name2);
  if (oCode != OC_OK)
  {
    return oCode;
  }
  oCode = SetListFind(setList, &set1, name1);
  if (oCode != OC_YES)
  {
    if (oCode == OC_NO)
    {
      printf("Set '%s' is not in list.\n", name1);
      return OC_OK;
    }
    return oCode;
  }
  oCode = SetListFind(setList, &set2, name2);
  if (oCode != OC_NO)
  {
    if (oCode == OC_YES)
    {
      printf("Set '%s' is already in list.\n", name2);
      return OC_OK;
    }
    return oCode;
  }
  oCode = SetGetCopy(set1, &set2, name2);
  if (oCode != OC_OK)
  {
    return oCode;
  }
  oCode = SetListAdd(setList, set2);
  if (oCode == OC_YES)
  {
    printf("Set '%s' is copied to set '%s'.\n", name1, name2);
    return OC_OK;
  }
  SetFree(set2);
  return oCode;
}

static OUT_CODE CmdSetRename(SetList_t* setList, BOOL* isCommandEnd)
{
  OUT_CODE oCode;
  char name1[81];
  char name2[81];
  Set_t* set;
  oCode = ScanSetName(name1);
  if (oCode != OC_OK)
  {
    return oCode;
  }
  oCode = ScanSetName(name2);
  if (oCode != OC_OK)
  {
    return oCode;
  }
  oCode = SetListFind(setList, &set, name2);
  if (oCode != OC_NO)
  {
    if (oCode == OC_YES)
    {
      printf("Set '%s' is already in list.\n", name2);
      return OC_OK;
    }
    return oCode;
  }
  oCode = SetListRemove(setList, &set, name1);
  if (oCode != OC_YES)
  {
    if (oCode == OC_NO)
    {
      printf("Set '%s' is not in list.\n", name1);
      return OC_OK;
    }
    return oCode;
  }

  oCode = SetRename(set, name2);
  if (oCode != OC_OK)
  {
    SetFree(set);
    return oCode;
  }
  oCode = SetListAdd(setList, set);
  if (oCode == OC_YES)
  {
    printf("Set '%s' is renamed to set '%s'.\n", name1, name2);
    return OC_OK;
  }
  SetFree(set);
  return oCode;
}

static OUT_CODE CmdHelp(SetList_t* setList, BOOL* isCommandEnd)
{
  printf("exit                                               - stop program.\n\n");
  printf("help                                               - prints help.\n\n");
  printf("print                                              - prints a set list.\n\n");
  printf("printall                                           - prints a set list with items.\n\n");
  printf("new <SET_NAME>                                     - creates an empty set named <SET_NAME> if it is not created yet.\n\n");
  printf("delete <SET_NAME>                                  - delets a set <SET_NAME> if it is in set list.\n\n");
  printf("sprint <SET_NAME>                                  - prints a set <SET_NAME> with items if it is in set list.\n\n");
  printf("copyto <SET_NAME> <SET_NEW_NAME>                   - creates a copy of set <SET_NAME> with name <SET_NEW_NAME>.\n\n");
  printf("rename <SET_NAME> <SET_NEW_NAME>                   - renames a set <SET_NAME> to <SET_NEW_NAME>.\n\n");
  printf("add <SET_NAME> \"<STRING>\"                          - adds an item <STRING> to set <SET_NAME>.\n\n");
  printf("remove <SET_NAME> \"<STRING>\"                       - removes an item <STRING> from set <SET_NAME>.\n\n");
  printf("has <SET_NAME> \"<STRING>\"                          - checks if item <STRING> is in set <SET_NAME>.\n\n");
  printf("cardinality <SET_NAME>                             - prints cardinality of set <SET_NAME>.\n\n");
  printf("union <SET_A_NAME> <SET_B_NAME> [<SET_C_NAME>]     - calculates union of <SET_A_NAME> and <SET_B_NAME> and saves it as <SET_C_NAME> or 'ans'.\n\n");
  printf("intersect <SET_A_NAME> <SET_B_NAME> [<SET_C_NAME>] - calculates intersection of <SET_A_NAME> and <SET_B_NAME> and saves it as <SET_C_NAME> or 'ans'.\n\n");
  printf("diff <SET_A_NAME> <SET_B_NAME> [<SET_C_NAME>]      - calculates difference of <SET_A_NAME> and <SET_B_NAME> and saves it as <SET_C_NAME> or 'ans'.\n\n");
  printf("symdiff <SET_A_NAME> <SET_B_NAME> [<SET_C_NAME>]   - calculates symetric difference of <SET_A_NAME> and <SET_B_NAME> and saves it as <SET_C_NAME> or 'ans'.\n\n");
  printf("isinclusion <SET_A_NAME> <SET_B_NAME>              - checks if set <SET_A_NAME> is included in set <SET_B_NAME>.\n\n");
  return OC_OK;
}

static OUT_CODE CmdSetBinOper(SetList_t* setList, BOOL* isCommandEnd, BOOL onlyA, BOOL AnB, BOOL onlyB, const char* operName)
{
  OUT_CODE oCode;
  char name1[81];
  char name2[81];
  char name3[81];
  Set_t* set1;
  Set_t* set2;
  Set_t* set3;
  Set_t* setToReplace = NULL;

  oCode = ScanSetName(name1);
  if (oCode != OC_OK)
  {
    return oCode;
  }
  oCode = ScanSetName(name2);
  if (oCode != OC_OK)
  {
    return oCode;
  }
  oCode = ScanSetName(name3);
  if (oCode != OC_OK)
  {
    strcpy(name3, "ans");
  }
  oCode = SetListFind(setList, &set1, name1);
  if (oCode != OC_YES)
  {
    if (oCode == OC_NO)
    {
      printf("Set '%s' is not in list.\n", name1);
      return OC_OK;
    }
    return oCode;
  }
  oCode = SetListFind(setList, &set2, name2);
  if (oCode != OC_YES)
  {
    if (oCode == OC_NO)
    {
      printf("Set '%s' is not in list.\n", name2);
      return OC_OK;
    }
    return oCode;
  }
  oCode = SetListFind(setList, &set3, name3);
  if (oCode != OC_NO)
  {
    if (oCode == OC_YES && strcmp(name3, "ans") != 0)
    {
      printf("Set '%s' is already in list.\n", name3);
      return OC_OK;
    }
    else if (oCode != OC_YES)
    {
      return oCode;
    }
  }
  oCode = SetBinOper(set1, set2, &set3, name3, onlyA, AnB, onlyB);
  if (oCode != OC_OK)
  {
    return oCode;
  }
  if (strcmp(name3, "ans") == 0)
  {
    oCode = SetListRemove(setList, &setToReplace, "ans");
    if (oCode == OC_YES)
    {
      SetFree(setToReplace);
    }
    else if (oCode != OC_NO)
    {
      SetFree(set3);
      return oCode;
    }
  }
  oCode = SetListAdd(setList, set3);
  if (oCode != OC_YES)
  {
    SetFree(set3);
    return oCode;
  }
  printf("%s of set '%s' and '%s' is added to list as set '%s'.\n", operName, name1, name2, name3);
  return OC_OK;
}

static OUT_CODE CmdSetUnion(SetList_t* setList, BOOL* isCommandEnd)
{
  return CmdSetBinOper(setList, isCommandEnd, TRUE, TRUE, TRUE, "Union");
}

static OUT_CODE CmdSetIntersection(SetList_t* setList, BOOL* isCommandEnd)
{
  return CmdSetBinOper(setList, isCommandEnd, FALSE, TRUE, FALSE, "Intersection");
}

static OUT_CODE CmdSetDifference(SetList_t* setList, BOOL* isCommandEnd)
{
  return CmdSetBinOper(setList, isCommandEnd, TRUE, FALSE, FALSE, "Difference");
}

static OUT_CODE CmdSetSymDifference(SetList_t* setList, BOOL* isCommandEnd)
{
  return CmdSetBinOper(setList, isCommandEnd, TRUE, FALSE, TRUE, "SymDifference");
}

static OUT_CODE CmdSetIsInclusion(SetList_t* setList, BOOL* isCommandEnd)
{
  OUT_CODE oCode;
  char name1[81];
  char name2[81];
  Set_t* set1;
  Set_t* set2;


  oCode = ScanSetName(name1);
  if (oCode != OC_OK)
  {
    return oCode;
  }
  oCode = ScanSetName(name2);
  if (oCode != OC_OK)
  {
    return oCode;
  }
  oCode = SetListFind(setList, &set1, name1);
  if (oCode != OC_YES)
  {
    if (oCode == OC_NO)
    {
      printf("Set '%s' is not in list.\n", name1);
      return OC_OK;
    }
    return oCode;
  }
  oCode = SetListFind(setList, &set2, name2);
  if (oCode != OC_YES)
  {
    if (oCode == OC_NO)
    {
      printf("Set '%s' is not in list.\n", name2);
      return OC_OK;
    }
    return oCode;
  }
  oCode = SetIsInclusion(set1, set2);
  switch (oCode)
  {
  case OC_YES:
    printf("Yes: Set '%s' is included in set '%s'.\n", name1, name2);
    return OC_OK;
    break;
  case OC_NO:
    printf("No : Set '%s' is not included in set '%s'.\n", name1, name2);
    return OC_OK;
    break;
  default:
    return oCode;
    break;
  }
}

static struct { char cmdStr[15]; CmdMethod_t* cmdMethod; } commands[] = {
  {"exit", NULL},
  {"help", CmdHelp},
  {"print", CmdPrintSetList},
  {"printall", CmdPrintSetListAll},
  {"new", CmdSetNew},
  {"delete", CmdSetDelete},
  {"sprint", CmdSetPrint},
  {"copyto", CmdSetCopyTo},
  {"rename", CmdSetRename},
  {"add", CmdSetAddItem},
  {"remove", CmdSetRemoveItem},
  {"has", CmdSetHasItem},
  {"cardinality", CmdSetCardinality},
  {"union", CmdSetUnion},
  {"intersect", CmdSetIntersection},
  {"diff", CmdSetDifference},
  {"symdiff", CmdSetSymDifference},
  {"isinclusion", CmdSetIsInclusion},
};

int ScanCommand()
{
  int i, commandId, len;
  char commandBuffer[21] = "";


  len = scanf("%*[\n\t ]");
  len = scanf("%20[^\n\t \"]", commandBuffer);
  commandId = -1;
  if (len != 0 && strlen(commandBuffer) != 0)
  {
    for (i = 0; i < COMMAND_COUNT; i++)
    {
      if (strncmp(commandBuffer, commands[i].cmdStr, 10) == 0)
      {
        commandId = i;
        break;
      }
    }
  }
  return commandId;
}

OUT_CODE ExecuteCommand(int commandId, SetList_t* setList, BOOL* isCommandEnd)
{
  if (commandId < 1 || commandId >= COMMAND_COUNT)
  {
    printf("Unknown command!\n");
    return OC_OK;
  }
  return commands[commandId].cmdMethod(setList, isCommandEnd);
}