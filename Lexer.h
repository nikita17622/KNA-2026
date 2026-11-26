#pragma once
#include "In.h"
#include "Log.h"
#include "LT.h"
#include "IT.h"
#include "Automats.h"


namespace LEXER
{
	extern int lastIndex;

	struct Lexer
	{
		LT::LexTable lexTable ;
		IT::IdTable idTable ;
		int line;

		Lexer();

		void LexicalAnalysis(In::IN& in, Log::LOG& log);
		void AddLex(char lexSymbol, int line);
		void AddIdentifikator(char lexSymbol, int line, char word[]);
		void AddLeteral(char lexSymbol, int line, char word[]);
		void FunctionCall(char& lexSymbol, int currentLine, char word[]);
		void CallCheckFunction(char* word, char& lexSymbol, int& len, int& c, int& lengthWord);
		void PrintTables(Log::LOG& log);
	};
}