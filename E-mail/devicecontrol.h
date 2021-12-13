#ifndef DEVICECONTROL_H
#define DEVICECONTROL_H

#include "common.h"
#include "net.h"


//主题字控制
int SubjectControl(mail_t *pmail, ctrl_t * subject);


//更新程序控制
int emitUpdate(const mail_t *pmail,ctrl_t * subject);

//命令字控制
int emitCommand(ctrl_t * subject);
int bulb_ctrl(int bulb, int bulb_ctrl);
int signal_ctrl(int signal);

// 发送回复邮件
int sendemail(int cfd, mail_t * pmail,char * content);
char *getCurrTime(void);
int sqlite3_update(void);

#endif

