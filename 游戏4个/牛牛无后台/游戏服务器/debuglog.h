#ifndef DEBUGLOG_HEAD_FILE
#define DEBUGLOG_HEAD_FILE

#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <fstream>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>




extern char* __file__;
extern int __line__;
int DebugLogOut(char *fmt, ...);
#define DebugLog __file__ = __FILE__, __line__ = __LINE__,DebugLogOut

using namespace std;
#endif