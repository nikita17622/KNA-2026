#include "Error.h"

namespace Error
{
	ERROR errors[ERROR_MAX_ENTRY] =
	{
		ERROR_ENTRY(0, "Недопустимый код ошибки "),
		ERROR_ENTRY(1, "Системный сбой "),
		ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4), ERROR_ENTRY_NODEF(5),
		ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),
		ERROR_ENTRY_NODEF10(10), ERROR_ENTRY_NODEF10(20), ERROR_ENTRY_NODEF10(30), ERROR_ENTRY_NODEF10(40), ERROR_ENTRY_NODEF10(50),
		ERROR_ENTRY_NODEF10(60), ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80), ERROR_ENTRY_NODEF10(90),

		ERROR_ENTRY(100, "Параметр -in должен быть задан "),
		ERROR_ENTRY_NODEF(101), ERROR_ENTRY_NODEF(102), ERROR_ENTRY_NODEF(103),
		ERROR_ENTRY(104, "Превышена длина входного параметра "),
		ERROR_ENTRY_NODEF(105), ERROR_ENTRY_NODEF(106), ERROR_ENTRY_NODEF(107),
		ERROR_ENTRY_NODEF(108), ERROR_ENTRY_NODEF(109),

		ERROR_ENTRY(110, "Ошибка при открытии файла с исходным кодом (-in) "),
		ERROR_ENTRY(111, "Недопустимый символ в исходном файле (-in) "),
		ERROR_ENTRY(112, "Ошибка при создании файла протокола (-log) "),
		ERROR_ENTRY_NODEF(113), ERROR_ENTRY_NODEF(114), ERROR_ENTRY_NODEF(115),
		ERROR_ENTRY_NODEF(116), ERROR_ENTRY_NODEF(117), ERROR_ENTRY_NODEF(118), ERROR_ENTRY_NODEF(119),
		ERROR_ENTRY(120, "Недопустимый символ в исходном файле (-in) "),
		ERROR_ENTRY(121, "Идентификатор превышает максимальную длину "),
		ERROR_ENTRY(122, "Незакрытая строковая константа "),
		ERROR_ENTRY(123, "Заглавная буква в идентификаторе "),
		ERROR_ENTRY(124, "Числовой литерал выходит за диапазон integer "),
		ERROR_ENTRY(125, "Строковый литерал превышает 255 символов "),
		ERROR_ENTRY(126,"В данной области видимости уже существует переменная с данным идентификатором "),
		ERROR_ENTRY(127,"Недопустимый символ "),
		ERROR_ENTRY(128,"Литерал превышает максимальную величину целочисленного значения "),
		ERROR_ENTRY(129,"Литерал превышает максимальную длину "),

		ERROR_ENTRY_NODEF10(130),  ERROR_ENTRY_NODEF10(140),  ERROR_ENTRY_NODEF10(150),
		ERROR_ENTRY_NODEF10(160),  ERROR_ENTRY_NODEF10(170),  ERROR_ENTRY_NODEF10(180),  ERROR_ENTRY_NODEF10(190),

		ERROR_ENTRY(200,"Недопустимый размер таблицы лексем "),
		ERROR_ENTRY(201,"Таблица лексем переполнена "),
		ERROR_ENTRY(202,"Выход за пределы таблицы лексем "),
		ERROR_ENTRY_NODEF(203),    ERROR_ENTRY_NODEF(204),    ERROR_ENTRY_NODEF(205),    ERROR_ENTRY_NODEF(206),    ERROR_ENTRY_NODEF(207), ERROR_ENTRY_NODEF(208), ERROR_ENTRY_NODEF(209),
		ERROR_ENTRY(210,"Недопустимый размер таблицы идентификаторов "),
		ERROR_ENTRY(211,"Таблица идентификаторов переполнена "),
		ERROR_ENTRY(212,"Выход за пределы таблицы идентификаторов "),
		ERROR_ENTRY_NODEF(213),    ERROR_ENTRY_NODEF(214),    ERROR_ENTRY_NODEF(215),    ERROR_ENTRY_NODEF(216),    ERROR_ENTRY_NODEF(217), ERROR_ENTRY_NODEF(218), ERROR_ENTRY_NODEF(219),
		ERROR_ENTRY_NODEF10(220),  ERROR_ENTRY_NODEF10(230),  ERROR_ENTRY_NODEF10(240),  ERROR_ENTRY_NODEF10(250),
		ERROR_ENTRY_NODEF10(260),  ERROR_ENTRY_NODEF10(270),  ERROR_ENTRY_NODEF10(280),  ERROR_ENTRY_NODEF10(290),
		ERROR_ENTRY_NODEF100(300), ERROR_ENTRY_NODEF100(400), ERROR_ENTRY_NODEF100(500),
		ERROR_ENTRY(600, "Неверная структура "),
		ERROR_ENTRY(601, "Ошибочный оператор "),
		ERROR_ENTRY(602, "Ошибка в выражении "),
		ERROR_ENTRY(603, "Ошибка в параметрах функции "),
		ERROR_ENTRY(604, "Ошибка в параметрах вызываемой функции "),
		ERROR_ENTRY_NODEF(605), ERROR_ENTRY_NODEF(606), ERROR_ENTRY_NODEF(607), ERROR_ENTRY_NODEF(608), ERROR_ENTRY_NODEF(609),
		ERROR_ENTRY_NODEF10(610),  ERROR_ENTRY_NODEF10(620),  ERROR_ENTRY_NODEF10(630),
		ERROR_ENTRY_NODEF10(640),  ERROR_ENTRY_NODEF10(650),  ERROR_ENTRY_NODEF10(660),
		ERROR_ENTRY_NODEF10(670),  ERROR_ENTRY_NODEF10(680),  ERROR_ENTRY_NODEF10(690),
		ERROR_ENTRY_NODEF100(700), ERROR_ENTRY_NODEF100(800), ERROR_ENTRY_NODEF100(900)
	};

	ERROR geterror(int id)
	{
		if (id < 0 || id >= ERROR_MAX_ENTRY) { return errors[0]; }
		return errors[id];
	}

	ERROR geterrorin(int id, int line, int col)
	{
		if (id < 0 || id >= ERROR_MAX_ENTRY) { return errors[0]; }
		ERROR e = errors[id];
		e.inext.line = (short)line;
		e.inext.col = (short)col;
		return e;
	}
}