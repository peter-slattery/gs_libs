/* date = May 10th 2021 2:24 pm */

#ifndef PLATFORM_WIN32_H
#define PLATFORM_WIN32_H

typedef struct win32_file_search_query_data
{
    WIN32_FIND_DATA FindFileData;
    HANDLE FileHandle;
} win32_file_search_query_data;

void
Win32_UpdateSearchQuery(file_search_query* Query, win32_file_search_query_data* Data)
{
    Query->HasFile = Data->FileHandle != INVALID_HANDLE_VALUE;
    if (Query->HasFile)
    {
        Query->IsDirectory = HasFlag(Data->FindFileData.dwFileAttributes, 
                                     FILE_ATTRIBUTE_DIRECTORY);
        Query->FileName = Data->FindFileData.cFileName;
    }
}

file_search_query 
Platform_BeginFileSearch(char* QueryString)
{
    file_search_query Result = {0};
    Result.QueryString = QueryString;
    Result.PlatformData = (void*)malloc(sizeof(win32_file_search_query_data));
    
    win32_file_search_query_data* Data = (win32_file_search_query_data*)Result.PlatformData;
    Data->FileHandle = FindFirstFile(QueryString, &Data->FindFileData);
    
    Win32_UpdateSearchQuery(&Result, Data);
    return Result;
}

bool 
Platform_FindNextFile(file_search_query* Query)
{
    win32_file_search_query_data* Data = (win32_file_search_query_data*)Query->PlatformData;
    
    bool Result = FindNextFile(Data->FileHandle, &Data->FindFileData);
    if (Result) 
    {
        Win32_UpdateSearchQuery(Query, Data);
    }
    
    return Result;
}


#endif //PLATFORM_WIN32_H
