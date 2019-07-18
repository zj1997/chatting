#include "logic.h"

typedef int (*RECV)(char *buf);

RECV arrRecv[5] ={
  recv_pchat,
  recv_tchat,
  recv_online,
  recv_hischat,
  recv_outchat
};


enum{
	I_UNKNOWN = 0,
	I_LOGIN_OK,
	I_LOGIN_ERROR,
	I_MAIN_NUM_ERROR,
	I_MAIN_NUM_OK,
	I_PCHAT_ID_OK,
	I_PCHAT_ID_ERROR,
	I_TCHAT_ERROR,
	I_TCHAT_OK
};

int giInfo = I_UNKNOWN;

char *szInfo[]=
{
	"unKnown!",
	"登录成功!",
	"登录失败!\n密码错误或账号不存在",
	"选择失败!\n选择数字不合法",
	"选择成功!",
	"用户已连接!",
	"连接失败!\n当前用户不存在",
	"连接失败!\n进入群聊失败",
	"进入群聊天室!"
};


//与服务器建立
int init_client()
{

	sfd=socket(AF_INET,SOCK_STREAM,0);
	if(-1==sfd)
	{
		perror("socket");
		exit(-1);
	}
	// int connect(int sockfd, const struct sockaddr *addr,
	//socklen_t addrlen);
	
	struct sockaddr_in saddr;
	saddr.sin_family=AF_INET;
	saddr.sin_addr.s_addr=inet_addr("192.168.195.133");
	saddr.sin_port=htons(8888);
	bzero(saddr.sin_zero,8);
	
	int ret=connect(sfd,(struct sockaddr*)&saddr,sizeof(struct sockaddr));
	if(-1==ret)
	{
		perror("connect");
		exit(-1);

	}
	printf("Success！\n");
	return sfd;
}




void select_fds(int sfd,fd_set *prfds)
{
	FD_ZERO(prfds);     // 将集合清零
	// 1.添加 sfd
	FD_SET(sfd,&rfds);  // 添加sfd到 rfds集合中 
	MAXFD = sfd;

	int ret = select(MAXFD+1,&rfds,NULL,NULL,NULL);
	if( ret == -1)
	{
		perror("select");
		exit(-1);
	}
	return ;
}


void *select_recv()
{
	int ret;
	char szBuf[1024*128] = {'\0'};
	char buf[1024*128] = {'\0'};
	char *szTmp;
	while(1)
	{
		select_fds(sfd,&rfds);
		if(FD_ISSET(sfd,&rfds))
		{
			int ret = recv(sfd,szBuf,sizeof(szBuf),0);
			if(-1 == ret)
			{
				perror("recv()");
				exit(-1);
			}
			strcpy(buf,szBuf);
			if(strlen(buf) != 0)
			{
				szTmp = strtok(szBuf,"*");
				arrRecv[atoi(szTmp)-1](buf);
				memset(szBuf,'\0',sizeof(szBuf));
				memset(buf,'\0',sizeof(buf));
			}
		}
	}

	return ;
}
//处理登录
int deal_login(char *szId,char *szPass)
{
	strcpy(Login_id,szId);
	giInfo = I_UNKNOWN;
	char szBuf[64] = {'\0'};
	char buf[64] = {'\0'};
	char *szTmp;
	sprintf(szBuf,"0:%s:%s",szId,szPass);       //1代表登录
	int ret =send(sfd,szBuf,strlen(szBuf),0);
	if(-1 == ret)
	{
		perror("send()");
		exit(-1);
	}
	memset(szBuf,'\0',sizeof(szBuf));

	ret = recv(sfd,szBuf,sizeof(szBuf),0);
	if(-1 == ret)
	{
		perror("send()");
		exit(-1);
	}
	strcpy(buf,szBuf);
	szTmp = strtok(szBuf,"*");
	if(1 == atoi(szTmp))//0:登录成功 1:登录失败
	{
		mkdir(Login_id,O_CREAT|0777);
		giInfo = I_LOGIN_OK;
		INFOUI(giInfo);
		szTmp = strtok(NULL,"*");
		if(szTmp != NULL)
		{
			recv_login(buf);
		}
		return 1;
	}
	else
	{
		memset(szBuf,'\0',sizeof(szBuf));
		giInfo = I_LOGIN_ERROR;
		INFOUI(giInfo);
		return 0;
	}
}

//处理主界面iNO 
int deal_Main(int INO)
{
	giInfo = I_UNKNOWN;
	if(INO < 1 || INO > 5)
	{
		giInfo = I_MAIN_NUM_ERROR;
		INFOUI(giInfo);
		return 1;
	}
	else
	{
		giInfo = I_MAIN_NUM_OK;
		INFOUI(giInfo);
		return 0;
	}
}

//打印当前在线用户
void deal_Online()
{
	char szBuf[512]={'\0'};
	int ret;
	strcpy(szBuf,"3:");//3:代表给服务器发送打印当前在线好友
	ret = send(sfd,szBuf,strlen(szBuf),0);
	if(-1 == ret)
	{
		perror("send()");
		exit(-1);
	}
	sleep(2);
	return ;
}


//查看聊天信息
void deal_Hischat(int inum,char *date)
{
	int ret;
	char szBuf[1024] = {'\0'};
	sprintf(szBuf,"4:%d:%s",inum,date);
	ret =send(sfd,szBuf,strlen(szBuf),0);
	if(-1 == ret)
	{
		perror("send()");
		exit(-1);
	}
	memset(szBuf,'\0',sizeof(szBuf));
	sleep(2);
	return ;
}


//私聊
void deal_Pchat(char *ID)
{
	giInfo = I_UNKNOWN;
	char buf[64] = {'\0'};
	char Buf[64] = {'\0'};
	char szBuf[64] = {'\0'};
	int ret;
	strcpy(stat1,ID);
	sprintf(szBuf,"5:%s",ID);
	send(sfd,szBuf,strlen(szBuf),0);
	while(1)
	{
		memset(szBuf,'\0',sizeof(szBuf));
		scanf("%s",buf);
		if(0 == strncmp("quit",buf,4))        //当发送quit时退出
		{
			break;
		}
		sprintf(szBuf,"1:%s:%s",ID,buf);
		ret =send(sfd,szBuf,strlen(szBuf),0);
		if(-1 == ret)
		{
			perror("send()");
			exit(-1);
		}	
	}//end while
	memset(stat1,'\0',sizeof(stat1));
	return ;
}//end


//群聊
void deal_Tchat()
{
	giInfo = I_UNKNOWN;
	char buf[64] = {'\0'};
	char Buf[64] = {'\0'};
	char szBuf[64] = {'\0'};
	int ret;
	strcpy(stat1,"ALL");
	sprintf(szBuf,"5:ALL");
	send(sfd,szBuf,strlen(szBuf),0);

	while(1)
	{
		memset(szBuf,'\0',sizeof(szBuf));
		scanf("%s",buf);
		if(0 == strncmp("quit",buf,4))        //当发送quit时退出
		{
			break;
		}
		sprintf(szBuf,"2:ALL:%s",buf);
		ret =send(sfd,szBuf,strlen(szBuf),0);
		if(-1 == ret)
		{
			perror("send()");
			exit(-1);
		}
	}//end while
	memset(stat1,'\0',sizeof(stat1));
	strcpy(stat1,"error");
	return ;
}
//退出
void deal_exit()
{
	char szBuf[8] = {'\0'};
	int ret;
	memset(szBuf,'\0',sizeof(szBuf));
	strcpy(szBuf,"6:");
	ret =send(sfd,szBuf,strlen(szBuf),0);
	if(-1 == ret)
	{
		perror("send()");
		exit(-1);
	}
	memset(szBuf,'\0',sizeof(szBuf));
	strcpy(stat1,"error");
	return ;
}

/*
*处理接收到的消息arrRecv[atoi(szTmp)-1](buf);
*/

//处理接收到的当前在线用户信息
int recv_online(char *szBuf)
{
	char *szTmp;
	int ONUM;
	int i;
	szTmp = strtok(szBuf,"*");
	szTmp = strtok(NULL,":");
	ONUM = atoi(szTmp);
	char Online[64] = {'\0'};
	printf("当前共有%d人在线\n",ONUM);
	for(i = 0;i<ONUM;++i)
	{

		szTmp = strtok(NULL,":");
		strcpy(Online,szTmp);
		printf("%s\n",Online);
		memset(Online,'\0',sizeof(Online));
	}
	memset(szBuf,'\0',sizeof(szBuf));
	return 0;
}

//处理接收的历史消息
int recv_hischat(char *szBuf)
{
	char *szTmp;
	szTmp = strtok(szBuf,"*");
	while(1)
	{
		szTmp = strtok(NULL,"*");
		if(szTmp == NULL)
		{
			break;
		}
		printf("%s\n",szTmp);				
	}
	memset(szBuf,'\0',sizeof(szBuf));
	return 0;
}

//处理接收的私聊消息
int recv_pchat(char *szBuf)
{
	char *szTmp;
	char buf[512] = {'\0'};
	char id[12]={'\0'};
	char BUF[512]={'\0'};
	char pathname[32]={'\0'};
	strcpy(buf,szBuf);
	szTmp = strtok(szBuf,"*");
	szTmp = strtok(NULL,":");
	if(strcmp(szTmp,stat1) == 0)
	{
		szTmp = strtok(buf,"*");
		szTmp = strtok(NULL,"*");
		printf("\t\t%s\n",szTmp);
	}
	else
	{
		printf("**%s发来消息**\n",szTmp);
		szTmp = strtok(buf,"*");
		szTmp = strtok(NULL,"*");
		strcpy(BUF,szTmp);
		szTmp = strtok(szTmp,":");
		strcpy(id,szTmp);
		szTmp=strtok(NULL,":");		
		sprintf(pathname,"./%s/%s.txt",Login_id,id);
		Save_Content(pathname,BUF);
		memset(pathname,'\0',sizeof(pathname));
	}
	return 0;
}

//处理接收的群聊消息
int recv_tchat(char *szBuf)
{
	char *szTmp;
	char buf[512] = {'\0'};
	char BUF[512] = {'\0'};
	char pathname[32] = {'\0'};
	strcpy(buf,szBuf);
	szTmp = strtok(szBuf,"*");
	if(strcmp("ALL",stat1) == 0)
	{
		szTmp = strtok(buf,"*");
		szTmp = strtok(NULL,"*");
		printf("\t\t%s\n",szTmp);
	}
	else
	{
		printf("*有新的群消息*\n");
		szTmp = strtok(buf,"*");
		szTmp = strtok(NULL,"*");
		sprintf(BUF,"ALL:%s",szTmp);		
		sprintf(pathname,"./%s/ALL.txt",Login_id);
		Save_Content(pathname,BUF);
		memset(pathname,'\0',sizeof(pathname));
	}
	return 0;
}

//处理接收的缓存消息
int recv_outchat(char *szBuf)
{
	char *szTmp;
	char ID[10] = {'\0'};
	char pathname[32] = {'\0'};
	szTmp = strtok(szBuf,"*");
	szTmp = strtok(NULL,"*");
	strcpy(ID,szTmp);
	while(1)
	{
		szTmp = strtok(NULL,"*");
		if(szTmp == NULL)
		{
			break;
		}
		printf("%s\n",szTmp);			
	}
	memset(szBuf,'\0',sizeof(szBuf));
	sprintf(pathname,"./%s/%s.txt",Login_id,ID);
	Read_Content(pathname);
	char del_pathname[32]={'\0'};
	sprintf(del_pathname,"> ./%s/%s.txt",Login_id,ID);
	system(del_pathname);
	memset(pathname,'\0',sizeof(pathname));
	memset(del_pathname,'\0',sizeof(del_pathname));
	return 0;
}

//处理缓存提示
int recv_login(char *szBuf)
{
	system("clear");
	char *szTmp;
	printf("###########################################################################\n");
	printf("\n\n");
	char c[8] = {'\0'};
	szTmp = strtok(szBuf,"*");
	while(1)
	{
		szTmp = strtok(NULL,"*");
		if(szTmp == NULL)
		{
			break;
		}
		printf("\t\t有最新%s的消息\n",szTmp);		
	}
	printf("###########################################################################\n");
	printf("###########################press any key to quit!##########################\n");
	system("stty -echo");
	scanf("%s",c);
	system("stty echo");
}

//输出错误信息
void INFOUI(int giInfo)
{
	printf("%s\n",szInfo[giInfo]);
	return ;
}
