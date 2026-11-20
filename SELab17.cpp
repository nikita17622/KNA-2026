#include <iostream>
#include <locale>
#include <cwchar>
#include <fstream>

#include "Error.h"
#include "Parm.h"
#include "Log.h"
#include "In.h"
#include "Lexer.h"
#include "LT.h"
#include "IT.h"
//#include "MFST.h"
//#include "GRB.h"

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

        //// ============================================================
        //// СИНТАКСИЧЕСКИЙ АНАЛИЗ
        //// ============================================================
        //LEX::LEX lex;
        //lex.lextable = Lexer::LexTable;

        //// Добавляем '$' в конец ТЛ
        //LT::Entry dollarEntry;
        //dollarEntry.lexema[0] = '$';
        //dollarEntry.sn = 0;
        //dollarEntry.idxTI = LT_TI_NULLIDX;
        //LT::Add(lex.lextable, dollarEntry);

        //MFST::Mfst mfst(lex, GRB::getGreibach());
        //mfst.start();

        //// ============================================================
        //// ВЫВОД ДЕРЕВА РАЗБОРА
        //// ============================================================
        //std::cout << std::endl;
        //std::cout << "--- ДЕРЕВО РАЗБОРА ---" << std::endl;
        //mfst.savededucation();
        //mfst.printrules();

        // ---------- очистка ----------
        /*LT::Delete(Lexer::LexTable);
        IT::Delete(Lexer::IdTable);*/

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