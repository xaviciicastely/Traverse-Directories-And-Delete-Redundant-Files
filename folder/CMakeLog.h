#include <stdlib.h>
#include <direct.h>
#include <string.h> 
#include <io.h> 
#include <stdio.h>  
#include <vector> 
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <time.h>
#include <unistd.h>
#include <windows.h>
#include <algorithm>
#include <tchar.h>
#include <accctrl.h>
#include <aclapi.h>
#include "md5.h"
#include "md5.cpp"
using namespace std;

#ifndef CMakeLog_h
#define CMakeLog_h
// 日志计入类
class CMakeLog
{
	protected:
	string logdir;
	
	public:
	CMakeLog();
	CMakeLog(string logdir);
	CMakeLog(CMakeLog &makelog);
	~CMakeLog();
	string GetCurrentDir();	
	string GetCurrentTime();
	long GetFileSize(string filepath);
	string GetFileType(string filepath);
	void GetFileCAWTime(string filepath, string &CreateTime, string &LastAccessTime, string &LastWriteTime);
	string GetFileOwner(string filepath);
	void WriteInLog(string msg);
};
CMakeLog::CMakeLog(){}

CMakeLog::CMakeLog(string cd)
{
	logdir = cd;
}

CMakeLog::CMakeLog(CMakeLog &makelog)
{
	this->logdir = makelog.logdir;
}

CMakeLog::~CMakeLog(){} 

string CMakeLog::GetCurrentDir()
{
	return logdir;
}

#endif