#ifndef APP_H_
#define APP_H_

#include "basedefine.h"


// 初始化程序系统级参数
bool initapp(int argc,char** argv);

// 初始化业务级参数
bool initinstance();

// 主循环
int run();

// 清理
int exitinstance();

// 程序入口
int main(int argc,char** argv);

#endif // APP_H_