#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    if(argc <= 1){
        fprintf(2, "wrong parametters\n");
        exit(1);
    }
    int timming = atoi(argv[1]);
    sleep(timming);
    exit(0);
}