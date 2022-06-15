#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAXSIZE 36
#define ONE '1'
#define ZERO '0'

void prime(int *fd){
    int val = 0;
    int i,pid,status;
    char buf[MAXSIZE];
    read(fd[0],buf,MAXSIZE);
    for(i=2;i<MAXSIZE;i++){
        if(buf[i] == ONE){
            val = i;
            break;
        }
    }
    if(val == 0){
        exit(0);
    }
    for(i=2;i<MAXSIZE;i++){
        if(i % val == 0){
            buf[i] = ZERO;
        }
    }
    fprintf(2,"prime %d\n",val);
    pid = fork();
    if(pid > 0){
        write(fd[1],buf,MAXSIZE);
        wait(&status);
    }else{
        prime(fd);
    }
}

int main(int argc , char *argv[]){
    int fd[2],status,pid;
    char buf[MAXSIZE];
    for(int i=0;i<MAXSIZE;i++){
        buf[i] = ONE;
    }
    if(pipe(fd) < 0){
        fprintf(2,"pipe wrong\n");
        exit(1);
    }

    pid = fork();
    if(pid > 0){
        buf[0] = ZERO;
        buf[1] = ZERO;
        write(fd[1],buf,MAXSIZE);
        wait(&status);
    }else{
        prime(fd);
    }

    exit(0);
}