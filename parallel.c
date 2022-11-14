/*
 * @Author: zy 953725892@qq.com
 * @Date: 2022-11-14 21:16:45
 * @LastEditors: zy 953725892@qq.com
 * @LastEditTime: 2022-11-14 23:37:07
 * @FilePath: /lab1/parallel.c
 * @Description: 并行化函数实现
 * 
 * Copyright (c) 2022 by zy 953725892@qq.com, All Rights Reserved. 
 */
#include "parallel.h"

//将一个任务分配到一个任务队列中去
void assign_task(Task *task,int queue_id){
    Push(&queues[queue_id],*task);
}

//线程执行的函数,从第i个任务队列中取出任务执行
void consume_task(int i){
    while(1){
        if(QueueEmpty(&queues[i])){
            sleep(1);
        }else{
            Task task = Pop(&queues[i]);
            if (task.id == -1){
                break;
            }else{
                calSingle(task.path,task.mode,task.suffix);
            }
        }
        
    }
}