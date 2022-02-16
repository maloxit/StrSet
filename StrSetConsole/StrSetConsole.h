#pragma once
#include "../StrSetLib/StrSetLib.h"

//Scan text line for allowed command. Returns command id or -1 if no correct command was found
int ScanCommand();

//Executes command with given id on setList
OUT_CODE ExecuteCommand(int commandId, SetList_t* setList, BOOL* isCommandEnd);
