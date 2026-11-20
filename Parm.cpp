#include "Parm.h"
#include "Error.h"
#include <cwchar>

namespace Parm
{
    PARM getparm(int argc, wchar_t* argv[])
    {
        PARM p = { 0 };
        bool in_found = false;

        for (int i = 1; i < argc; i++)
        {
            if (wcslen(argv[i]) >= PARM_MAX_SIZE)
            {
                throw ERROR_THROW(104);
            }

            if (wcsncmp(argv[i], PARM_IN, wcslen(PARM_IN)) == 0)
            {
                wcscpy_s(p.in, PARM_MAX_SIZE, argv[i] + wcslen(PARM_IN));
                in_found = true;
            }
            else if (wcsncmp(argv[i], PARM_OUT, wcslen(PARM_OUT)) == 0)
            {
                wcscpy_s(p.out, PARM_MAX_SIZE, argv[i] + wcslen(PARM_OUT));
            }
            else if (wcsncmp(argv[i], PARM_LOG, wcslen(PARM_LOG)) == 0)
            {
                wcscpy_s(p.log, PARM_MAX_SIZE, argv[i] + wcslen(PARM_LOG));
            }
        }

        if (!in_found)
        {
            throw ERROR_THROW(100);
        }

        if (wcslen(p.out) == 0)
        {
            wcscpy_s(p.out, p.in);
            wcscat_s(p.out, PARM_OUT_DEFAULT_EXT);
        }

        if (wcslen(p.log) == 0)
        {
            wcscpy_s(p.log, p.in);
            wcscat_s(p.log, PARM_LOG_DEFAULT_EXT);
        }

        return p;
    }
}