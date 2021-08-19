/* date = May 10th 2021 2:24 pm */

#ifndef PLATFORM_H
#define PLATFORM_H

#define CArrayLength(arr) sizeof(arr) / sizeof(arr[0])
#define HasFlag(data, flag) (((data) & (flag)) != 0)

typedef struct file_search_query
{
    char* QueryString;
    
    bool HasFile;
    char* FileName;
    bool IsDirectory;
    
    void* PlatformData;
} file_search_query;

file_search_query Platform_BeginFileSearch(char* QueryString);
bool Platform_FindNextFile(file_search_query* Query);

#endif //PLATFORM_H
