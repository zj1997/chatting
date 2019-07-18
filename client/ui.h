#include "logic.h"
enum
{
   SW_QUIT = -1,
   SW_LOGIN,
   SW_PCHAT,
   SW_TCHAT,
   SW_ONLINE,
   SW_HISCHAT,
   SW_EXIT,
   SW_MAIN
};

int ExitUI();         //0 ------退出  
int PchatUI();		  //1 ------私聊界面
int TchatUI();	      //2 ------群聊界面 
int OnlineUI();	      //3 ------查看在线好友界面   
int HischatUI();      //4 ------历史聊天界面
int LoginUI();        //5　------登录界面
int MainUI();         //6　------主界面

