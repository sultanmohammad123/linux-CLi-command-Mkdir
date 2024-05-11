#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


#define FALSE 0
#define TRUE 1

static int parentFlag = FALSE;
static mode_t mode = 0777;

int parse_mode(const char *modeStr, mode_t *mode) {
  
    
    return TRUE;
}

void createPath(char *path, mode_t mode) {
    char *p;
    for (p = strchr(path + 1, '/'); p; p = strchr(p + 1, '/')) {
        *p = '\0';
        mkdir(path, mode);
        *p = '/';
    }

    mkdir(path, mode);
}

int mkdir_main(int argc, char **argv) {
    int i = FALSE;
    argc--;
    argv++;

    while (argc > 0 && **argv == '-') {
        while (i == FALSE && *++(*argv)) {
        
            switch (**argv) {
                case 'm':
                    if (--argc == 0)
                        return FALSE;
                    mode = 0;
                    if (parse_mode(*(++argv), &mode) == FALSE) {
                        return FALSE;
                    }
                    umask(0);
                    i = TRUE;
                    break;
                case 'p':
                    parentFlag = TRUE;
                    break;
                default:
                    return FALSE;
            }
        }
        argc--;
        argv++;
    }

    if (argc < 1) {
        return FALSE;
    }

    while (argc > 0) {
        char buf[PATH_MAX];
        strcpy(buf, *argv);

        if (parentFlag == TRUE) {
            createPath(buf, mode);
        } else {
            mkdir(buf, mode);
        }

        argc--;
        argv++;
    }

    return TRUE;
}
int main(int argc, char **argv){
        return mkdir_main(argc, argv);
} 
