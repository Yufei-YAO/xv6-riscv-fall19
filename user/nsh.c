
#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#define EXEC   5
#define PIPE   2
#define LT      3
#define GT    4
#define READCOMAND 1
#define READARGS 6
void prasecmd(char *p){

    char argsbuf[10][20];
    char *(args[10]);
    int end =strlen(p);
    int now =0;
    int start =0;
    int state=0;
    int index;
    int pid;
    char command[20];
    int pe[2];
    char fbuf[20];
    while(now!=end){

        switch(state){
            case 0:{
                
                if(p[now]==' '){
                    now++;
                    state=0;
                }
                else {
                    state=READCOMAND;
                }
            }break;
            case READCOMAND:{
                while(now<end&&p[now]==' '){
                    now++;
                }
                int t=0;
                for(int i=start;i!=now;i++){
                    command[t++] = p[i];
                }
                command[t]='\0';
                state =READARGS;
                index=0;
                args[index]=0;
                

            }break;
            case READARGS:{
                if(p[now]==' '){
                    now++;
                }
                else if(p[now]=='|'){
                    state =PIPE;
                }else if(p[now]=='<'){
                    state =LT;
                }else if(p[now]=='>'){
                    state =GT;
                }else {
                    start = now;
                    while(now!=end &&p[now]!=' '){
                        now++;
                    }
                    int tp=0;
                    for(int i=start;i<now;i++){
                        argsbuf[index][tp++]=p[i];
                    }
                    argsbuf[index][tp]='\0';
                    args[index]=argsbuf[index];
                    args[++index]=0;
                    state =READARGS;
                }
            }break;
            case PIPE:{
                pipe(pe);
                if((pid=fork())==0){  //子
                    close(pe[0]);
                    close(1);
                    dup(pe[1]);
                    exec(command,args);
                    exit(-1);
                }else{     //fuqin
                    close(pe[1]);
                    close(0);
                    dup(pe[0]);
                    wait(0);
                    state = 0;
                }
            }break;
            case LT:{
                now++;
                while(now!=end&&now==' '){
                    now++;
                }
                start =now;
                while(now!=end&&now!=' '){
                    now++;
                }
                int tmpf=0;
                if(p[start]=='/'||p[start]=='.'){
                    for(int i=start;i<now;i++){
                        fbuf[tmpf++]=p[i];
                    }
                    fbuf[tmpf]='\0';
                }else{
                    fbuf[0]='.';
                    fbuf[1]='/';
                    tmpf=2;
                    for(int i=start;i<now;i++){
                        fbuf[tmpf++]=p[i];
                    }
                    fbuf[tmpf]='\0';
                }
                close(0);
                open(fbuf,O_RDONLY);
                state = READARGS;
            }break;
            case GT:{
                now++;
                while(now!=end&&now==' '){
                    now++;
                }
                start =now;
                while(now!=end&&now!=' '){
                    now++;
                }
                int tmpf=0;
                if(p[start]=='/'||p[start]=='.'){
                    for(int i=start;i<now;i++){
                        fbuf[tmpf++]=p[i];
                    }
                    fbuf[tmpf]='\0';
                }else{
                    fbuf[0]='.';
                    fbuf[1]='/';
                    tmpf=2;
                    for(int i=start;i<now;i++){
                        fbuf[tmpf++]=p[i];
                    }
                    fbuf[tmpf]='\0';
                }
                close(1);
                open(fbuf,O_WRONLY);
                state = READARGS;
            }break;
            default:{

            }break;
        }
    } 
    if((pid=fork())==0){
        exec(command,args);
    }else{
        wait(0);
    }
}

int main(void){

    char buf[512];
    printf("@ ");
    int len;
    while((len = read(0,(void*)buf,512))!=0){
        buf[511]='\0';
        prasecmd(buf);
        printf("@ ");
    }


}