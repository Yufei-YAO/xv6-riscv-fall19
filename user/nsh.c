
#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#define EXEC   5
#define PIPE   2
#define LT      3
#define GT    4
#define READCOMAND 1
#define READARGS 6

void strcat(char *d,char *s){
    int from=0;
    while(d[from]!='\0'){
        d++;
    }
    int ss=0;
    while(s[ss]!='\0'){
        d[from++]=s[ss++];
    }
    return;
}

void prasecmd(char *p){

    char argsbuf[50][30];
    char *(args[50]);
    int end =strlen(p);
    int now =0;
    int start =0;
    int state=0;
    int index=0;
    int pid;
    char command[20];
    int pe[2];
    char fbuf[40];
    while(now!=end){

        switch(state){
            case 0:{
                argsbuf[0][0]='\0';
                args[0]=0;
                if(p[now]==' '){
                    now++;
                    state=0;
                }
                else {
                    start=now;
                    state=READCOMAND;
                }
            }break;
            case READCOMAND:{
                index =0;
                while(now<end&&p[now]!=' '){
                    now++;
                }
              
                int t=0;
                for(int i=start;i!=now;i++){
                    command[t++] = p[i];
                }
                command[t]='\0';
                t=0;
                for(int i=start;i!=now;i++){
                    argsbuf[index][t++] = p[i];
                }
                argsbuf[index][t]='\0';
                args[index]=argsbuf[index];
                args[++index]=0;
                state =READARGS;
                
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
                now++;
                if((pid=fork())==0){  //子
                    close(pe[0]);
                    close(1);
                    dup(pe[1]);
                    exec(command,args);
                    printf("exec %s failed\n",command);
                    exit(-1);
                }else{     //fuqin
                    close(pe[1]);
                    close(0);
                    dup(pe[0]);
                    wait(0);
                    state= 0;
                }
            }break;
            case LT:{
                now++;
                while(now!=end&&p[now]==' '){
                    now++;
                }
                start =now;
                while(now!=end&&p[now]!=' '){
                    now++;
                }
                int tmpf=0;
                if(p[start]=='/'||p[start]=='.'){
                    for(int i=start;i<now;i++){
                        fbuf[tmpf++]=p[i];
                    }
                    fbuf[tmpf]='\0';
                }else{
                    for(int i=start;i<now;i++){
                        fbuf[tmpf++]=p[i];
                    }
                    fbuf[tmpf]='\0';
                }
                close(0);
                open(fbuf,O_RDWR);
                state = READARGS;
            }break;
            case GT:{
                now++;
                while(now!=end&&p[now]==' '){
                    now++;
                }
                start =now;
                while(now!=end&&p[now]!=' '){
                    now++;
                }
                int tmpf=0;
                if(p[start]=='/'||p[start]=='.'){
                    for(int i=start;i<now;i++){
                        fbuf[tmpf++]=p[i];
                    }
                    fbuf[tmpf]='\0';
                }else{
                    for(int i=start;i<now;i++){
                        fbuf[tmpf++]=p[i];
                    }
                    fbuf[tmpf]='\0';
                }
                close(1);
                open(fbuf,O_RDWR | O_CREATE);
                state = READARGS;
            }break;
            default:{

            }break;
        }
    }
    wait(0);
    exec(command,args);
    printf("exec %s failed\n",command);
    exit(-1);
    
}
int main(void){
    char buf[100];
    char sbuf[80];
    printf("@");
    while(strlen(gets(sbuf,80))!=0){
        memset(buf,0,sizeof(buf));
        if(strcmp("exit",sbuf)==0){
            break;
        }
        int slen = strlen(sbuf);
        if(sbuf[slen-1]=='\n'){
            sbuf[slen-1]='\0';
            slen = strlen(sbuf);
        }
        while(sbuf[slen-1]=='\\'){
            strcat(buf,sbuf);
            memset(sbuf,0,sizeof(sbuf));
            gets(sbuf,80);
            if(sbuf[slen-1]=='\n'){
                sbuf[slen-1]='\0';
                slen = strlen(sbuf);
            }
        
            if(strlen(sbuf)==0){
                break;
            }
        }
        strcat(buf,sbuf);
        int pid=fork();
        if(pid==0){
            prasecmd(buf);
            exit(-1);
        }
        wait(0);
        memset(sbuf,0,sizeof(sbuf));
        printf("@");
    }
    exit(-1);
}
