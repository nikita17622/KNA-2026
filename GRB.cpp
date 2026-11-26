#include "GRB.h"
#include <cstring>
#define GRB_ERROR_SERIES 600
namespace GRB
{
#define NS(n) Rule::Chain::N(n)
#define TS(n) Rule::Chain::T(n)

    // Операции в выражениях: 'v' (+ - * /), % , '[' (<<), ']' (>>), < , >
    // Одна строка OPS('X') разворачивается в 6 цепочек вида  <операция> X
#define OPS(n) Rule::Chain(2, TS('v'), NS(n)), Rule::Chain(2, TS('%'), NS(n)), \
               Rule::Chain(2, TS('['), NS(n)), Rule::Chain(2, TS(']'), NS(n)), \
               Rule::Chain(2, TS('<'), NS(n)), Rule::Chain(2, TS('>'), NS(n))

    // Грамматика в нормальной форме Грейбаха: каждая цепочка начинается с терминала,
    // у цепочек одного правила разные первые терминалы (разбор без возвратов).
    // Лексемы: f=fn, i=идентификатор, m=main, t=тип, L=let, n=input, o=loop, p=print, r=return,
    //          l=литерал (в т.ч. true/false), ^ это ->, v=+ - * /, [ это <<, ] это >>, $=дно стека.
    // Ленте (ТЛ) нужен завершающий '$'.
    // Коды ошибок: 600 - структура программы, 601 - оператор, 602 - выражение,
    //              603 - параметры функции, 604 - параметры вызова.
    Greibach greibach(NS('S'), TS('$'),
        20, // число правил

        // ---- S  программа: одна или несколько функций (main - тоже функция)
        Rule(NS('S'), GRB_ERROR_SERIES + 0, 1,
            Rule::Chain(3, TS('f'), NS('H'), NS('K'))                                               // S -> f H K   : fn <имя> ( ... ) -> тип { тело }, затем K (след. функция / конец)
        ),

        // ---- H  имя функции после fn: идентификатор или main
        Rule(NS('H'), GRB_ERROR_SERIES + 0, 2,
            Rule::Chain(3, TS('i'), TS('('), NS('P')),                                               // H -> i ( P   : обычная функция: имя-идентификатор, '(' и параметры
            Rule::Chain(3, TS('m'), TS('('), NS('P'))                                               // H -> m ( P   : точка входа: fn main(...)  (main - ключевое слово, лексема 'm')
        ),

        // ---- P  параметры функции после '(' и далее: ) -> тип {
        Rule(NS('P'), GRB_ERROR_SERIES + 3, 2,
            Rule::Chain(5, TS(')'), TS('^'), TS('t'), TS('{'), NS('N')),                             // P -> ) ^ t { N   : пустой список параметров: ) -> тип { блок
            Rule::Chain(4, TS('i'), TS(':'), TS('t'), NS('Y'))                                      // P -> i : t Y   : первый параметр  имя : тип  и продолжение Y
        ),

        // ---- Y  продолжение списка параметров
        Rule(NS('Y'), GRB_ERROR_SERIES + 3, 2,
            Rule::Chain(5, TS(')'), TS('^'), TS('t'), TS('{'), NS('N')),                             // Y -> ) ^ t { N   : параметры закончились: ) -> тип { блок
            Rule::Chain(5, TS(','), TS('i'), TS(':'), TS('t'), NS('Y'))                             // Y -> , i : t Y   : ещё параметр (после ',' параметр обязателен: f(a,) - ошибка)
        ),

        // ---- K  что может идти после закрывающей } функции
        Rule(NS('K'), GRB_ERROR_SERIES + 0, 3,
            Rule::Chain(2, TS(';'), NS('U')),                                                        // K -> ; U   : необязательная ';' после закрывающей }
            Rule::Chain(3, TS('f'), NS('H'), NS('K')),                                               // K -> f H K   : следующая функция сразу после }
            Rule::Chain(1, TS('$'))                                                                 // K -> $   : конец программы
        ),

        // ---- U  что может идти после }; функции
        Rule(NS('U'), GRB_ERROR_SERIES + 0, 2,
            Rule::Chain(3, TS('f'), NS('H'), NS('K')),                                               // U -> f H K   : следующая функция после };
            Rule::Chain(1, TS('$'))                                                                 // U -> $   : конец программы после };
        ),

        // ---- N  блок: операторы до закрывающей } (} входит в N). Один и тот же N у функций, loop и ?:
        Rule(NS('N'), GRB_ERROR_SERIES + 1, 8,
            Rule::Chain(1, TS('}')),                                                                 // N -> }   : конец блока (пустой блок или после последнего оператора)
            Rule::Chain(6, TS('L'), TS('i'), TS(':'), TS('t'), NS('X'), NS('N')),                    // N -> L i : t X N   : let имя : тип ...   + следующий оператор
            Rule::Chain(3, TS('i'), NS('Q'), NS('N')),                                               // N -> i Q N   : присваивание или вызов функции (разбирает Q)
            Rule::Chain(5, TS('p'), TS('('), NS('C'), TS(';'), NS('N')),                             // N -> p ( C ; N   : print( выражение );
            Rule::Chain(6, TS('n'), TS('('), TS('i'), TS(')'), TS(';'), NS('N')),                    // N -> n ( i ) ; N   : input( имя );
            Rule::Chain(3, TS('r'), NS('E'), NS('N')),                                               // N -> r E N   : return выражение;
            Rule::Chain(3, TS('o'), NS('Z'), NS('N')),                                               // N -> o Z N   : цикл (разбирает Z)
            Rule::Chain(12, TS('?'), TS('('), NS('C'), TS('^'), TS('{'), NS('N'), TS(':'), TS('^'), TS('{'), NS('N'), TS(';'), NS('N')) // N -> ? ( C ^ { N : ^ { N ; N   : ? (усл) -> { тело } : -> { тело };
        ),

        // ---- X  хвост let: ';' или '= выражение'
        Rule(NS('X'), GRB_ERROR_SERIES + 1, 2,
            Rule::Chain(1, TS(';')),                                                                 // X -> ;   : let имя : тип;
            Rule::Chain(2, TS('='), NS('E'))                                                        // X -> = E   : let имя : тип = выражение;
        ),

        // ---- Q  хвост оператора, начатого идентификатором: присваивание или вызов
        Rule(NS('Q'), GRB_ERROR_SERIES + 1, 2,
            Rule::Chain(2, TS('='), NS('E')),                                                        // Q -> = E   : имя = выражение;
            Rule::Chain(3, TS('('), NS('A'), TS(';'))                                               // Q -> ( A ;   : вызов функции как оператор: имя( ... );
        ),

        // ---- Z  хвост loop: с пост- или пред-условием
        Rule(NS('Z'), GRB_ERROR_SERIES + 1, 2,
            Rule::Chain(6, TS('{'), NS('N'), TS('?'), TS('('), NS('C'), TS(';')),                    // Z -> { N ? ( C ;   : loop { тело } ? (усл);       - с постусловием
            Rule::Chain(6, TS('?'), TS('('), NS('C'), TS('{'), NS('N'), TS(';'))                    // Z -> ? ( C { N ;   : loop ? (усл) { тело };       - с предусловием
        ),

        // ---- E  выражение, заканчивающееся ';' (';' входит в выражение)
        Rule(NS('E'), GRB_ERROR_SERIES + 2, 3,
            Rule::Chain(2, TS('i'), NS('I')),                                                        // E -> i I   : начинается с идентификатора (переменная или вызов)
            Rule::Chain(2, TS('l'), NS('M')),                                                        // E -> l M   : начинается с литерала
            Rule::Chain(3, TS('('), NS('C'), NS('M'))                                               // E -> ( C M   : начинается со скобок ( ... )
        ),

        // ---- I  выражение после идентификатора (до ';')
        Rule(NS('I'), GRB_ERROR_SERIES + 2, 8,
            Rule::Chain(3, TS('('), NS('A'), NS('M')),                                               // I -> ( A M   : вызов функции имя( ... ), затем хвост M
            Rule::Chain(1, TS(';')),                                                                 // I -> ;   : конец выражения
            OPS('E')                                                                                // I -> op E   (op: + - * / % << >> < >)   : бинарная операция и продолжение выражения
        ),

        // ---- M  хвост выражения (до ';'): конец или операция и продолжение
        Rule(NS('M'), GRB_ERROR_SERIES + 2, 7,
            Rule::Chain(1, TS(';')),                                                                 // M -> ;   : конец выражения
            OPS('E')                                                                                // M -> op E   (op: + - * / % << >> < >)   : бинарная операция и продолжение выражения
        ),

        // ---- C  выражение, заканчивающееся ')' (')' входит в выражение): print, условия, скобки
        Rule(NS('C'), GRB_ERROR_SERIES + 2, 3,
            Rule::Chain(2, TS('i'), NS('J')),                                                        // C -> i J   : начинается с идентификатора
            Rule::Chain(2, TS('l'), NS('O')),                                                        // C -> l O   : начинается с литерала
            Rule::Chain(3, TS('('), NS('C'), NS('O'))                                               // C -> ( C O   : вложенные скобки
        ),

        // ---- J  выражение после идентификатора (до ')')
        Rule(NS('J'), GRB_ERROR_SERIES + 2, 8,
            Rule::Chain(3, TS('('), NS('A'), NS('O')),                                               // J -> ( A O   : вызов функции внутри скобок
            Rule::Chain(1, TS(')')),                                                                 // J -> )   : конец выражения - закрывающая )
            OPS('C')                                                                                // J -> op C   (op: + - * / % << >> < >)   : бинарная операция и продолжение выражения
        ),

        // ---- O  хвост выражения (до ')')
        Rule(NS('O'), GRB_ERROR_SERIES + 2, 7,
            Rule::Chain(1, TS(')')),                                                                 // O -> )   : конец выражения - закрывающая )
            OPS('C')                                                                                // O -> op C   (op: + - * / % << >> < >)   : бинарная операция и продолжение выражения
        ),

        // ---- W  аргумент вызова: выражение, заканчивающееся ',' или ')'
        Rule(NS('W'), GRB_ERROR_SERIES + 4, 3,
            Rule::Chain(2, TS('i'), NS('V')),                                                        // W -> i V   : аргумент начинается с идентификатора
            Rule::Chain(2, TS('l'), NS('R')),                                                        // W -> l R   : аргумент - литерал
            Rule::Chain(3, TS('('), NS('C'), NS('R'))                                               // W -> ( C R   : аргумент начинается со скобок
        ),

        // ---- V  аргумент после идентификатора
        Rule(NS('V'), GRB_ERROR_SERIES + 4, 9,
            Rule::Chain(3, TS('('), NS('A'), NS('R')),                                               // V -> ( A R   : аргумент - вызов функции
            Rule::Chain(1, TS(')')),                                                                 // V -> )   : последний аргумент, закрывающая )
            Rule::Chain(2, TS(','), NS('W')),                                                        // V -> , W   : после аргумента ',' и следующий аргумент
            OPS('W')                                                                                // V -> op W   (op: + - * / % << >> < >)   : бинарная операция и продолжение выражения
        ),

        // ---- R  хвост аргумента: ')' - конец списка, ',' - следующий аргумент
        Rule(NS('R'), GRB_ERROR_SERIES + 4, 8,
            Rule::Chain(1, TS(')')),                                                                 // R -> )   : последний аргумент, закрывающая )
            Rule::Chain(2, TS(','), NS('W')),                                                        // R -> , W   : после аргумента ',' и следующий аргумент
            OPS('W')                                                                                // R -> op W   (op: + - * / % << >> < >)   : бинарная операция и продолжение выражения
        ),

        // ---- A  аргументы вызова после '(' (может быть пусто)
        Rule(NS('A'), GRB_ERROR_SERIES + 4, 4,
            Rule::Chain(1, TS(')')),                                                                 // A -> )   : пустой список аргументов
            Rule::Chain(2, TS('i'), NS('V')),                                                        // A -> i V   : первый аргумент - идентификатор
            Rule::Chain(2, TS('l'), NS('R')),                                                        // A -> l R   : первый аргумент - литерал
            Rule::Chain(3, TS('('), NS('C'), NS('R'))                                               // A -> ( C R   : первый аргумент - скобки
        )

    );


	Rule::Chain::Chain(short psize, GRBALPHABET s, ...)
	{
		nt = new GRBALPHABET[size = psize];
		int* p = (int*)&s;
		for (short i = 0; i < psize; ++i) nt[i] = (GRBALPHABET)p[i];
	};

	Rule::Rule(GRBALPHABET pnn, int piderror, short psize, Chain c, ...)
	{
		nn = pnn;
		iderror = piderror;
		chains = new Chain[size = psize];
		Chain* p = &c;
		for (int i = 0; i < size; i++) chains[i] = p[i];
	};

	Greibach::Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottom, short psize, Rule r, ...)
	{
		startN = pstartN;
		stbottomT = pstbottom;
		rules = new Rule[size = psize];
		Rule* p = &r;
		for (int i = 0; i < size; i++) rules[i] = p[i];
	};

	Greibach getGreibach() { return greibach; };

	short Greibach::getRule(GRBALPHABET pnn, Rule& prule)
	{
		short rc = -1;
		short k = 0;
		while (k < size && rules[k].nn != pnn) k++;
		if (k < size) prule = rules[rc = k];
		return rc;
	};

	Rule Greibach::getRule(short n)
	{
		Rule rc;
		if (n < size) rc = rules[n];
		return rc;
	};

	char* Rule::getCRule(char* b, short nchain) // получить правило в виде №->цепочка
	{
		char bchain[200];
		b[0] = Chain::alphabet_to_char(nn); b[1] = '-'; b[2] = '>'; b[3] = 0x00;
		chains[nchain].getCChain(bchain);
		strcat_s(b, sizeof(bchain) + 5, bchain);
		return b;
	};

	short Rule::getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j)
	{
		short rc = -1;
		while (j < size && chains[j].nt[0] != t) ++j;
		rc = (j < size ? j : -1);
		if (rc >= 0) pchain = chains[rc];
		return rc;
	};

	char* Rule::Chain::getCChain(char* b) // получить правую сторону правила
	{
		for (int i = 0; i < size; i++) b[i] = Chain::alphabet_to_char(nt[i]);
		b[size] = 0x00;
		return b;
	};
}