#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/*
 Usage message displayed when incorrect command-line arguments are provided.
 */
static const char mkdir_usage[] = "Usage: mkdir [OPTION] DIRECTORY...\n\n"
"Create the directory if they do not already exist\n\n"
"Options:\n"
"\t-m\tset permission \n"
"\t-p\tno error if existing, make parent directories as needed\n";

#define FALSE 0
#define TRUE 1

static int parentFlag = FALSE;  
static mode_t mode = 0777;      

/*
     message: The error message to display.
 */
void usage(const char *message) {
    fprintf(stderr, "%s", message);
    exit(FALSE);
}

/*
  Parse the permission mode argument.
 */
int parse_mode(const char *modeStr, mode_t *mode) {
    char *endptr;
    long int parsedMode = strtol(modeStr, &endptr, 8); // Parse as octal
    if (*endptr != '\0' || parsedMode < 0 || parsedMode > 0777) { // Check for valid octal
        return FALSE;
    }
    *mode = (mode_t)parsedMode;
    return TRUE;
}

/*
  Create the parent directories of the given path.
 */
void createPath(char *path, mode_t mode) {
    char *p;
    for (p = strchr(path + 1, '/'); p; p = strchr(p + 1, '/')) {
        *p = '\0';
        if (mkdir(path, mode) == -1) {
            if (errno != EEXIST) {
                perror(path);
                exit(FALSE);
            }
        }
        *p = '/';
    }

    if (mkdir(path, mode) == -1) {
        if (errno != EEXIST) {
            perror(path);
            exit(FALSE);
        }
    }
}

 
int mkdir_main(int argc, char **argv) {
    int i = FALSE;
    argc--;
    argv++;

    /* Parse any options */
    while (argc > 0 && **argv == '-') {
        while (i == FALSE && *++(*argv)) {
            switch (**argv) {
                case 'm':
                    if (--argc == 0)
                        usage(mkdir_usage);
                    /* Find the specified modes */
                    if (parse_mode(*(++argv), &mode) == FALSE) {
                        fprintf(stderr, "Unknown mode: %s\n", *argv);
                        exit(FALSE);
                    }
                    i = TRUE;
                    break;
                case 'p':
                    parentFlag = TRUE;
                    break;
                default:
                    usage(mkdir_usage);
            }
        }
        argc--;
        argv++;
    }

    if (argc < 1) {
        usage(mkdir_usage);
    }

    while (argc > 0) {
        char buf[PATH_MAX];
        strcpy(buf, *argv);

        if (parentFlag == TRUE) {
            createPath(buf, mode);
        } else {
            if (mkdir(buf, mode) == -1) {
                if (errno != EEXIST) {
                    perror(buf);
                    exit(FALSE);
                }
            }
        }

        argc--;
        argv++;
    }
    return TRUE;
}

int main(int argc, char **argv) {
    return mkdir_main(argc, argv);
}
