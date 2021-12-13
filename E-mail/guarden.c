#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#define filename "time"
void while_write_time();
int main()
{
    //1.创建子进程
    pid_t pid = fork();
    //2.结束父进程,找干爹套钱，准备创建公司
    if (pid != 0)
    {
        printf("01.创建子进程，父进程结束\n");
        exit(0);
    }
    //3.创建会话，并担任会话组组长(创建公司，并担任公式董事长)
    if (setsid() < 0)
    {
        printf("创建会话(公司)失败\n");
    }
	// 执行程序
	if(execlp("./smtp_pop","", NULL) == -1)
	{
		perror("execlp");
		exit(-1);
	}
    //4.切换工作目录(不想在老家待了,公司想去大城市发展)
    chdir("/tmp");

    //5.更改权限(买断这块地的权限)
    umask(0);

    //6.关闭继承过来的父进程的文件描述符(以前的无用的东西)
    for (size_t i = 0; i < getdtablesize(); i++)
    {
        close(i);
    }
	while(1)
	{
		;
	}

    return 0;
}


