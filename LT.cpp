#include "LT.h"
#include "Error.h"
#include <cstring>
#include <iostream>

namespace LT
{
	LexTable Create(int size)
	{
		if (size < 1 || size > LT_MAXSIZE) { throw ERROR_THROW(200); }

		LexTable lextable;
		lextable.size = 0;
		lextable.maxsize = size;
		lextable.table = new Entry[size];
		return lextable;
	}

	void Add(LexTable& lextable, Entry entry)
	{
		if (lextable.table == nullptr) { throw ERROR_THROW(203); }
		if (lextable.size >= lextable.maxsize) { throw ERROR_THROW(201); }
		lextable.table[lextable.size] = entry;
		lextable.size++;
	}

	Entry GetEntry(LexTable& lextable, int number)
	{
		if (lextable.table == nullptr) { throw ERROR_THROW(203); }
		if (number < 0 || number >= lextable.size) { throw ERROR_THROW(202); }
		return lextable.table[number];
	}

	void Delete(LexTable& lextable)
	{
		if (lextable.table == nullptr) { throw ERROR_THROW(203); }
		delete[] lextable.table;
		lextable.table = nullptr;
		lextable.size = 0;
	}
}