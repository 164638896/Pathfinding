#include "File.h"

CFile::CFile()
{
	name = "Lai.Wen";
}

CFile::~CFile()
{
	//fclose(m_file);
}

void CFile::Init()
{
	//m_file = fopen(name,"w");
}

bool CFile::Read(int& num)
{
	FILE *f; 
	char t;
	if ((f=fopen(name,"r"))==NULL)//这里是为了对文件不存在等错误处理
	{  
		return false;
		//printf("file not exist!\n"); 
		//exit(0); 
	}//如果确信没问题可以只用写f=fopen(name,"r") r表示只读，还可以rb等表示进制读写 
	while (!feof(f))//feof函数用于判断文件读写是否结束，如果是将返回NULL 
	{  
		fscanf(f,"%c",&t);//格式化读入，也可以用%d读入整数等，也可以用其他的函数，可以参考函数手册 
		//printf("%c.",t); 
	}
	//puts("\n"); 
	fclose(f);//关闭文件
	return true;
}

bool CFile::Write(int num)
{
	FILE *f;	//文件定义 

	if ((f=fopen(name,"w"))==NULL)//w表示写文件
	{ 
		return false;
		//printf("write err!\n");
		////出错处理  
		//exit(0);
	} 

	fprintf(f,"%d\n",num);

	fclose(f);

	return true;
}