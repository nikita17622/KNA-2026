#pragma once
#define ID_MAXSIZE     10 
#define TI_MAXSIZE     4096  
#define TI_INT_DEFAULT 0x0000000
#define TI_STR_DEFAULT 0x00  
#define TI_NULLIDX     0xffffffff
#define TI_STR_MAXSIZE 255      

namespace IT
{
	enum IDDATATYPE { INT = 1, STR = 2, SYMBOL = 3,BOOL = 4, NONE = 5 };
	enum IDTYPE { V = 1, F = 2, P = 3, L = 4 };

	struct Entry
	{
		int idxFirstLT;
		char id[ID_MAXSIZE];
		IDDATATYPE iddatatype;
		IDTYPE idtype;

		union
		{
			int valInt;
			char valSymbol;
			bool valBool ;
			struct
			{
				char len;
				char str[TI_STR_MAXSIZE + 1];
			}valStr;

		} value;
	};

	struct IdTable
	{
		int maxsize;
		int size;
		Entry* table;
	};

	IdTable Create(int size);
	void    Add(IdTable& idtable, Entry entry);
	Entry   GetEntry(IdTable& idtable, int number);
	int     IsId(IdTable& idtable, char id[ID_MAXSIZE]);
	void    Delete(IdTable& idtable);
}