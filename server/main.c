#include "ui.h"

// 1.定义一个函数指针类型  FUNC
typedef int (*FUNC)(void);


// 2.定义函数指针数组 
FUNC arrFunc[5] ={
	ExitUI,         //0 ------退出  
	RegisterUI,     //1 ------注册界面
	SearchUI,       //2 ------查询界面               
	Open_CloseUI,   //3 ------开启/关闭服务器界面                
	MainUI          //4　------主界面
};

int main(int argc,char **argv)
{
	phead=ReadFile("data.txt");
	int iNO = SW_MAIN;   // 主界面
	// 界面的切换
	while(iNO != -1)
	{
		iNO = arrFunc[iNO]();
	}

	return 0;
}
