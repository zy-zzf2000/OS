/*
 * @Author: zy 953725892@qq.com
 * @Date: 2022-11-13 20:21:09
 * @LastEditors: zy 953725892@qq.com
 * @LastEditTime: 2022-11-14 12:46:13
 * @FilePath: /lab1/main.c
 * @Description: 
 * 
 * Copyright (c) 2022 by zy 953725892@qq.com, All Rights Reserved. 
 */
#include <stdio.h>
#include <sys/stat.h>
#include <linux/fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include "string.h"
#include <dirent.h>

#define MAXLINE 1024

int all = 0;
int blank_ignore = 0;
int recursive = 0;
int suffix = 0;
char suffix_name[MAXLINE];
int help = 0;

/**
 * @description: 检查一个文件类型和访问权限
 * @param {char*} path : 文件路径
 * @return {*} ： -1：文件类型错误 -2：文件权限错误  1：文件为普通文件  2：文件为目录文件
 */
int checkType(char* path){
    struct stat buf;
    fstatat(AT_FDCWD, path, &buf, 0);
    //首先检查文件权限
    if(buf.st_mode & S_IRUSR){
        if(S_ISDIR(buf.st_mode)){
            printf("文件为目录文件\n");
            return 2;
        }else if(S_ISREG(buf.st_mode)){
            printf("文件为普通文件\n");
            return 1;
        }else{
            printf("文件类型错误\n");
            return -1;
        }
    }else{
        return -2;
    }
}

/**
 * @description: 统计一个普通文件的行数
 * @param {char*} path:文件路径
 * @param {int} mode：是否忽略空行；1：忽略空行；0：不忽略空行
 * @return {*}：-1：文件打开错误；other：文件行数
 */
int calSingle(char* path,int mode){
    char* filepath = (char*) malloc(strlen(path)+strlen("./"));
    sprintf(filepath,"./%s",path);
    FILE* fp = fopen(filepath,"r");
    if(fp == NULL){
        printf("文件打开失败\n");
        return -1;
    }
    int line = 0;
    if(mode == 1){
        //如果忽略空行，用fgets读取文件
        char buf[MAXLINE];
        while(fgets(buf,MAXLINE,fp)!=NULL){
            if(strcmp(buf,"\n")==0){
                continue;
            }
            line++;
        }
    }else{
        //如果不忽略空行，用fgetc读取文件
        char buf;
        while((buf = fgetc(fp))!=EOF){
            if(buf == '\n'){
                line++;
            }
        }
        line++;
    }
    printf("文件行数为：%d\n",line);
    return line;
}

/**
 * @description: 
 * @param {char*} path：文件路径
 * @param {int} mode：是否忽略空行；1：忽略空行；0：不忽略空行
 * @param {int} recursive：是否递归处理子文件；1：递归；0：不递归
 * @return {*}: -1：目录打开错误；other：文件行数
 */
int calDir(char* path,int mode,int recursive){
    DIR *dir;
    struct dirent *dp;
    int lines = 0;

    char* dirpath = (char*) malloc(strlen(path)+strlen("./"));
    sprintf(dirpath,"./%s",path);
    if((dir=opendir(dirpath))==NULL){
        printf("目录打开失败\n");
        return -1;
    }
    while((dp = readdir(dir)) != NULL){
        //跳过.和..目录
        if(strcmp(dp->d_name,".")==0 || strcmp(dp->d_name,"..")==0){
            continue;
        }
        //构造子文件路径
        char* filepath = (char*) malloc(strlen(path)+strlen(dp->d_name)+strlen("/"));
        sprintf(filepath,"%s/%s",path,dp->d_name);
        int type = checkType(filepath);
        if (type == 1){
            int singleline = calSingle(filepath,mode);
            if(singleline>0){
                lines += singleline;
            }
        }else if(type==2 && recursive==1){
            int dirline = calDir(filepath,mode,recursive);
            if(dirline>0){
                lines += dirline;
            }
        }
    }
    return lines;
}

int main(int argc,char *argv[])
{
    int options;
    int long_option_idx;
    char* optstring = "Abrs:h";
    static struct option long_options[] = {
        {"all",     no_argument,       0,  'A' },
        {"suffix",  required_argument, 0,  's' },
        {"help",    no_argument,       0,  'h' },
        {0,         0,                 0,   0  } 
    };
    
    while((options = getopt_long(argc, argv, optstring, long_options, &long_option_idx)) != -1){
        switch(options){
            case 'A':
                all = 1;
                break;
            case 'b':
                blank_ignore = 1;
                break;
            case 'r':
                recursive = 1;
                break;
            case 's':
                suffix = 1;
                strcpy(suffix_name,optarg);
                break;
            case 'h':
                help = 1;
                break;
            default:
                printf("err command!\n");
                return 0;
                break;
        }
    }

    //TODO:处理help指令
    if(help == 1){
        printf("Usage: lc [OPTION] ... [FILE | DIR] ...\n");
        printf("Print newline counts for each FILE or DIR, and a total line if more than one FILE is specified.\n");
        printf("\n");
        printf("With no FILE, or when FILE is -, read standard input.\n");
        printf("\n");
        printf("Mandatory arguments to long options are mandatory for short options too.\n");
        printf("    -A, --all                     print the total line number only\n");
        printf("    -b                            ignore the Blank line when counting\n");
        printf("    -r                            count directory Recursively\n");
        printf("    -s, --suffix=FILE-SUFFIX      only count the file with specified Suffix\n");
        printf("    -h, --help                    display this Help and exit\n");
        return 0;
    }

    //TODO:处理文件参数
    for(int i = optind; i < argc; i++){
        printf("argv[%d] is %s\n", i, argv[i]);
        int cal = calSingle(argv[i],1);
        printf("文件行数为：%d\n",cal);
    }

    int lines = calDir("test",0,1);
    printf("文件夹行数为：%d\n",lines);
    return 0;
}

