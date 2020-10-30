#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    if(argc  != 1){
        fprintf(2, "Usage: pingpong\n");
        exit(1);
    }
    int p[2];
    
    char buf[20];
    pipe(p);
    int pid;
    
    int o[2];
    pipe(o);
    pid = fork();
    if(0==pid){
        close(p[1]);
        close(o[0]);
        read(p[0], buf,  sizeof(buf));
        if(strlen(buf)){
            printf("%d: received %s\n", getpid(),buf);
        }   
        strcpy(buf, "pong");
        write(o[1],buf, sizeof(buf));
        close(o[1]);
        exit(0);
    }
    else{
        close(p[0]);
        strcpy(buf, "ping");
        write(p[1],buf, sizeof(buf));
        close(p[1]);
        read(o[0],buf, sizeof(buf));
        printf("%d: received %s\n", getpid(),buf);
        close(o[0]);
        wait(0);
        exit(0);

    }
   
}