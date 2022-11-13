/*
 * @Author: zy 953725892@qq.com
 * @Date: 2022-11-13 20:21:09
 * @LastEditors: zy 953725892@qq.com
 * @LastEditTime: 2022-11-13 22:38:11
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

//TODO:检查一个文件类型和访问权限(Done)
/**
 * @description: 
 * @param {char*} path : 文件路径
 * @return {*} ： -1：文件类型错误 -2：文件权限错误  1：文件为普通文件  2：文件为目录文件
 */
int checkType(char* path){
    struct stat buf;
    fstatat(AT_FDCWD, path, &buf, 0);
    //首先检查文件类型
    if(S_ISREG(buf.st_mode)){
        //检查文件权限
        if(buf.st_mode & S_IRUSR){
            printf("文件为普通文件，且有读权限\n");
            return 1;
        }
        else{
            return -2;
        }
    }else if(S_ISDIR(buf.st_mode)){
        printf("文件为目录文件\n");
        return 2;
    }
    return -1;
}


//TODO:完成命令行参数解析
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
                printf("A or all\n");
                break;
            case 'b':
                printf("b\n");
                break;
            case 'r':
                printf("r\n");
                break;
            case 's':
                printf("s or suffix\n");
                printf("suffix is %s\n", optarg);
                break;
            case 'h':
                printf("h or help\n");
                break;
            default:
                printf("err!\n");
                break;
        }
    }

    //输出剩余文件名
    for(int i = optind; i < argc; i++){
        printf("argv[%d] is %s\n", i, argv[i]);
    }
    
    return 0;
}

