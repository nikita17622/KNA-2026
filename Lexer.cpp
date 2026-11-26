#include "Lexer.h"
#include "Error.h"
#include <cstring>
#include <cstdlib>
#include <string>
#include <iomanip>
#include <cstdio>
#include <cstdarg>
#include <climits>
using namespace std;
namespace LEXER
{
	IT::IDDATATYPE iddatatype = IT::IDDATATYPE::NONE;   // тип данных последнего слова-типа / литерала
	IT::IDTYPE idtype = IT::IDTYPE::V;                  // нужен только сигнатуре Automats, лексер его больше не использует
	bool islet = false;                                 // сейчас накоплен литерал в кавычках
	int lastIndex = 0;                                  // не используется (оставлен, т.к. объявлен в Lexer.h)

	// ---- состояние для определения вида идентификатора (V / F / P) и типов ----
	static int  lastDecl = -1;      // индекс в ТИ последнего ОБЪЯВЛЕННОГО идентификатора: ему достанется тип из "имя: тип"
	static int  curFunc = -1;       // индекс в ТИ последней объявленной функции: ей достанется тип из "-> тип"
	static bool inHeader = false;   // мы внутри скобок заголовка функции fn имя( ... ) -> идентификаторы здесь параметры
	static char nextSym = '\0';     // ближайший непробельный символ после текущего слова (отличает вызов f( от переменной)
	static int  litCount = 0;       // счётчик для имён литералов в ТИ: L0, L1, ...

	Lexer::Lexer()
	{
		lexTable = LT::Create(LT_MAXSIZE);
		idTable = IT::Create(TI_MAXSIZE);
		line = 0;
	}

	void Lexer::AddLex(char lexSymbol, int line)
	{
		LT::Entry ltentry;
		ltentry.lexema[0] = lexSymbol;
		ltentry.idxTI = LT_TI_NULLIDX;
		ltentry.numberStr = line;
		LT::Add(lexTable, ltentry);
		islet = false;
	}

	// Литерал получает СВОЮ запись в ТИ (idtype = L). Раньше значение затирало последний идентификатор.
	void Lexer::AddLeteral(char lexSymbol, int line, char word[])
	{
		islet = false;
		IT::Entry e = {};                               // обнуляем: value = 0
		e.idtype = IT::L;
		e.iddatatype = iddatatype;
		e.idxFirstLT = lexTable.size;
		sprintf_s(e.id, "L%d", litCount++);

		switch (iddatatype)
		{
		case IT::INT:    e.value.valInt = (int)strtoll(word, nullptr, 10); break;
		case IT::BOOL:   e.value.valBool = (strcmp(word, "true") == 0); break;
		case IT::SYMBOL: e.value.valSymbol = word[0]; break;
		case IT::STR:
		{
			size_t n = strlen(word);                    // длина уже проверена при чтении (<= STR_CAP)
			memcpy(e.value.valStr.str, word, n);
			e.value.valStr.str[n] = '\0';
			e.value.valStr.len = (char)n;               // в IT.h лучше сделать unsigned char len
			break;
		}
		default: break;
		}
		IT::Add(idTable, e);

		LT::Entry ltentry;
		ltentry.lexema[0] = lexSymbol;
		ltentry.numberStr = line;
		ltentry.idxTI = idTable.size - 1;               // индекс ТОЛЬКО ЧТО добавленной записи
		LT::Add(lexTable, ltentry);
	}

	// Вид идентификатора определяем по ПРЕДЫДУЩЕЙ лексеме в ТЛ:
	//   fn  <имя>            -> F (функция)
	//   let <имя>            -> V (переменная)
	//   fn имя( <имя> : ...  -> P (параметр: внутри скобок заголовка, после '(' или ',')
	//   иначе - использование: имя( -> F (вызов), остальное -> V
	void Lexer::AddIdentifikator(char lexSymbol, int line, char word[])
	{
		islet = false;
		if (strlen(word) >= ID_MAXSIZE) word[ID_MAXSIZE - 1] = '\0';   // усечение ДО поиска в ТИ, иначе длинное имя не найдётся

		char prev = lexTable.size > 0 ? lexTable.table[lexTable.size - 1].lexema[0] : '\0';
		bool decl = true;
		IT::IDTYPE kind;
		if (prev == LEX_FUNCTION) kind = IT::F;
		else if (prev == LEX_LET) kind = IT::V;
		else if (inHeader && (prev == LEX_LEFTHESIS || prev == LEX_COMMA)) kind = IT::P;
		else { decl = false; kind = (nextSym == '(') ? IT::F : IT::V; }

		int idx = IT::IsId(idTable, word);
		if (idx == -1)                                  // новый идентификатор
		{
			IT::Entry e = {};
			e.iddatatype = IT::NONE;                    // тип придёт позже: он записывается после имени ("x: integer")
			e.idtype = kind;
			e.idxFirstLT = lexTable.size;
			strncpy_s(e.id, sizeof(e.id), word, _TRUNCATE);
			IT::Add(idTable, e);
			idx = idTable.size - 1;
		}
		if (decl)
		{
			lastDecl = idx;
			if (kind == IT::F) curFunc = idx;
		}

		LT::Entry ltentry;
		ltentry.lexema[0] = lexSymbol;
		ltentry.numberStr = line;
		ltentry.idxTI = idx;                            // был инвертирован: новый -> NULL, существующий -> size
		LT::Add(lexTable, ltentry);
	}

	void Lexer::FunctionCall(char& lexSymbol, int currentLine, char word[])
	{
		if (lexSymbol == '\0') return;
		char prev = lexTable.size > 0 ? lexTable.table[lexTable.size - 1].lexema[0] : '\0';

		if (lexSymbol == LEX_ID) AddIdentifikator(lexSymbol, currentLine, word);
		else if (lexSymbol == LEX_LITERAL) AddLeteral(lexSymbol, currentLine, word);
		else if (lexSymbol == LEX_MAIN && prev == LEX_FUNCTION) AddIdentifikator(lexSymbol, currentLine, word);  // fn main: функция тоже попадает в ТИ
		else
		{
			if (lexSymbol == 't')       // слово-тип стоит ПОСЛЕ имени
			{
				if (prev == LEX_COLON && lastDecl >= 0) idTable.table[lastDecl].iddatatype = iddatatype;   // x: integer
				else if (prev == LEX_ARROW && curFunc >= 0) idTable.table[curFunc].iddatatype = iddatatype; // -> integer
			}
			else if (lexSymbol == LEX_LEFTHESIS)   // '(' сразу после "fn имя" открывает список параметров
			{
				int n = lexTable.size;
				char p2 = n >= 2 ? lexTable.table[n - 2].lexema[0] : '\0';
				inHeader = (prev == LEX_ID || prev == LEX_MAIN) && p2 == LEX_FUNCTION;
			}
			else if (lexSymbol == LEX_RIGHTHESIS) inHeader = false;
			AddLex(lexSymbol, currentLine);
		}
		lexSymbol = '\0';
		iddatatype = IT::IDDATATYPE::NONE;     // сброс: иначе тип "протекает" в следующие слова
		idtype = IT::IDTYPE::V;
	}

	void Lexer::CallCheckFunction(char* word, char& lexSymbol, int& len, int& c, int& lengthWord)
	{
		word[lengthWord] = '\0';
		Automats::FindFST(word, lexSymbol, idtype, iddatatype, len, c, islet);
		lengthWord = 0;
	}

	void Lexer::LexicalAnalysis(In::IN& in, Log::LOG& log)
	{
		char word[256] = "";
		char currentSymbol, lexSymbol = '\0';
		int currentPos = 0, currentLine = 1, lengthWord = 0;
		const int STR_CAP = (int)sizeof(IT::Entry::value.valStr.str) - 1;   // макс. символов строки (без '\0')

		// добавить символ в строку-литерал с проверкой границы
		auto put = [&](char ch) { if (lengthWord >= STR_CAP) throw ERROR_THROW(129); word[lengthWord++] = ch; };

		for (int i = 0; i <= in.size; i++)    // i == in.size: '\0' работает как стоп-символ и сбрасывает последнее слово
		{
			currentSymbol = in.text[i];
			currentPos++;

			int k = i;                         // ближайший непробельный символ (текст оканчивается '\0')
			while (in.text[k] == ' ' || in.text[k] == '\t') k++;
			nextSym = in.text[k];

			if (currentSymbol == '\n' || currentSymbol == '\0')
			{
				CallCheckFunction(word, lexSymbol, currentLine, currentPos, lengthWord);
				FunctionCall(lexSymbol, currentLine, word);      // ДО currentLine++: лексема остаётся на своей строке
				currentLine++;
				currentPos = 0;
			}
			else if (currentSymbol == '\t' || currentSymbol == ' ' || currentSymbol == '\r')
			{
				CallCheckFunction(word, lexSymbol, currentLine, currentPos, lengthWord);
			}
			else if (strchr("(){}+=/*,;%:?", currentSymbol))
			{
				CallCheckFunction(word, lexSymbol, currentLine, currentPos, lengthWord);
				FunctionCall(lexSymbol, currentLine, word);

				word[lengthWord++] = currentSymbol;
				CallCheckFunction(word, lexSymbol, currentLine, currentPos, lengthWord);
			}
			else if (currentSymbol == '\"')
			{
				CallCheckFunction(word, lexSymbol, currentLine, currentPos, lengthWord);
				FunctionCall(lexSymbol, currentLine, word);      // слово перед кавычкой не должно потеряться

				currentSymbol = in.text[++i];
				currentPos++;
				while (currentSymbol != '\"')
				{
					if (currentSymbol == '\0') throw ERROR_THROW(129);          // кавычка не закрыта
					if (currentSymbol == '\\' && in.text[i + 1] != '\0')        // \" и \\ не закрывают строку
					{
						put(currentSymbol);
						currentSymbol = in.text[++i];
					}
					put(currentSymbol);
					currentSymbol = in.text[++i];
				}
				iddatatype = IT::IDDATATYPE::STR;
				islet = true;
				CallCheckFunction(word, lexSymbol, currentLine, currentPos, lengthWord);
			}
			else if (currentSymbol == '\'')
			{
				CallCheckFunction(word, lexSymbol, currentLine, currentPos, lengthWord);
				FunctionCall(lexSymbol, currentLine, word);

				currentSymbol = in.text[++i];
				currentPos++;
				if (currentSymbol == '\'' || currentSymbol == '\0') throw ERROR_THROW(129);   // пустой или незакрытый symbol
				word[lengthWord++] = currentSymbol;
				if (in.text[++i] != '\'') throw ERROR_THROW(129);

				iddatatype = IT::IDDATATYPE::SYMBOL;
				islet = true;
				CallCheckFunction(word, lexSymbol, currentLine, currentPos, lengthWord);
			}
			else if (currentSymbol == '<' || currentSymbol == '>')    // < << > >>
			{
				CallCheckFunction(word, lexSymbol, currentLine, currentPos, lengthWord);
				FunctionCall(lexSymbol, currentLine, word);

				word[lengthWord++] = currentSymbol;
				if (in.text[i + 1] == currentSymbol) { word[lengthWord++] = currentSymbol; i++; }
				CallCheckFunction(word, lexSymbol, currentLine, currentPos, lengthWord);
			}
			else if (currentSymbol == '-')                            // - и ->
			{
				CallCheckFunction(word, lexSymbol, currentLine, currentPos, lengthWord);
				FunctionCall(lexSymbol, currentLine, word);

				word[lengthWord++] = currentSymbol;
				if (in.text[i + 1] == '>') { word[lengthWord++] = '>'; i++; }
				CallCheckFunction(word, lexSymbol, currentLine, currentPos, lengthWord);
			}
			else
			{
				if (lengthWord >= (int)sizeof(word) - 1) throw ERROR_THROW_IN(127, currentLine, currentPos);   // слишком длинное слово
				word[lengthWord++] = currentSymbol;
			}
			FunctionCall(lexSymbol, currentLine, word);
		}
	}

	void Lexer::PrintTables(Log::LOG& log)
	{
		if (!log.stream) return;

		// ============================================================
		// 1. ТАБЛИЦА ЛЕКСЕМ (ТЛ)
		// ============================================================
		*(log.stream) << "            ТАБЛИЦА ЛЕКСЕМ (ТЛ)\n";
		int currentLine = -1;                 // был char: после строки 127 номера строк ломались

		for (int i = 0; i < lexTable.size; i++)
		{
			LT::Entry e = lexTable.table[i];
			if (e.numberStr != currentLine) {
				if (currentLine != -1) *(log.stream) << "\n";
				currentLine = e.numberStr;
				*(log.stream) << " Строка " << setw(3) << e.numberStr << ": ";
			}
			*(log.stream) << e.lexema[0];
		}
		*(log.stream) << "\n\n";

		// ============================================================
		// 2. ТАБЛИЦА ИДЕНТИФИКАТОРОВ (ТИ)
		// ============================================================
		*(log.stream) << "                     ТАБЛИЦА ИДЕНТИФИКАТОРОВ (ТИ)\n";
		*(log.stream) << "=======================================================================================\n";
		*(log.stream) << setw(5) << left << "ID"
			<< setw(15) << left << "Имя"
			<< setw(12) << left << "Тип данных"
			<< setw(15) << left << "Тип ID"
			<< setw(10) << left << "Индекс ТЛ"
			<< "Значение" << "\n";
		*(log.stream) << "=======================================================================================\n";

		for (int i = 0; i < idTable.size; i++)
		{
			IT::Entry e = idTable.table[i];

			const char* typeStr = "NONE";
			if (e.iddatatype == IT::INT)    typeStr = "INT";
			if (e.iddatatype == IT::STR)    typeStr = "STR";
			if (e.iddatatype == IT::SYMBOL) typeStr = "SYMBOL";
			if (e.iddatatype == IT::BOOL)   typeStr = "BOOL";

			const char* idtypeStr = "";
			switch (e.idtype) {
			case IT::V: idtypeStr = "Переменная"; break;
			case IT::F: idtypeStr = "Функция";    break;
			case IT::P: idtypeStr = "Параметр";   break;
			case IT::L: idtypeStr = "Литерал";    break;
			}

			*(log.stream) << setw(5) << left << i
				<< setw(15) << left << e.id
				<< setw(12) << left << typeStr
				<< setw(15) << left << idtypeStr
				<< setw(10) << left << e.idxFirstLT;

			// значение есть только у литералов; читаем то поле union, которое записывали
			if (e.idtype != IT::L)                     *(log.stream) << "-";
			else if (e.iddatatype == IT::INT)          *(log.stream) << e.value.valInt;
			else if (e.iddatatype == IT::STR)          *(log.stream) << "\"" << e.value.valStr.str << "\"";
			else if (e.iddatatype == IT::SYMBOL)       *(log.stream) << "'" << e.value.valSymbol << "'";   // был valStr.str: читал мусор
			else if (e.iddatatype == IT::BOOL)         *(log.stream) << (e.value.valBool ? "true" : "false"); // был valInt == 1: читал мусор

			*(log.stream) << "\n";
		}
		*(log.stream) << "=======================================================================================\n\n";
	}
}