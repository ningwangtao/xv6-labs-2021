#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MSGSIZE 16

int main(int argc,char *argv[]){
    
    int fd[2],status;
    char buf[MSGSIZE];
    pipe(fd);

    if(fork() > 0){
        write(fd[1],"ping",MSGSIZE);
        wait(&status);
        read(fd[0],buf,MSGSIZE);
        fprintf(2,"%d: received %s\n",getpid(),buf);
    }else{
        read(fd[0],buf,MSGSIZE);
        fprintf(2,"%d: received %s\n",getpid(),buf);
        write(fd[1],"pong",MSGSIZE);
    }
    
    exit(0);
}