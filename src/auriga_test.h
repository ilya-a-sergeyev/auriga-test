#ifndef __AURIGA_TEST__
#define __AURIGA_TEST__

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <jpeglib.h>
#include <syslog.h>


#define MAX_FILENAME 1024
#define LOG_TO_STDOUT

/* log subsystem */
void *log_init(const char *prefix);
void log_destroy(void *log_buf);

#ifdef LOG_TO_STDOUT
#define log_inf(...) fprintf(stdout, __VA_ARGS__)
#define log_err(...) fprintf(stderr, __VA_ARGS__)
#else
#define log_err(...) syslog(LOG_ERR, __VA_ARGS__)
#define log_inf(...) syslog(LOG_INFO, __VA_ARGS__)
#endif


/* program arguments and parser */
extern char out_filename[];
extern char in_filename[];

int parse_av(int argc, char **argv);

#endif
