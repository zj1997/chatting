#include "data.h"

#define STDIN 0 

static fd_set rfds; 
static int MAXFD;
static int sfd;
static char Login_id[32];
static char stat1[8];


void select_fds(int sfd,fd_set *prfds);     //select
void *select_recv();						//接收线程
int deal_login(char *szId,char *szPass);//登录处理函数
int deal_Main(int INO);    //处理主界面
void deal_Online();         //打印当前在线用户
void deal_Hischat(int inum,char *date);      //查看聊天信息
void deal_Pchat(char *ID);  //1v1聊天
void deal_Tchat();			//群聊
void deal_exit();           //处理退出 
void INFOUI(int giInfo);//打印错误信息


int recv_online(char *buf);        //处理接收到的当前在线用户信息
int recv_hischat(char *szBuf);     //处理接收到的聊天记录
int recv_pchat(char *szBuf);	   //处理私聊
int recv_tchat(char *szBuf);	   //处理群聊
int recv_outchat(char *szBuf);	   //处理离线消息提醒
