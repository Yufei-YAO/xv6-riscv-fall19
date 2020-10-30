#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), 0, DIRSIZ-strlen(p));
  return buf;
}


void find(char * path,char*name){
    char buf[50],*p;
    struct dirent de;
    struct stat st;
    int fd;
    if((fd = open(path, 0)) < 0){
      fprintf(2, "find: %s: No such file or directory\n", path);
      return;
    }

    if(fstat(fd, &st) < 0){
      fprintf(2, "find: %s: Cannot stat\n", path);
      close(fd);
      return;
    }
    switch(st.type){
        case T_FILE:
            if(0==strcmp(fmtname(path), name)){
                printf("%s\n",path);
            }
            break;
        case T_DIR:
            strcpy(buf, path);
            p = buf+strlen(buf);
            *p++ = '/';
            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                if(de.inum == 0|| de.inum == 1)
                  continue;
                if(strcmp(de.name,".")==0||strcmp(de.name,"..")==0){
                  continue;
                }
                memmove(p, de.name, DIRSIZ);
                if(stat(buf, &st) < 0){
                    printf("find: %s:Cannot stat \n", buf);
                    continue;
                }
                find(buf, name);
            }
      }
    close(fd);
}

int
main(int argc, char *argv[])
{
    if(argc  <1 || argc>3){
        fprintf(2, "Usage: find <dir> filename\n");
        exit(1);
    }
    if(argc==2){
        find(".",argv[1]);
    }
    else{
        find(argv[1],argv[2]);
    }
    exit(0);
}
