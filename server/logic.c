#include "logic.h"
#include "pthread.h"


enum{
	I_UNKNOWN = 0,
	I_CHOICE_OK, 
	I_CHOICE_ERROR,
	I_REGISTER_ID_LENGTHERROR,
	I_REGISTER_PASSWD_LENGTHERROR,
	I_REGISTER_ID_REUSE,
	I_REGISTER_OK,
	I_OPEN_CLOSE_CHOICE_ERROR,
	I_OPEN_SUCCESS,
	I_OPEN_ERROR,
	I_CLOSE_SUCCESS,
	I_CLOSE_ERROR	
};

int giInfo=I_UNKNOWN;
char *szInfo[]=
{
	"unKnown!",
	"选择成功!",   
	"选择失败!\n没有这个选项!",
	"注册失败!\nID长度不能小于3",
	"注册失败!\n密码长度不能少于6位!",
	"注册失败!\n此ID已被注册",
	"注册成功!",
	"开启或关闭失败!\n没有这个选项!",
	"开启服务器成功!",
	"开启服务器失败!服务器现在已经处于开启状态!",
	"关闭服务器成功!",
	"关闭服务器失败!服务器现在已经处于关闭状态!"
};


typedef int (*Func)(char *,int);
// 定义函数指针数组 
Func arrFunc_client[7] ={
	Login,          //0 ------登录  
	Pchat,          //1 ------私聊
	Tchat,          //2 ------群聊               
	Search_online,  //3 ------查看在线好友                
	Record,         //4 ------聊天记录
	Out_content,    //5 ------发离线消息
	Exit            //6 ------退出
};

//处理客户端登录
int Login(char *szBuf,int cfd)
{
	int ret = 0;
	char buf[10] = {'\0'};
	char *szTmp = NULL;
	char type[8] = {'\0'};
	szTmp = strtok(szBuf,":");
	strcpy(type,szTmp);
	char id[15] = {'\0'};
	char passwd[15] = {'\0'};
	szTmp = strtok(NULL,":");
	strcpy(id,szTmp);
	szTmp = strtok(NULL,":");
	strcpy(passwd,szTmp);
	char BUF[32]={'\0'};
	Check_login(id,passwd,cfd,BUF);
	send(cfd,BUF,strlen(BUF),0);
	printf("login send:%s\n",BUF);
	memset(BUF,'\0',sizeof(BUF));
	return 0;
}

//客户端私聊
int Pchat(char *szBuf,int cfd)
{
	char *szTmp = NULL;
	szTmp = strtok(szBuf,":");   
	char id[20]={'\0'};
	char buf[128]={'\0'};
	szTmp = strtok(NULL,":");
	strcpy(id,szTmp);
	szTmp = strtok(NULL,":");
	strcpy(buf,szTmp);
	char BUF[64]={'\0'};
	int recv_cfd=-1;
	Search_cfd(id,&recv_cfd);
	int ret = Deal_Pchat(id,buf,cfd,BUF);
	if(ret == 1)  	
	{
		send(recv_cfd,BUF,strlen(BUF),0);
		printf("私聊send:%s\n",BUF);
	}
	memset(BUF,'\0',sizeof(BUF));
	return 0;
}



//客户端群聊
int Tchat(char *szBuf,int cfd)
{
	char *szTmp = NULL;
	szTmp = strtok(szBuf,":");   
	char buf[128]={'\0'};
	szTmp = strtok(NULL,":");
	szTmp = strtok(NULL,":");
	strcpy(buf,szTmp);	
	char BUF[64]={'\0'};
	Save_group_content(cfd,buf,BUF);
	char Online_cfd[64];	
	Deal_Tchat(cfd,buf,Online_cfd);
	printf("online_cfd:%s\n",Online_cfd);
	szTmp = strtok(Online_cfd,"*");
	printf("szTmp:%s\n",szTmp);
	while(szTmp!=NULL)
	{
		cfd = atoi(szTmp);
		send(cfd,BUF,strlen(BUF),0);
		szTmp = strtok(NULL,"*");
	}
	printf("send:%s\n",BUF);
	memset(BUF,'\0',sizeof(BUF));
	return 0;
}


//客户端查看在线好友              
int Search_online(char *szBuf,int cfd)
{	
	char BUF[512]={'\0'};
	Online_User(BUF);
	int ret = send(cfd,BUF,strlen(BUF),0);//发送所有在线用户信息
	if(-1 == ret)
	{
		perror("send()");
		exit(-1);
	}
	memset(BUF,'\0',sizeof(BUF));
	printf("客户端查看在线好友 send:%s\n",BUF);
	return 0;
}
//客户端查看聊天记录       
int Record(char *szBuf,int cfd)
{
	char BUF[1024*64];
	char *szTmp = NULL;
	szTmp = strtok(szBuf,":");   
	char judge[5]={'\0'};
	char time[20]={'\0'};
	szTmp = strtok(NULL,":");
	strcpy(judge,szTmp);
	szTmp = strtok(NULL,":");
	strcpy(time,szTmp);
	if(strcmp(judge,"2")==0)
	{
		Tchat_Record(cfd,time,BUF);
	}
	else if(strcmp(judge,"1")==0)
	{
		Pchat_Record(time,cfd,BUF);
	}
	send(cfd,BUF,strlen(BUF),0);
	printf("聊天记录send:%s\n",BUF);
	memset(BUF,'\0',sizeof(BUF));
	return 0;
}

//发送离线消息给客户端
int Out_content(char *szBuf,int cfd)
{
	char *szTmp = NULL;
	szTmp = strtok(szBuf,":");   
	char id[20]={'\0'};
	szTmp = strtok(NULL,":");
	strcpy(id,szTmp);
	char BUF[1024*64];
	Outline_Content(cfd,id,BUF);
	send(cfd,BUF,sizeof(BUF),0);
	printf("send %s\n",BUF);
	memset(BUF,'\0',sizeof(BUF));
	return 0;
} 
//客户端退出
int  Exit(char *szBuf,int cfd)
{		
	return 0;
}


int arrClientFd[MAXFD];
int iClientConn;
int iMaxFd;

//处理客户端
void HandleClient(int cfd,int i,fd_set *pfds)
{

	if(FD_ISSET(cfd,pfds))
	{
		int ret = 0;
		char szBuf[128] = {'\0'};
		char *szTmp = NULL;
		char type[8] = {'\0'};
		char buf[MAXLENGTH]={0};
		int len = -1;
		len = recv(cfd,buf,sizeof(buf),0);
		printf("recv:%s\n",buf);
		strcpy(szBuf,buf);
		if( len <= 0)  // client closed
		{		
			// 当前Client在线数减1
			iClientConn -= 1;
			FD_CLR(cfd,pfds);
			Change_stat(cfd);
			// 关闭cfd
			sleep(5);
			close(cfd); 	
			arrClientFd[i] = -1;
			return ;		
		}
		else
		{
			szTmp = strtok(buf,":");
			strcpy(type,szTmp);
			arrFunc_client[atoi(type)](szBuf,cfd);//调用处理函数
		}		
	}//if	
	return ;
}
/* 000 init global*/
void InitGlobal()
{
	int i;
	for(i=0; i<MAXFD; ++i)
		arrClientFd[i] = -1;

	iClientConn = 0;

	int iMaxFd = -1;

	return ;
}

//初始化socket
int Socket_init()
{
	int sfd = socket(AF_INET,SOCK_STREAM,0);
	if( -1 == sfd)
	{
		perror("socket");
		exit(-1);
	}
	int optval = 1;
	socklen_t optlen = sizeof(int);
	int ret = setsockopt(sfd,SOL_SOCKET,
			SO_REUSEADDR,
			(void *)&optval,optlen);
	if( -1 == ret)
	{
		perror("setsockopt");
		exit(-1);
	}
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = INADDR_ANY;
	saddr.sin_port = htons(PORT);
	bzero(saddr.sin_zero,8);
	ret = bind(sfd,(struct sockaddr *)&saddr,
			sizeof(struct sockaddr));
	if( -1 == ret)
	{
		perror("bind");
		exit(-1);
	}
	ret = listen(sfd,20);
	if( -1 == ret)
	{
		perror("listen");
		exit(-1);
	}
	return sfd;
}

/*002 init fds*/
void init_fds(int sfd,fd_set *pfds)
{
	/*1. fds 清零*/
	FD_ZERO(pfds);

	/*2. 将sfd添加到fds中*/
	FD_SET(sfd,pfds);	
	iMaxFd = sfd;	

	/*3. 将有效的cfd添加到fds中*/
	int i;
	for(i=0; i<MAXFD; ++i)
	{
		if( -1 == arrClientFd[i])
			continue;

		int cfd = arrClientFd[i];
		FD_SET(cfd,pfds);
		if(cfd > iMaxFd)
			iMaxFd = cfd;
	}

	return ;
}


/*003.关闭所有的文件描述符*/
void close_fd(int sfd)
{
	/*关闭所有的 Client fd*/
	int i;
	for(i=0; i<MAXFD; ++i)
		close(arrClientFd[i]);

	/*关闭Server fd*/
	close(sfd);
	return ;
}

/*判断sfd是否可读 - 客户是否连接*/
void check_sfd(int sfd,fd_set *pfds)
{
	if( FD_ISSET(sfd,pfds))
	{
		struct sockaddr_in caddr;
		int len = sizeof(struct sockaddr);

		/*1.接受客户端的连接*/
		int cfd = accept(sfd,(struct sockaddr *)&caddr,&len);
		if( -1 == cfd)
		{
			perror("accept");
			return ;
		}

		/*2.当前的客户连接数*/
		iClientConn += 1;

		/*3.保存Client fd*/
		int i;
		for(i=0; i<MAXFD; ++i)
		{
			if(-1 == arrClientFd[i])
			{
				arrClientFd[i] = cfd;
				break;
			}
		} 

		/*4.打印客户端的地址*/
		printf("client[%s:%d] is connected!\n",inet_ntoa(caddr.sin_addr),ntohs(caddr.sin_port));
	}
	return ;
}

//检查cfd是否可读
void check_cfd(fd_set *pfds)
{
	int i;
	for(i=0; i< MAXFD;++i)
	{
		int cfd = arrClientFd[i];
		if( cfd > 0)
			HandleClient(cfd,i,pfds);
	}

	return ;
}

//线程处理函数
void *Threadhandler(void *arg)
{
	/*0.init global var*/
	InitGlobal();

	/*1.init socket*/
	int sfd = Socket_init();
	if( -1 == sfd)
		exit(-1);

	/*2.circle handle*/
	fd_set fds;
	int ret = -1;

	while(1)
	{
		init_fds(sfd,&fds);
		ret = select(iMaxFd+1,&fds,NULL,NULL,NULL);
		if( ret < 0)
		{
			perror("select");
			exit(-1);
		}
		/*判断cfd是否可读 - 客户是否发送了信息*/
		check_cfd(&fds); 
		/*判断sfd是否可读 - 客户是否连接*/
		check_sfd(sfd,&fds);
	}
	/*3.关闭所有的文件描述符*/
	close_fd(sfd);
}

//处理主界面的选择
int  deal_Main(int choice)
{
	if(choice<0||choice>3)
	{
		giInfo = I_CHOICE_ERROR;
		return 0;  
	}
	else 
	{
		giInfo = I_CHOICE_OK;
		return 1;  
	}
}


//处理注册
int deal_Register(char *id,char *szName,char *szPass)
{
	if(strlen(id)<3)
	{
		giInfo = I_REGISTER_ID_LENGTHERROR;
		return 0;  
	}
	if(strlen(szPass)<6)
	{
		giInfo = I_REGISTER_PASSWD_LENGTHERROR;
		return 0;  
	}

	//判断ID是否已经被使用过
	if(0==Judge_id(id))
	{
		giInfo = I_REGISTER_ID_REUSE;
		return 0;  
	}
	else
	{
		phead = User_register(id,szName,szPass);
		giInfo = I_REGISTER_OK;
		return 1;  
	}
}

//处理开关服务器
int deal_Open_Close(int i)
{
	if(i<1||i>2)
	{
		giInfo =I_OPEN_CLOSE_CHOICE_ERROR;
		return 0;
	}
	else if(i==1)
	{
		if(Server_stat==0)
		{
			giInfo =I_OPEN_SUCCESS;
			Server_stat=1;
			Set_stat();
			pthread_create(&thid,NULL,Threadhandler,NULL);
		}
		else
		{
			giInfo =I_OPEN_ERROR;
		}
		return 0;
	}
	if(Server_stat==1)
	{
		giInfo =I_CLOSE_SUCCESS;
		Server_stat=0;
		pthread_cancel(thid);
	}
	else
	{
		giInfo =I_CLOSE_ERROR;
	}
	return 0;
}

//显示执行结果信息
int InfoUI()
{
	printf("%s\n",szInfo[giInfo]);
	return 0;
}

