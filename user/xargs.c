#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

#define BUFFER_SIZE 128

//创建子进程并执行指定的命令
void execute_command(char *command, char *args[]) {
    if (fork() == 0) { //子进程
        exec(command, args);
        fprintf(2, "exec %s failed\n", command);
        exit(1);
    } else { //父进程
        int status;
        wait(&status); //等待子进程完成
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(2, "lack of parameters\n");
        exit(1);
    }

    char *command = argv[1]; //保存要执行的命令
    char *args[MAXARG]; //保存命令和参数
    int arg_count = 0;

    //复制初始的命令
    for (int i = 1; i < argc && arg_count < MAXARG - 1; i++) {
        args[arg_count++] = argv[i];
    }
    args[arg_count] = 0; //数组以0结尾

    char buffer[BUFFER_SIZE]; //缓冲区
    int n;

    //从标准输入中读
    while ((n = read(0, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[n] = '\0'; //末尾添加'\0'

        //把输入分割，将每个子字符串作为参数添加到args数组中
        char *token = buffer;
        while (*token && arg_count < MAXARG - 1) {
            args[arg_count++] = token;
            while (*token && *token != ' ' && *token != '\n') {
                token++;
            }
            if (*token == ' ' || *token == '\n') {
                *token = '\0';
                token++;
            }
        }
        args[arg_count] = 0;
        //运行command
        execute_command(command, args);
        //重置arg_count
        arg_count = argc - 1;
    }
    exit(0);
}