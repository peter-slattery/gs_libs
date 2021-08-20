#ifndef PLATFORM_OSX_H
#define PLATFORM_OSX_H

// struct dirent {
//     ino_t          d_ino;       /* inode number */
//     off_t          d_off;       /* offset to the next dirent */
//     unsigned short d_reclen;    /* length of this record */
//     unsigned char  d_type;      /* type of file; not supported
//                                    by all file system types */
//     char           d_name[256]; /* filename */
// };

typedef struct {
	DIR* Directory;
	struct dirent* DirEntAt;
} osx_file_search_query_data;

void
ReadDirPrintErr(int err)
{
	switch (err) {
		case EBADF:
			printf("EBADF: Invalid file descriptor\n");
			break;
		case EACCES:
			printf("EACCES: Search permission denied\n");
			break;
		case ELOOP:
			printf("ELOOP: A loop in symbolic links exists\n");
			break;
		case ENAMETOOLONG:
			printf("ENAMETOOLONG: Pathname is longer than NAME_MAX\n");
			break;
		case ENOENT:
			printf("ENOENT: No existing directory at path\n");
			break;
		case ENOTDIR:
			printf("ENOTDIR: path or fd is not a directory\n");
			break;
		case EMFILE:
			printf("EMFILE: all available file descriptors are open already\n");
			break;
		case ENFILE:
			printf("ENFILE: too many files are open on the system\n");
			break;
	}
}

file_search_query
Platform_BeginFileSearch(char* QueryString)
{
	file_search_query Result = {0};
	Result.QueryString = QueryString;
	Result.PlatformData = (void*)malloc(sizeof(osx_file_search_query_data));

	osx_file_search_query_data* Data = (osx_file_search_query_data*)Result.PlatformData;
	Data->Directory = opendir(QueryString);
	if (Data->Directory == NULL)
	{
		printf("Error: opendir returned null. Errno: %d\n", errno);
		printf("Query String: %s\n", QueryString);
		ReadDirPrintErr(errno);
		return (file_search_query){0};
	}

	Platform_FindNextFile(&Result);
	return Result;
}

bool
Platform_FindNextFile(file_search_query* Query)
{
	bool Result = false;

	osx_file_search_query_data* Data = (osx_file_search_query_data*)Query->PlatformData;
	struct dirent* DirEnt = readdir(Data->Directory);
	if (DirEnt != NULL)
	{
		Data->DirEntAt = DirEnt;
		Result = true;

		Query->HasFile = true;
		Query->IsDirectory = (DirEnt->d_type & DT_DIR) != 0;
		Query->FileName = DirEnt->d_name;

		printf("Found Dir %s - Is Dir: %s\n", Query->FileName, Query->IsDirectory ? "True" : "False");
	}
	else
	{
		closedir(Data->Directory);
	}

	return Result;
}

#endif // PLATFORM_OSX_H