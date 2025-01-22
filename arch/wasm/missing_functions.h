#ifndef UA_MISSING_FUNCTIONS_H_
#define UA_MISSING_FUNCTIONS_H_

#define SO_REUSEADDR 2
#define SO_REUSEPORT 15

#include <sys/types.h> 
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <wasi_socket_ext.h>

static int pipe2(int pipefd[2], int flags) {
    return 0;
}

static const char *gai_strerror(int errcode) {
    return "error";
}

static int getnameinfo(const struct sockaddr *sa, socklen_t salen, char *host, size_t hostlen, char *serv, size_t servlen, int flags) {
    return 0;
}

#define AI_PASSIVE 1
#define NI_NUMERICHOST 1
#define SO_ERROR 4

#endif /* UA_MISSING_FUNCTIONS_H_ */
