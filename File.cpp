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
	if ((f=fopen(name,"r"))==NULL)//������Ϊ�˶��ļ������ڵȴ�����
	{  
		return false;
		//printf("file not exist!\n"); 
		//exit(0); 
	}//���ȷ��û�������ֻ��дf=fopen(name,"r") r��ʾֻ����������rb�ȱ�ʾ���ƶ�д 
	while (!feof(f))//feof���������ж��ļ���д�Ƿ����������ǽ�����NULL 
	{  
		fscanf(f,"%c",&t);//��ʽ�����룬Ҳ������%d���������ȣ�Ҳ�����������ĺ��������Բο������ֲ� 
		//printf("%c.",t); 
	}
	//puts("\n"); 
	fclose(f);//�ر��ļ�
	return true;
}

bool CFile::Write(int num)
{
	FILE *f;	//�ļ����� 

	if ((f=fopen(name,"w"))==NULL)//w��ʾд�ļ�
	{ 
		return false;
		//printf("write err!\n");
		////������  
		//exit(0);
	} 

	fprintf(f,"%d\n",num);

	fclose(f);

	return true;
}