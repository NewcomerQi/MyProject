/*****************
 *1.主控模块
 *功能：初始化邮件体参数，套接字生成，进程建立，各模块顺序调用
 * ************/

#include "parsemail.h"
#include "net.h"
#include "devicecontrol.h"

pthread_mutex_t mutex_w;	//　收锁
pthread_mutex_t mutex_r;	//　发锁
typedef struct Accept
{
	int sfd_new;
} Accept_t;

typedef struct other
{
	int sfd_new;
	int sfd;
	ctrl_t ctrl;
	mail_t *pmail;
}Other_t;

void *start_send(void *arg);
void *start_recv(void *arg);

int main()
{
#if 0
		ctrl_t ctrl = {0};
		mail_t *pmail = (mail_t *)malloc(sizeof(mail_t));
		int sfd = ftp_init(IP, MYPORT);

		int sfd_new = ftp_accept(sfd, IP, MYPORT);
		pop3connection(sfd_new, &ctrl, pmail);

		close(sfd);
		close(sfd_new);
		free(pmail);
		pmail = NULL;
}
#endif

	{
		int sfd = ftp_init(IP, atoi(PORT));
		int sfd_pop3 = ftp_init(IP, MYPORT);

		pthread_t newthread;
		pthread_t pop3;

		pthread_mutex_init(&mutex_w, NULL);	//没有上锁
		pthread_mutex_init(&mutex_r, NULL);	//没有上锁
		pthread_mutex_lock(&mutex_r);		//先给收上锁

	//	Accept_t Accept1 = {0};
	//	Accept_t Accept2 = {0};

		while(1)
		{

		//	Accept1.sfd_new = sfd;
		//	Accept2.sfd_new = sfd_pop3;

			ctrl_t ctrl = {0};
			mail_t *pmail = (mail_t *)malloc(sizeof(mail_t));

			Other_t other = {0};
			other.sfd_new = sfd;
			other.sfd = sfd_pop3;
			other.ctrl = ctrl;
			other.pmail = pmail;

			pthread_create(&newthread, NULL, start_recv, (void*)&other);
			pthread_create(&pop3, NULL, start_send, (void*)&other);
			pthread_detach(newthread);
			pthread_detach(pop3);
			free(pmail);
		}
		close(sfd);
		close(sfd_pop3);
	}
	/*
	{
		ctrl_t ctrl = {0};
		mail_t *pmail = (mail_t *)malloc(sizeof(mail_t));
		int sfd = ftp_init(IP, MYPORT);

		int sfd_new = ftp_accept(sfd, IP, MYPORT);
		while(1)
		{
			pop3connection(sfd_new, &ctrl, pmail);

		}
		close(sfd);
		close(sfd_new);
		free(pmail);
		pmail = NULL;

	}
	*/
	return 0;
}
void *start_recv(void *arg)
{

	pthread_mutex_lock(&mutex_w);
//	Accept_t Accept = *(Accept_t *)(arg);
//	int sfd = Accept.sfd_new;
	Other_t other = *(Other_t *)arg;
	int sfd = other.sfd_new;

	ctrl_t ctrl = other.ctrl;
	mail_t *pmail = other.pmail;
	int sfd_new = ftp_accept(sfd, IP, atoi(PORT));

	handleconnection(sfd_new,pmail);

	parseMail(pmail, &ctrl);
	SubjectControl(pmail,&ctrl);

	char *a = strstr(pmail->raw, ".\r\n");
	char *b = pmail->raw;
	b[strlen(b) - strlen(a)] = '\0';	

	pmail->len = strlen(b);	// 邮件长度
	strcpy(pmail->raw, b);		// 邮件内容

	/*
	int fd = open(pmail->send, O_CREAT | O_WRONLY | O_TRUNC, 0666);	// 创建邮件文件
	if(fd == -1)
	{
		perror("open(error)");
		exit(-1);
	}
	write(fd, pmail->raw, strlen(pmail->raw));
	close(fd);
*/
	close(sfd_new);


	//int sfd_new = ftp_accept(sfd, IP, atoi(PORT));
	//pop3connection(sfd_new, &ctrl, pmail);
	/*
	int sfd;

		Other_t other = {0};
		sfd = ftp_init(IP, MYPORT);
		pthread_t newthread;
		int sfd_new = ftp_accept(sfd, IP, MYPORT);
		other.sfd_new = sfd_new;
		other.ctrl = ctrl;
		other.pmail = pmail;

		pthread_create(&newthread, NULL, start_send, (void*)&other);
		pthread_detach(newthread);
		*/

	close(sfd);
	//free(pmail);
	//pmail = NULL;
	pthread_mutex_unlock(&mutex_r);	//为读解锁
	return NULL;
}

void *start_send(void *arg)
{
	//Other_t other = *(Other_t *)arg;
	//int sfd = other.sfd_new;

	pthread_mutex_lock(&mutex_r);	// 为读上锁
	Other_t other = *(Other_t *)arg;
	int sfd = other.sfd;

	ctrl_t ctrl = other.ctrl;
	mail_t *pmail = other.pmail;

//	Accept_t Accept = *(Accept_t *)(arg);
	//int sfd = Accept.sfd_new;
//	int sfd = other.sfd_new;
	int sfd_new = ftp_accept(sfd, IP, MYPORT);
//	ctrl_t ctrl = {0};
//	mail_t *pmail = (mail_t *)malloc(sizeof(mail_t));
	pop3connection(sfd_new, &ctrl, pmail);

	close(sfd_new);
//	free(pmail);
//	pmail = NULL;
	pthread_mutex_unlock(&mutex_w);	//为写解锁
	return NULL;
}
