#pragma once                        
#define IN_MAX_LEN_TEXT 1024*1024   
#define IN_CODE_ENDL '\n'           


#define IN_CODE_TABLE {\
    In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::T, In::IN::T, In::IN::F, In::IN::F, In::IN::I, In::IN::F, In::IN::F,\
    In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F,\
    In::IN::T, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::T, In::IN::T, In::IN::T, In::IN::T, In::IN::T, In::IN::T, In::IN::T, In::IN::T, In::IN::T,\
    In::IN::T, In::IN::T, In::IN::T, In::IN::T, In::IN::T, In::IN::T, In::IN::T, In::IN::T, In::IN::T, In::IN::T, In::IN::F, In::IN::T, In::IN::F, In::IN::T, In::IN::F, In::IN::F,\
    In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F,\
    In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F,\
    In::IN::F, In::IN::T, In::IN::T, In::IN::T, In::IN::T, In::IN::T, In::IN::T, In::IN::T, In::IN::T, In::IN::T, In::IN::T, In::IN::T, In::IN::T, In::IN::T, In::IN::T, In::IN::T,\
    In::IN::T, In::IN::T, In::IN::T, In::IN::T, In::IN::T, In::IN::T, In::IN::T, In::IN::T, In::IN::T, In::IN::T, In::IN::T, In::IN::T, In::IN::F, In::IN::T, In::IN::F, In::IN::F,\
    In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F,\
    In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F,\
    In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F,\
    In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F,\
    In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F,\
    In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F,\
    In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F,\
    In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F, In::IN::F,\
}
#define SEPARATOR '\n'

namespace In
{
    struct IN    
    {
        enum { T = 1024, F = 2048, I = 4096 };   
        int size;      
        int lines;     
        int ignore;     
        char* text;    
        int code[256]; 

        char* redactedText;
        int sizeRedact;
    };
    IN getin(wchar_t infile[]);   
    void ProcessingText(IN& in);
};
