#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

#define MAXSIZE 16

int main(int argc,char *argv[]){
    sleep(15);
    char buf[MAXSIZE];
    if(read(0,buf,MAXSIZE) < 0){
        fprintf(2,"xargs:wrong input\n");
        exit(1);
    }

    char *_xargv[MAXARG];
    int _xargc = 0;
    int status,pid;
    char *p = buf;

    for(int i=1;i<argc;i++){
        _xargv[_xargc] = argv[i];
        _xargc++;
    }
    
    for(int i=0;i<MAXSIZE;i++){
        if(buf[i] == '\n'){
            pid = fork();
            if(pid>0){
                p = &buf[i+1];
                wait(&status);
            }else{
                buf[i] = '\0';
                _xargv[_xargc] = p;
                _xargc++;
                _xargv[_xargc] = 0;

                exec(_xargv[0],_xargv);
                fprintf(2,"xargs: child process wrong\n");
                exit(1);
            }
        }
    }
    exit(0);
}