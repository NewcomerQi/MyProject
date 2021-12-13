#include<stdio.h>
#include<string.h>

int main()
{
	FILE * fp = fopen("table.txt", "r");
	if(fp == NULL)
	{
		perror("fopen()");
		return -1;
	}
	char buf[1024] = "";
	fread(buf,sizeof(buf), 1,fp);
	printf("buf = %s\n", buf);
	fclose(fp);
	return 0;

}
