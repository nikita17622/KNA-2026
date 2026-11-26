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
	bool IsKeyWord(const char* word, char& KeyWordCode, IDTYPE& idtype, IDDATATYPE& iddatatype)
	{
		{
			// integer
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

			// bool
			{
				FST::FST fst((char*)word, 5,
					FST::NODE(1, FST::RELATION('b', 1)),
					FST::NODE(1, FST::RELATION('o', 2)),
					FST::NODE(1, FST::RELATION('o', 3)),
					FST::NODE(1, FST::RELATION('l', 4)),
					FST::NODE());
				if (FST::execute(fst)) { KeyWordCode = LEX_BOOL; iddatatype = BOOL; return true; }
			}

			// symbol 
			{
				FST::FST fst((char*)word, 7,
					FST::NODE(1, FST::RELATION('s', 1)),
					FST::NODE(1, FST::RELATION('y', 2)),
					FST::NODE(1, FST::RELATION('m', 3)),
					FST::NODE(1, FST::RELATION('b', 4)),
					FST::NODE(1, FST::RELATION('o', 5)),
					FST::NODE(1, FST::RELATION('l', 6)),
					FST::NODE());
				if (FST::execute(fst)) { KeyWordCode = LEX_SYMBOL; iddatatype = SYMBOL; return true; }
			}

			// string 
			{
				FST::FST fst((char*)word, 7,
					FST::NODE(1, FST::RELATION('s', 1)),
					FST::NODE(1, FST::RELATION('t', 2)),
					FST::NODE(1, FST::RELATION('r', 3)),
					FST::NODE(1, FST::RELATION('i', 4)),
					FST::NODE(1, FST::RELATION('n', 5)),
					FST::NODE(1, FST::RELATION('g', 6)),
					FST::NODE());
				if (FST::execute(fst)) { KeyWordCode = LEX_STRING; iddatatype = STR; return true; }
			}


			// fn 
			{
				FST::FST fst((char*)word, 3,
					FST::NODE(1, FST::RELATION('f', 1)),
					FST::NODE(1, FST::RELATION('n', 2)),
					FST::NODE());
				if (FST::execute(fst)) { KeyWordCode = LEX_FUNCTION; idtype = F; return true; }
			}

			// let 
			{
				FST::FST fst((char*)word, 4,
					FST::NODE(1, FST::RELATION('l', 1)),
					FST::NODE(1, FST::RELATION('e', 2)),
					FST::NODE(1, FST::RELATION('t', 3)),
					FST::NODE());
				if (FST::execute(fst)) { KeyWordCode = LEX_LET; idtype = V; return true; }
			}

			// return
			{
				FST::FST fst((char*)word, 7,
					FST::NODE(1, FST::RELATION('r', 1)),
					FST::NODE(1, FST::RELATION('e', 2)),
					FST::NODE(1, FST::RELATION('t', 3)),
					FST::NODE(1, FST::RELATION('u', 4)),
					FST::NODE(1, FST::RELATION('r', 5)),
					FST::NODE(1, FST::RELATION('n', 6)),
					FST::NODE());
				if (FST::execute(fst)) { KeyWordCode = LEX_RETURN; return true; }
			}

			// print
			{
				FST::FST fst((char*)word, 6,
					FST::NODE(1, FST::RELATION('p', 1)),
					FST::NODE(1, FST::RELATION('r', 2)),
					FST::NODE(1, FST::RELATION('i', 3)),
					FST::NODE(1, FST::RELATION('n', 4)),
					FST::NODE(1, FST::RELATION('t', 5)),
					FST::NODE());
				if (FST::execute(fst)) { KeyWordCode = LEX_PRINT; return true; }
			}

			// input
			{
				FST::FST fst((char*)word, 6,
					FST::NODE(1, FST::RELATION('i', 1)),
					FST::NODE(1, FST::RELATION('n', 2)),
					FST::NODE(1, FST::RELATION('p', 3)),
					FST::NODE(1, FST::RELATION('u', 4)),
					FST::NODE(1, FST::RELATION('t', 5)),
					FST::NODE());
				if (FST::execute(fst)) { KeyWordCode = LEX_INPUT; return true; }
			}

			// main
			{
				FST::FST fst((char*)word, 5,
					FST::NODE(1, FST::RELATION('m', 1)),
					FST::NODE(1, FST::RELATION('a', 2)),
					FST::NODE(1, FST::RELATION('i', 3)),
					FST::NODE(1, FST::RELATION('n', 4)),
					FST::NODE());
				if (FST::execute(fst)) { KeyWordCode = LEX_MAIN; return true; }
			}

			// loop
			{
				FST::FST fst((char*)word, 5,
					FST::NODE(1, FST::RELATION('l', 1)),
					FST::NODE(1, FST::RELATION('o', 2)),
					FST::NODE(1, FST::RELATION('o', 3)),
					FST::NODE(1, FST::RELATION('p', 4)),
					FST::NODE());
				if (FST::execute(fst)) { KeyWordCode = LEX_LOOP; return true; }
			}

			// true
			{
				FST::FST fst((char*)word, 5,
					FST::NODE(1, FST::RELATION('t', 1)),
					FST::NODE(1, FST::RELATION('r', 2)),
					FST::NODE(1, FST::RELATION('u', 3)),
					FST::NODE(1, FST::RELATION('e', 4)),
					FST::NODE());
				if (FST::execute(fst)) { KeyWordCode = LEX_LITERAL; iddatatype = BOOL; return true; }
			}

			// false
			{
				FST::FST fst((char*)word, 6,
					FST::NODE(1, FST::RELATION('f', 1)),
					FST::NODE(1, FST::RELATION('a', 2)),
					FST::NODE(1, FST::RELATION('l', 3)),
					FST::NODE(1, FST::RELATION('s', 4)),
					FST::NODE(1, FST::RELATION('e', 5)),
					FST::NODE());
				if (FST::execute(fst)) { KeyWordCode = LEX_LITERAL; iddatatype = BOOL; return true; }
			}
		}
	}


	bool IsSeparator(const char* word, char& sepCode, IDTYPE& idtype)
	{
	
		{
			if (strlen(word) == 0) { return false; }

			if (strlen(word) == 2)
			{
				// <<
				{
					FST::FST fst((char*)word, 3,
						FST::NODE(1, FST::RELATION('<', 1)),
						FST::NODE(1, FST::RELATION('<', 2)),
						FST::NODE());
					if (FST::execute(fst)) { sepCode = LEX_LSHIFT; return true; }
				}
				// >>
				{
					FST::FST fst((char*)word, 3,
						FST::NODE(1, FST::RELATION('>', 1)),
						FST::NODE(1, FST::RELATION('>', 2)),
						FST::NODE());
					if (FST::execute(fst)) { sepCode = LEX_RSHIFT; return true; }
				}
				{
					FST::FST fst((char*)word, 3,
						FST::NODE(1, FST::RELATION('-', 1)),
						FST::NODE(1, FST::RELATION('>', 2)),
						FST::NODE());
					if (FST::execute(fst)) { sepCode = LEX_ARROW; return true; }
				}
				return false;
			}

			if (strlen(word) > 2) { return false; }

			char simbol = word[0];

			{
				FST::FST fst((char*)word, 2, FST::NODE(1, FST::RELATION(';', 1)), FST::NODE());
				if (FST::execute(fst)) { sepCode = LEX_SEMICOLON; return true; }
			}
			{
				FST::FST fst((char*)word, 2, FST::NODE(1, FST::RELATION(',', 1)), FST::NODE());
				if (FST::execute(fst)) { sepCode = LEX_COMMA; return true; }
			}
			{
				FST::FST fst((char*)word, 2, FST::NODE(1, FST::RELATION('{', 1)), FST::NODE());
				if (FST::execute(fst)) { sepCode = LEX_LEFTBRACE; return true; }
			}
			{
				FST::FST fst((char*)word, 2, FST::NODE(1, FST::RELATION('}', 1)), FST::NODE());
				if (FST::execute(fst)) { sepCode = LEX_BRACELET; return true; }
			}
			{
				FST::FST fst((char*)word, 2, FST::NODE(1, FST::RELATION('(', 1)), FST::NODE());
				if (FST::execute(fst)) { sepCode = LEX_LEFTHESIS; idtype = IT::P; return true; }
			}
			{
				FST::FST fst((char*)word, 2, FST::NODE(1, FST::RELATION(')', 1)), FST::NODE());
				if (FST::execute(fst)) { sepCode = LEX_RIGHTHESIS; idtype = IT::V; return true; }
			}
			{
				FST::FST fst((char*)word, 2, FST::NODE(1, FST::RELATION('+', 1)), FST::NODE());
				if (FST::execute(fst)) { sepCode = LEX_PLUS; return true; }
			}
			{
				FST::FST fst((char*)word, 2, FST::NODE(1, FST::RELATION('-', 1)), FST::NODE());
				if (FST::execute(fst)) { sepCode = LEX_MINUS; return true; }
			}
			{
				FST::FST fst((char*)word, 2, FST::NODE(1, FST::RELATION('*', 1)), FST::NODE());
				if (FST::execute(fst)) { sepCode = LEX_STAR; return true; }
			}
			{
				FST::FST fst((char*)word, 2, FST::NODE(1, FST::RELATION('/', 1)), FST::NODE());
				if (FST::execute(fst)) { sepCode = LEX_DIRSLASH; return true; }
			}
			{
				FST::FST fst((char*)word, 2, FST::NODE(1, FST::RELATION('=', 1)), FST::NODE());
				if (FST::execute(fst)) { sepCode = LEX_ASSIGN; return true; }
			}
			{
				FST::FST fst((char*)word, 2, FST::NODE(1, FST::RELATION('%', 1)), FST::NODE());
				if (FST::execute(fst)) { sepCode = LEX_PERCENT; return true; }
			}
			{
				FST::FST fst((char*)word, 2, FST::NODE(1, FST::RELATION('?', 1)), FST::NODE());
				if (FST::execute(fst)) { sepCode = LEX_QUESTION; return true; }
			}
			{
				FST::FST fst((char*)word, 2, FST::NODE(1, FST::RELATION(':', 1)), FST::NODE());
				if (FST::execute(fst)) { sepCode = LEX_COLON; return true; }
			}
			{
				FST::FST fst((char*)word, 2, FST::NODE(1, FST::RELATION('>', 1)), FST::NODE());
				if (FST::execute(fst)) { sepCode = LEX_GREATER; return true; }
			}
			{
				FST::FST fst((char*)word, 2, FST::NODE(1, FST::RELATION('<', 1)), FST::NODE());
				if (FST::execute(fst)) { sepCode = LEX_LESS; return true; }
			}

			return false;
		}
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

				FST::NODE(37, FST::RELATION('a', 1), FST::RELATION('b', 1), FST::RELATION('c', 1), FST::RELATION('d', 1), FST::RELATION('e', 1),
					FST::RELATION('f', 1), FST::RELATION('g', 1), FST::RELATION('h', 1), FST::RELATION('i', 1), FST::RELATION('j', 1),
					FST::RELATION('k', 1), FST::RELATION('l', 1), FST::RELATION('m', 1), FST::RELATION('n', 1), FST::RELATION('o', 1),
					FST::RELATION('p', 1), FST::RELATION('q', 1), FST::RELATION('r', 1), FST::RELATION('s', 1), FST::RELATION('t', 1), FST::RELATION('_', 1),
					FST::RELATION('u', 1), FST::RELATION('v', 1), FST::RELATION('w', 1), FST::RELATION('x', 1), FST::RELATION('y', 1), FST::RELATION('z', 1),
					FST::RELATION('0', 1), FST::RELATION('1', 1), FST::RELATION('2', 1), FST::RELATION('3', 1), FST::RELATION('4', 1),
					FST::RELATION('5', 1), FST::RELATION('6', 1), FST::RELATION('7', 1), FST::RELATION('8', 1), FST::RELATION('9', 1)));

			if (FST::execute(fst))
			{
				IdCode = LEX_ID;
				return true;
			}
			return false;
		}
	}

	bool IsLiteral(const char* word, char& LetCode, IDTYPE& idtype, int len, int c, IDDATATYPE& iddata)
	{
		{
			FST::FST fst((char*)word, 2,
				FST::NODE(11, FST::RELATION('0', 1), FST::RELATION('1', 1), FST::RELATION('2', 1), FST::RELATION('3', 1), FST::RELATION('4', 1),
					FST::RELATION('5', 1), FST::RELATION('6', 1), FST::RELATION('7', 1), FST::RELATION('8', 1), FST::RELATION('9', 1), FST::RELATION('-', 1)),

				FST::NODE(10, FST::RELATION('0', 1), FST::RELATION('1', 1), FST::RELATION('2', 1), FST::RELATION('3', 1), FST::RELATION('4', 1),
					FST::RELATION('5', 1), FST::RELATION('6', 1), FST::RELATION('7', 1), FST::RELATION('8', 1), FST::RELATION('9', 1)));

			if (FST::execute(fst))
			{
				if (iddata != STR && iddata != SYMBOL)
				{
					errno = 0;
					char* end = nullptr;
					long long val = strtoll(word, &end, 10);
					if (errno == ERANGE || val > INT_MAX || val < INT_MIN) throw ERROR_THROW_IN(128, len, c);
					iddata = INT;
				}
				LetCode = LEX_LITERAL;
				idtype = L;
				return true;
			}
		}

		{
			FST::FST fst((char*)word, 2,
				FST::NODE(37, FST::RELATION('a', 1), FST::RELATION('b', 1), FST::RELATION('c', 1), FST::RELATION('d', 1), FST::RELATION('e', 1),
					FST::RELATION('f', 1), FST::RELATION('g', 1), FST::RELATION('h', 1), FST::RELATION('i', 1), FST::RELATION('j', 1),
					FST::RELATION('k', 1), FST::RELATION('l', 1), FST::RELATION('m', 1), FST::RELATION('n', 1), FST::RELATION('o', 1),
					FST::RELATION('p', 1), FST::RELATION('q', 1), FST::RELATION('r', 1), FST::RELATION('s', 1), FST::RELATION('t', 1), FST::RELATION('_', 1),
					FST::RELATION('u', 1), FST::RELATION('v', 1), FST::RELATION('w', 1), FST::RELATION('x', 1), FST::RELATION('y', 1), FST::RELATION('z', 1),
					FST::RELATION('0', 1), FST::RELATION('1', 1), FST::RELATION('2', 1), FST::RELATION('3', 1), FST::RELATION('4', 1),
					FST::RELATION('5', 1), FST::RELATION('6', 1), FST::RELATION('7', 1), FST::RELATION('8', 1), FST::RELATION('9', 1)),

				FST::NODE(38, FST::RELATION('a', 1), FST::RELATION('b', 1), FST::RELATION('c', 1), FST::RELATION('d', 1), FST::RELATION('e', 1),
					FST::RELATION('f', 1), FST::RELATION('g', 1), FST::RELATION('h', 1), FST::RELATION('i', 1), FST::RELATION('j', 1),
					FST::RELATION('k', 1), FST::RELATION('l', 1), FST::RELATION('m', 1), FST::RELATION('n', 1), FST::RELATION('o', 1), FST::RELATION(' ', 1),
					FST::RELATION('p', 1), FST::RELATION('q', 1), FST::RELATION('r', 1), FST::RELATION('s', 1), FST::RELATION('t', 1), FST::RELATION('_', 1),
					FST::RELATION('u', 1), FST::RELATION('v', 1), FST::RELATION('w', 1), FST::RELATION('x', 1), FST::RELATION('y', 1), FST::RELATION('z', 1),
					FST::RELATION('0', 1), FST::RELATION('1', 1), FST::RELATION('2', 1), FST::RELATION('3', 1), FST::RELATION('4', 1),
					FST::RELATION('5', 1), FST::RELATION('6', 1), FST::RELATION('7', 1), FST::RELATION('8', 1), FST::RELATION('9', 1)));

			if (FST::execute(fst))
			{
				LetCode = LEX_LITERAL;
				idtype = L;
				return true;
			}
		}
	}

	bool FindFST(const char* word, char& WordCode, IDTYPE& idtype, IDDATATYPE& iddatatype, int len, int c, bool islet)
	{
		if (strlen(word) == 0)return false;
		if (IsKeyWord(word, WordCode, idtype, iddatatype)) return true;
		if (IsSeparator(word, WordCode, idtype)) return true;
		if (IsIdintificator(word, WordCode) && !islet) return true;
		if (IsLiteral(word, WordCode, idtype, len, c, iddatatype)) return true;
		throw ERROR_THROW_IN(127, len, c); //
	}

}
