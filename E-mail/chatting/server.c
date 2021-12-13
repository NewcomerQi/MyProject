#include "tcp.h"

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("too few param\n");
		exit(-1);
	}
	int sfd = ftp_init(argv[1], atoi(PORT));
	pthread_t newthread;
	int fd = open("ipbase.txt", O_WRONLY |O_APPEND|O_CREAT|O_TRUNC, 0666);
	if(fd == -1)
	{
		perror("open()");
		exit(-1);
	}
	int i = 1;
	char buf[50] = "";
	char buf1[50] = "";
	struct sockaddr_in cid_addr;
	Accept_t Accept = {0};
	while(1)
	{
		int sfd_new = ftp_accept(sfd, argv[1], atoi(PORT), &cid_addr);

		read(sfd_new, buf, sizeof(buf));

		sprintf(buf1,"%d,%s\n",i, buf);
		i++;
		write(fd, buf1, strlen(buf1));
		memset(buf, 0, sizeof(buf));

		Accept.sfd_new = sfd_new;
		Accept.my_addr = cid_addr;
		pthread_create(&newthread, NULL, start_rountine, (void*)&Accept);
		pthread_detach(newthread);
	}

	close(sfd);
	close(fd);
	return 0;
}
