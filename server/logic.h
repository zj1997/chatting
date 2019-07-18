#include "data.h"

#define PORT    	8888
#define BACKLOG 	10
#define MAXFD   	20
#define MAXLENGTH 	256


static pthread_t thid=0; //线程id
static int Server_stat=0;//服务器的状态(0:关闭,1:开启)
//处理通信
int  Login(char *szBuf,int cfd);           //0 ------登录  
int  Pchat(char *szBuf,int cfd);		   //1 ------私聊
int  Tchat(char *szBuf,int cfd);		   //2 ------群聊               
int Search_online(char *szBuf,int cfd);    //3 ------查看在线好友                
int Record(char *szBuf,int cfd);           //4　------聊天记录
int Out_content(char *szBuf,int cfd);      //5 ------发送离线消息
int   Exit(char *szBuf,int cfd);           //6 ------退出
int  InfoUI(); //打印提示信息

void deal_Open();//处理开启服务器
void deal_Close();//处理关闭服务器
int  deal_Register(char *id,char *szName,char *szPass);//处理注册
int  deal_Main(int choice);//处理主函数



