
#include "In.h"
#include <fstream>
#include "Error.h"

using namespace std;

namespace In {

    IN getin(wchar_t infile[])
    {
        size_t len = wcslen(infile) + 1;
        char* filename = new char[len];
        size_t converted;
        wcstombs_s(&converted, filename, len, infile, len - 1);

        ifstream fin(infile);

        IN newIn = {};
        newIn.lines = 1;
        newIn.text = new char[IN_MAX_LEN_TEXT] {};

        int temp[256] = IN_CODE_TABLE;
        for (int i = 0; i < 256; i++)
            newIn.code[i] = temp[i];

        if (!fin.is_open())
            throw ERROR_THROW(110);

        unsigned char ch;
        int pos = 0;

        while (fin.get(reinterpret_cast<char&>(ch))) {
            pos++;
            if (newIn.code[ch] == newIn.T) {
                newIn.text[newIn.size] = ch;
                newIn.size++;
            }

            else if (newIn.code[ch] == newIn.F)
                throw ERROR_THROW_IN(111, newIn.lines, pos);

            else if (newIn.code[ch] == newIn.I)
                newIn.ignore++;

            if (ch == IN_CODE_ENDL  || ch == '\r') {
                newIn.lines++;
                pos = 0;
            }
        }

        fin.close();

        return newIn;
    }

    void ProcessingText(IN& in) {
        int size = strlen(in.text); int position = 0;
        in.redactedText = new char[size] {}; in.redactedText[0] = '\r';
        char lastSimbol;

        for (int i = 0; i < size; i++)
        {
            char c = in.text[i];

            if ((c == ' '  || c == '\n') && in.redactedText[0] == '\r')
                continue;

            if (i > 0)
            {
                if (c == ' ' || c == '\n')
                {
                    if (lastSimbol == c) continue;
                }
            }

            if (c == '\n')
            {
                in.redactedText[position++] = SEPARATOR;
                lastSimbol = in.text[i];
                continue;
            }

            in.redactedText[position++] = c;
            lastSimbol = in.text[i];
        }
    }
}