#include "CTraverseDir.h"
#include "CStatDir.h"
#include "CDelFile.h"
#include "CMakeLog.h"

int main(void) 
{ 
	// 获取目录名 
	char buf[256]; 
	cout<<"Please input directory name:"; 
	gets(buf); 
	
	// 构造类对象 
	CStatDir statdir; 
	CDelFile delfile;
	CMakeLog makelog("D:\\folder"); 
	long file_size; 
	string CreateTime, LastAccessTime, LastWriteTime; 
	string filetype; string owner;
	
	// 设置要遍历的目录 
	if (!statdir.SetInitDir(buf)) 
	{ 
		cout<< "The specified directory doesn't exist!" <<endl; 
		return 0; 
	}
	string startmsgbuf = buf; 
	string startmsg = "The program has begun traversing the specified directory...The directory is: " + startmsgbuf; 
	makelog.WriteInLog(startmsg); 
	// 开始遍历 

	vector<string>file_vec = statdir.BeginTraverseFilenames("*.*"); 
/*	for(vector<string>::const_iterator it = file_vec.begin(); it < file_vec.end(); ++it) 
	{
		file_size = makelog.GetFileSize(*it);
		makelog.GetFileCAWTime(*it,CreateTime, LastAccessTime, LastWriteTime);
		filetype =makelog.GetFileType(*it);
		owner = makelog.GetFileOwner(*it);
		cout<<*it<<'\t'<<"File Type:"<<filetype<<'\t'<<"File Size:"<<file_size<<" B"<<'\t'<<"File Owner:"<<owner<<endl;
		// cout<<"Creation Time:"<<CreateTime<<","<<"Last Access Time:"<<LastAccessTime<<","<<"Last Modification Time:"<<LastWriteTime<<endl;
	} */	
	printf("File number: %d\n",file_vec.size()); 
	system("pause"); 
	delfile.DelRF(file_vec);
	system("pause"); 
	
/*	for(vector<string>::const_iterator itt = file_vec.begin(); itt < file_vec.end(); ++itt) 
	std::cout<<*itt<<std::endl; 
*/
	printf("Final file number: %d\n",file_vec.size());
	system("pause");

	return 0;
} 