#ifndef BASE64_H
#define BASE64_H

#include <stdio.h>
#include <string.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define BASE64_PAD64 '='


char* base64_encode(const char *data);
char* base64_decode(const char *bdata); 

#endif

