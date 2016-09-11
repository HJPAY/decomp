/* decomp.c */

/* 
 * decompress minified javascript.
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

#define APPNAME "decomp"
#define TRUE 1
#define FALSE 0


static int s_indents = 0;
static int s_spaces = 4;

static void usage(void);
static int putspace(void);

int main(int argc, char *argv[])
{
    struct stat st;
    int ret = 1;
    FILE *fp = NULL;
    int c = 0;
    int opt = 0;
    int err = 0;


    if (argc < 2) {
        usage();
        return ret;
    }

    while((opt = getopt(argc, argv, "i:")) != -1) {
        switch (opt) {
        case 'i':
            s_spaces = atoi(optarg);
            break;
        default:
            usage();
            exit(EXIT_FAILURE);
        }
    }


    /****************/
    /* main process */
    /****************/
    memset(&st, 0x00, sizeof(st));
    if (stat(argv[optind], &st) == -1) {
        fprintf(stdout, "cannot find the file.\n");
        exit(EXIT_FAILURE);
    }

    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "error:cannot open the file.\n");
        exit(EXIT_FAILURE);
    }

    while (!((c = fgetc(fp)) == EOF)) {
        switch (c) {
        case '{':
            fprintf(stdout, "{\n");
            s_indents++;
            err = putspace();
            break;
        case ';':
            fprintf(stdout, ";\n");
            err = putspace();
            break;
        case '}':
            fprintf(stdout, "\n");
            s_indents--;
            err = putspace();
            fprintf(stdout, "}\n");
            err = putspace();
            break;
        default:
            fprintf(stdout, "%c", c);
            break;
        }
        if (err) {
            fclose(fp);
            fprintf(stderr, "\nparse error!!\n");
            exit(EXIT_FAILURE);
        }
    }
    fclose(fp);
    exit(EXIT_SUCCESS);
}

/* 
 * return: is error occured
 */
int putspace(void)
{
    char *space = NULL;

    space = malloc(sizeof(char) * s_spaces * s_indents);
    if (space == NULL) {
        return TRUE;
    }
    memset(space, ' ', sizeof(char) * s_spaces * s_indents);
    fprintf(stdout, "%s", space);
    free(space);
    return FALSE;
}

void usage(void)
{
    fprintf(stdout, "Usage: "APPNAME" [-i indents] file\n");
}
