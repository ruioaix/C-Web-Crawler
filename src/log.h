#ifndef __LOG_H
#define __LOG_H

enum LOGLEVEL {
	LOG_FATAL,
	LOG_WARN,
	LOG_OP,
	LOG_INFO,
	LOG_DBG,
};

#include <stdio.h>
#include <stdlib.h>

#define LOG(level, ...) do {\
	if (level <= loglevel) {\
		if (logfile == stdout || logfile == stderr) { \
			if (level==LOG_FATAL) fprintf(logfile, "[\033[7;31m%17s\033[1;37m:\033[1;34m%-4d \033[1;35m%12s()\033[0m] =>> ", __FILE__, __LINE__, __FUNCTION__);\
			if (level==LOG_WARN)  fprintf(logfile, "[\033[1;33m%17s\033[1;37m:\033[1;34m%-4d \033[1;35m%12s()\033[0m] =>> ", __FILE__, __LINE__, __FUNCTION__);\
			if (level==LOG_OP)    fprintf(logfile, "[\033[7;36m%17s\033[1;37m:\033[1;34m%-4d \033[1;35m%12s()\033[0m] =>> ", __FILE__, __LINE__, __FUNCTION__);\
			if (level==LOG_INFO)  fprintf(logfile, "[\033[1;32m%17s\033[1;37m:\033[1;34m%-4d \033[1;35m%12s()\033[0m] =>> ", __FILE__, __LINE__, __FUNCTION__);\
			if (level==LOG_DBG)   fprintf(logfile, "[\033[7;37m%17s\033[1;37m:\033[1;34m%-4d \033[1;35m%12s()\033[0m] =>> ", __FILE__, __LINE__, __FUNCTION__);\
		} \
		else { \
			fprintf(logfile, "[%17s:%-4d %12s()] =>> ", __FILE__, __LINE__, __FUNCTION__);\
		} \
		fprintf(logfile, __VA_ARGS__);\
		fprintf(logfile, "\n");\
		fflush(logfile);\
	}\
	if (level == LOG_FATAL) {\
		exit(-1);\
	}\
} while(0)

extern enum LOGLEVEL loglevel;
extern FILE *logfile;

#endif
