#pragma once
#include "../StrSetLib/StrSetLib.h"

int ScanCommand();

OUT_CODE ExecuteCommand(int commandId, SetList_t* setList, BOOL* isCommandEnd);
