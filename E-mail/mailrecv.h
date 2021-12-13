#ifndef MAILRECV_H
#define MAILRECV_H

#include <stdio.h>
#include <string.h>
#include "common.h"
#include "parsemail.h"
#include "base64.h"
#include <unistd.h>

//接收用户名
int getusername(int cfd, table_t * p);
// 接收密码
int getpassword(int cfd, table_t * p);
// 接收地址
int getFromAddress(int cfd, mail_t * pmail);
int getToAddress(int cfd, mail_t * pmail);
int getbody(int cfd, mail_t * pmail);
int getslave(int cfd, mail_t * pmail, char * str);

int getuser_pop(int cfd, table_t * p);
int getpass_pop(int cfd, table_t * p);


#endif
