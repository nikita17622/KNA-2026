#pragma once
#include "IT.h"
#include "FST.h"

namespace Automats
{
	bool IsKeyWord(const char* word, char& KeyWordCode, IT::IDTYPE& idtype, IT::IDDATATYPE& iddatatype);
	bool IsSeparator(const char* word, char& sepCode, IT::IDTYPE& idtype);
	bool IsIdintificator(const char* word, char& IDCode);
	bool IsLiteral(const char* word, char& LetCode, IT::IDTYPE& idtype);
	bool FindFST(const char* word, char& WordCode, IT::IDTYPE& idtype, IT::IDDATATYPE& iddatatype,int len, int c);
}



