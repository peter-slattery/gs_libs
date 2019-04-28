
#ifdef DEBUG
#ifndef Assert
#define Assert(condition) if (!(condition)) { *((int*)0) = 5; }
#endif
#endif

struct string
{
    char* Data;
    int32_t Length;
    int32_t Max;
};


