
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    if(argc < 2){
        fprintf(2, "Usage: sleep time\n");
        exit(1);
    }
    int a =atoi(argv[1]);
    if(a<0){
        fprintf(2, "time should be positive\n");
        exit(1);
    }
    
    sleep(a);
    exit(0);
}