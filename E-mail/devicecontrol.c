#include "devicecontrol.h"


//主题字控制
int SubjectControl(mail_t *pmail, ctrl_t * subject)
{
	const char *command = subject->command;
	if(NULL == command)
		return -1;

	if(strstr("8LED 7SHU MOTO", command)) // 命令字控制
	{
		emitCommand(subject);
		if(subject->result == 0)
		{
			strcpy(pmail->subject, command);
			sprintf(pmail->subject, "%s %d %d",command, subject->bulb, subject->bulb_ctrl);
			strcat(pmail->subject, " control success");
		}
		else
		{
			sprintf(pmail->subject, "%s %d %d",command, subject->bulb, subject->bulb_ctrl);
			strcat(pmail->subject, " control error");
		}
	}
	else if(strncmp(command, "CHANGTABLE", strlen("CHANGTABLE")) == 0)	// 更新程序控制
	{
		if(emitUpdate(pmail, subject) == 0)
		{
			strcat(pmail->subject, " success");
		}
		else
		{
			strcat(pmail->subject, " failed");
		}
	}
	else 
	{
		strcat(pmail->subject, " error");
	}
	return 0;

	
}

//更新程序控制
int emitUpdate(const mail_t *pmail,ctrl_t * subject)
{
	const char *command = subject->command;
	if(NULL == command)
		return -1;

	FILE *fp = fopen(USERFILE, "w+");
	if(fp == NULL)
	{
		perror("fopen error");
		return -1;
	}
	int len = fwrite(pmail->atta, strlen(pmail->atta), 1, fp);
	if(len <= 0)
	{
		fclose(fp);
		return -1;
	}
	fclose(fp);
	if(sqlite3_update() == 0)
	{
		return 0;
	}
	else
		return -1;
}

int sqlite3_update(void)
{	
	FILE *fp = NULL;
	fp = fopen(USERFILE, "r");
	if(fp == NULL)
	{
		perror("fopen()");
		return -1;
	}
	char name[10][20] = {0};
	char pass[10][20] = {0};
	int i = 0;
	while(feof(fp) == 0)
	{
		fscanf(fp, "%s %s\n", name[i], pass[i]);
		i++;
	}
	fclose(fp);

	sqlite3 *ppDb;	//数据库操作句柄
	if(sqlite3_open(sqlite3_database_filename, &ppDb) != 0) // 打开数据库
	{
		printf("sqlite3_open error:%s\n", sqlite3_errmsg(ppDb));
		return -1;
	}

	char **pazResult;	// 用于存储数据返回值
	int pnRow;			// 行
	int pnColumn;		// 列
	char *pzErrmsg;		// 错误码

	char SQL_new[128] = "";

	for(int j = 0; j < i; j++)
	{
		const char *SQL_buf = "update %s set 用户名='%s',密码='%s' where 用户名='%s'";
		sprintf(SQL_new, SQL_buf,sqlite3_table_filename,name[j],pass[j], name[j]);
		if(sqlite3_get_table(ppDb, SQL_new, &pazResult, &pnRow, &pnColumn, &pzErrmsg) != 0)
			break;
	}

	sqlite3_close(ppDb);
	return 0;

}
//命令字控制
int emitCommand(ctrl_t * subject)
{
	const char *command = subject->command;
	if(strcmp(command, "8LED") == 0)
	{
		if(bulb_ctrl(subject->bulb, subject->bulb_ctrl) == 0)
			subject->result = 0;
		else
			subject->result = -1;
		return 0;
	}
	else if(strcmp(command, "7SHU") == 0)
	{
		if(signal_ctrl(subject->signal) == 0)
			subject->result = 0;
		else
			subject->result = -1;
		return 0;
	}
	else if(strcmp(command, "MOTO") == 0)
	{
		subject->result = 0;
		return 0;
	}
	else
		return -1;
}

// 控制ＬＥＤ灯
int bulb_ctrl(int bulb, int bulb_ctrl)
{
	int LED = 0;
	if(bulb > 8 || bulb_ctrl < 0 || bulb_ctrl > 1)
		return -1;

	if(bulb_ctrl == 0)
	{
		LED = LED & ~(1 << bulb);
		;
	}
	else
	{
		LED = LED | (1 << bulb);
		;
	}
	return 0;
}
// 信号灯控制
int signal_ctrl(int signal)
{
	return 0;
}

// 发送回复邮件
int sendemail(int cfd, mail_t * pmail, char * content)
{
	char buf[MAX_MAIL] = "";
	char *date = getCurrTime();
	char *arr[20];
	int k = 1;
	char *p = content;
	arr[0] = content;
	while(*p)
	{
		if(*p == '\n')
		{
			*p = '\0';
			arr[k] = p+1;
			k++;
		}
		p++;
	}

	char *a = arr[1];
	char *b = strchr(a, '<') + 1;
	b[strlen(b) - 2] = '\0';	// recv;
	char *c = arr[2];
	char *d = strchr(c, '<') + 1;
	d[strlen(d) - 2] = '\0'; //send;
	char send[30] = "";
	strcpy(send, d);
	d[strlen(d) - strlen("@qq.com")] = '\0';	// send
	char *subject = arr[k-1];

	sprintf(buf, "Message-ID: <001101c93cc4$1ed2ae30$5400a8c0@liuxiaoforever>\r\n"
				  "From: \"%s\" <%s> \r\n"
				  "To: <%s>\r\n"
				  "Subject: %s\r\n"
				  "Date: %s\r\n"
				  "MIME-VERSION: 1.0\r\n"
				  "Content-Type:multipart/alternative;\r\n"
				  "X-Priority: 3..X-MSMail-Priority: Normal\r\n"
				  "X-Mailer: Microsoft Outlook Express 6.00.2900.3138\r\n"
				  "X-MimeOLE: Produced By Microsoft MimeOLE V6.00.2900.3198\r\n"
				  "\r\n"
				  "This is response of %s's device!\r\n.\r\n",d,send,b,subject, date,d);
	if(write(cfd, buf, strlen(buf)) <= 0)
		return -1;
	return 0;
}

char *getCurrTime(void)
{
	time_t t = time(NULL);
	struct tm * gt;

	gt = localtime(&t);

	char *asct = asctime(gt);
	return asct;

}
