#include "Automats.h"
#include "Error.h"
#include "Lexer.h"
#include "LT.h"
#include "IT.h"
#include <cstring>
#include <cstdlib>

using namespace IT;

namespace Automats
{
	
	bool IsKeyWord(const char* word, char& KeyWordCode, IDTYPE& idtype,IDDATATYPE& iddatatype)
	{
		{
			FST::FST fst((char*)word, 8,
				FST::NODE(1, FST::RELATION('i', 1)),
				FST::NODE(1, FST::RELATION('n', 2)),
				FST::NODE(1, FST::RELATION('t', 3)),
				FST::NODE(1, FST::RELATION('e', 4)),
				FST::NODE(1, FST::RELATION('g', 5)),
				FST::NODE(1, FST::RELATION('e', 6)),
				FST::NODE(1, FST::RELATION('r', 7)),
				FST::NODE());
			if (FST::execute(fst))
			{
				KeyWordCode = LEX_INTEGER;
				iddatatype = INT;
				return true;
			}
		}
		//int
		{
			FST::FST fst((char*)word, 7,
				FST::NODE(1, FST::RELATION('s', 1)),
				FST::NODE(1, FST::RELATION('t', 2)),
				FST::NODE(1, FST::RELATION('r', 3)),
				FST::NODE(1, FST::RELATION('i', 4)),
				FST::NODE(1, FST::RELATION('n', 5)),
				FST::NODE(1, FST::RELATION('g', 6)),
				FST::NODE());
			if (FST::execute(fst))
			{
				KeyWordCode = LEX_STRING;
				iddatatype = STR;
				return true;
			}
		}
		//str
		{
			FST::FST fst((char*)word, 9,
				FST::NODE(1, FST::RELATION('f', 1)),
				FST::NODE(1, FST::RELATION('u', 2)),
				FST::NODE(1, FST::RELATION('n', 3)),
				FST::NODE(1, FST::RELATION('c', 4)),
				FST::NODE(1, FST::RELATION('t', 5)),
				FST::NODE(1, FST::RELATION('i', 6)),
				FST::NODE(1, FST::RELATION('o', 7)),
				FST::NODE(1, FST::RELATION('n', 8)),
				FST::NODE());
			if (FST::execute(fst))
			{
				KeyWordCode = LEX_FUNCTION;
				iddatatype = NONE;
				idtype = F;
				return true;
			}
		}
		//fun
		{
			FST::FST fst((char*)word, 8,
				FST::NODE(1, FST::RELATION('d', 1)),
				FST::NODE(1, FST::RELATION('e', 2)),
				FST::NODE(1, FST::RELATION('c', 3)),
				FST::NODE(1, FST::RELATION('l', 4)),
				FST::NODE(1, FST::RELATION('a', 5)),
				FST::NODE(1, FST::RELATION('r', 6)),
				FST::NODE(1, FST::RELATION('e', 7)),
				FST::NODE());
			if (FST::execute(fst))
			{
				KeyWordCode = LEX_DECLARE;
				return true;
			}

		}
		//dec
		{
			FST::FST fst((char*)word, 7,
				FST::NODE(1, FST::RELATION('r', 1)),
				FST::NODE(1, FST::RELATION('e', 2)),
				FST::NODE(1, FST::RELATION('t', 3)),
				FST::NODE(1, FST::RELATION('u', 4)),
				FST::NODE(1, FST::RELATION('r', 5)),
				FST::NODE(1, FST::RELATION('n', 6)),
				FST::NODE());
			if (FST::execute(fst))
			{
				KeyWordCode = LEX_RETURN;
				return true;
			}
		}
		//ret
		{
			FST::FST fst((char*)word, 6,
				FST::NODE(1, FST::RELATION('p', 1)),
				FST::NODE(1, FST::RELATION('r', 2)),
				FST::NODE(1, FST::RELATION('i', 3)),
				FST::NODE(1, FST::RELATION('n', 4)),
				FST::NODE(1, FST::RELATION('t', 5)),
				FST::NODE());
			if (FST::execute(fst))
			{
				KeyWordCode = LEX_PRINT;
				return true;
			}
		}
		//prin
		{
			FST::FST fst((char*)word, 5,
				FST::NODE(1, FST::RELATION('m', 1)),
				FST::NODE(1, FST::RELATION('a', 2)),
				FST::NODE(1, FST::RELATION('i', 3)),
				FST::NODE(1, FST::RELATION('n', 4)),
				FST::NODE());
			if (FST::execute(fst))
			{
				KeyWordCode = LEX_MAIN;
				return true;
			}
		}
		//main




		return false;
	}

	bool IsSeparator(const char* word, char& sepCode, IDTYPE& idtype)
	{
		if (strlen(word) != 1) { return false; }
		char simbol = word[0];

		{
			FST::FST fst((char*)word, 2, FST::NODE(1, FST::RELATION(';', 1)), FST::NODE());
			if (FST::execute(fst))
			{
				sepCode = LEX_SEMICOLON;
				return true;
			}
		}
		{
			FST::FST fst((char*)word, 2, FST::NODE(1, FST::RELATION(',', 1)), FST::NODE());
			if (FST::execute(fst))
			{
				sepCode = LEX_COMMA;
				return true;
			}
		}
		{
			FST::FST fst((char*)word, 2, FST::NODE(1, FST::RELATION('{', 1)), FST::NODE());
			if (FST::execute(fst))
			{
				sepCode = LEX_LEFTBRACE;
				return true;
			}
		}
		{
			FST::FST fst((char*)word, 2, FST::NODE(1, FST::RELATION('}', 1)), FST::NODE());
			if (FST::execute(fst))
			{
				sepCode = LEX_BRACELET;
				return true;
			}
		}
		{
			FST::FST fst((char*)word, 2, FST::NODE(1, FST::RELATION('(', 1)), FST::NODE());
			if (FST::execute(fst))
			{
				sepCode = LEX_LEFTHESIS;
				idtype = P;
				return true;
			}
		}
		{
			FST::FST fst((char*)word, 2, FST::NODE(1, FST::RELATION(')', 1)), FST::NODE());
			if (FST::execute(fst))
			{
				idtype = V;
				sepCode = LEX_RIGHTHESIS;
				return true;
			}
		}
		{
			FST::FST fst((char*)word, 2, FST::NODE(1, FST::RELATION('+', 1)), FST::NODE());
			if (FST::execute(fst))
			{
				sepCode = LEX_PLUS;
				return true;
			}
		}
		{
			FST::FST fst((char*)word, 2, FST::NODE(1, FST::RELATION('-', 1)), FST::NODE());
			if (FST::execute(fst))
			{
				sepCode = LEX_MINUS;
				return true;
			}
		}
		{
			FST::FST fst((char*)word, 2, FST::NODE(1, FST::RELATION('*', 1)), FST::NODE());
			if (FST::execute(fst))
			{
				sepCode = LEX_STAR;
				return true;
			}
		}
		{
			FST::FST fst((char*)word, 2, FST::NODE(1, FST::RELATION('/', 1)), FST::NODE());
			if (FST::execute(fst))
			{
				sepCode = LEX_DIRSLASH;
				return true;
			}
		}
		{
			FST::FST fst((char*)word, 2, FST::NODE(1, FST::RELATION('=', 1)), FST::NODE());
			if (FST::execute(fst))
			{
				sepCode = LEX_ASSIGN;
				return true;
			}
		}
		return false;
	}

	bool IsIdintificator(const char* word, char& IdCode)
	{
		{
			FST::FST fst((char*)word, 2,
				FST::NODE(27, FST::RELATION('a', 1), FST::RELATION('b', 1), FST::RELATION('c', 1), FST::RELATION('d', 1), FST::RELATION('e', 1),
					FST::RELATION('f', 1), FST::RELATION('g', 1), FST::RELATION('h', 1), FST::RELATION('i', 1), FST::RELATION('j', 1),
					FST::RELATION('k', 1), FST::RELATION('l', 1), FST::RELATION('m', 1), FST::RELATION('n', 1), FST::RELATION('o', 1),
					FST::RELATION('p', 1), FST::RELATION('q', 1), FST::RELATION('r', 1), FST::RELATION('s', 1), FST::RELATION('t', 1), FST::RELATION('_', 1),
					FST::RELATION('u', 1), FST::RELATION('v', 1), FST::RELATION('w', 1), FST::RELATION('x', 1), FST::RELATION('y', 1), FST::RELATION('z', 1)),

				FST::NODE(27, FST::RELATION('a', 1), FST::RELATION('b', 1), FST::RELATION('c', 1), FST::RELATION('d', 1), FST::RELATION('e', 1),
					FST::RELATION('f', 1), FST::RELATION('g', 1), FST::RELATION('h', 1), FST::RELATION('i', 1), FST::RELATION('j', 1),
					FST::RELATION('k', 1), FST::RELATION('l', 1), FST::RELATION('m', 1), FST::RELATION('n', 1), FST::RELATION('o', 1),
					FST::RELATION('p', 1), FST::RELATION('q', 1), FST::RELATION('r', 1), FST::RELATION('s', 1), FST::RELATION('t', 1), FST::RELATION('_', 1),
					FST::RELATION('u', 1), FST::RELATION('v', 1), FST::RELATION('w', 1), FST::RELATION('x', 1), FST::RELATION('y', 1), FST::RELATION('z', 1)));

			if (FST::execute(fst))
			{
				IdCode = LEX_ID;
				return true;
			}
			return false;
		}
	}

	bool IsLiteral(const char* word, char& LetCode, IDTYPE& idtype)
	{
		{
			FST::FST fst((char*)word, 2,
				FST::NODE(10, FST::RELATION('0', 1), FST::RELATION('1', 1), FST::RELATION('2', 1), FST::RELATION('3', 1), FST::RELATION('4', 1),
					FST::RELATION('5', 1), FST::RELATION('6', 1), FST::RELATION('7', 1), FST::RELATION('8', 1), FST::RELATION('9', 1)),

				FST::NODE(10, FST::RELATION('0', 1), FST::RELATION('1', 1), FST::RELATION('2', 1), FST::RELATION('3', 1), FST::RELATION('4', 1),
					FST::RELATION('5', 1), FST::RELATION('6', 1), FST::RELATION('7', 1), FST::RELATION('8', 1), FST::RELATION('9', 1)));

			if (FST::execute(fst))
			{
				char* end;
				long val = strtol(word,&end,10);
				if (val > INT_MAX || val < INT_MIN) throw ERROR_THROW(128);//
				LetCode = LEX_LITERAL;
				idtype = L;
				return true;
			}
		}

		{
			FST::FST fst((char*)word, 2,
				FST::NODE(26, FST::RELATION('a', 1), FST::RELATION('b', 1), FST::RELATION('c', 1), FST::RELATION('d', 1), FST::RELATION('e', 1),
					FST::RELATION('f', 1), FST::RELATION('g', 1), FST::RELATION('h', 1), FST::RELATION('i', 1), FST::RELATION('j', 1),
					FST::RELATION('k', 1), FST::RELATION('l', 1), FST::RELATION('m', 1), FST::RELATION('n', 1), FST::RELATION('o', 1),
					FST::RELATION('p', 1), FST::RELATION('q', 1), FST::RELATION('r', 1), FST::RELATION('s', 1), FST::RELATION('t', 1), 
					FST::RELATION('u', 1), FST::RELATION('v', 1), FST::RELATION('w', 1), FST::RELATION('x', 1), FST::RELATION('y', 1), FST::RELATION('z', 1)),

				FST::NODE(26, FST::RELATION('a', 1), FST::RELATION('b', 1), FST::RELATION('c', 1), FST::RELATION('d', 1), FST::RELATION('e', 1),
					FST::RELATION('f', 1), FST::RELATION('g', 1), FST::RELATION('h', 1), FST::RELATION('i', 1), FST::RELATION('j', 1),
					FST::RELATION('k', 1), FST::RELATION('l', 1), FST::RELATION('m', 1), FST::RELATION('n', 1), FST::RELATION('o', 1),
					FST::RELATION('p', 1), FST::RELATION('q', 1), FST::RELATION('r', 1), FST::RELATION('s', 1), FST::RELATION('t', 1), 
					FST::RELATION('u', 1), FST::RELATION('v', 1), FST::RELATION('w', 1), FST::RELATION('x', 1), FST::RELATION('y', 1), FST::RELATION('z', 1)));

			if (FST::execute(fst))
			{
				LetCode = LEX_LITERAL;
				idtype = L;
				return true;
			}
		}

	}

	bool FindFST(const char* word, char& WordCode, IDTYPE& idtype, IDDATATYPE& iddatatype,int len,int c)
	{
		if (strlen(word) == 0)return false;
		if (IsKeyWord(word, WordCode, idtype, iddatatype)) return true;
		if (IsSeparator(word, WordCode, idtype)) return true;
		if (IsIdintificator(word, WordCode)) return true;
		if (IsLiteral(word, WordCode, idtype)) return true;
		throw ERROR_THROW_IN(127,len,c); //
	}

}


//#include "Automats.h"
//#include "LT.h"
//#include <cstring>
//
//
//namespace Automats
//{
//	bool IsKeyWord(const char* word, char& KeyWordCode)
//	{
//		{
//			FST::FST fst((char*)word, 8,
//				FST::NODE(1, FST::RELATION('i', 1)),
//				FST::NODE(1, FST::RELATION('n', 2)),
//				FST::NODE(1, FST::RELATION('t', 3)),
//				FST::NODE(1, FST::RELATION('e', 4)),
//				FST::NODE(1, FST::RELATION('g', 5)),
//				FST::NODE(1, FST::RELATION('e', 6)),
//				FST::NODE(1, FST::RELATION('r', 7)),
//				FST::NODE());
//			if (FST::execute(fst))
//			{
//				KeyWordCode = LEX_INTEGER;
//				return true;
//			}
//		}
//		//int
//		{
//			FST::FST fst((char*)word, 7,
//				FST::NODE(1, FST::RELATION('s', 1)),
//				FST::NODE(1, FST::RELATION('t', 2)),
//				FST::NODE(1, FST::RELATION('r', 3)),
//				FST::NODE(1, FST::RELATION('i', 4)),
//				FST::NODE(1, FST::RELATION('n', 5)),
//				FST::NODE(1, FST::RELATION('g', 6)),
//				FST::NODE());
//			if (FST::execute(fst))
//			{
//				KeyWordCode = LEX_STRING;
//				return true;
//			}
//		}
//		//str
//		{
//			FST::FST fst((char*)word, 9,
//				FST::NODE(1, FST::RELATION('f', 1)),
//				FST::NODE(1, FST::RELATION('u', 2)),
//				FST::NODE(1, FST::RELATION('n', 3)),
//				FST::NODE(1, FST::RELATION('c', 4)),
//				FST::NODE(1, FST::RELATION('t', 5)),
//				FST::NODE(1, FST::RELATION('i', 6)),
//				FST::NODE(1, FST::RELATION('o', 7)),
//				FST::NODE(1, FST::RELATION('n', 8)),
//				FST::NODE());
//			if (FST::execute(fst))
//			{
//				KeyWordCode = LEX_FUNCTION;
//				return true;
//			}
//		}
//		//fun
//		{
//			FST::FST fst((char*)word, 8,
//				FST::NODE(1, FST::RELATION('d', 1)),
//				FST::NODE(1, FST::RELATION('e', 2)),
//				FST::NODE(1, FST::RELATION('c', 3)),
//				FST::NODE(1, FST::RELATION('l', 4)),
//				FST::NODE(1, FST::RELATION('a', 5)),
//				FST::NODE(1, FST::RELATION('r', 6)),
//				FST::NODE(1, FST::RELATION('e', 7)),
//				FST::NODE());
//			if (FST::execute(fst))
//			{
//				KeyWordCode = LEX_DECLARE;
//				return true;
//			}
//
//		}
//		//dec
//		{
//			FST::FST fst((char*)word, 7,
//				FST::NODE(1, FST::RELATION('r', 1)),
//				FST::NODE(1, FST::RELATION('e', 2)),
//				FST::NODE(1, FST::RELATION('t', 3)),
//				FST::NODE(1, FST::RELATION('u', 4)),
//				FST::NODE(1, FST::RELATION('r', 5)),
//				FST::NODE(1, FST::RELATION('n', 6)),
//				FST::NODE());
//			if (FST::execute(fst))
//			{
//				KeyWordCode = LEX_RETURN;
//				return true;
//			}
//		}
//		//ret
//		{
//			FST::FST fst((char*)word, 6,
//				FST::NODE(1, FST::RELATION('p', 1)),
//				FST::NODE(1, FST::RELATION('r', 2)),
//				FST::NODE(1, FST::RELATION('i', 3)),
//				FST::NODE(1, FST::RELATION('n', 4)),
//				FST::NODE(1, FST::RELATION('t', 5)),
//				FST::NODE());
//			if (FST::execute(fst))
//			{
//				KeyWordCode = LEX_PRINT;
//				return true;
//			}
//		}
//		//prin
//		{
//			FST::FST fst((char*)word, 5,
//				FST::NODE(1, FST::RELATION('m', 1)),
//				FST::NODE(1, FST::RELATION('a', 2)),
//				FST::NODE(1, FST::RELATION('i', 3)),
//				FST::NODE(1, FST::RELATION('n', 4)),
//				FST::NODE());
//			if (FST::execute(fst))
//			{
//				KeyWordCode = LEX_MAIN;
//				return true;
//			}
//		}
//		//main
//		return false;
//	}
//
//	bool IsSeparator(const char* word, char& sepCode)
//	{
//		if (strlen(word) != 1) { return false; }
//		char simbol = word[0];
//
//		FST::FST fst((char*)word, 2, FST::NODE(1, FST::RELATION(simbol, 1)), FST::NODE());
//		if (FST::execute(fst))
//		{
//			switch (simbol)
//			{
//			case ';': sepCode = LEX_SEMICOLON; return true;
//			case ',': sepCode = LEX_COMMA; return true;
//			case '{': sepCode = LEX_LEFTBRACE; return true;
//			case '}': sepCode = LEX_BRACELET; return true;
//			case '(': sepCode = LEX_LEFTHESIS; return true;
//			case ')': sepCode = LEX_RIGHTHESIS; return true;
//			case '+': sepCode = LEX_PLUS; return true;
//			case '-': sepCode = LEX_MINUS; return true;
//			case '*': sepCode = LEX_STAR; return true;
//			case '/': sepCode = LEX_DIRSLASH; return true;
//			case '=': sepCode = LEX_ASSIGN; return true;
//			default: return false;
//			}
//		}
//		return false;
//	}
//
//	bool IsIdentifier(const char* word)
//	{
//		int len = (int)strlen(word);
//		if (len == 0) return false;
//
//		for (int i = 0; i < len; i++)
//		{
//			char simbol = word[i];
//			if (!((simbol >= 'a' && simbol <= 'z') || (simbol >= 'A' && simbol <= 'Z') || (simbol >= '0' && simbol <= '9') || simbol == '_')) { return false; }
//		}
//		return true;
//	}
//
//}