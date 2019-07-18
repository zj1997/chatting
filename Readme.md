#           **基于Linux的即时通讯系统项目总结**

---


​       这是一个Linux程序设计的课程设计项目，对于初学Linux的新手来说比较适合练手，该项目的设计思路主要来源是老师给的需求，不足之处，还望指点。 该项目主要采用linux的socket通信，采用文件进行数据的存储和持久化，多线程和多进程的模式进行用户和服务器的交互。*`



#### **1.后台管理系统起始界面：**

![avatar](projectPic/2.PNG)

#####   （1）后台添加账号模块



![avatar](projectPic/3.PNG)



#####   （2）后台用户详细列表模块 ，提醒部分代表此时该用户的信息列表的状态



![avatar](projectPic/4.PNG)



##### （3）该部分为服务器的开启关闭控制


![avatar](projectPic/5.PNG)



####  2.客户端的起始界面：



 ![avatar](projectPic/8.PNG)

   **根据账号密码进行登录，然后跳转主菜单界面**



​       **注意：用户登录成功后若有未读信息则会提醒**



![avatar](projectPic/9.PNG)

​    ***该部分为用户的主菜单界面**




![avatar](projectPic/7.PNG)



##### （1）该部分为用户之间私聊，即时通讯在线来信

![avatar](projectPic/10.PNG)





**客户端的聊天等各项动态，度武器后台都会以日志的形式进行存储**




![avatar](projectPic/11.PNG)



##### （2）此部分为群聊，在线的用户均可加入群聊进行聊天

![avatar](projectPic/14.PNG)





##### （3）此部分为查询在线的好友



![avatar](projectPic/12.PNG)



##### （4）此部分主要为用户私聊，群聊的聊天记录查询，根据日期进行查询

![avatar](projectPic/13.PNG)



  **在线用户会有用户和群的消息提醒**



![avatar](projectPic/16.PNG)



**！！！该部分为客户端退出后的信息显示：**



![avatar](projectPic/17.PNG)




**一个简单的demo小项目供初学者练手。您要是喜欢还请多多star。。。。 O(∩_∩)O哈哈~**