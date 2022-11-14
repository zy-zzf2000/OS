/*
 * @Author: zy 953725892@qq.com
 * @Date: 2022-11-14 23:53:53
 * @LastEditors: zy 953725892@qq.com
 * @LastEditTime: 2022-11-15 01:28:47
 * @FilePath: /lab1/global.c
 * @Description: 用于定义全局变量
 * 
 * Copyright (c) 2022 by zy 953725892@qq.com, All Rights Reserved. 
 */
#ifndef _GLOBAL_H_ 	
#define _GLOBAL_H_

#include "queue.h"
#define MAXLINE 1024
#define MAXTHREAD 20       //最大线程数


int all = 0;
int blank_ignore = 0;
int recursive = 0;
int use_suffix = 0;
char suffix_name[MAXLINE];
int help = 0;


//多线程相关
int enable_thread = 0;          //是否开启多线程
int thread_num = 0;             //线程数
Queue queues[MAXTHREAD];   //每个线程对应的任务队列
int next_queue_idx = 0;        //记录下一个任务队列的索引
int parallel_total = 0;    //并行化模式下用于统计总行数

#endif