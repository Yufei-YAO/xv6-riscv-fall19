
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int
main(int argc, char *argv[])
{
    char * args[MAXARG];
    char buf[50];
    int ac=0;
    for(int i=1;i<argc;i++){
        args[ac++]=argv[i];
    }
    args[ac]=0;
    char *p=buf;
    int len;
    int d=ac;
    while((len=read(0,buf,sizeof(buf)))!=0){
        for(int i=0;i<len;i++){
            if(buf[i]=='\n'){
                buf[i]=0;
                args[ac++]=p;
                args[ac]=0;
                ac=d;
                p=buf;
                if(fork()==0){
                    exec(argv[1],args);
                }
                wait(0);
            }else if(buf[i]==' '){
                buf[i]=0;
                args[ac++]=p;
                p=&buf[i+1];
            }
        }
    }
    exit(0);
}