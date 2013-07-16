/*
 * FileHelper.h
 *
 *  Created on: 2013-7-15
 *      Author: sun
 */

#ifndef FILEHELPER_H_
#define FILEHELPER_H_

namespace sdfs {

class FileHelper {
private:
	FileHelper();
	virtual ~FileHelper();
public:
	static int check_and_mkdir(const char *base_path, const char *dirname);
	static bool is_file_exist(const char *file);
	static bool is_dir(const char *path);
};

} /* namespace sdfs */
#endif /* FILEHELPER_H_ */
