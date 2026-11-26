#include <iostream>
#include <locale>
#include <cwchar>
#include <fstream>
#include "MFST.h" // магазинный автомат
#include "GRB.h"
#include "Error.h"
#include "Parm.h"
#include "Log.h"
#include "In.h"
#include "Lexer.h"
#include "LT.h"
#include "IT.h"
#include <tchar.h>


int wmain(int argc, wchar_t* argv[])
{
	setlocale(LC_ALL, "rus");
	Log::LOG log;
	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv);
		log = Log::getlog(parm.log);
		In::IN in = In::getin(parm.in);

		Log::WriteLog(log);
		Log::WriteParm(log, parm);
		Log::WriteIn(log, in);

		LEXER::Lexer lexer;
		lexer.LexicalAnalysis(in, log);
		lexer.PrintTables(log);

		MFST_TRACE_START // отладка

		MFST::Mfst mfst(lexer, GRB::getGreibach()); // автомат
		mfst.start();        // старт синтаксического анализа
		mfst.savededucation();
		mfst.printrules();

		LT::Delete(lexer.lexTable);
		IT::Delete(lexer.idTable);

		Log::Close(log);
		delete[] in.text;

		std::cout << "Лексический и синтаксический анализ выполнены." << std::endl;
	}

	catch (Error::ERROR e)
	{
		Log::WriteError(log, e);
		Log::Close(log);
		std::cout << "Ошибка " << e.id << ": " << e.message << std::endl;
	}

	system("pause");
	return 0;
}