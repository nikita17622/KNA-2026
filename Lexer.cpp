#include "Lexer.h"
#include "Error.h"
#include <cstring>
#include <string>
#include <iomanip>
#include <cstdio>
#include <cstdarg>
#include <climits>
using namespace std;
namespace LEXER
{
	IT::IDDATATYPE iddatatype = IT::IDDATATYPE::NONE;
	IT::IDTYPE idtype = IT::IDTYPE::V;

	Lexer::Lexer()
	{
		lexTable = LT::Create(LT_MAXSIZE);
		IdTable = IT::Create(TI_MAXSIZE);
		line = 0;

	}

	void Lexer::AddLex(char lexSymbol, int line)
	{
		LT::Entry ltentry;
		ltentry.lexema[0] = lexSymbol;
		ltentry.idxTI = LT_TI_NULLIDX;
		ltentry.numberStr = line;
		LT::Add(lexTable, ltentry);
	}

	void Lexer::AddIdentifikator(char lexSymbol, int line, char word[])
	{
		int id = IsId(IdTable, word);

		LT::Entry ltentry;
		ltentry.lexema[0] = lexSymbol;
		ltentry.numberStr = line;
		ltentry.idxTI = id == -1 ? LT_TI_NULLIDX : IdTable.size;
		if (id == -1)
		{
			IT::Entry itentry;
			itentry.iddatatype = iddatatype;
			itentry.idtype = idtype;
			strncpy_s(itentry.id, sizeof(itentry.id), word, 5);
			itentry.id[ID_MAXSIZE] = '\0';
			itentry.idxFirstLT = lexTable.size;
		    IT:Add(IdTable, itentry);
		}
		LT::Add(lexTable, ltentry);
	}

	void Lexer::LexicalAnalysis(In::IN& in, Log::LOG& log)
	{

		char word[256];
		char currentSymbol, lexSymbol = '\0';
		int currentPos = 0, currentLine = 1, lengthWord = 0;

		for (int i = 0; i < in.size; i++)
		{
			currentSymbol = in.text[i];
			currentPos++;

			if (currentSymbol == '\n' || currentSymbol == '\r')
			{
				currentLine++;
				word[lengthWord] = '\0';
				Automats::FindFST(word, lexSymbol, idtype, iddatatype, currentLine, currentPos);
				lengthWord = 0;
				currentPos = 0;
			}
			else if (currentSymbol == '\t' || currentSymbol == ' ')
			{
				word[lengthWord] = '\0';
				Automats::FindFST(word, lexSymbol, idtype, iddatatype, currentLine, currentPos);
				lengthWord = 0;
			}
			else if (strchr("(){}+-=/*,;", currentSymbol))
			{
				word[lengthWord] = '\0';
				if (Automats::FindFST(word, lexSymbol, idtype, iddatatype, currentLine, currentPos));
				lengthWord = 0;
				if (lexSymbol == 'i' || lexSymbol == 'l')
				{
					AddIdentifikator(lexSymbol, currentLine, word);
					lexSymbol = '\0';
				}
				if (lexSymbol != '\0')
				{
					AddLex(lexSymbol, currentLine);
					lexSymbol = '\0';
				}

				word[lengthWord++] = currentSymbol;
				word[lengthWord] = '\0';
				Automats::FindFST(word, lexSymbol, idtype, iddatatype, currentLine, currentPos);
				lengthWord = 0;
			}
			else if (currentSymbol == '\'')
			{

				word[lengthWord] = '\0';
				Automats::FindFST(word, lexSymbol, idtype, iddatatype, currentLine, currentPos);
				lengthWord = 0;

				currentSymbol = in.text[++i]; 
				currentPos++;

				while (currentSymbol != '\'')
				{
					if (lengthWord == TI_STR_MAXSIZE + 1) throw ERROR_THROW(129);
					word[lengthWord++] = currentSymbol;
					currentSymbol = in.text[++i];	
				}
				word[lengthWord] = '\0';
				Automats::IsLiteral(word, lexSymbol, idtype);
				lengthWord = 0;
			}
			else
			{
				word[lengthWord] = currentSymbol;
				lengthWord++;
			}

			if (lexSymbol == 'i' || lexSymbol == 'l')
			{
				AddIdentifikator(lexSymbol, currentLine, word);
				lexSymbol = '\0';
			}
			if (lexSymbol != '\0')
			{
				AddLex(lexSymbol, currentLine);
				lexSymbol = '\0';
			}


		}
	}

	void Lexer::PrintTables(Log::LOG& log)
	{
		if (!log.stream) return;
		*(log.stream) << "            ТАБЛИЦА ЛЕКСЕМ (ТЛ)\n";
		char currentLine = -1;

		for (int i = 0; i < lexTable.size; i++)
		{
			LT::Entry e = lexTable.table[i];
			if (e.numberStr != currentLine) {
				if (currentLine != -1) *(log.stream) << "\n";
				currentLine = e.numberStr;
				*(log.stream) << " Строка " << setw(2) << e.numberStr << ": ";
			}

			*(log.stream) << e.lexema[0];
		}
		*(log.stream) << "\n\n";

		*(log.stream) << "         ТАБЛИЦА ИДЕНТИФИКАТОРОВ (ТИ)\n";
		*(log.stream) << setw(5) << left << "ID"
			<< setw(10) << left << "Имя" << setw(10) << left << "Тип данных" << setw(15) << left << "Тип ID" << setw(10) << left << "Индекс ТЛ" << "Значение" << "\n";


		for (int i = 0; i < IdTable.size; i++)
		{
			IT::Entry e = IdTable.table[i];

			string typeStr;
			if (e.iddatatype == IT::INT)  typeStr = "INT";
			if (e.iddatatype == IT::STR)  typeStr = "STR";
			if (e.iddatatype == IT::NONE)  typeStr = "NONE";

			string idtypeStr = "";
			switch (e.idtype) {
			case IT::V: idtypeStr = "Переменная"; break;
			case IT::F: idtypeStr = "Функция"; break;
			case IT::P: idtypeStr = "Параметр"; break;
			case IT::L: idtypeStr = "Литерал"; break;
			}

			*(log.stream) << setw(5) << left << i << setw(10) << left << e.id << setw(10) << left << typeStr << setw(15) << left << idtypeStr << setw(10) << left << e.idxFirstLT;
			*(log.stream) << "\n";
		}
	}
}





//#include "Lexer.h"
//#include "Error.h"
//#include "Automats.h"
//#include <cstring>
//#include <cstdio>
//#include <cstdarg>
//#include <climits>
//
//namespace Lexer
//{
//	LT::LexTable LexTable = LT::Create(LT_MAXSIZE);
//	IT::IdTable IdTable = IT::Create(TI_MAXSIZE);
//
//	bool IsLetter(char c) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_'; }
//	bool IsDigit(char c) { return c >= '0' && c <= '9'; }
//
//	namespace
//	{
//		const int NONE = (int)TI_NULLIDX;   // "нет индекса" (-1), одинаково для LT и IT
//
//		// Форматированная строка в протокол (Log::WriteLine требует завершающий "")
//		void Row(Log::LOG& log, const char* fmt, ...)
//		{
//			char buf[512];
//			va_list a; va_start(a, fmt); vsnprintf(buf, sizeof(buf), fmt, a); va_end(a);
//			Log::WriteLine(log, buf, "");
//		}
//
//		void AddLex(char lex, int line, int idx = NONE)
//		{
//			LT::Entry e;
//			e.lexema[0] = lex; e.numberStr = line; e.idxTI = idx;
//			LT::Add(LexTable, e);
//		}
//
//		int AddId(const char* id, const char* scope, int type)
//		{
//			IT::Entry e = {};   // обнуляем: value = 0, valueStr = nullptr
//			strncpy_s(e.id, sizeof(e.id), id, _TRUNCATE);
//			strncpy_s(e.scope, sizeof(e.scope), scope, _TRUNCATE);
//			e.iddatatype = type;
//			IT::Add(IdTable, e);
//			return IdTable.size - 1;
//		}
//
//		// Поиск использования идентификатора: сначала в текущей области, затем в global
//		// (так находятся имена функций и глобальные объявления)
//		int FindId(char* id, char* scope)
//		{
//			char global[] = "global";
//			int idx = IT::IsId(IdTable, id, scope);
//			return idx != NONE ? idx : IT::IsId(IdTable, id, global);
//		}
//
//		// Паттерн "<id> = <литерал> ;": проверяем две последние лексемы в LT и символ после литерала.
//		// Вызывать ДО добавления лексемы литерала. Возвращает индекс id в IT или NONE.
//		int AssignTarget(const In::IN& in, int next)
//		{
//			while (next < in.size && (in.text[next] == ' ' || in.text[next] == '\t')) next++;
//			int n = LexTable.size;
//			if (n < 2 || next >= in.size || in.text[next] != ';') return NONE;
//			if (LexTable.table[n - 1].lexema[0] != LEX_ASSIGN || LexTable.table[n - 2].lexema[0] != LEX_ID) return NONE;
//			return LexTable.table[n - 2].idxTI;
//		}
//
//		// Вывод ТЛ (построчно) и ТИ (с областью видимости) в протокол
//		void PrintTables(Log::LOG& log)
//		{
//			Row(log, "");
//			Row(log, "---------- LEXEME TABLE ----------");
//			for (int i = 0; i < LexTable.size; )
//			{
//				int line = LexTable.table[i].numberStr, n = 0;
//				char buf[256];
//				for (; i < LexTable.size && LexTable.table[i].numberStr == line && n < 255; i++)
//					buf[n++] = LexTable.table[i].lexema[0];
//				buf[n] = '\0';
//				Row(log, "%03d  %s", line, buf);
//			}
//
//			Row(log, "");
//			Row(log, "---------- IDENTIFIER TABLE ----------");
//			Row(log, "%-4s %-6s %-16s %-8s %s", "N", "id", "scope", "type", "value");
//			for (int i = 0; i < IdTable.size; i++)
//			{
//				IT::Entry& e = IdTable.table[i];
//				char val[300] = "";
//				if (e.iddatatype == IT_INTEGER) sprintf_s(val, "%d", e.value);
//				else if (e.valueStr) sprintf_s(val, "'%s'", e.valueStr);
//				const char* type = e.iddatatype == IT_INTEGER ? "integer" : e.iddatatype == IT_STRING ? "string" : "-";
//				Row(log, "%-4d %-6s %-16s %-8s %s", i, e.id, e.scope, type, val);
//			}
//		}
//	}
//
//	void LexicalAnalysis(In::IN& in, Log::LOG& log)
//	{
//		// Текущая область видимости. Идентификатор в IT уникален парой (id, scope),
//		// поэтому x в main -> (x, "main"), а x в функции fi -> (x, "function_fi") - это разные записи.
//		char currentScope[SCORE_MAXSIZE] = "global";
//		int line = 1, lineStart = 0, depth = 0;
//		int declType = 0;             // тип из последнего integer/string, ожидающий идентификатор
//		bool expectFuncName = false;  // сразу после 'function' ждём имя функции
//
//		for (int i = 0; i < in.size; i++)
//		{
//			char c = in.text[i];
//			int col = i - lineStart + 1;
//
//			if (c == ' ' || c == '\t') continue;
//			if (c == '\n') { line++; lineStart = i + 1; continue; }
//
//			// ---------- слово: ключевое слово или идентификатор ----------
//			if (IsLetter(c))
//			{
//				int start = i;
//				while (i < in.size && (IsLetter(in.text[i]) || IsDigit(in.text[i]))) i++;
//				int len = i - start;
//				i--;                                    // остаёмся на последней букве слова
//				char word[SCORE_MAXSIZE] = "", code = 0;
//				if (len >= (int)sizeof(word)) throw ERROR_THROW_IN(121, line, col);
//				memcpy(word, in.text + start, len);
//
//				if (Automats::IsKeyWord(word, code))
//				{
//					AddLex(code, line);
//					// integer и string дают одну лексему 't', поэтому тип берём по тексту слова
//					if (!strcmp(word, "integer")) declType = IT_INTEGER;
//					else if (!strcmp(word, "string")) declType = IT_STRING;
//					else if (code == LEX_FUNCTION) expectFuncName = true;
//					else if (code == LEX_MAIN) strncpy_s(currentScope, sizeof(currentScope), "main", _TRUNCATE);
//				}
//				else
//				{
//					if (len >= ID_MAXSIZE) throw ERROR_THROW_IN(121, line, col);
//					// объявление (был integer/string) ищем только в текущей области,
//					// использование - в текущей, затем в global
//					int idx = declType ? IT::IsId(IdTable, word, currentScope) : FindId(word, currentScope);
//					if (idx == NONE) idx = AddId(word, currentScope, declType);
//					AddLex(LEX_ID, line, idx);
//					declType = 0;
//
//					// Имя функции уже записано в ТЕКУЩУЮ (global) область, а все дальнейшие
//					// идентификаторы (параметры, локальные) попадут в function_<имя>.
//					if (expectFuncName)
//					{
//						expectFuncName = false;
//						snprintf(currentScope, sizeof(currentScope), "function_%s", word);
//					}
//				}
//				continue;
//			}
//
//			// ---------- целочисленный литерал ----------
//			if (IsDigit(c))
//			{
//				long long v = 0;
//				while (i < in.size && IsDigit(in.text[i]))
//				{
//					v = v * 10 + (in.text[i++] - '0');
//					if (v > INT_MAX) throw ERROR_THROW_IN(124, line, col);
//				}
//				int target = AssignTarget(in, i);       // i - первая позиция после литерала
//				if (target != NONE && IdTable.table[target].iddatatype == IT_INTEGER)
//					IdTable.table[target].value = (int)v;   // несовпадение типов - задача семантики
//				AddLex(LEX_LITERAL, line);
//				i--;
//				continue;
//			}
//
//			// ---------- строковый литерал в одинарных кавычках (" запрещена таблицей In) ----------
//			if (c == '\'')
//			{
//				int start = ++i;
//				while (i < in.size && in.text[i] != '\'' && in.text[i] != '\n') i++;
//				if (i >= in.size || in.text[i] != '\'') throw ERROR_THROW_IN(111, line, col);   // нет закрывающей '
//				int len = i - start;
//				if (len > 255) throw ERROR_THROW_IN(125, line, col);
//
//				int target = AssignTarget(in, i + 1);
//				if (target != NONE && IdTable.table[target].iddatatype == IT_STRING)
//				{
//					IT::Entry& e = IdTable.table[target];
//					delete[] e.valueStr;                // повторное присваивание: освобождаем старое
//					e.valueStr = new char[len + 1];
//					memcpy(e.valueStr, in.text + start, len);
//					e.valueStr[len] = '\0';
//				}
//				AddLex(LEX_LITERAL, line);
//				continue;
//			}
//
//			// ---------- разделитель / оператор (через FST) ----------
//			char sep[2] = { c, '\0' }, code = 0;
//			if (!Automats::IsSeparator(sep, code)) throw ERROR_THROW_IN(111, line, col);
//			AddLex(code, line);
//
//			// Границы области: любая функция и main заканчиваются своей закрывающей '}',
//			// после чего возвращаемся в global.
//			if (c == '{') depth++;
//			else if (c == '}' && --depth <= 0)
//			{
//				depth = 0;
//				strncpy_s(currentScope, sizeof(currentScope), "global", _TRUNCATE);
//			}
//		}
//
//		PrintTables(log);
//	}
//}
