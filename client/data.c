#include "data.h"

void Save_Content(char *pathname,char *BUF)
{
	char buf[64] = {'\0'};
	strcpy(buf,BUF);
	int fd=open(pathname,O_WRONLY|O_CREAT,0664);
	if(-1==fd)
	{
		perror("open()");
		exit(-1);
	}
	lseek(fd,0,SEEK_END);
	write(fd,buf,sizeof(buf));
	memset(buf,'\0',sizeof(buf));
	close(fd);
	return ;
}

void Read_Content(char *pathname)
{
	char buf[64]={'\0'};
	int fd=open(pathname,O_RDONLY);
	if(-1==fd)
	{
		return ;
	}
	int ret =0;
	while(1)
	{
		ret = read(fd,buf,sizeof(buf));;
		if(ret == 0)
			break;
		printf("%s\n",buf);
		memset(buf,'\0',sizeof(buf));
	}
	close(fd);
	return ;
}
