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


#ifndef CTraverseDir_h
#define CTraverseDir_h

class CTraverseDir 
{ 
protected: 
	// 存放初始目录的绝对路径，以'\'结尾 
	char m_szInitDir[_MAX_PATH]; 

public: 
	// 缺省构造器 
	CTraverseDir(); 
	~CTraverseDir();
	// 设置初始目录为dir，如果返回false，表示目录不可用 
	bool SetInitDir(const char *dir); 
	
	// 开始遍历初始目录及其子目录下由filespec指定类型的文件 
	// filespec可以使用通配符 * ?，不能包含路径。 
	// 如果返回false，表示遍历过程被用户中止 
	bool BeginTraverse(const char *filespec); 
	vector<string> BeginTraverseFilenames(const char *filespec); 

protected: 
	// 遍历目录dir下由filespec指定的文件 
	// 对于子目录,采用迭代的方法 
	// 如果返回false,表示中止遍历文件 
	bool TraverseDir(const char *dir,const char *filespec); 
	vector<string> GetDirFilenames(const char *dir,const char *filespec); 
	// 函数TraverseDir每找到一个文件,就调用ProcessFile 
  // 并把文件名作为参数传递过去 
	// 如果返回false,表示中止遍历文件 
	virtual bool ProcessFile(const char *filename); 
	
	// 函数TraverseDir每进入一个目录,就调用ProcessDir 
	// 并把正在处理的目录名及上一级目录名作为参数传递过去 
	// 如果正在处理的是初始目录,则parentdir=NULL 
	virtual void ProcessDir(const char *currentdir,const char *parentdir); 
};

#endif