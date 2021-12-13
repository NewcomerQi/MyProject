#include "mailrecv.h"

// 接受用户名
int getusername(int cfd, table_t * p)
{
	char buf[128] = "";
	read(cfd, buf, sizeof(buf) - 1);
	buf[strlen(buf) - 2] = '\0';
	char *str = base64_decode(buf);
	if(verusername(str, p) == -1)	// 验证用户名
	{
		return -1;
	}
	return 0;
}
//接受密码
int getpassword(int cfd, table_t * p)
{
	char buf[128] = "";
	read(cfd, buf, sizeof(buf) - 1);
	buf[strlen(buf) - 2] = '\0';
	char *str = base64_decode(buf);
	if(verpassword(str, p) == -1)	// 验证密码
	{
		return -1;
	}
	return 0;
}

int getFromAddress(int cfd, mail_t * pmail)	// 接收地址
{
	char buf[128] = "";
	read(cfd, buf, sizeof(buf) - 1);

	buf[strlen(buf) - 2] = '\0';

	// 提取地址
	char *p = buf;
	char *q = NULL;
	while(*p)
	{
		if(*p == '<')
		{
			*p = '\0';
			q = p+1;
			break;
		}
		p++;
	}
	q[strlen(q) -1 ] = '\0';
	strcpy(pmail->send, q);

	return 0;
}
// 接受接收方地址
int getToAddress(int cfd, mail_t * pmail)
{
	char buf[128] = "";
	read(cfd, buf, sizeof(buf) - 1);

	buf[strlen(buf) - 2] = '\0';

	// 提取地址
	char *p = buf;
	char *q = NULL;
	while(*p)
	{
		if(*p == '<')
		{
			*p = '\0';
			q = p+1;
			break;
		}
		p++;
	}
	q[strlen(q) -1 ] = '\0';
	strcpy(pmail->recv, q);

	return 0;
}

// 接受邮件内容
int getbody(int cfd, mail_t * pmail)
{
	char buf[MAX_MAIL] = "";
	read(cfd, pmail->raw, sizeof(pmail->raw) - 1);
	char *str = NULL;
	char *array = NULL;
	sleep(1);
	if((str = strstr(pmail->raw, "filename=")) == NULL)	// 判断是否有附件
	{
		//read(cfd, pmail->raw, sizeof(pmail->raw) - 1);
	}
	else
	{
		read(cfd, buf, sizeof(buf) - 1);
		getslave(cfd, pmail, str);	// 提取附件
	}

	if((array = strstr(pmail->raw, "filename=")) != NULL)
	{
		char *filename = strstr(array, "\"") + 1;
		char *filename_2 = strstr(filename, "\"");
		filename[strlen(filename) - strlen(filename_2)] = '\0';
		strcpy(pmail->filename, filename);	// 提取附件名
	}

	return 0;
}
// 提取附件
int getslave(int cfd, mail_t * pmail, char * str)
{
	char *p = strstr(str, "\r\n\r\n");	// 定位到开头\r\n\r\n

	for(int i = 0; i < strlen(p); i++)	// 去掉开头\r\n\r\n
	{
		p[i] = p[i + 4];
	}
	char *q = strstr(p, "------=");// 定位到结尾\r\n\r\n

	p[strlen(p) - strlen(q) - 2] = '\0';	// 去掉结尾\r\n\r\n
	strcpy(pmail->atta, p);
	return 0;
}

// 得到接收方用户名
int getuser_pop(int cfd, table_t * p)
{
	char buf[128] = "";
	read(cfd, buf, sizeof(buf) - 1);
	char * str = strstr(buf, "USER") + strlen("USER") + 1;
	str[strlen(str) - 2] = '\0';

	if(verusername(str, p) == -1)	// 验证用户名
	{
		return -1;
	}
	return 0;
}

// 得到接收方密码
int getpass_pop(int cfd, table_t * p)
{
	char buf[128] = "";
	read(cfd, buf, sizeof(buf) - 1);
	char * str = strstr(buf, "PASS") + strlen("PASS") + 1;
	str[strlen(str) - 2] = '\0';

	if(verpassword(str, p) == -1)	// 验证密码
	{
		return -1;
	}
	return 0;
}
