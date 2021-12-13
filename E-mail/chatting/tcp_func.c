#include "tcp.h"

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

	printf("%s %d success connect....\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
	return sfd_new;
}

void *start_rountine(void *arg)
{
	Accept_t Accept = *(Accept_t *)(arg);

	int sfd_new = Accept.sfd_new;
	struct sockaddr_in cid_addr = Accept.my_addr;

	char username[20] = "";
	char password[20] = "";
	char buf[1024];
	int ret;
	while(1)
	{
		memset(buf, 0, sizeof(buf));
		ret = recv(sfd_new, buf, sizeof(buf),0);
		if(ret == -1)
		{
			print_quit(cid_addr);
			close(sfd_new);
			break;
		}
		if(strcmp(buf, "1") == 0)
		{
			my_register(sfd_new);
		}
		else if(strcmp(buf, "2") == 0)
		{
			memset(username, 0, sizeof(username));	
			memset(password, 0, sizeof(password));
			recv(sfd_new, username, sizeof(username),0);	// 用户名
			recv(sfd_new, password, sizeof(password),0);	// 密码
			Stu_t * stu = sqlite3_find(atoi(username), password);
			if(stu != NULL)
			{
				sqlite3 *ppDb;	//数据库操作句柄
				if(sqlite3_open(sqlite3_database_filename, &ppDb) != 0) // 打开数据库
				{
					printf("sqlite3_open error:%s\n", sqlite3_errmsg(ppDb));
					break;
				}
				sprintf(buf, "%d %s %s %d %s\n", stu->id, stu->password, stu->name, stu->age, stu->sex);
				free(stu);
				write(sfd_new, buf, strlen(buf));	//传送个人信息

				read(sfd_new, password, sizeof(password));
				memset(buf, 0, sizeof(buf));

				sprintf(buf, "%s", username);	// 传输文件
				write(sfd_new, buf, strlen(buf));

				read(sfd_new, buf, sizeof(buf));

				char *p = strchr(buf, ' ');
				*p = '\0';
				menu(buf, ppDb, sfd_new);
			}
			else
			{
				memset(buf, 0, sizeof(buf));
				sprintf(buf, "账户或密码错误");
				write(sfd_new, buf, strlen(buf));
			}
		}
		else if(strcmp(buf, "4") == 0)
		{
			retrieve_password(sfd_new);
		}
		else if(strncmp("quit", buf, strlen("quit")) == 0)
		{
			close(sfd_new);
			print_quit(cid_addr);
			break;
		}
	}
	return NULL;
}

void print_quit(struct sockaddr_in client_addr)
{
	printf("%s %d quit....\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

}

void menu(char *username, sqlite3 * ppDb, int sfd_new)
{
	char buf[1024] = "";
	while(1)
	{
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "请选择\n1.修改信息\n2.注销帐号\n3.退出帐号\n4.找人聊天\n5.接收文件");

		write(sfd_new, buf, strlen(buf));
		memset(buf, 0, sizeof(buf));
		read(sfd_new, buf, sizeof(buf));

		if(strchr("12345", buf[0]) != NULL)
		{
			if(atoi(buf) == 3)
			{
				write(sfd_new, buf, strlen(buf));
				break;
			}
			switch (atoi(buf))
			{
			case 1:
				write(sfd_new, buf, strlen(buf)); 
				read(sfd_new, buf, sizeof(buf));
				sqlite3_update(username, ppDb, sfd_new);
				read(sfd_new, buf, sizeof(buf));
				break;
			case 2:
				write(sfd_new, buf, strlen(buf));
				read(sfd_new, buf, sizeof(buf));
				if(sqlite3_delete(username, ppDb, sfd_new) == 0)
					return;
			case 4:
				write(sfd_new, buf, strlen(buf));
				read(sfd_new, buf, sizeof(buf));

				memset(buf, 0, sizeof(buf));
				chating(sfd_new);
				read(sfd_new, buf, sizeof(buf));
				memset(buf, 0, sizeof(buf));
				break;
			case 5:
				write(sfd_new, buf, strlen(buf));
				read(sfd_new, buf, sizeof(buf));

				memset(buf, 0, sizeof(buf));

				file_send(sfd_new);
				read(sfd_new, buf, sizeof(buf));
				memset(buf, 0, sizeof(buf));
				break;

			default: break;
			}
		}
		else
		{
			memset(buf, 0, sizeof(buf));
			sprintf(buf, "只能选择1,2,3,4或5");
			write(sfd_new, buf, strlen(buf));
			read(sfd_new, buf, sizeof(buf));
		}
	}

}
void file_send(int sfd_new)
{
	char file[20][20] = {0};
	char judge[20] = "";
	int flag = 1;
	int i = 0;
	char buf[1024];
	int len = 0;
	while(1)
	{
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "请选择：\n1.发送文件\n2.接受文件\n3.退出\n");
		write(sfd_new, buf, strlen(buf));	// 发送选择

		memset(buf, 0, sizeof(buf));
		read(sfd_new, buf, sizeof(buf));	// 接受选择

		if(strchr("123", buf[0]) != NULL)
		{
			if(atoi(buf) == 3)
			{
				write(sfd_new, buf, strlen(buf));
				break;
			}
			else if(atoi(buf) == 1)
			{
				write(sfd_new, buf, strlen(buf));

				memset(buf, 0, sizeof(buf));
				
				read(sfd_new ,buf, sizeof(buf));	// 接收来自客户端的文件名
				if(strcmp(buf, "文件不存在") == 0)
					continue;
				int fd = open(buf, O_WRONLY|O_CREAT, 0777);
				if(fd < 0)
				{
					perror("open()");
					break;
				}
				write(sfd_new, buf, strlen(buf));
				while(1)
				{
					memset(buf, 0, sizeof(buf));
					len = read(sfd_new, buf, sizeof(buf));		// 接收文件内容
					write(fd, buf, len);
					if(len < 1024)
						break;
				}
			}	
			else if(atoi(buf) == 2)
			{
				write(sfd_new, buf, strlen(buf));
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
				write(sfd_new, buf, strlen(buf));	// 发送所有文件给客户端
				fclose(fp);

				while(1)
				{
					memset(buf, 0, sizeof(buf));
					read(sfd_new, buf, sizeof(buf));	// 接收客户选择的文件
					if(buf[0] == '\n')
					{
						break;
					}
					strcpy(judge, buf);
					for(int j = 0; j < i-1; j++)
					{	
						flag = 1;
						if(strncmp(file[j], buf, strlen(file[j])) == 0)
						{
							flag = 0;
							write(sfd_new, buf, strlen(buf));	// 发送客户选择的文件

							read(sfd_new,buf, strlen(buf));

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
								write(sfd_new, buf, len);
							}
							break;
						}
					}
					if(flag == 1)
					{
						memset(buf, 0, sizeof(buf));
						sprintf(buf, "%s : 这个文件不存在\n", judge);
						write(sfd_new, buf, strlen(buf));
					}
				}
			}	
		}	
		else
		{
			memset(buf, 0, sizeof(buf));
			sprintf(buf, "只能选择1,2或3");
			write(sfd_new, buf, strlen(buf));
			read(sfd_new, buf, sizeof(buf));
		}
	}
}
void chating(int sfd_new)
{
	int j = 0;
	char buf[1024] = "";
	char ipbase[1024] = "";
	char file[10][50] = {0};
	char *arr[20] = {0};
	FILE *fd = fopen("ipbase.txt","r");
	if(fd == NULL)
	{
		perror("fopen()");
		exit(-1);
	}
	fseek(fd, 0, SEEK_SET);
	int i = 0;
	while(feof(fd) == 0)
	{
		fscanf(fd, "%s",file[i]);
		i++;
	}
	fseek(fd, 0, SEEK_SET);
	fread(ipbase, sizeof(ipbase),1,fd);
	fclose(fd);
	sprintf(buf, "请选择你的好友:\n%s", ipbase);
	write(sfd_new, buf, strlen(buf));

	memset(buf, 0,sizeof(buf));
	read(sfd_new, buf, sizeof(buf));		// 接收选择的好友
	for(j = 1; j <= i; j++)
	{
		if(j == atoi(buf))
		{
			int k = 1;
			char *p = file[j-1];
			arr[0] = file[j - 1];
			for(0; *p != '\0'; p++)
			{
				if(*p == ',')
				{
					*p = '\0';
					arr[k] = p+1;
					k++;
				}
			}
			break;
		}
	}
	if(j >= i)
	{
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "好友不存在");
		write(sfd_new, buf, strlen(buf));
		return;
	}
	memset(buf, 0, sizeof(buf));
	strcpy(buf, arr[1]);
	write(sfd_new, buf, strlen(buf));	// 发送ＩＰ

	read(sfd_new, buf, sizeof(buf));

	memset(buf, 0, sizeof(buf));
	strcpy(buf, arr[2]);
	write(sfd_new, buf, strlen(buf));	// 发送ＰＯＲＴ

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
void my_register(int sfd_new)
{
	char buf[1024] = "";

	sqlite3 *ppDb;	//数据库操作句柄
	if(sqlite3_open(sqlite3_database_filename, &ppDb) != 0) // 打开数据库
	{
		printf("sqlite3_open error:%s\n", sqlite3_errmsg(ppDb));
		return;
	}

	char **pazResult;	// 用于存储数据返回值
	int pnRow;			// 行
	int pnColumn;		// 列
	char *pzErrmsg;		// 错误码
	Stu_t stu = {0};
	char qq_num[20] = "";

	memset(buf, 0, sizeof(buf));
	sprintf(buf, "请输入帐号");
	write(sfd_new, buf, strlen(buf));

	memset(buf, 0, sizeof(buf));
	read(sfd_new, buf, sizeof(buf));
	strcpy(qq_num, buf);
	stu.id = atoi(buf);

	memset(buf, 0, sizeof(buf));
	sprintf(buf, "请输入密码");
	write(sfd_new, buf, strlen(buf));

	memset(buf, 0, sizeof(buf));
	read(sfd_new, buf, sizeof(buf));
	strcpy(stu.password, buf);

	memset(buf, 0, sizeof(buf));
	sprintf(buf,"请输入姓名");
	write(sfd_new, buf, strlen(buf));

	memset(buf, 0, sizeof(buf));
	read(sfd_new, buf, sizeof(buf));
	strcpy(stu.name, buf);

	memset(buf, 0, sizeof(buf));
	sprintf(buf, "请输入年龄");
	write(sfd_new, buf, strlen(buf));

	memset(buf, 0, sizeof(buf));
	read(sfd_new, buf, sizeof(buf));
	stu.age = atoi(buf);

	memset(buf, 0, sizeof(buf));
	sprintf(buf,"请输入性别");
	write(sfd_new, buf, strlen(buf));

	memset(buf, 0, sizeof(buf));
	read(sfd_new, buf, sizeof(buf));
	strcpy(stu.sex, buf);

	const char *SQL_insert= "insert into %s values(%d, '%s', '%s', %d, '%s')";
	char SQL_buf[1024] = "";
	sprintf(SQL_buf, SQL_insert, sqlite3_table_filename, stu.id, stu.password, stu.name, stu.age, stu.sex);
	if(sqlite3_get_table(ppDb, SQL_buf, &pazResult, &pnRow, &pnColumn, &pzErrmsg) != 0)
	{
		memset(buf, 0, sizeof(buf));
		sqlite3_close(ppDb);
		sprintf(buf,"有相同的帐号,增加失败\n");
		write(sfd_new, buf, strlen(buf));
		sqlite3_close(ppDb);
		return;
	}
	else
	{
		memset(buf, 0, sizeof(buf));
		sprintf(buf,"增加成功");
		write(sfd_new, buf, strlen(buf));

		memset(buf, 0, sizeof(buf));
		sprintf(buf,"mkdir %s", qq_num);
		
		system(buf);

	}
}

void retrieve_password(int sfd_new)
{
	char buf[1024] = "";
	char username[20] = "";

	sqlite3 *ppDb;	//数据库操作句柄
	if(sqlite3_open(sqlite3_database_filename, &ppDb) != 0) // 打开数据库
	{
		printf("sqlite3_open error:%s\n", sqlite3_errmsg(ppDb));
		return;
	}

	while(1)
	{
		memset(buf, 0, sizeof(buf));
		sprintf(buf,"请输入你的用户名(按回车退出)");
		write(sfd_new, buf, strlen(buf));

		read(sfd_new, username, sizeof(username));
		if(username[0] == '\n')
		{
			sqlite3_close(ppDb);
			break;
		}

		if(sqlite3_retrieve(ppDb, username) == -1)
		{
			memset(buf, 0, sizeof(buf));
			sprintf(buf, "用户名不存在");
			write(sfd_new, buf, strlen(buf));
			read(sfd_new, buf, sizeof(buf));
			continue;
		}
		else
		{
			write(sfd_new, buf, strlen(buf));
			read(sfd_new, buf, sizeof(buf));
			char first_password[20] = "";
			char second_password[20] = "";
			while(1)
			{
				memset(buf, 0, sizeof(buf));
				sprintf(buf, "请输入新密码(按回车退出)");
				write(sfd_new, buf, strlen(buf));

				read(sfd_new, first_password, sizeof(first_password));
				if(first_password[0] == '\n')
				{
					sqlite3_close(ppDb);
					return;
				}
				memset(buf, 0, sizeof(buf));
				sprintf(buf, "请再输入一次新密码");
				write(sfd_new, buf, strlen(buf));

				read(sfd_new, second_password, sizeof(second_password));

				if(strcmp(first_password, second_password) == 0)
				{
					if(amend_password(ppDb, username, first_password) == -1)
					{
						memset(buf, 0, sizeof(buf));
						sprintf(buf, "修改失败");
						write(sfd_new, buf, strlen(buf));	
						sqlite3_close(ppDb);
						return;
					}
					else
					{
						memset(buf, 0, sizeof(buf));
						sprintf(buf, "修改成功");
						write(sfd_new, buf, strlen(buf));
						sqlite3_close(ppDb);
						return;
					}
				}
				else
				{
					memset(buf, 0, sizeof(buf));
					sprintf(buf, "两次密码不一样");
					write(sfd_new, buf, strlen(buf));
					read(sfd_new, buf, sizeof(buf));
					memset(first_password, 0, sizeof(first_password));
					memset(second_password, 0, sizeof(second_password));
				}
			}
		}
	}
}
