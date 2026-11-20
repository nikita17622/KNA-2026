#pragma once
#include <stack>
#include <iostream>
#include <iomanip>       
#include "GRB.h"
#include "LT.h"
#include "Error.h"

#define MFST_DIAGN_NUMBER 3
#define MFST_DIAGN_MAXSIZE 2*ERROR_MAXSIZE_MESSAGE


#define MFST_TRACE_START \
    std::cout << std::setw(4) << std::left << "Шаг" << ": " \
              << std::setw(30) << std::left << "Правило" \
              << std::setw(35) << std::left << "Входная лента" \
              << std::setw(20) << std::left << "Стек" << std::endl;

#define MFST_TRACE(step, rule, lenta, st) \
    std::cout << std::setw(4) << std::left << step << ": " \
              << std::setw(30) << std::left << rule \
              << std::setw(35) << std::left << lenta \
              << std::setw(20) << std::left << st << std::endl;

namespace LEX
{
    struct LEX
    {
        LT::LexTable lextable;
    };
}

class MFSTSTSTACK : public std::stack<short>
{
public:
    using std::stack<short>::c;
};

namespace MFST
{
    struct MfstState
    {
        short lenta_position;
        short nrule;
        short nrulechain;
        MFSTSTSTACK st;

        MfstState();
        MfstState(short pposition, MFSTSTSTACK pst, short pnrulechain);
        MfstState(short pposition, MFSTSTSTACK pst, short pnrule, short pnrulechain);
    };

    class MFSTSTATE : public std::stack<MfstState>
    {
    public:
        using std::stack<MfstState>::c;
    };

    struct Mfst
    {
        enum RC_STEP
        {
            NS_OK, NS_NORULE, NS_NORULECHAIN, NS_ERROR,
            TS_OK, TS_NOK, LENTA_END, SURPRISE
        };

        struct MfstDiagnosis
        {
            short lenta_position;
            RC_STEP rc_step;
            short nrule;
            short nrule_chain;

            MfstDiagnosis();
            MfstDiagnosis(short plenta_position, RC_STEP prc_step, short pnrule, short pnrule_chain);
        } diagnosis[MFST_DIAGN_NUMBER];

        GRBALPHABET* lenta;
        short lenta_position;
        short nrule;
        short nrulechain;
        short lenta_size;
        GRB::Greibach grebach;
        LEX::LEX lex;
        MFSTSTSTACK st;
        MFSTSTATE storestate;

        Mfst();
        Mfst(LEX::LEX plex, GRB::Greibach pgrebach);

        char* getCSt(char* buf);
        char* getClenta(char* buf, short pos, short n = 25);
        char* getDiagnosis(short n, char* buf);
        bool savestate();
        bool reststate();
        bool push_chain(GRB::Rule::Chain chain);
        RC_STEP step();
        bool start();
        bool savediagnosis(RC_STEP pprc_step);
        void printrules();

        struct Deducation
        {
            short size;
            short* nrules;
            short* nrulechains;
            Deducation() { size = 0; nrules = 0; nrulechains = 0; }
        } deducation;

        bool savededucation();
    };
}