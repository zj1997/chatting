#include "data.h"

//将所有用户的状态位置０,cfd置－１
void Set_stat()
{
	STNODE *s;
	s = phead;
	while (s != NULL)
	{
		s->stUser.cfd = -1;
		s->stUser.stat=0;
		s = s->pNext;
	}
	WriteFile(phead);
	return ;
}
//改变下线用户的状态位
void Change_stat(int cfd)
{
	STNODE *USER = NULL;
	USER = Search_user(cfd);
	printf("%s退出\n",USER->stUser.id); 			
	USER->stUser.stat =0;
	USER->stUser.cfd = -1;
	WriteFile(phead);
	return ;
}
//读文件
STNODE *ReadFile(char *pathname)
{
	STNODE *s, *p;
	FILE *fp;
	if ((fp = fopen (pathname, "r+t")) == NULL)
	{
		printf ("文件为空!\n");
		return NULL;
	}
	s = malloc (sizeof (STNODE));
	fread (s, sizeof (STNODE), 1, fp);
	if (s == NULL)
	{
		printf ("文件为空!\n");
		phead = NULL;
		return NULL;
	}
	p = s;
	phead = s;
	while (!feof (fp))//feof:循环读取
	{

		s = malloc (sizeof (STNODE));
		if (fread (s, sizeof (STNODE), 1, fp) != 1)
		{
			free (s);
			break;
		}
		p->pNext = s ;
		p = s;
	}
	p->pNext = NULL;
	fclose (fp);
	return phead;

}

//保存修改的用户的信息
void WriteFile(STNODE *p)
{
	STNODE *s;
	s = phead;
	FILE *fp;
	if ((fp = fopen ("data.txt", "w+t")) == NULL)
	{
		printf ("文件保存失败!\n");
		exit (0);
	}
	while (s != NULL)
	{
		fwrite (s, sizeof (STNODE), 1, fp);
		s = s->pNext;
	}
	fclose (fp);
}



//注册用户
STNODE* User_register(char *id,char *szName,char *szPass)
{
	mkdir(id,O_CREAT|0777);
	STNODE *s=NULL;
	s = (STNODE*)malloc(sizeof(STNODE));

	strcpy(s->stUser.id,id);
	strcpy(s->stUser.szName,szName);
	strcpy(s->stUser.szPass,szPass);
	s->stUser.stat=0;
	s->stUser.cfd =-1;
	memset(s->stUser.notice,'\0',sizeof(s->stUser.notice));
	if(phead==NULL)
	{
		phead=s;
		phead->pNext=NULL;

	}
	else
	{
		s->pNext=phead->pNext;
		phead->pNext=s;
	}
	WriteFile(phead);
	return phead;
}

//check_login
void Check_login(char *id,char *szPass,int cfd,char *BUF)
{
	int ret1=1;
	int ret2=1;
	STNODE *p;
	p = phead;
	while(p!=NULL)
	{
		ret1=strcmp(id,p->stUser.id);
		ret2=strcmp(szPass,p->stUser.szPass);
		if((ret1==0)&&(ret2==0))
		{
			if(p->stUser.stat==0)
			{	
				p->stUser.stat=1;
				p->stUser.cfd=cfd;
				sprintf(BUF,"1%s",p->stUser.notice);
				memset(p->stUser.notice,'\0',sizeof(p->stUser.notice));
				WriteFile(phead);
				return ;  
			}
		}
		p=p->pNext;
	}
	strcpy(BUF,"0");
	return ;
}

//找cfd
void Search_cfd(char *id,int *recv_cfd)
{
	STNODE *p;
	p = phead;
	while(p!=NULL)
	{
		if(0==strcmp(id,p->stUser.id))
		{
			*recv_cfd =p->stUser.cfd;
			break;
		}
		p=p->pNext;
	}
	return ;
}

//通过cfd找用户
STNODE *Search_user(int cfd)
{ 
	STNODE *p;
	STNODE *USER = NULL;
	p = phead;
	while(p!=NULL)
	{
		if(cfd==p->stUser.cfd)
		{
			USER = p;
			break;
		}
		p=p->pNext;
	}
	return USER;
}

//保存离线私聊消息(USER给P发消息,P不在线)
void Save_Outline_Pchat_Content(STNODE *USER,STNODE *p,char *buf)
{
	char pathname[32]={'\0'};
	char szBuf[32]={'\0'};
	time_t iTime = 0;
	iTime = time(NULL);
	struct tm *pstTime = NULL;
	pstTime=localtime(&iTime);
	strftime(szBuf,sizeof(szBuf),"%Y%m%d_%H:%M:%S",pstTime);
	char BUF[64]={'\0'};
	sprintf(BUF,"*%s %s say:%s",szBuf,USER->stUser.id,buf);
	sprintf(pathname,"./%s/%s.txt",p->stUser.id,USER->stUser.id);
	int fd = open(pathname,O_CREAT|O_WRONLY,0664);
	if(-1==fd)
	{
		perror("open()");
		return ;
	}
	lseek(fd,0,SEEK_END);
	int ret = write(fd,BUF,strlen(BUF));
	if(-1==ret)
	{
		perror("write()");
		return ;
	}
	close(fd);
	return ;
}
//保存私聊聊天记录
void Save_personal_content(char *name,STNODE *p,STNODE *USER,char *buf)
{
	char pathname[30]={'\0'};
	char szBuf[32]={'\0'};
	time_t iTime = 0;
	iTime = time(NULL);
	struct tm *pstTime = NULL;
	pstTime=localtime(&iTime);
	strftime(szBuf,sizeof(szBuf),"%Y%m%d",pstTime);//获得当前时间
	sprintf(pathname,"./%s/%s.txt",p->stUser.id,szBuf);
	int fd = open(pathname,O_CREAT|O_WRONLY,0664);
	if(-1==fd)
	{
		perror("open()");
		return ;
	}
	memset(szBuf,'\0',sizeof(szBuf));
	strftime(szBuf,sizeof(szBuf),"%H:%M:%S",pstTime);
	char BUF[64]={'\0'};
	if(0==strcmp(p->stUser.szName,name))
	{
		sprintf(BUF,"*%s  你对%s说 %s",szBuf,USER->stUser.szName,buf);
	}
	else
	{
		sprintf(BUF,"*%s  %s说 %s",szBuf,name,buf);
	}
 	lseek(fd,0,SEEK_END);
	int ret = write(fd,BUF,strlen(BUF));
	if(-1==ret)
	{
		perror("write()");
		return ;
	}
	return ;
}
//p不在线,保存群聊离线信息
void Save_Outline_Tchat_Content(STNODE *USER,STNODE *p,char *buf)
{
	char pathname[32]={'\0'};
	char szBuf[32]={'\0'};
	time_t iTime = 0;
	iTime = time(NULL);
	struct tm *pstTime = NULL;
	pstTime=localtime(&iTime);
	strftime(szBuf,sizeof(szBuf),"%Y%m%d_%H:%M:%S",pstTime);
	char BUF[64]={'\0'};
	sprintf(BUF,"*%s %s(%s) say:%s",szBuf,USER->stUser.szName,USER->stUser.id,buf);
	sprintf(pathname,"./%s/group.txt",p->stUser.id);
	int fd = open(pathname,O_CREAT|O_WRONLY,0664);
	if(-1==fd)
	{
		perror("open()");
		return ;
	}
	lseek(fd,0,SEEK_END);
	int ret = write(fd,BUF,strlen(BUF));
	if(-1==ret)
	{
		perror("write()");
		return ;
	}
	close(fd);
	return ;
}

//发送离线缓存消息
void Outline_Content(int cfd,char *id,char *BUF)
{
	char pathname[24]={'\0'};
	STNODE *USER = NULL;
	USER = Search_user(cfd);
	printf("USER->cfd:%d\n",USER->stUser.cfd);
	if(strcmp(id,"ALL")==0)
	{
		sprintf(pathname,"./%s/group.txt",USER->stUser.id);
	}
	else
	{
		if(Judge_id(id)==1)
		{
			sprintf(BUF,"5*没有这个ID");
		}	
		sprintf(pathname,"./%s/%s.txt",USER->stUser.id,id);
	}
	printf("pathname %s\n",pathname);
	struct stat Buff;
	stat(pathname,&Buff);
	if(Buff.st_size>0)
	{
		printf("qwe\n");
		int fd = open(pathname,O_RDONLY);
		if(-1==fd)
		{
			perror("open()");
			sprintf(BUF,"5*%s*没有离线消息",id);
			return ;
		}
		int ret =0;
		char Content[1024*64]={'\0'};
	    ret = read(fd,Content,sizeof(Content));
	    if(-1==ret)
	    {
	    	perror("read()");
	    	return ;
	    }	
		sprintf(BUF,"5*%s%s",id,Content);
		memset(Content,'\0',sizeof(Content));
		char szTmp[32]={'\0'};
		sprintf(szTmp,"> %s",pathname);
		system(szTmp);
		close(fd);
		return ;
	}
	sprintf(BUF,"5*%s*没有离线消息",id);
	return ;	
}

//保存群聊的聊天记录
void Save_group_content(int cfd,char *buf,char *BUF)
{
	char pathname[30]={'\0'};
	char szBuf[32]={'\0'};
	STNODE *USER = NULL;
	USER = Search_user(cfd);
	time_t iTime = 0;
	iTime = time(NULL);
	struct tm *pstTime = NULL;
	pstTime=localtime(&iTime);
	strftime(szBuf,sizeof(szBuf),"%Y%m%d",pstTime);
	sprintf(pathname,"./group/%s.txt",szBuf);
	int fd = open(pathname,O_CREAT|O_WRONLY,0664);
	if(-1==fd)
	{
		perror("open()");
		return ;
	}
	memset(szBuf,'\0',sizeof(szBuf));
	strftime(szBuf,sizeof(szBuf),"%H:%M:%S",pstTime);
	char szTmp[64]={'\0'};
	sprintf(szTmp,"*%s %s(%s) say:%s",szBuf,USER->stUser.szName,USER->stUser.id,buf);
	sprintf(BUF,"2*%s(%s) say:%s",USER->stUser.szName,USER->stUser.id,buf);
	//从文件尾开始写
	lseek(fd,0,SEEK_END);
	int ret = write(fd,szTmp,strlen(szTmp));
	if(-1==ret)
	{
		perror("write()");
		return ;
	}
	close(fd);
	return ;
}

//发送私聊历史消息
void Pchat_Record(char *Time,int cfd,char *BUF)
{
	int fd;
	int ret =0;
	char pathname[30]={'\0'};
	STNODE *USER = NULL;
	USER = Search_user(cfd);
	sprintf(pathname,"./%s/%s.txt",USER->stUser.id,Time);
	fd = open(pathname,O_RDONLY);
	if(-1==fd)
	{
		perror("open()");
		send(USER->stUser.cfd,"4*没有这个日期的消息",20,0);
		printf("send:4*没有这个日期的消息\n");
		return ;
	}
	printf("name:%s\n",pathname);
	char Content[1024*64]={'\0'};
	ret=read(fd,Content,sizeof(Content));
	if(ret==-1)//文件结束
	{
		perror("read()");
		return ;
	}
	printf("content:%s\n",Content);
	sprintf(BUF,"4%s",Content);
	memset(Content,'\0',sizeof(Content));
	close(fd);
	return ;
}

//发送群聊历史消息
void Tchat_Record(int cfd,char *Time,char *BUF)
{
	int ret =0 ;
	char pathname[30]={'\0'};
	sprintf(pathname,"./group/%s.txt",Time);
	int fd = open(pathname,O_RDONLY);
	if(-1==fd)
	{
		perror("open()");
		send(cfd,"4*没有这个日期的消息",20,0);
		printf("send:4*没有这个日期的消息\n");
		return ;
	}
	char Content[1024*64]={'\0'};
	ret=read(fd,Content,sizeof(Content));
	if(ret==-1)
	{
		perror("read()");
		return ;
	}
	sprintf(BUF,"4%s",Content);
	memset(Content,'\0',sizeof(Content));
	close(fd);
	return ;
}

//判断id是否已经被注册过
int Judge_id(char *id)
{
	STNODE *p;
	p=phead;
	while(p!=NULL)
	{
		if(strcmp(p->stUser.id,id)==0)
		{
			return 0;
		}
		p=p->pNext;
	}
	return 1;
}

//查询信息
void Search_id()
{
	STNODE *p;
	p=phead;
	while(p!=NULL)
	{  
		printf("\t昵称:%-8s",p->stUser.szName);
		printf("\t帐号:%-8s",p->stUser.id);
		printf("\t密码:%-8s",p->stUser.szPass);
		printf("\t状态:%-8d",p->stUser.stat);
		printf("\t提醒:%-8s",p->stUser.notice);
		printf("\tcfd:%-8d\n",p->stUser.cfd);

		p=p->pNext;
	}
	return ;
}

//处理私聊消息
int Deal_Pchat(char *id,char *buf,int cfd,char *BUF)
{
	int ret=0;
	STNODE *p;
	p = phead;
	STNODE *USER = NULL;
	USER = Search_user(cfd);
	while(p!=NULL)
	{
		ret=strcmp(id,p->stUser.id);
		if(ret==0)//找到ID
		{
			Save_personal_content(USER->stUser.szName,p,USER,buf);
			Save_personal_content(USER->stUser.szName,USER,p,buf);
			if(1==p->stUser.stat)//该ID在线
			{
				sprintf(BUF,"1*%s:%s",USER->stUser.id,buf);
				return 1;
			}
			else
			{
				Save_Outline_Pchat_Content(USER,p,buf);
				if(strstr(p->stUser.notice,USER->stUser.id)==NULL)
				{
					memset(buf,'\0',sizeof(buf));
					sprintf(buf,"*%s",USER->stUser.id);
					strcat(p->stUser.notice,buf);
				}
				WriteFile(phead);
				return 0;
			}
		}
		p=p->pNext;
	}//while(p!=NULL)	
	strcpy(BUF,"^");//没有那个ID
	return 1;
}

//处理群聊消息
void Deal_Tchat(int cfd,char *buf,char *Online_cfd)
{
	STNODE *p;
	p=phead;
	STNODE *USER = NULL;
	USER = Search_user(cfd);
	int ret = 0;
	char szBuf[32]={'\0'};
	while(p!=NULL)
	{
		if(p->stUser.stat ==1)
		{
			sprintf(szBuf,"*%d",p->stUser.cfd);
			strcat(Online_cfd,szBuf);
			memset(szBuf,'\0',sizeof(szBuf));
		}
		else
		{
			Save_Outline_Tchat_Content(USER,p,buf);
			if(strstr(p->stUser.notice,"group")==NULL)
			{
				strcat(p->stUser.notice,"*group");
			}
		}
		p=p->pNext;
	}
	WriteFile(phead);
	return ;
}


void Online_User(char *BUF)
{
	char buf[512]={'\0'};
	STNODE *p;
	p=phead;
	int i=0;
	while(p!=NULL)
	{
		if(p->stUser.stat ==1)
		{
			sprintf(buf,"%s:%s(%s)",buf,p->stUser.szName,p->stUser.id);   	      
			i++;
		}
		p=p->pNext;
	}	
	sprintf(BUF,"3*%d%s:",i,buf);
	return ;
}

