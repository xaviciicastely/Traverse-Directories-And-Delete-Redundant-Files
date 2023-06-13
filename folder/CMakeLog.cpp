#include "CMakeLog.h"

string CMakeLog::GetCurrentTime()
{
	struct tm *ct;
	time_t timenow;
	time(&timenow);
	ct = localtime(&timenow);
	char temp[200];
	memset(temp,'\0',sizeof temp);
	int year, month, day, hour, minute, second;
	year = ct->tm_year + 1900; 
	month = ct->tm_mon + 1;
	day = ct->tm_mday;
	hour = ct->tm_hour;
	minute = ct->tm_min;
	second = ct->tm_sec;
	sprintf(temp,"%04d-%02d-%02d %02d:%02d:%02d ", year, month, day, hour, minute, second);
	string currentime = temp;
	return currentime; 
}

// 获取文件创建时间、文件大小、文件类型、文件路径 

void CMakeLog::GetFileCAWTime(string filepath, string &CreateTime, string &LastAccessTime, string &LastWriteTime)
{
	HANDLE hfile;
	FILETIME ftCreate, ftAccess, ftWrite;
	SYSTEMTIME stUTC, stLocal;
    char temp[200];
    memset(temp,'\0',sizeof temp);
    
	 
    /*把filepath中的单斜杠转成双斜杠以生成句柄hfile
	string::size_type pos = 0;
    while((pos = filepath.find('\\',pos)) != string::npos)
    {
    	filepath.insert(pos,"\\");
    	pos = pos + 2;
    }
    cout<< filepath<<endl;*/
    
    // 获取文件句柄 
	hfile = CreateFileA(filepath.c_str(), 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	
	// 判断句柄是否获取成功
	if(hfile == INVALID_HANDLE_VALUE)
	{
		cout<< "Get the dictionary failed!" <<endl;
		CloseHandle(hfile);
	}
	if (!GetFileTime(hfile, &ftCreate, &ftAccess, &ftWrite)) 
	{
		cout << "error!" << endl;
	}
	FileTimeToSystemTime(&ftCreate, &stUTC);  
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);// 将文件创建时间转化为系统时间
	sprintf(temp, "%04d-%02d-%02d %02d:%02d:%02d",stLocal.wYear, stLocal.wMonth, stLocal.wDay, stLocal.wHour, stLocal.wMinute, stLocal.wSecond);
	CreateTime = temp;
	
	FileTimeToSystemTime(&ftAccess, &stUTC);  
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal); // 将文件最近访问时间转化为系统时间
	sprintf(temp, "%04d-%02d-%02d %02d:%02d:%02d", stLocal.wYear, stLocal.wMonth, stLocal.wDay, stLocal.wHour, stLocal.wMinute, stLocal.wSecond);
	LastAccessTime = temp;
	
	FileTimeToSystemTime(&ftWrite, &stUTC);  
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);// 将文件最近修改时间转化为系统时间
	sprintf(temp, "%04d-%02d-%02d %02d:%02d:%02d", stLocal.wYear, stLocal.wMonth, stLocal.wDay, stLocal.wHour, stLocal.wMinute, stLocal.wSecond);
	LastWriteTime = temp;

}

long CMakeLog::GetFileSize(string filepath)
{
	FILE *fp;
	fp = fopen(filepath.c_str(),"r");
	long file_size = -1;
	long cur_offset = ftell(fp);	// 获取当前偏移位置
	if (cur_offset == -1) {
		printf("ftell failed :%s\n", strerror(errno));
		return -1;
	}
	if (fseek(fp, 0, SEEK_END) != 0) {	// 移动文件指针到文件末尾
		printf("fseek failed: %s\n", strerror(errno));
		return -1;
	}
	file_size = ftell(fp);	// 获取此时偏移值，即文件大小
	if (file_size == -1) {
		printf("ftell failed :%s\n", strerror(errno));
	}
	if (fseek(fp, cur_offset, SEEK_SET) != 0) {	// 将文件指针恢复初始位置
		printf("fseek failed: %s\n", strerror(errno));
		return -1;
	}
	fclose(fp);
	return file_size;
	
}

string CMakeLog::GetFileType(string filepath)
{
	string::size_type pos = 0;
	pos = filepath.find('.',pos); // 找到文件路径"."的位置
    if(pos != string::npos) 
    {
    	return filepath.substr(pos+1,string::npos);	// 截取"."后面的字符串，即为文件类型 
    }
    else
	{
    	return "Unknown File Type";	
    }
}

string CMakeLog::GetFileOwner(string filepath)
{
	DWORD dwRtnCode = 0;
	PSID pSidOwner = NULL;
	BOOL bRtnBool = TRUE;
	LPTSTR AcctName = NULL;
	LPTSTR DomainName = NULL;
	DWORD dwAcctName = 1, dwDomainName = 1;
	SID_NAME_USE eUse = SidTypeUnknown;
	HANDLE hFile;
	PSECURITY_DESCRIPTOR pSD = NULL;

	// 获取文件句柄 
	hFile = CreateFile((LPCWSTR)filepath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	
	// 判断句柄是否获取成功 
	if (hFile == INVALID_HANDLE_VALUE) {
	          DWORD dwErrorCode = 0;
	
	          dwErrorCode = GetLastError();
	          _tprintf(TEXT("CreateFile error = %d\n"), dwErrorCode);
	          return "Unknown Owner";
	}
	
	// 获取文件信息 
	dwRtnCode = GetSecurityInfo(hFile, SE_FILE_OBJECT, OWNER_SECURITY_INFORMATION, &pSidOwner, NULL, NULL, NULL, &pSD);
	
	// 判断获取是否成功 
	if (dwRtnCode != ERROR_SUCCESS) 
	{
		DWORD dwErrorCode = 0;
		dwErrorCode = GetLastError();
		cout<<"GetSecurityInfo error ="<<dwErrorCode<<endl;
		return "Unknown Owner";
	}
	
	// 获取用户SID
	bRtnBool = LookupAccountSid(NULL, pSidOwner, AcctName, (LPDWORD)&dwAcctName, DomainName, (LPDWORD)&dwDomainName,&eUse);
	
	// 获取用户名所占内存大小 
	AcctName = (LPTSTR)GlobalAlloc(GMEM_FIXED,dwAcctName);
	
	// 用户内存分配是否成功 
	if (AcctName == NULL) 
	{
		DWORD dwErrorCode = 0;
		dwErrorCode = GetLastError();
		_tprintf(TEXT("GlobalAlloc error = %d\n"), dwErrorCode);
		return "Unknown Owner";
	}
	// 获取域名所占内存大小 
    DomainName = (LPTSTR)GlobalAlloc(GMEM_FIXED, dwDomainName);

    if (DomainName == NULL)
	{
 		DWORD dwErrorCode = 0;
   		dwErrorCode = GetLastError();
   		cout<< "GlobalAlloc error = " << dwErrorCode <<endl;
   		return "Unknown Owner";

    }
	
    // 获取用户名！ 
    bRtnBool = LookupAccountSid(
          NULL,                   // 电脑名称 
          pSidOwner,              // 安全标识 
          AcctName,               // 用户名
          (LPDWORD)&dwAcctName,   // 用户名占内存大小 
          DomainName,             // 域名 
          (LPDWORD)&dwDomainName, // 域名占内存大小 
          &eUse);                 // SID类型 

  	// 判断是否获取成功 
    if (bRtnBool == FALSE) 
	{
          DWORD dwErrorCode = 0;
          dwErrorCode = GetLastError();
          if (dwErrorCode == ERROR_NONE_MAPPED)
          {
          	cout<< "Account owner not found for specified SID!";
          }
          else 
          {
		  	cout<< "Error in LookupAccountSid!";
          }
          CloseHandle(hFile);
		  return "Unknown Owner";

    } 
	else if (bRtnBool == TRUE) 
	{
  	CloseHandle(hFile);
 		return (LPCSTR)AcctName;
	}     
}

void CMakeLog::WriteInLog(string msg)
{
	ofstream outfile;
	string llogdir = logdir+"\\"+"log.txt"; 
	outfile.open(llogdir.c_str(),ios::app); // 文件的物理地址，文件的打开方式, 如果没有会自动创建
	if (outfile.is_open())
	{
		outfile<< GetCurrentTime()<<endl << msg <<endl;
		outfile.close();
	}
}
