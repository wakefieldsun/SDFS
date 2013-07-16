/*
 * FileHelper.cpp
 *
 *  Created on: 2013-7-15
 *      Author: sun
 */

#include "FileHelper.h"
#include "common_define.h"
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

namespace sdfs {

FileHelper::FileHelper() {
	// TODO Auto-generated constructor stub

}

FileHelper::~FileHelper() {
	// TODO Auto-generated destructor stub
}

bool FileHelper::is_file_exist(const char *filename)
{
	return access(filename, 0) == 0;
}

int FileHelper::check_and_mkdir(const char *base_path, const char *dirname)
{
		char data_path[MAX_PATH_SIZE];
		snprintf(data_path, sizeof(data_path), "%s/%s", base_path,dirname);
		if (!is_file_exist(data_path))
		{
			if (mkdir(data_path, 0755) != 0)
			{
				fprintf(stderr, "mkdir \"%s\" fail, " \
					"errno: %d, error info: %s", \
					data_path, errno, STRERROR(errno));
				return errno != 0 ? errno : EPERM;
			}
		}

		return 0;
}

bool FileHelper::is_dir(const char *path)
{
	struct stat buf;
	if(stat(path, &buf) != 0)
		return false;
	return S_ISDIR(buf.st_mode);
}

} /* namespace sdfs */
