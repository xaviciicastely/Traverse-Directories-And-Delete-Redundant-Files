#include "CTraverseDir.h"

CTraverseDir::CTraverseDir() 
{ 
	// 用当前目录初始化m_szInitDir 
	getcwd(m_szInitDir,_MAX_PATH); 
	
	// 如果目录的最后一个字母不是'\',则在最后加上一个'\' 
	int len=strlen(m_szInitDir); 
	if (m_szInitDir[len-1] != '\\') 
	strcat(m_szInitDir,"\\"); 
} 
 
bool CTraverseDir::SetInitDir(const char *dir) 
{ 
	// 先把dir转换为绝对路径 
	if (_fullpath(m_szInitDir,dir,_MAX_PATH) == NULL) 
	return false; 
	
	// 判断目录是否存在 
	if (_chdir(m_szInitDir) != 0) 
	return false; 
	
	// 如果目录的最后一个字母不是'\',则在最后加上一个'\' 
	int len=strlen(m_szInitDir); 
	if (m_szInitDir[len-1] != '\\') 
	strcat(m_szInitDir,"\\"); 
	
	return true; 
} 
 
vector<string> CTraverseDir::BeginTraverseFilenames(const char *filespec) 
{ 
	ProcessDir(m_szInitDir,NULL); 
	return GetDirFilenames(m_szInitDir,filespec); 
} 
 
bool CTraverseDir::BeginTraverse(const char *filespec) 
{ 
	ProcessDir(m_szInitDir,NULL); 
	return TraverseDir(m_szInitDir,filespec); 
} 
 
bool CTraverseDir::TraverseDir(const char *dir,const char *filespec) 
{ 
	_chdir(dir); 
	
	// 首先查找dir中符合要求的文件 
	long hFile; 
	_finddata_t fileinfo; 
	if ((hFile=_findfirst(filespec,&fileinfo)) != -1) 
	{ 
	do 
	{ 
	  // 检查是不是目录 
	  // 如果不是,则进行处理 
	  if (!(fileinfo.attrib & _A_SUBDIR)) 
	  { 
	    char filename[_MAX_PATH]; 
	    strcpy(filename,dir); 
	    strcat(filename,fileinfo.name); 
	    cout << filename << endl; 
	    if (!ProcessFile(filename)) 
	      return false; 
	  } 
	} while (_findnext(hFile,&fileinfo) == 0); 
	_findclose(hFile); 
	} 
	// 查找dir中的子目录 
	// 因为在处理dir中的文件时，派生类的ProcessFile有可能改变了 
	// 当前目录，因此还要重新设置当前目录为dir。 
	// 执行过_findfirst后，可能系统记录下了相关信息，因此改变目录 
	// 对_findnext没有影响。 
	_chdir(dir); 
	if ((hFile=_findfirst("*.*",&fileinfo)) != -1) 
	{ 
	do 
	{ 
	  // 检查是不是目录 
	  // 如果是,再检查是不是 . 或 ..  
	  // 如果不是,进行迭代 
	  if ((fileinfo.attrib & _A_SUBDIR)) 
	  { 
	    if (strcmp(fileinfo.name,".") != 0 && strcmp 
	      (fileinfo.name,"..") != 0) 
	    { 
	      char subdir[_MAX_PATH]; 
	      strcpy(subdir,dir); 
	      strcat(subdir,fileinfo.name); 
	      strcat(subdir,"\\"); 
	      ProcessDir(subdir,dir); 
	      if (!TraverseDir(subdir,filespec)) 
	        return false; 
	    } 
	  } 
	} while (_findnext(hFile,&fileinfo) == 0); 
	_findclose(hFile); 
	} 
	return true; 
} 
 
vector<string> CTraverseDir::GetDirFilenames(const char *dir,const char *filespec) 
{ 
	_chdir(dir); 
	vector<string>filename_vector; 
	filename_vector.clear(); 
	
	// 首先查找dir中符合要求的文件 
	long hFile; 
	_finddata_t fileinfo; 
	if ((hFile=_findfirst(filespec,&fileinfo)) != -1) 
	{ 
	do 
	{ 
	  // 检查是不是目录 
	  // 如果不是,则进行处理 
	  if (!(fileinfo.attrib & _A_SUBDIR)) 
	  { 
	    char filename[_MAX_PATH]; 
	    strcpy(filename,dir); 
	    strcat(filename,fileinfo.name); 
	    filename_vector.push_back(filename); 
	  } 
	} while (_findnext(hFile,&fileinfo) == 0); 
	_findclose(hFile); 
	} 
	// 查找dir中的子目录 
	// 因为在处理dir中的文件时，派生类的ProcessFile有可能改变了 
	// 当前目录，因此还要重新设置当前目录为dir。 
	// 执行过_findfirst后，可能系统记录下了相关信息，因此改变目录 
	// 对_findnext没有影响。  
	_chdir(dir); 
	if ((hFile=_findfirst("*.*",&fileinfo)) != -1) 
	{ 
	do 
	{ 
	  // 检查是不是目录 
	  // 如果是,再检查是不是 . 或 ..  
	  // 如果不是,进行迭代 
		if ((fileinfo.attrib & _A_SUBDIR)) 
		{ 
		    if (strcmp(fileinfo.name,".") != 0 && strcmp(fileinfo.name,"..") != 0) 
		    { 
		      char subdir[_MAX_PATH]; 
		      strcpy(subdir,dir); 
		      strcat(subdir,fileinfo.name); 
		      strcat(subdir,"\\"); 
		      ProcessDir(subdir,dir); 
		      vector<string>tmp= GetDirFilenames(subdir,filespec); 
		      for (vector<string>::iterator it=tmp.begin();it<tmp.end();it++) 
		      { 
		        filename_vector.push_back(*it); 
		      } 
	    } 
	  } 
	} 
	while (_findnext(hFile,&fileinfo) == 0); 
	_findclose(hFile); 
	} 
	return filename_vector; 
} 

CTraverseDir::~CTraverseDir(){} 
 
bool CTraverseDir::ProcessFile(const char *filename) 
{ 
	return true; 
} 

void CTraverseDir::ProcessDir(const char  *currentdir,const char *parentdir){} 
 
