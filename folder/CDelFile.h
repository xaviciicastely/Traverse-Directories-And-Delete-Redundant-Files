#include <stdlib.h>
#include <direct.h>
#include <string.h> 
#include <io.h> 
#include <stdio.h>  
#include <vector> 
#include "iostream" 
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

#pragma comment(lib, "advapi32.lib")
using namespace std; 

#ifndef CDelFile_h
#define CDelFile_h

class CDelFile
{
	friend class CMakeLog;
	public:
	CDelFile();
	~CDelFile(); 
	string GetMD5(string filename);
 	void DelRF(vector<string> &file_vec);
};

#endif

