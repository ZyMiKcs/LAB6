#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void *receive_msg(void *arg);
void *send_msg(void *arg);

char *get_string(void);
void error_msg(int condition, const char *fmt, ...);