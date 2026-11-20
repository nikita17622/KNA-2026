#pragma once
#include "Error.h"

typedef short GRBALPHABET;   // символы: терминалы > 0, нетерминалы < 0

#define NS(n) GRB::Rule::Chain::N(n)
#define TS(t) GRB::Rule::Chain::T(t)
#define GRB_ERROR_SERIES 600

namespace GRB
{
    struct Rule
    {
        GRBALPHABET nn;          // нетерминал (левый символ правила) < 0
        int iderror;             // идентификатор диагностического сообщения
        short size;              // количество цепочек - правых частей правила

        struct Chain
        {
            short size;          // длина цепочки
            GRBALPHABET* nt;     // цепочка терминалов (>0) и нетерминалов (<0)

            Chain() { size = 0; nt = 0; };
            Chain(short psize, GRBALPHABET s, ...);

            char* getCChain(char* b);

            static GRBALPHABET T(char t) { return GRBALPHABET(t); };
            static GRBALPHABET N(char n) { return -GRBALPHABET(n); };
            static bool isT(GRBALPHABET s) { return s > 0; };
            static bool isN(GRBALPHABET s) { return !isT(s); };
            static char alphabet_to_char(GRBALPHABET s) { return isT(s) ? char(s) : char(-s); };
        }*chains;   // массив цепочек

        Rule() { nn = 0x00; size = 0; }
        Rule(GRBALPHABET pnn, int piderror, short psize, Chain c, ...);

        char* getCRule(char* b, short nchain);
        short getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j);
    };

    struct Greibach
    {
        short size;                // количество правил
        GRBALPHABET startN;        // стартовый символ
        GRBALPHABET stbottomT;     // дно стека
        Rule* rules;               // множество правил

        Greibach() { size = 0; startN = 0; stbottomT = 0; rules = 0; }
        Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottomT, short psize, Rule r, ...);

        short getRule(GRBALPHABET pnn, Rule& prule);
        Rule getRule(short n);
    };

    Greibach getGreibach();
}