#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* fmtname(char* path){
    static char buf[DIRSIZ];
    char *p;

    //strlen("helloworld'\0'"  = 10)
    for(p = path + strlen(path); p >= path && *p != '/';p--){
        ;
    }
    p++;
    memmove(buf,p,strlen(p) + 1);
    return buf;
}

void find(char* path,char* filename){
    char buf[512],*p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path,0)) < 0){
        fprintf(2,"find: connot open %s\n",path);
        exit(1);
    }
    
    //get the info from fd, return to the struct stat
    if(fstat(fd,&st) < 0){
        fprintf(2,"find: connot stst %s\n",path);
        close(fd);
        exit(1);
    }

    switch (st.type)
    {
    case T_FILE:
        /* code */
        if(strcmp(fmtname(path),filename) == 0){
            printf("%s\n",path);
        }
        break;
    case T_DIR:
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
            fprintf(2,"find: path is too long\n");
            break;
        }
        strcpy(buf,path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd,&de,sizeof(de)) == sizeof(de))
        {
            //no files
            if(de.inum == 0){
                continue;
            }
            memmove(p,de.name,DIRSIZ);
            p[strlen(de.name)] = '\0';
            if(strcmp(de.name,".") == 0 || strcmp(de.name,"..") == 0){
                continue;
            }
            find(buf,filename);
        }
        break;
    }
    close(fd);
}

int main(int argc,char *argv[]){
    if(argc < 3){
        fprintf(2,"find: wrong parameters\n");
        exit(1);
    }
    find(argv[1],argv[2]);
    exit(0);
}