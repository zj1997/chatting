#include "ui.h"




typedef int (*FUNC)(void);



// 2.定义函数指针数组 
FUNC arrFunc[7] ={
  LoginUI,         //0 ------退出  
  PchatUI,		   //1 ------私聊界面
  TchatUI,		   //2 ------群聊界面 
  OnlineUI, 	   //3 ------查看在线好友界面  
  HischatUI,	   //4 ------查看聊天记录界面
  ExitUI,          //5　------登录界面
  MainUI           //6　------主界面
};



int main(int argc,char **argv)
{
        init_client(); 
        int iNO = SW_LOGIN;   // 登录界面
        iNO = arrFunc[iNO]();
		pthread_t pid = 0;
        pthread_create(&pid,NULL,select_recv,NULL);         	
    	while(iNO != -1)
    	{
    		
        	iNO = arrFunc[iNO]();
   	    }
     	close(sfd);
     	pthread_cancel(pid);
    	return 0;
}
