/**************
 *2．网络套接字模块(smpt, pop3锁步流程)
 *功能：建立网络连接套接字
 *
 *
 *******************/



#include "net.h"

// STMP协议通信，接收函数
int handleconnection(int cfd, mail_t * pmail)
{
	if(cfd < 2 || NULL == pmail)
		return -1;

	char * response220 = "220 192.168.2.166 ok\r\n";
	char * response250_HELO = "250-192.168.2.166\r\n250-PIPELINING\r\n250-SIZE 52428800\r\n250-AUTH LOGIN PLAIN\r\n250-AUTH=LOGIN\r\n250-MAILCOMPRESS\r\n250 BITMIME\r\n";
	char * response334_user = "334 VXNlcm5hbWU6\r\n";
	char * response334_pass = "334 UGFzc3dvcmQ6\r\n";
	char * response235 = "235 Authenticatin successful\r\n";
	char * response250_ok = "250 OK\r\n";
	char * response_354 = "354 End data with <CR><LF>.<CR><LF>\r\n";
	char * response_221 = "221 Bye\r\n";

	write(cfd, response220, strlen(response220));	// 220代表服务器准备就绪
	char buf[1024] = "";
	read(cfd, buf, sizeof(buf) - 1);
	if(strncmp(buf, "HELO", strlen("HELO")) && strncmp(buf, "EHLO", strlen("EHLO")))
	{
		perror("HELO or EHLO error");
		return -1;
	}

	write(cfd, response250_HELO, strlen(response250_HELO));	// 250，代表请求的命令成功完成　
	read(cfd, buf, sizeof(buf) - 1);
	if(strncmp(buf, "AUTH LOGIN", strlen("AUTH LOGIN")))	// 判断是否为AUTH_LOGIN
	{
		perror("AUTH LOGIN error");
		return -1;
	}
	write(cfd, response334_user, strlen(response334_user));	// 向客户端发送334

	table_t table = {0};
	if(getusername(cfd, &table))	// 接收客户发送的用户名
	{
		perror("getusername error");
		return -1;
	}

	write(cfd, response334_pass, strlen(response334_pass));		//向客户端发送334
	if(getpassword(cfd, &table))	// 接收密码
	{
		perror("getpassword error");
		return -1;
	}
	write(cfd, response235, strlen(response235));	// 向客户发送235
	if(getFromAddress(cfd, pmail))	// 接收发件人地址
	{
		perror("getFromAddress error");
		return -1;
	}
	write(cfd, response250_ok, strlen(response250_ok));	// 向客户端发送250

	if(getToAddress(cfd, pmail))	// 接收收件人地址
	{
		perror("getToAddress error");
		return -1;
	}

	write(cfd, response250_ok, strlen(response250_ok));	// 向客户端发送250
	read(cfd, buf, sizeof(buf) - 1);
	if(strncmp(buf, "DATA", strlen("DATA")))	// 邮件内容
	{
		perror("DATA error");
		return -1;
	}
	write(cfd, response_354, strlen(response_354));	// 向客户端发送354
	if(getbody(cfd, pmail))
	{
		perror("getbody error");
		return -1;
	}
	write(cfd, response250_ok, strlen(response250_ok));

	read(cfd, buf, sizeof(buf));
	if(strncmp("QUIT",buf, strlen("QUIT")) != 0)	// 是否退出
	{
		//perror("net.c: 90 QUIT error");
		//return -1;
	}
	write(cfd, response_221, strlen(response_221));
	return 0;

}

// ＰＯＰ３协议锁步流程
int pop3connection(int sfd_new, ctrl_t * subject, mail_t *pmail)
{
	char buf[128] = "";
	sprintf(buf, "+OK POP3 mail server ready\r\n");
	write(sfd_new, buf, strlen(buf));	
	table_t table = {0};
	if(getuser_pop(sfd_new,&table))	// 获取用户名
	{
		perror("getuser_pop error");
		return -1;
	}

	sprintf(buf, "+OK \r\n");
	write(sfd_new, buf, strlen(buf));	// 获取密码
	if(getpass_pop(sfd_new, &table))
	{
		perror("getpass_pop error");
		return -1;
	}

	sprintf(buf, "+OK\r\n");
	write(sfd_new, buf, strlen(buf));	// 发送有多少邮件,多少字节

	read(sfd_new, buf, sizeof(buf) - 1);
	if(strncmp("STAT", buf, strlen("STAT")))
	{
		close(sfd_new);
		return -1;
	}

	char filename[20] = "";
	sprintf(filename, "%s%s", table.username, "@qq.com");	//邮箱名
	DIR *dir = NULL;
	dir = opendir(filename);	// 打开邮箱,判断邮箱是否存在
	if(dir == NULL)
	{
		perror("邮箱不存在");
		return -1;
	}
	int i = 0;
	FILE *fp = NULL;
	int len = 0;
	int sum_len = 0;
	struct dirent *ptr;


	FILE * fd = NULL;

	fd = fopen("E_mail.txt", "w+");		// 创建一个文件来存储有多少封邮件，分别是多大
	if(fd == NULL)
		perror("fopen()");
	while((ptr = readdir(dir)) != NULL)	// 循环打开邮件
	{
		if(strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)	// 忽视掉.,..目录
			continue;
		sprintf(buf, "%s/%s", filename, ptr->d_name);	// 邮箱下的邮件名
		fp = fopen(buf, "r+");	// 打开邮件
		if(fp == NULL)
		{
			closedir(dir);
			perror("fopen()");
			return -1;
		}
		fseek(fp, 0, SEEK_END);	// 设置到邮件末尾
		len = ftell(fp);	// 邮件长度
		sum_len += len;	// 所有邮件长度相加
		i++;
		fprintf(fd, "%d %d", i, len);	//将邮件个数和长度存到E_mail.txt中
		fclose(fp);	// 关闭打开的邮件
	}
	closedir(dir);	// 关闭邮箱
	fclose(fd);	// 关闭E_mail.txt文件

	sprintf(buf, "+OK %d %d\r\n", i, sum_len);	// 发送有多少邮件分别有多少个字节
	write(sfd_new, buf, strlen(buf));

	read(sfd_new, buf, sizeof(buf) - 1);
	if(strncmp("LIST", buf, strlen("LIST")))
	{
		close(sfd_new);
		return -1;
	}
	char All_Email[1024] = "";

	fd = fopen("E_mail.txt", "r+");	// 打开E_mail.txt文件(存储邮件个数和各邮件大小)
	if(fd == NULL)
	{
		perror("fopen");
		return -1;
	}
	fread(All_Email, sizeof(All_Email),1, fd);	// 将E_mail.txt中的内容都写入到All_Email中
	sprintf(buf, "+OK %d message\r\n%s\r\n.\r\n", i, All_Email);	// 组装有多少邮件，每个邮件分别多大
	fseek(fd, 0, SEEK_SET);	// 定位到文件开头

	write(sfd_new, buf, strlen(buf));

	memset(buf,0,sizeof(buf));	// 清空buf
	read(sfd_new, buf, sizeof(buf) - 1);
	if(strncmp("RETR 1", buf, strlen("RETR 1")))	// 获取第一封邮件内容
	{
		close(sfd_new);
		return -1;
	}

	int one;
	int one_mail;
	fscanf(fd, "%d %d",&one, &one_mail);	// 读取E_mail.txt中的第一个行(个数和大小)
	fclose(fd);

	char content[MAX_MAIL] = "";	// 存储邮件内容
	dir = opendir(filename);	// 打开邮箱
	if(dir == NULL)
	{
		perror("opendir");
		return -1;
	}
	char E_MAIL[50] = "";	//存储邮件名
	while((ptr = readdir(dir)) != NULL)
	{
		if(strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)	// 忽略., .. 目录
			continue;
		sprintf(E_MAIL, "%s/%s", filename, ptr->d_name);	// 组装邮件路径
		fp = fopen(E_MAIL, "r+");	// 打开邮件
		if(fp == NULL)
			return -1;
		fseek(fp, 0, SEEK_END);	// 定位到邮件末尾
		len = ftell(fp);	// 获取邮件长度
		fseek(fp, 0, SEEK_SET);		// 定位到邮件开头
		fread(content, len,1, fp);	// 将邮件内容写入到content中
		fclose(fp); //关闭邮件
		break;
	}
	closedir(dir);	// 关闭邮箱

	sprintf(buf, "+OK %d octets\r\n", one_mail);	// 将邮件中多少字节发给客户端
	write(sfd_new, buf, strlen(buf));

	if(sendemail(sfd_new, pmail, content) == -1)	// 发送邮件
		return -1;

	read(sfd_new, buf, sizeof(buf) - 1);
	if(strncmp("DELE 1", buf, strlen("DELE 1")))	// 删除第一份邮件命令
	{
		close(sfd_new);
		return -1;
	}
	remove(E_MAIL);	// 删除第一份邮件

	sprintf(buf, "+OK message 1 deleted\r\n");
	write(sfd_new, buf, strlen(buf));

	read(sfd_new, buf, sizeof(buf) - 1);
	if(strncmp("QUIT", buf, strlen("QUIT")))	// 退出命令
	{
		close(sfd_new);
		return -1;
	}

	sprintf(buf, "+OK\r\n");
	write(sfd_new, buf, strlen(buf));
	close(sfd_new);

	return 0;
}

int ftp_init(const char *ip, short port)
{// socket(), bind(), listen(),返回socket描述符
	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == sfd)
	{
		perror("sfd");
		exit(-1);
	}
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(struct sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr(ip);
	int opt = 1;
	// sockfd为需要端口复用的套接字
	setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt,sizeof(opt));

	//绑定
	if(bind(sfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
	{
		printf("bind():%s\n", strerror(errno));

		close(sfd);
		exit(-1);
	}

	// 监听
	if(-1 == listen(sfd, 5))
	{
		perror("listen()");
		close(sfd);
		exit(-1);
	}
	return sfd;
}

int ftp_accept(int sfd, const char *ip, short port)
{// 同意connect()
	struct sockaddr_in client_addr;
	memset(&client_addr, 0, sizeof(struct sockaddr_in));
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = port;
	client_addr.sin_addr.s_addr = inet_addr(ip);
	int len = sizeof(struct sockaddr);

	int sfd_new = accept(sfd, (struct sockaddr*)&client_addr, (socklen_t *)&len);
	if(-1 == sfd_new)
	{
		perror("accpet");
		close(sfd);
		close(sfd_new);
		exit(-1);
	}
	printf("%s %d success connect....\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
	return sfd_new;
}
