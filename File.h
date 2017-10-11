#ifndef C_FILE_H
#define C_FILE_H
#include "head.h"

class CFile
{
public:
	CFile();
	~CFile();

	void Init();
	bool Read(int& num);
	bool Write(int num);
private:
	FILE* m_file; 
	char* name;
};

#endif


