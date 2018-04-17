#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/stat.h>
#include <stdarg.h>

#define LOG_PREFIX_BUF_SIZE 1024

void *log_init(const char *prefix)
{
#ifdef LOG_TO_STDOUT
	return NULL;
#else
	char *syslog_prefix = (char*) malloc(LOG_PREFIX_BUF_SIZE);
	if (syslog_prefix) {
		snprintf(syslog_prefix, LOG_PREFIX_BUF_SIZE, "%s", prefix);
		openlog(syslog_prefix, LOG_PERROR | LOG_PID, LOG_USER);
	}
	return syslog_prefix;
#endif
}

void log_destroy(void *log_buf)
{
#ifndef LOG_TO_STDOUT
	if (log_buf) {
		free(log_buf);
	}
#endif
}

