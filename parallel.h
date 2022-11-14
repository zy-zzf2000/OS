/*
 * @Author: zy 953725892@qq.com
 * @Date: 2022-11-14 21:08:29
 * @LastEditors: zy 953725892@qq.com
 * @LastEditTime: 2022-11-14 23:32:04
 * @FilePath: /lab1/parallel.h
 * @Description: 存放并行过程用到的函数与数据
 * 
 * Copyright (c) 2022 by zy 953725892@qq.com, All Rights Reserved. 
 */
#ifndef _PARALLEL_H_ 	
#define _PARALLEL_H_

#include <pthread.h>
#include "queue.h"

#define MAXTHREAD 20       //最大线程数
int enable_thread = 0;          //是否开启多线程
int thread_num = 0;             //线程数
Queue queues[MAXTHREAD];   //每个线程对应的任务队列
int next_queue_idx;        //记录下一个任务队列的索引

//将一个任务分配到一个任务队列中去
void assign_task(Task *task,int queue_id);

//线程执行的函数,从第i个任务队列中取出任务执行
void consume_task(int i);


#endif