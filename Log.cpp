#include "Log.h"
#include <ctime>
#include "Error.h"
#include <cstdarg>
#include <iostream>

namespace Log
{
	LOG getlog(wchar_t logfile[])
	{
		LOG log;
		wcscpy_s(log.logfile, logfile);
		log.stream = new std::ofstream(logfile, std::ios::binary);

		if (!log.stream->is_open()) { throw ERROR_THROW(112); }
		return log;
	}

	void WriteLine(LOG& log, const char* c, ...)
	{
		if (!log.stream) return;
		va_list args;
		va_start(args, c);
		const char* ptr = c;
		while (ptr && strlen(ptr) > 0)
		{
			*(log.stream) << ptr;
			ptr = va_arg(args, const char*);
		}

		va_end(args);
		*(log.stream) << "\r\n";
	}

	void WriteLine(LOG& log, const wchar_t* c, ...)
	{
		if (!log.stream) return;
		va_list args;
		va_start(args, c);
		const wchar_t* ptr = c;
		char buffer[512];

		while (ptr && wcslen(ptr) > 0)
		{
			size_t converted;
			wcstombs_s(&converted, buffer, sizeof(buffer), ptr, _TRUNCATE);
			*(log.stream) << buffer;
			ptr = va_arg(args, const wchar_t*);
		}

		va_end(args);
		*(log.stream) << "\r\n";
	}

	void WriteLog(LOG& log)
	{
		if (!log.stream) return;
		time_t t = time(nullptr);
		tm t_str;
		localtime_s(&t_str, &t);
		char time_buf[100];
		strftime(time_buf, sizeof(time_buf), "%d.%m.%Y %H:%M:%S", &t_str);

		*(log.stream) << "         Протокол работы транслятора         \r\n";
		*(log.stream) << " Дата и время создания:" << time_buf << "\r\n\r\n";
	}

	void WriteParm(LOG& log, Parm::PARM parm)
	{
		if (!log.stream) return;
		char buf[PARM_MAX_SIZE];
		size_t conv;

		*(log.stream) << "               Входные параметры             \r\n";
		wcstombs_s(&conv, buf, sizeof(buf), parm.in, _TRUNCATE);
		*(log.stream) << "-in:  " << buf << "\r\n";
		wcstombs_s(&conv, buf, sizeof(buf), parm.out, _TRUNCATE);
		*(log.stream) << "-out: " << buf << "\r\n";
		wcstombs_s(&conv, buf, sizeof(buf), parm.log, _TRUNCATE);
		*(log.stream) << "-log: " << buf << "\r\n\r\n";
	}

	void WriteIn(LOG& log, In::IN in)
	{
		if (!log.stream) return;
		*(log.stream) << "        Результаты ввода исходного кода        \r\n";
		*(log.stream) << "Количество строк:           " << in.lines << "\r\n";
		*(log.stream) << "Количество символов:        " << in.size << "\r\n";
		*(log.stream) << "Количество пропущенных:     " << in.ignore << "\r\n\r\n";
	}

	void WriteError(LOG& log, Error::ERROR error)
	{
		if (log.stream && log.stream->is_open())
		{
			*(log.stream) << "                     Ошибка                    \r\n";
			*(log.stream) << "Код ошибки: " << error.id << "\r\n";
			*(log.stream) << "Сообщение:  " << error.message << "\r\n";

			if (error.inext.line != -1)
			{
				*(log.stream) << "Строка:     " << error.inext.line << "\r\n";
				*(log.stream) << "Позиция:    " << error.inext.col  << "\r\n";
			}
		}

		else
		{
			std::cout << "Ошибка " << error.id << " : " << error.message;

			if (error.inext.line != -1)
			{
				std::cout << ", строка " << error.inext.line << ", позиция " << error.inext.col;
			}
			std::cout << std::endl;
		}
	}

	void Close(LOG& log)
	{
		if (log.stream)
		{
			log.stream->close();
			delete log.stream;
			log.stream = nullptr;
		}
	}
}