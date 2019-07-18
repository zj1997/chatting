#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>          
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>	
#include <time.h>
#include <pthread.h>
typedef struct _STUSER
{
	char notice[128];     // 存放提示消息
	int cfd;              // 用户的cfd(用于通信)
	int  stat;            // 用户的状态(在线或离线)
	char id[15];          // 用户ID
	char szName[64];      // 用户昵称
	char szPass[64];      // 密   码
}STUSER;

typedef struct _STNODE
{
	STUSER   stUser;           // 数据域
	struct _STNODE *pNext;     // 指针域
}STNODE,*pST;
STNODE *phead;

void    WriteFile(STNODE *p);       // 写文件
STNODE *ReadFile(char *pathname);   // 读文件
STNODE *User_register(char *id,char *szName,char *szPass);//注册用户信息
void Check_login(char *id,char *szPass,int cfd,char *BUF);//验证登录

void    Set_stat();         //将所有用户的状态位置０,cfd置－１
void Change_stat(int cfd);  //用户退出,改变用户状态

STNODE *Search_user(int cfd);     //通过cfd找到用户
void    Search_cfd(char *id,int *recv_cfd); //私聊时根据ID找cfd

int  Judge_id(char *id);     //判断ID是否已经被注册
void Search_id();            //查询用户信息
void Online_User(char *BUF); //将在线好友整理成一个字符串


void Pchat_Record(char *Time,int cfd,char *BUF);  //私聊历史消息整理到BUF里
void Tchat_Record(int cfd,char *Time,char *BUF);  //群聊历史消息整理到BUF里
void Outline_Content(int cfd,char *id,char *BUF); //离线缓存消息整理到BUF里

void Deal_Tchat(int cfd,char *buf,char *Online_cfd);  //处理群聊消息
int  Deal_Pchat(char *id,char *buf,int cfd,char *BUF);//处理私聊消息

void Save_personal_content(char *name,STNODE *p,STNODE *USER,char *buf);//保存私聊聊天记录
void Save_group_content(int cfd,char *buf,char *BUF);                   //保存群聊的聊天记录
void Save_Outline_Pchat_Content(STNODE *USER,STNODE *p,char *buf);      //保存不在线的私聊信息
void Save_Outline_Tchat_Content(STNODE *USER,STNODE *p,char *buf);      //保存不在线的群聊消息

