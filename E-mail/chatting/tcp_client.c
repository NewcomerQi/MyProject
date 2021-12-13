#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<signal.h>
#include<arpa/inet.h>
#include<errno.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<pthread.h>
#include<sqlite3.h>
int tcp_connect(char *ip, short port);
int ftp_init(const char *ip, short port);
int ftp_accept(int sfd, const char *ip, short port, struct sockaddr_in *cid_addr);
char *s_gets(char *st, int n);
void my_register(int sfd);
void my_login(int sfd);
void retrieve_password(int sfd);
void *start_rountine(void *arg);
void *start_client(void *arg);
void file_send(int sfd);

typedef struct Accept
{
	struct sockaddr_in my_addr;
	int sfd_new;
} Accept_t;

typedef struct ip_port
{
	char ip[50];
	short port;
} Ip_Port;

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		printf("too few param\n");
		exit(-1);
	}

	//char buf[10] = "";
	int sfd = ftp_init(argv[1], atoi(argv[2]));
	pthread_t newthread;
	pthread_t client_tid;

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	Ip_Port ip_port = {0};
	struct sockaddr_in cid_addr;
	Accept_t Accept = {0};
	strcpy(ip_port.ip, argv[1]);
	ip_port.port = atoi(argv[2]);
	int i = 0;
	while(1)
	{
		if(i == 0)
		{
			pthread_create(&client_tid, NULL, start_client, (void*)&ip_port);
			pthread_detach(client_tid);
			i++;
		}

		int sfd_new = ftp_accept(sfd, argv[1], atoi(argv[2]), &cid_addr);
		Accept.sfd_new = sfd_new;
		Accept.my_addr = cid_addr;

		pthread_create(&newthread, NULL, start_rountine, (void*)&Accept);	// 服务端
		pthread_detach(newthread);
	}
	close(sfd);
	return 0;
}
//服务端
void *start_rountine(void *arg)
{
	int len = 0;
	Accept_t Accept = *(Accept_t *)(arg);

	int sfd_new = Accept.sfd_new;
	struct sockaddr_in cid_addr = Accept.my_addr;

	char buf[1024];

	while(1)
	{
		memset(buf, 0, sizeof(buf));
		read(sfd_new, buf, sizeof(buf));
		if(strncmp("quit", buf, strlen("quit")) == 0)
		{
			break;
		}
		puts(buf);
		if(strcmp("传输文件", buf) == 0)
		{
			write(sfd_new, buf, strlen(buf));
			
			memset(buf, 0, sizeof(buf));
			read(sfd_new, buf, sizeof(buf));
			int fd = open(buf, O_WRONLY|O_CREAT, 0666);
			if(fd ==  -1)
			{
				perror("open()");
				break;
			}
			memset(buf, 0, sizeof(buf));
			sprintf(buf, "continue");
			write(sfd_new, buf, strlen(buf));
			while(1)
			{
				memset(buf, 0, sizeof(buf));
				len = read(sfd_new, buf, sizeof(buf));		// 接收文件内容
				write(fd, buf, len);
				if(len < 1024)
					break;
			}
			
			printf("传送完毕\n");
			memset(buf, 0, sizeof(buf));
			sprintf(buf, "continue");
			write(sfd_new, buf, strlen(buf));
			close(fd);
		}
	}
	close(sfd_new);

}
//客户端
void *start_client(void *arg)
{	
	char buf[1024] = "";
	char ch;
	Ip_Port ip_port = *(Ip_Port*)(arg);

	//int sfd = tcp_connect(ip_port.ip,ip_port.port);		// 连接自己的服务端

	int sfd2 = tcp_connect("127.0.0.1", 10000);	// 连接总服务端

	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%s,%d", ip_port.ip, ip_port.port);
	write(sfd2, buf, strlen(buf));		// 发送本机服务端ＩＰ和PORT

	while(1)
	{
		memset(buf, 0, sizeof(buf));
		puts("请选择:");
		printf("1.注册账户\n");
		printf("2.登陆账户\n");
		printf("3.退出\n");
		printf("4.忘记密码\n");
		while((ch = getchar()) != '3')
		{
			while(getchar() != '\n')
				continue;
			if(strchr("1234", ch) == NULL)
				puts("Please enter only 1, 2, 3, 4:");
			else
				break;
		}
		if(ch == '1')
		{
			my_register(sfd2);
		}
		else if(ch == '2')
		{
			my_login(sfd2);
		}
		else if(ch == '4')
		{
			retrieve_password(sfd2);
		}
		else
		{
			memset(buf, 0, sizeof(buf));
			strcpy(buf, "quit");
			write(sfd2,buf,strlen(buf));
			close(sfd2);
			exit(0);
		}
	}
	close(sfd2);

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
	int addr_len = sizeof(struct sockaddr);

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
int ftp_accept(int sfd, const char *ip, short port, struct sockaddr_in *cid_addr)
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
	*cid_addr = client_addr;

	printf("%s %d success connect....\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));	//客户端ＩＰ和ＰＯＲＴ
	return sfd_new;
}

int tcp_connect(char *ip, short port)
{// 客户端连接
	// 生成socket描述符
	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == sfd)
	{
		perror("socket");
		exit(-1);
	}

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr(ip);
	int len = sizeof(struct sockaddr);

	//连接
	printf("connecting....\n");
	if(-1 == connect(sfd, (const struct sockaddr*)&server_addr, (socklen_t)len))
	{
		perror("connect");
		close(sfd);
		exit(-1);
	}
	printf("connected success\n");
	return sfd;
}
char *s_gets(char *st, int n)
{
	char *find = NULL;
	char *ret_val = NULL;
	ret_val = fgets(st, n, stdin);
	if(ret_val)
	{
		find = strchr(st, '\n');
		if(find)
			*find = '\0';
		else
			while(getchar() != '\n')
				continue;
	}
	return ret_val;
}
void my_register(int sfd)
{
	char buf[1024] = "";
	strcpy(buf, "1");
	write(sfd, buf, strlen(buf));
	//id
	memset(buf, 0, sizeof(buf));
	read(sfd, buf, sizeof(buf));
	puts(buf);
	memset(buf, 0, sizeof(buf));
	s_gets(buf, sizeof(buf));
	write(sfd, buf, strlen(buf));
	// 密码
	memset(buf, 0, sizeof(buf));
	read(sfd, buf, sizeof(buf));
	puts(buf);
	memset(buf, 0, sizeof(buf));
	s_gets(buf, sizeof(buf));
	write(sfd, buf, strlen(buf));
	// 姓名
	memset(buf, 0, sizeof(buf));
	read(sfd, buf, sizeof(buf));
	puts(buf);
	memset(buf, 0, sizeof(buf));
	s_gets(buf, sizeof(buf));
	write(sfd, buf, strlen(buf));

	// 年龄
	memset(buf, 0, sizeof(buf));
	read(sfd, buf, sizeof(buf));
	puts(buf);
	memset(buf, 0, sizeof(buf));
	s_gets(buf, sizeof(buf));
	write(sfd, buf, sizeof(buf));

	//性别
	memset(buf, 0, sizeof(buf));
	read(sfd, buf, sizeof(buf));
	puts(buf);
	memset(buf, 0, sizeof(buf));
	s_gets(buf, sizeof(buf));
	write(sfd, buf, sizeof(buf));

	memset(buf, 0, sizeof(buf));
	read(sfd, buf, sizeof(buf));
	puts(buf);
}

void my_login(int sfd)
{
	char ip[20];
	char port[20];
	char buf[1024] = "";
	char username[20];
	char password[20];
	char file[50];
	strcpy(buf, "2");
	write(sfd, buf, strlen(buf));

	puts("登陆界面");
	puts("请输入用户名:");

	while(strlen(s_gets(username, sizeof(username))) == 0)
	{
		printf("You enter nothing\n");
		printf("Please enter message(enter quit to quit):\n");
	}
	write(sfd, username, strlen(username));
	puts("请输入密码:");
	while(strlen(s_gets(password, sizeof(password))) == 0)
	{
		printf("You enter nothing\n");
		printf("Please enter message(enter quit to quit):\n");
	}
	write(sfd, password, strlen(password));

	memset(buf, 0, sizeof(buf));
	read(sfd, buf, sizeof(buf));
	if(strcmp(buf, "账户或密码错误") != 0)
	{
		
		printf("登陆成功\n");
		printf("%s", buf);	// 输出个人信息
		memset(buf, 0, sizeof(buf));

		sprintf(buf,"continue");
		write(sfd, buf, strlen(buf));
		
		memset(buf, 0, sizeof(buf));
		read(sfd, buf, sizeof(buf));	// 接受文件名

		sprintf(file, "mkdir %s", buf);	// 创建目录
		system(file);

		write(sfd, buf, strlen(buf));

		while(1)
		{
			memset(buf, 0, sizeof(buf));
			read(sfd, buf, sizeof(buf));
			puts(buf);	// 请选择

			memset(buf, 0, sizeof(buf));
			s_gets(buf, sizeof(buf));	// buf = 1
			write(sfd, buf, strlen(buf));	// 发送选择

			memset(buf, 0, sizeof(buf));
			read(sfd, buf, sizeof(buf));

			if(strcmp(buf, "1") == 0) 
			{
				sprintf(buf, "continue");
				write(sfd, buf, strlen(buf));

				memset(buf, 0, sizeof(buf));
				read(sfd, buf, sizeof(buf));
				puts(buf);	// 修改方式

				memset(buf, 0, sizeof(buf));
				s_gets(buf, sizeof(buf));	//　选择方式
				write(sfd, buf, strlen(buf));

				if(atoi(buf) == 6)
				{
					memset(buf, 0, sizeof(buf));
					read(sfd, buf, sizeof(buf));	//接收是否成功
					puts(buf);
					write(sfd, buf, strlen(buf));
				}
				else
				{
					memset(buf, 0, sizeof(buf));
					read(sfd, buf, sizeof(buf));	// 请输入新账户
					puts(buf);

					memset(buf, 0, sizeof(buf));
					s_gets(buf, sizeof(buf));	//　新账户
					write(sfd, buf, strlen(buf));

					memset(buf, 0, sizeof(buf));
					read(sfd, buf, sizeof(buf));	//接收是否成功
					puts(buf);
					write(sfd, buf, strlen(buf));
				}
			}
			else if(strcmp(buf, "2") == 0)
			{
				sprintf(buf, "continue");
				write(sfd, buf, strlen(buf));

				memset(buf, 0, sizeof(buf));
				read(sfd, buf, sizeof(buf));
				puts(buf);	//确认是否删除

				memset(buf, 0, sizeof(buf));
				s_gets(buf, sizeof(buf));	//　选择方式
				write(sfd, buf, strlen(buf));

				memset(buf, 0, sizeof(buf));
				read(sfd, buf, sizeof(buf));	//接收是否成功
				if(strcmp(buf, "删除成功") == 0)
				{
					puts(buf);
					return;
				}
				else
				{
					puts(buf);
					write(sfd, buf, strlen(buf));
				}
			}
			else if(strcmp(buf, "4") == 0)
			{
				sprintf(buf, "continue");
				write(sfd, buf, strlen(buf));

				memset(buf, 0, sizeof(buf));
				read(sfd, buf, sizeof(buf));	// 所有好友
				printf("%s", buf);

				memset(buf, 0, sizeof(buf));
				s_gets(buf, sizeof(buf));	//　选择好友
				write(sfd, buf, strlen(buf));

				memset(ip, 0, sizeof(ip));
				read(sfd, ip, sizeof(ip));	// 接收ＩＰ
				if(strcmp(ip, "好友不存在") == 0)
				{
					write(sfd, buf, strlen(buf));
					puts(ip);
				}
				else
				{

					write(sfd, ip, strlen(ip));

					memset(port, 0, sizeof(port));

					read(sfd, port, sizeof(port));	// 接收ＰＯＲＴ

					int sfd_new = tcp_connect(ip, atoi(port));	// 连接成功

					char file[20][20] = {0};
					int i = 0;
					int len = 0;
					while(1)
					{
						memset(buf, 0, sizeof(buf));
						s_gets(buf, sizeof(buf));		// 发送消息给别人服务器
						write(sfd_new, buf, strlen(buf));
						if(strncmp("quit", buf, strlen("quit")) == 0)
						{
							close(sfd_new);
							break;
						}

						if(strcmp(buf, "传输文件") == 0)
						{
							read(sfd_new, buf, sizeof(buf));
							FILE *fp = fopen("file.txt", "w+");	
							if(fp == NULL)
							{
								perror("fopen()");
								return;
							}
							memset(buf, 0, sizeof(buf));
							system("ls > file.txt");
							fread(buf, sizeof(buf), 1, fp);
							fseek(fp, 0, SEEK_SET);
							i = 0;
							while(feof(fp) == 0)
							{
								fscanf(fp, "%s", file[i]);
								i++;
							}
							fclose(fp);
							printf("所有文件:\n");
							puts(buf);
							printf("请选择要传送的文件:");
							memset(buf, 0, sizeof(buf));
							s_gets(buf, sizeof(buf));
							write(sfd_new, buf, strlen(buf));
							int fd = open(buf, O_RDONLY);
							if(fd == -1)
							{
								perror("517:open");
								continue;
							}
							memset(buf, 0, sizeof(buf));
							read(sfd_new, buf, sizeof(buf));

							while(1)	// 传送文件内容
							{
								memset(buf, 0, sizeof(buf));
								len = read(fd, buf, sizeof(buf));
								if(len == 0)
									break;
								write(sfd_new, buf, len);
							}
							puts("传送完毕");
							memset(buf, 0, sizeof(buf));
							read(sfd_new, buf, sizeof(buf));
							close(fd);
							
						}
					}	
					write(sfd, buf, strlen(buf));
				}
			}
			else if(strcmp(buf, "5") == 0)
			{
				sprintf(buf, "continue");
				write(sfd, buf, strlen(buf));

				memset(buf, 0, sizeof(buf));
				file_send(sfd);
				sprintf(buf,"continue");
				write(sfd, buf, strlen(buf));
			}
			else if(strcmp(buf, "3") == 0)
			{
				puts("退出成功");
				break;
			}
			else
			{
				puts(buf);
				write(sfd, buf, strlen(buf));
			}
		}
	}
	else
	{
		puts(buf);
	}

}
void file_send(int sfd)
{
	int len = 0;
	char buf[1024] = "";
	char judge[20] = "";
	int i = 0;
	char file[20][20] = {0};
	int flag = 1;

	while(1)
	{
		memset(buf, 0, sizeof(buf));
		read(sfd, buf ,sizeof(buf));	// 发送或接收文件
		printf("%s", buf);
		memset(buf, 0, sizeof(buf));
		s_gets(buf, sizeof(buf));	//做出选择

		write(sfd, buf, strlen(buf));	// 发送选择

		memset(buf, 0, sizeof(buf));
		read(sfd, buf ,sizeof(buf));	// 发送或接收文件

		if(atoi(buf) == 1)
		{
			//write(sfd, buf, strlen(buf));
			memset(buf, 0, sizeof(buf));

			FILE *fp = fopen("file.txt", "w+");	
			if(fp == NULL)
			{
				perror("fopen()");
				return;
			}
			memset(buf, 0, sizeof(buf));
			system("ls > file.txt");
			fread(buf, sizeof(buf), 1, fp);
			fseek(fp, 0, SEEK_SET);

			i = 0;
			while(feof(fp) == 0)
			{
				fscanf(fp, "%s", file[i]);
				i++;
			}
			printf("所有文件:\n");
			puts(buf);
			puts("请输入文件名");
			fclose(fp);

			memset(buf, 0, sizeof(buf));
			s_gets(buf, sizeof(buf));

			strcpy(judge, buf);

			for(int j = 0; j < i-1; j++)
			{
				flag = 1;
				if(strncmp(file[j], buf, strlen(file[j])) == 0)
				{
					flag = 0;
					write(sfd, buf, strlen(buf));
					read(sfd, buf,sizeof(buf));
					int fd = open(file[j], O_RDONLY);
					if(fd < 0)
					{
						perror("open()");
						break;
					}
					while(1)	// 传送文件内容
					{
						memset(buf, 0, sizeof(buf));
						len = read(fd, buf, sizeof(buf));
						if(len == 0)
							break;
						write(sfd, buf, len);
					}
					puts("传送完毕");
					break;
				}
			}
			if(flag == 1)
			{
				memset(buf, 0, sizeof(buf));
				printf("%s: 这个文件不存在\n", judge);
				sprintf(buf, "文件不存在");
				write(sfd, buf, strlen(buf));
			}
		}	
		else if(atoi(buf) == 2)
		{
			write(sfd, buf, strlen(buf));
			memset(buf, 0, sizeof(buf));

			read(sfd, buf, sizeof(buf));

			printf("来自服务端所有文件:\n%s", buf);	// 来自服务端所有文件

			while(1)
			{
				printf("请选择一个文件(输入回车退出):\n");	// 选择文件
				memset(buf, 0, sizeof(buf));

				s_gets(buf, sizeof(buf));
				if(strlen(buf) == 0)
				{
					sprintf(buf, "\n");
					write(sfd, buf, strlen(buf));	// 发送退出信号给服务端
					break;
				}
				memset(judge, 0, sizeof(judge));

				strcpy(judge, buf);

				write(sfd, buf, strlen(buf));	// 发送选择文件

				memset(buf, 0, sizeof(buf));

				read(sfd, buf, sizeof(buf));	// 再次接受选择的文件，二次确认

				if(strcmp(judge, buf) == 0)		// 比较选择的文件和服务端发来的文件是否相同
				{
					int fd = open(buf, O_WRONLY|O_CREAT, 0777);
					if(fd < 0)
					{
						printf("flag2\n");
						perror("open()");
						exit(-1);
					}
					memset(buf, 0, sizeof(buf));

					sprintf(buf, "continue");
					write(sfd, buf, strlen(buf));

					while(1)
					{
						memset(buf, 0, sizeof(buf));
						len = read(sfd, buf, sizeof(buf));		// 接收文件内容
						write(fd, buf, len);
						if(len < 1024)
							break;
					}
					close(fd);
					printf("传送完毕\n");
				}
				else
				{
					printf("%s", buf);
				}
			}
		}
		else if(atoi(buf) == 3)
		{
			break;
		}
		else
		{
			puts(buf);
			write(sfd, buf, strlen(buf));
		}
	}
}
void retrieve_password(int sfd)
{
	char buf[1024] = "";
	strcpy(buf, "4");
	write(sfd, buf, strlen(buf));
	while(1)
	{
		memset(buf, 0, sizeof(buf));
		read(sfd, buf, sizeof(buf));
		puts(buf);

		memset(buf, 0, sizeof(buf));
		s_gets(buf, sizeof(buf));
		if(strlen(buf) == 0)
		{
			buf[0] = '\n';
			write(sfd, buf, strlen(buf)); //输入用户名
			break;
		}
		write(sfd, buf, strlen(buf)); //输入用户名

		memset(buf, 0, sizeof(buf));
		read(sfd, buf, sizeof(buf)); //获取结果

		if(strcmp(buf, "用户名不存在") == 0)
		{
			puts(buf);
			write(sfd, buf, strlen(buf));
			continue;
		}
		else
		{
			write(sfd,buf, strlen(buf));
			while(1)
			{
				memset(buf, 0, sizeof(buf));
				read(sfd, buf, sizeof(buf)); //获取结果
				puts(buf);
				memset(buf, 0, sizeof(buf));
				s_gets(buf, sizeof(buf));	// 新密码一次
				if(strlen(buf) == 0)
				{
					buf[0] = '\n';
					write(sfd, buf, strlen(buf));
					return;
				}
				write(sfd,buf, strlen(buf));
				memset(buf, 0, sizeof(buf));

				read(sfd, buf, sizeof(buf));
				puts(buf);

				memset(buf, 0, sizeof(buf));
				s_gets(buf, sizeof(buf));	// 新密码二次
				write(sfd, buf, strlen(buf));

				memset(buf, 0, sizeof(buf));
				read(sfd, buf, sizeof(buf));
				if(strcmp(buf, "两次密码不一样") == 0)
				{
					puts(buf);
					write(sfd, buf, strlen(buf));
				}
				else
				{
					puts(buf);
					return;
				}
			}
		}
	}
}
