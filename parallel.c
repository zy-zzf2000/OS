/*
 * @Author: zy 953725892@qq.com
 * @Date: 2022-11-14 21:16:45
 * @LastEditors: zy 953725892@qq.com
 * @LastEditTime: 2022-11-15 00:43:32
 * @FilePath: /lab1/parallel.c
 * @Description: 并行化函数实现
 * 
 * Copyright (c) 2022 by zy 953725892@qq.com, All Rights Reserved. 
 */
#include "parallel.h"

void init(){
    pthread_mutex_init(&queueidxlock,NULL);
    pthread_mutex_init(&paralltotalllock,NULL);
    for(int i=0;i<thread_num;i++){
        initQueue(&queues[i]);
    }
}
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


void calDir_parallel(char* path,int mode,int recursive,char* suffix){
    DIR *dir;
    struct dirent *dp;
    if((dir=opendir(path))==NULL){
        //printf("目录打开失败\n");
        return ERROR_FILE_OPEN;
    }

    while((dp = readdir(dir)) != NULL){
        //跳过.和..目录
        if(strcmp(dp->d_name,".")==0 || strcmp(dp->d_name,"..")==0){
            continue;
        }
        //构造子文件路径
        char* filepath = (char*) malloc(strlen(path)+strlen(dp->d_name)+strlen("/")+10);
        sprintf(filepath,"%s/%s",path,dp->d_name);
        int type = checkType(filepath);
        
        if(type>0){
            if (type == 1){
                //需要将该任务分配到一个任务队列中去
                Task task;
                task.path = filepath;
                task.id = 1;
                task.mode = mode;
                task.suffix = suffix;

                pthread_mutex_lock(&queueidxlock);
                int queue_id = next_queue_idx;
                Push(&queues[queue_id],task);
                next_queue_idx = (next_queue_idx+1)%thread_num;
                pthread_mutex_unlock(&queueidxlock);
                
            }else if(type==2 && recursive==1){
                //如果是目录，则递归调用
                calDir_parallel(filepath,mode,recursive,suffix);
            }
        }else{
            print_result(filepath,type);
        }
    }
    closedir(dir);
}

void callfinish(){
    for(int i=0;i<thread_num;i++){
        Task task;
        task.id = -1;
        Push(&queues[i],task);
    }
}