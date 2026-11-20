#include "IT.h"
#include "Error.h"
#include <cstring>
#include <iostream>

namespace IT
{
	IdTable Create(int size)
	{
		if (size < 1 || size > TI_MAXSIZE) { throw ERROR_THROW(210); }

		IdTable idtable;
		idtable.maxsize = size;
		idtable.size = 0;
		idtable.table = new Entry[size];
		return idtable;
	}

	void Add(IdTable& idtable, Entry entry)
	{
		if (idtable.table == nullptr) { throw ERROR_THROW(213); } //не создана 
		if (idtable.size >= idtable.maxsize) { throw ERROR_THROW(211); }
		idtable.table[idtable.size++] = entry;
	}

	Entry GetEntry(IdTable& idtable, int number)
	{
		if (idtable.table == nullptr) { throw ERROR_THROW(213); } //не создана 
		if (number < 0 || number >= idtable.size) { throw ERROR_THROW(212); }
		return idtable.table[number];
	}

	int IsId(IdTable& idtable, char id[ID_MAXSIZE])
	{
		if (idtable.table == nullptr) { throw ERROR_THROW(213); } //не создана 
		for (int i = 0; i < idtable.size; i++)
		{
			if (strcmp(idtable.table[i].id, id) == 0) { return i; }
		}
		return TI_NULLIDX;
	}

	void Delete(IdTable& idtable)
	{
		if (idtable.table == nullptr) { throw ERROR_THROW(213); } //не создана 
		delete[] idtable.table;
		idtable.table = nullptr;
		idtable.size = 0;
	}
}