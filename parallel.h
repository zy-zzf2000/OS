/*
 * @Author: zy 953725892@qq.com
 * @Date: 2022-11-14 21:08:29
 * @LastEditors: zy 953725892@qq.com
 * @LastEditTime: 2022-11-15 00:42:44
 * @FilePath: /lab1/parallel.h
 * @Description: 存放并行过程用到的函数与数据
 * 
 * Copyright (c) 2022 by zy 953725892@qq.com, All Rights Reserved. 
 */
#ifndef _PARALLEL_H_ 	
#define _PARALLEL_H_

#include <pthread.h>
#include <stdlib.h>
#include <dirent.h>
#include "queue.h"
#include "myerror.h"
#include "global.h"

pthread_mutex_t  queueidxlock;  //用于保护next_queue_idx
pthread_mutex_t  paralltotalllock;  //用于保护parallel_total

//初始化并行的锁、队列
void init();

//将一个任务分配到一个任务队列中去
void assign_task(Task *task,int queue_id);

//线程执行的函数,从第i个任务队列中取出任务执行
void consume_task(int i);

//并行化模式下，main线程执行的calDir，将任务分配到任务队列中去，而不调用calSingle
void calDir_parallel(char* path,int mode,int recursive,char* suffix);

//在main线程分配完任务后，向所有队列中分别插入一个结束标志
void callfinish();

#endif