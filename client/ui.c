#include "ui.h"



//登录界面
int LoginUI()
{
	system("clear");
	printf("=============AHUT Linux 兴趣小组===============\n");
	printf("\n");
	printf("=========组长赵杰 组员：李晓 闻斌 李威=========\n");
	printf("\n");
	printf("============组员：李晓 闻斌 李威===============\n");
	printf("\n");
	printf("                 QQ开始登录                    \n");
	printf("\n");
	char id[8] = {'\0'};
	char passwd[20];
	int i = 0;
	printf("                      账号:");
	scanf("%s",id);
	printf("\n");
	printf("                      密码:");
	system("stty -echo");
	scanf("%s",passwd);
	system("stty echo");
	printf("\n");
	return deal_login(id,passwd)?SW_MAIN:SW_LOGIN;
}

//主界面
int MainUI()
{
	system("clear");
	printf("##########################################################################\n");
	printf("+++++++++++++++++++++++++++    欢迎进入QQ系统   ++++++++++++++++++++++++++\n");
	printf("@                                                                        @\n");
	printf("@               part1.个人聊天                   part3.我的在线Q友       @\n");
	printf("@                                                                        @\n");
	printf("@               part2.大家一起聊                 part4.查看最近聊天记录  @\n");
	printf("@                                                                        @\n");
	printf("@                                part5.退出QQ                            @\n");
	printf("@                                                                        @\n");
	printf("##########################################################################\n");
	int i;
	printf("亲，你需要我做些什么:");
	scanf("%d",&i);
	return deal_Main(i)?SW_MAIN:i;
	
	
}


//退出界面
int ExitUI()
{
	system("clear");
	printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
	printf("$                                                                        $\n");
	printf("$                                                                        $\n");
	printf("$                                                                        $\n");
	printf("$                              Good Bye!                                 $\n");
	printf("$                                                                        $\n");
	printf("$                                                                        $\n");
	printf("$                                                                        $\n");
	printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
	deal_exit();
	return SW_QUIT;
}



//查看在线好友界面
int OnlineUI()
{
	printf("##########################################################################\n");
	printf("##########################       Friend On Line       ####################\n");
	printf("当前在线好友:");
	deal_Online();
	char c[8] = {'\0'};
	printf("#                          press any key to quit!!!                      #\n");
	system("stty -echo");
	scanf("%s",c);
	system("stty echo");
	return SW_MAIN;
}


	  
//私聊界面
int PchatUI()
{
	system("clear");
	printf("##########################################################################\n");
	printf("################             chat with friend             ################\n");
	char id[8] = {'\0'};
	printf("请选择你的Q友:");
	scanf("%s",id);
	printf("##########################################################################\n");
	printf("和%s正在愉快的聊天中^_^        \n",id);
	deal_Pchat(id);	
	return SW_MAIN;
}

//群聊界面
int TchatUI()
{
	system("clear");
	printf("##########################################################################\n");
	printf("###############    大家一起嗨（安工大计算机学院16级群）       ############\n");
	printf("##########################################################################\n");
	printf("大家正在愉快的玩耍。。。。     \n");
	deal_Tchat();
	return SW_MAIN;
}

//查看聊天记录界面
int HischatUI()
{
	system("clear");
	printf("##########################################################################\n");
	printf("#########################        聊天记录        #########################\n");
	printf("#                                                                        #\n");
	printf("#              1.好友聊天                     2.群组聊天                 #\n");
	printf("#                                                                        #\n");
	printf("##########################################################################\n");
	int i;
	printf("请输入需要查询的部分:");
	scanf("%d",&i);
	printf("请输入查询的时间段(如20190704):");
	char date[10] = {'\0'};
	scanf("%s",date);
	deal_Hischat(i,date);
	char c[8] = {'\0'};
	printf("***************************press any key to meau**************************\n");
	system("stty -echo");
	scanf("%s",c);
	system("stty echo");
	return SW_MAIN;
}



