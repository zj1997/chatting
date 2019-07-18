#include "ui.h"

//退出界面  
int   ExitUI() 
{
	// 1.绘制界面
	system("clear");
	printf("\n\n\n\n                                  Good Bye,QQ!                             \n\n\n\n");
	sleep(3);
	system("clear");
	return SW_QUIT;
}        


//注册界面
int   RegisterUI()
{   
	//1.绘制界面
	system("clear");
	printf("***********************************  Regiser  ************************************\n\n\n\n");
	char id[15];          
	char szName[64];      
	char szPass[64]; 
	//2.获取用户输入
	printf("                请输入QQ号：");
	scanf("%s",id);
	printf("                请输入QQ昵称：");
	scanf("%s",szName);
	printf("                请输入QQ密码：");
	scanf("%s",szPass);
	//3.数据向底层传递
	int ret = deal_Register(id,szName,szPass);
	InfoUI();
	sleep(2);
	return ret?SW_MAIN:SW_REGISTER;
} 

//查询界面      
int SearchUI() 
{
	char c[10]={'\0'};
	//1.绘制界面
	system("clear");
	printf("******************************The QQer List*************************\n\n\n\n");
	char id[15];
	//3.数据向底层传递
	Search_id();
	printf("按任意键+回车继续\n");
	system("stty -echo");
	scanf("%s",c);
	system("stty echo");
	return SW_MAIN;
}

//开启服务器界面              
int Open_CloseUI()
{
	system("clear");
	printf("**************************************************************************\n");
	printf("************************      Server control　    ************************\n");
	printf("*                                                                        *\n");
	printf("*              (1)open server                (2)close server             *\n");
	printf("*                                                                        *\n");
	printf("**************************************************************************\n");
	int i;
	printf("\t\t please choose:");
	scanf("%d",&i);
	deal_Open_Close(i);
	InfoUI();
	sleep(2);
	return SW_MAIN; 
}                                              
//主界面
int  MainUI()
{
	int iChoice = 0;        // i : int 
	// 1.绘制界面
	system("clear");
	printf("                                                         作者：赵杰（AHUT)\n");
	printf("\n");
	printf("************************欢迎进入QQ系统后台管理系统******************\n\n\n\n");
	printf("                                                                          \n");
	printf("               1.QQ register                  2.QQer List                 \n");
	printf("                                                                          \n");
	printf("               3.Server Control               4.Quit System               \n");
	printf("                                                                      \n\n\n");    
	printf("                        please choose the part: ");
	scanf("%d",&iChoice);   // 2.获取用户输入
	// 3.传递数据
	int ret = deal_Main(iChoice);
	InfoUI();
	sleep(2);
	return ret?iChoice:SW_MAIN;
}  



