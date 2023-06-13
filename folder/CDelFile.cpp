#include "CMakeLog.h"
#include "CDelFile.h"

CDelFile::CDelFile(){}
CDelFile::~CDelFile(){}

string CDelFile::GetMD5(string filename)
{
	ifstream in(filename.c_str(), ios::binary);
	if (!in)
		return "";
	MD5 md5;
	std::streamsize length;
	char buffer[1024];
	while (!in.eof()) {
		in.read(buffer, 1024);
		length = in.gcount();
		if (length > 0)
			md5.update(buffer, length);
	}
	in.close();
	return md5.toString();
}

void CDelFile::DelRF(vector<string> &file_vec)
{
	string file1_md5,file2_md5;
	string file1_name,file2_name;
	long file1_sizel,file2_sizel; stringstream file1_sizes,file2_sizes; string file1_size, file2_size;
	string file1_CreateTime, file2_CreateTime;
	string file1_LastAccessTime, file2_LastAccessTime; 
	string file1_LastWriteTime, file2_LastWriteTime; 
	string file1_type, file2_type; 
	string file1_owner, file2_owner;
	string delrf1_msg,delrf2_msg;
	CMakeLog cmakelog("D:\\folder");
	int if_find_rf = 0;
	for(vector<string>::iterator i = file_vec.begin(); i < file_vec.end(); ++i) 
	{
		file1_name = *i;
		// 获取文件MD5码并存储至字符串 
		file1_md5 = GetMD5(file1_name);
		// 读取另一个文件MD5码并存储至字符串
		file1_sizel = cmakelog.GetFileSize(file1_name);
		file1_sizes << file1_sizel;
		file1_sizes >> file1_size;
		file1_type = cmakelog.GetFileType(file1_name);
		cmakelog.GetFileCAWTime(file1_name, file1_CreateTime, file1_LastAccessTime, file1_LastWriteTime);
		file1_owner = cmakelog.GetFileOwner(file1_name);
		delrf1_msg = "File Path & Name: "+file1_name+"\n"+"File Type: "+file1_type+"\n"+"File Size: "+file1_size+" B"+"\n" + "File MD5: " + file1_md5+"\n"
		+"File Owner: "+file1_owner+"\n"+ "Creation Time: "+file1_CreateTime+"\n"+"Last Access Time: "+file1_LastAccessTime+"\n"+"Last Modification Time: "
		+file1_LastWriteTime+"\n"+"\n";
		for(vector<string>::iterator j = i + 1; j < file_vec.end(); ++j)
		{
			file2_name = *j;
			file2_md5 = GetMD5(file2_name);
			if(file1_md5 != file2_md5)
			{
				continue;
			}		
			else	// md5码都相同执行删除文件操作并弹出file_vec中的元素 
			{	
				file2_sizel = cmakelog.GetFileSize(file2_name);
				file2_sizes << file2_sizel;
				file2_sizes >> file2_size;
				file2_type = cmakelog.GetFileType(file2_name);
				cmakelog.GetFileCAWTime(file2_name, file2_CreateTime, file2_LastAccessTime, file2_LastWriteTime);
				file2_owner = cmakelog.GetFileOwner(file2_name);
				delrf2_msg += "File Path & Name: "+file2_name+"\n"+"File Type: "+file2_type+"\n"+"File Size: "+file2_size+" B"+"\n" + "File MD5: " + file2_md5
				+"\n"+"File Owner: "+file2_owner+"\n"+ "Creation Time: "+file2_CreateTime+"\n"+"Last Access Time: "+file2_LastAccessTime+"\n"+
				"Last Modification Time: "+file2_LastWriteTime+"\n"+"\n"; 
				remove(file2_name.c_str());
				if_find_rf = 1;
				j = file_vec.erase(j);
				--j;
				string delrf_msg = "Found a group of redundant files:\n" + delrf1_msg + delrf2_msg;
				cout <<delrf_msg<<endl;
				cmakelog.WriteInLog(delrf_msg);
			}		
		}
		delrf1_msg = "\0";
		delrf2_msg = "\0";
	}
	if(!if_find_rf)
	{
 		cout << "The specified directory has no redundant files!" <<endl;
		cmakelog.WriteInLog("The specified directory has no redundant files!");
	}
}