#include "logic.h"

enum
{
	SW_QUIT = -1,
	SW_EXIT,
	SW_REGISTER,
	SW_SEARCH,
	SW_OPEN_CLOSE,
	SW_MAIN
};

int   ExitUI();         //退出界面  
int   RegisterUI();     //注册界面
int   SearchUI();       //查询界面               
int   Open_CloseUI();   //开启或关闭服务器界面                
int   MainUI();         //主界面
