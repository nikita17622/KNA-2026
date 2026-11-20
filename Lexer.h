#pragma once
#include "In.h"
#include "Log.h"
#include "LT.h"
#include "IT.h"
#include "Automats.h"


namespace LEXER
{
	struct Lexer
	{
		int line;

		LT::LexTable lexTable;
		IT::IdTable IdTable;

		Lexer();

		void LexicalAnalysis(In::IN& in, Log::LOG& log);
		void AddLex(char lexSymbol, int line);
		void AddIdentifikator(char lexSymbol, int line, char word[]);
		void PrintTables(Log::LOG& log);
	};
}