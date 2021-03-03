#include "StrSetLib.h"
#include <string.h>
#pragma warning(disable:4996)

typedef struct StringListItem_t_
{
  struct StringListItem_t_* next;
  char* string;
} StringListItem_t;

struct Set_t_
{
  char* name;
  int cardinality;
  StringListItem_t* list;
};

typedef struct SetListItem_t_
{
  struct SetListItem_t_* next;
  Set_t* set;
} SetListItem_t;

struct SetList_t_
{
  int count;
  SetListItem_t* list;
};

OUT_CODE SetListGet(SetList_t** setList_p)
{
  if (!setList_p)
  {
    return E_NULL_REF;
  }
  *setList_p = malloc(sizeof(SetList_t));
  if (!*setList_p)
  {
    return E_MEMALLOCATION_ERROR;
  }
  (*setList_p)->count = 0;
  (*setList_p)->list = NULL;
  return OC_OK;
}

OUT_CODE SetListCount(SetList_t* setList, int* count_p)
{
  if (!setList)
  {
    return E_NULL_REF;
  }
  *count_p = setList->count;
  return OC_OK;
}

OUT_CODE SetListAdd(SetList_t* setList, Set_t* set)
{
  int cmprOut;
  SetListItem_t rootItem;
  SetListItem_t* newItem;
  SetListItem_t* item;
  SetListItem_t* prevItem;
  if (!setList || !set || !(set->name))
  {
    return E_NULL_REF;
  }

  newItem = malloc(sizeof(SetListItem_t));
  if (!newItem)
  {
    return E_MEMALLOCATION_ERROR;
  }
  newItem->set = set;
  newItem->next = NULL;

  rootItem.next = setList->list;
  for (prevItem = &rootItem, item = rootItem.next; item != NULL; item = prevItem->next)
  {
    cmprOut = strcmp(newItem->set->name, item->set->name);
    if (cmprOut < 0)
    {
      prevItem->next = newItem;
      newItem->next = item;
      break;
    }
    if (cmprOut == 0)
    {
      free(newItem);
      return OC_NO;
    }
    prevItem = item;
  }
  if (item == NULL)
  {
    prevItem->next = newItem;
  }
  setList->list = rootItem.next;
  setList->count++;
  return OC_YES;
}

OUT_CODE SetListFind(SetList_t* setList, Set_t** outSet, const char* name)
{
  int cmprOut;
  SetListItem_t rootItem;
  SetListItem_t* item;
  SetListItem_t* prevItem;
  if (!setList || !outSet || !name)
  {
    return E_NULL_REF;
  }

  rootItem.next = setList->list;
  for (prevItem = &rootItem, item = rootItem.next; item != NULL; item = prevItem->next)
  {
    cmprOut = strcmp(name, item->set->name);
    if (cmprOut < 0)
    {
      return OC_NO;
    }
    if (cmprOut == 0)
    {
      *outSet = item->set;
      return OC_YES;
    }
    prevItem = item;
  }
  return OC_NO;
}

OUT_CODE SetListRemove(SetList_t* setList, Set_t** outSet, const char* name)
{
  int cmprOut;
  SetListItem_t rootItem;
  SetListItem_t* item;
  SetListItem_t* prevItem;
  if (!setList || !outSet || !name)
  {
    return E_NULL_REF;
  }

  rootItem.next = setList->list;
  for (prevItem = &rootItem, item = rootItem.next; item != NULL; item = prevItem->next)
  {
    cmprOut = strcmp(name, item->set->name);
    if (cmprOut < 0)
    {
      return OC_NO;
    }
    if (cmprOut == 0)
    {
      prevItem->next = item->next;
      *outSet = item->set;
      free(item);
      setList->list = rootItem.next;
      setList->count--;
      return OC_YES;
    }
    prevItem = item;
  }
  return OC_NO;
}

OUT_CODE SetListFree(SetList_t* setList)
{
  if (!setList)
  {
    return E_NULL_REF;
  }
  SetListItem_t* item;
  SetListItem_t* nextItem;
  for (item = setList->list; item != NULL; item = nextItem)
  {
    nextItem = item->next;
    SetFree(item->set);
    free(item);
  }
  free(setList);
  return OC_OK;
}

OUT_CODE SetListPrint(SetList_t* setList, FILE* stream, const char* setFormat)
{
  SetListItem_t* item;
  if (!setList || !stream || !setFormat)
  {
    return E_NULL_REF;
  }
  if (setList->count == 0)
  {
    return OC_NO;
  }
  for (item = setList->list; item != NULL; item = item->next)
  {
    fprintf(stream, setFormat, item->set->name);
  }
  return OC_YES;
}

OUT_CODE SetListPrintAll(SetList_t* setList, FILE* stream, const char* setFormat, const char* itemsStart, const char* itemsFormat, const char* itemsEnd)
{
  SetListItem_t* item;
  OUT_CODE oCode;
  if (!setList || !stream || !setFormat || !itemsStart || !itemsFormat || !itemsEnd)
  {
    return E_NULL_REF;
  }
  if (setList->count == 0)
  {
    return OC_NO;
  }
  for (item = setList->list; item != NULL; item = item->next)
  {
    oCode = SetPrintItemList(item->set, stream, setFormat, itemsStart, itemsFormat, itemsEnd);
    if (oCode != OC_OK)
    {
      return oCode;
    }
  }
  return OC_YES;
}

static OUT_CODE ItemGet(const char* string, StringListItem_t** outItem_p)
{
  char* itemString;
  int len;
  if (!string || !outItem_p || (len = strlen(string)) > 80)
  {
    return E_NULL_REF;
  }
  *outItem_p = malloc(sizeof(StringListItem_t));
  itemString = malloc(sizeof(char) * (len + 1));

  if (!itemString || !*outItem_p)
  {
    if (itemString)
    {
      free(itemString);
    }
    if (*outItem_p)
    {
      free(*outItem_p);
    }
    return E_MEMALLOCATION_ERROR;
  }
  strncpy(itemString, string, len + 1);
  (*outItem_p)->string = itemString;
  (*outItem_p)->next = NULL;
  return OC_OK;
}

static OUT_CODE ItemFree(StringListItem_t* item)
{
  if (!item)
  {
    return E_NULL_REF;
  }
  if (item->string)
  {
    free(item->string);
  }
  free(item);
  return OC_OK;
}

OUT_CODE SetGet(Set_t** const output, const char* name)
{
  char* str;
  int len;
  if (output == NULL || name == NULL || (len = strlen(name)) > 80)
  {
    return E_NULL_REF;
  }
  str = malloc(sizeof(char) * (len + 1));
  *output = malloc(sizeof(Set_t));
  if (!(*output) || !str)
  {
    if (str)
    {
      free(str);
    }
    if (*output)
    {
      free(output);
    }
    return E_MEMALLOCATION_ERROR;
  }
  strncpy(str, name, (len + 1));
  (*output)->name = str;
  (*output)->cardinality = 0;
  (*output)->list = NULL;
  return OC_OK;
}

OUT_CODE SetFree(Set_t* set)
{
  StringListItem_t* item, * nextItem;
  if (set == NULL)
  {
    return E_NULL_REF;
  }
  item = set->list;
  while (item != NULL)
  {
    nextItem = item->next;
    ItemFree(item);
    item = nextItem;
  }
  free(set->name);
  free(set);
  return OC_OK;
}

OUT_CODE SetName(Set_t* set, char** name_p)
{
  if (!set)
  {
    return E_NULL_REF;
  }
  *name_p = set->name;
  return OC_OK;
}

OUT_CODE SetRename(Set_t* set, const char* name)
{
  char* str;
  int len;
  if (!set || !name || (len = strlen(name)) > 80)
  {
    return E_NULL_REF;
  }
  str = malloc(sizeof(char) * (len + 1));
  if (!str)
  {
    return E_MEMALLOCATION_ERROR;
  }
  free(set->name);
  strncpy(str, name, (len + 1));
  set->name = str;
  return OC_OK;
}

OUT_CODE SetCardinality(Set_t* set, int* cardinality_p)
{
  if (!set)
  {
    return E_NULL_REF;
  }
  *cardinality_p = set->cardinality;
  return OC_OK;
}

OUT_CODE SetGetCopy(Set_t* setFrom, Set_t** setTo_p, const char* name)
{
  OUT_CODE oCode;
  StringListItem_t* item;
  StringListItem_t rootCopyItem;
  StringListItem_t* precCopyItem;
  StringListItem_t* copyItem;
  if (!setFrom || !setTo_p)
  {
    return E_NULL_REF;
  }
  oCode = SetGet(setTo_p, name);
  if (oCode != OC_OK)
  {
    return oCode;
  }
  rootCopyItem.next = (*setTo_p)->list;
  item = setFrom->list;
  precCopyItem = &rootCopyItem;
  while (item != NULL)
  {
    oCode = ItemGet(item->string, &copyItem);
    if (oCode != OC_OK)
    {
      (*setTo_p)->list = rootCopyItem.next;
      SetFree(*setTo_p);
      return oCode;
    }
    precCopyItem->next = copyItem;
    (*setTo_p)->cardinality++;
    precCopyItem = copyItem;
    item = item->next;
  }
  (*setTo_p)->list = rootCopyItem.next;
  return OC_OK;
}

OUT_CODE SetHasItem(Set_t* set, const char* string)
{
  StringListItem_t* item;
  int cmprOut;
  if (!set || !string)
  {
    return E_NULL_REF;
  }
  for (item = set->list; item != NULL; item = item->next)
  {
    cmprOut = strcmp(string, item->string);
    if (cmprOut > 0)
    {
      continue;
    }
    if (cmprOut < 0)
    {
      return OC_NO;
    }
    if (cmprOut == 0)
    {
      return OC_YES;
    }
  }
  return OC_NO;
}

OUT_CODE SetAddItem(Set_t* set, const char* string)
{
  OUT_CODE oCode;
  int cmprOut;
  StringListItem_t rootItem;
  StringListItem_t* newItem;
  StringListItem_t* item;
  StringListItem_t* prevItem;
  if (!set || !string)
  {
    return E_NULL_REF;
  }

  oCode = ItemGet(string, &newItem);
  if (oCode != OC_OK)
  {
    return oCode;
  }

  rootItem.next = set->list;
  for (prevItem = &rootItem, item = rootItem.next; item != NULL; item = prevItem->next)
  {
    cmprOut = strcmp(newItem->string, item->string);
    if (cmprOut < 0)
    {
      prevItem->next = newItem;
      newItem->next = item;
      break;
    }
    if (cmprOut == 0)
    {
      ItemFree(newItem);
      return OC_NO;
    }
    prevItem = item;
  }
  if (item == NULL)
  {
    prevItem->next = newItem;
  }
  set->list = rootItem.next;
  set->cardinality++;
  return OC_YES;
}

OUT_CODE SetRemoveItem(Set_t* set, const char* string)
{
  int cmprOut;
  StringListItem_t rootItem;
  StringListItem_t* item;
  StringListItem_t* prevItem;
  if (!set || !string)
  {
    return E_NULL_REF;
  }

  rootItem.next = set->list;
  for (prevItem = &rootItem, item = rootItem.next; item != NULL; item = prevItem->next)
  {
    cmprOut = strcmp(string, item->string);
    if (cmprOut < 0)
    {
      return OC_NO;
    }
    if (cmprOut == 0)
    {
      prevItem->next = item->next;
      ItemFree(item);
      set->list = rootItem.next;
      set->cardinality--;
      return OC_YES;
    }
    prevItem = item;
  }
  return OC_NO;
}

OUT_CODE SetPrintItemList(Set_t* set, FILE* stream, const char* setFormat, const char* itemsStart, const char* itemsFormat, const char* itemsEnd)
{
  StringListItem_t* item;
  if (!set || !stream || !setFormat || !itemsStart || !itemsFormat || !itemsEnd)
  {
    return E_NULL_REF;
  }
  fprintf(stream, setFormat, set->name);
  fprintf(stream, itemsStart);
  for (item = set->list; item != NULL; item = item->next)
  {
    fprintf(stream, itemsFormat, item->string);
  }
  fprintf(stream, itemsEnd);
  return OC_OK;
}

OUT_CODE SetBinOper(Set_t* setA, Set_t* setB, Set_t** output, const char* name, BOOL onlyA, BOOL AnB, BOOL onlyB)
{
  OUT_CODE oCode;
  int cmprOut;
  StringListItem_t rootItem;
  StringListItem_t* newItem;
  StringListItem_t* lastItem;
  StringListItem_t* itemA;
  StringListItem_t* itemB;

  if (!setA || !setB || !output || !name)
  {
    return E_NULL_REF;
  }
  oCode = SetGet(output, name);
  if (oCode != OC_OK)
  {
    return oCode;
  }
  rootItem.next = (*output)->list;
  lastItem = &rootItem;
  itemA = setA->list;
  itemB = setB->list;
  while (itemA && itemB)
  {
    cmprOut = strcmp(itemA->string, itemB->string);
    if (cmprOut < 0)
    {
      if (onlyA)
      {
        oCode = ItemGet(itemA->string, &newItem);
        if (oCode != OC_OK)
        {
          (*output)->list = rootItem.next;
          SetFree(*output);
          return E_MEMALLOCATION_ERROR;
        }
        lastItem->next = newItem;
        lastItem = newItem;
        (*output)->cardinality++;
      }
      itemA = itemA->next;
    }
    if (cmprOut > 0)
    {
      if (onlyB)
      {
        oCode = ItemGet(itemB->string, &newItem);
        if (oCode != OC_OK)
        {
          (*output)->list = rootItem.next;
          SetFree(*output);
          return E_MEMALLOCATION_ERROR;
        }
        lastItem->next = newItem;
        lastItem = newItem;
        (*output)->cardinality++;
      }
      itemB = itemB->next;
    }
    if (cmprOut == 0)
    {
      if (AnB)
      {
        oCode = ItemGet(itemA->string, &newItem);
        if (oCode != OC_OK)
        {
          (*output)->list = rootItem.next;
          SetFree(*output);
          return E_MEMALLOCATION_ERROR;
        }
        lastItem->next = newItem;
        lastItem = newItem;
        (*output)->cardinality++;
      }
      itemA = itemA->next;
      itemB = itemB->next;
    }
  }
  if ((itemA && onlyA) || (itemB && onlyB))
  {
    if (!itemA)
    {
      itemA = itemB;
    }
    while (itemA)
    {
      oCode = ItemGet(itemA->string, &newItem);
      if (oCode != OC_OK)
      {
        (*output)->list = rootItem.next;
        SetFree(*output);
        return E_MEMALLOCATION_ERROR;
      }
      lastItem->next = newItem;
      lastItem = newItem;
      itemA = itemA->next;
      (*output)->cardinality++;
    }
  }
  (*output)->list = rootItem.next;
  return OC_OK;
}

OUT_CODE SetUnion(Set_t* setA, Set_t* setB, Set_t** output, const char* name)
{
  return SetBinOper(setA, setB, output, name, TRUE, TRUE, TRUE);
}

OUT_CODE SetIntersection(Set_t* setA, Set_t* setB, Set_t** output, const char* name)
{
  return SetBinOper(setA, setB, output, name, FALSE, TRUE, FALSE);
}

OUT_CODE SetDifference(Set_t* setA, Set_t* setB, Set_t** output, const char* name)
{
  return SetBinOper(setA, setB, output, name, TRUE, FALSE, FALSE);
}

OUT_CODE SetSymDifference(Set_t* setA, Set_t* setB, Set_t** output, const char* name)
{
  return SetBinOper(setA, setB, output, name, TRUE, FALSE, TRUE);
}

OUT_CODE SetIsInclusion(Set_t* setA, Set_t* setB)
{
  int cmprOut;
  StringListItem_t* itemA;
  StringListItem_t* itemB;
  if (!setA || !setB)
  {
    return E_NULL_REF;
  }
  itemA = setA->list;
  itemB = setB->list;
  while (itemA && itemB)
  {
    cmprOut = strcmp(itemA->string, itemB->string);
    if (cmprOut < 0)
    {
      return OC_NO;
    }
    if (cmprOut > 0)
    {
      itemB = itemB->next;
    }
    if (cmprOut == 0)
    {
      itemA = itemA->next;
      itemB = itemB->next;
    }
  }
  if (!itemA)
  {
    return OC_YES;
  }
  else
  {
    return OC_NO;
  }
}

