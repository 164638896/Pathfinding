#pragma once
#include <windows.h>
#include <map>
#include <vector>
#include <set>
#include <list>
#include <assert.h>
#include"time.h"
#include "mmsystem.h"//导入声音头文件
#pragma comment(lib,"winmm.lib")//导入声音头文件库

using namespace std;

#define RGBA(r,g,b,a)          (r | (g << 8) | (b << 16) | (a << 24))

#define CLEAR_COLOR		RGB(255,255,255) 

#define  SAFE_DELETE(p) if(p) { delete p; p = NULL;}

#define WINDOW_SIZE_X  600
#define WINDOW_SIZE_Y  550
#define GIRD_SIZE    50
#define PIC_SIZE     41
#define RAND_GRID    3

#define GRID_HEIGHT   10
#define GRID_WIDTH   10
#define GRID_WIN   5

enum E_COLOR
{
	E_COLOR_NULL  = -1,
	E_BLACK		  = 0,
	E_RED		  = 1,
	E_GREEN		  = 2,
	E_BLUE		  = 3,
	E_YELLOW	  = 4,
	E_PURPLE	  = 5,
	E_CYAN		  = 6,
	E_GRAY		  = 7,
	E_ORANGE	  = 8,
	E_COlOR_MAX
};
