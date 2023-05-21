#include "clientfunc.h"

void *receive_msg(void *arg) {
    int sockfd = *(int *)arg;
    while (1) {
        int size, res, code;
        res = read(sockfd, &code, sizeof(int));
        if (res == -1) {
            perror("Receiving code error");
            exit(1);
        }
        if (res == 0) {
            printf("Server disabled\n");
            exit(0);
        }

        if (code == 0) {
            res = read(sockfd, &size, sizeof(int));
            if (res == -1) {
                perror("Receiving size error");
                exit(1);
            }
            if (res == 0) {
                printf("Server disabled\n");
                exit(0);
            }

            char *msg = malloc(sizeof(char) * size);
            res = read(sockfd, msg, size);
            if (res == -1) {
                perror("Receiving message error");
                exit(1);
            }

            if (res == 0) {
                printf("Server disabled\n");
                free(msg);
                exit(0);
            } else {
                printf("%s\n", msg);
                free(msg);
            }
        }
        if (code == 1) {
            int count, number;
            read(sockfd, &count, sizeof(int));

            printf("+++++++++++++++++++\n");
            printf("Members (Total: %d):\n", count);
            for (int i = 0; i < count; i++) {
                read(sockfd, &number, sizeof(int));
                printf("   - User-%d\n", number - 3);
            }
            printf("+++++++++++++++++++\n");
        }
    }
}

void *send_msg(void *arg) {
    int sockfd = *(int *)arg;
    while (1) {
        char *msg = get_string();
        if (msg) {
            int size = strlen(msg) + 1;
            int res = write(sockfd, msg, size);

            if (res == -1) {
                perror("Send error");
                exit(1);
            }

            if (res == 0 || strcmp(msg, "/exit") == 0) {
                exit(0);
            }

            free(msg);
        }
    }
}
void error_msg(int condition, const char *format, ...) {
    if (condition) {
        va_list argp;
        va_start(argp, format);
        vprintf(format, argp);
        va_end(argp);
        exit(EXIT_FAILURE);
    }
}

char *get_string() {
    char *string = malloc(sizeof(char));
    int i = 0;
    char ch;

    while ((ch = getchar()) != '\n') {
        string[i++] = ch;
        void *tmp = realloc(string, sizeof(char) * (i + 1));
        error_msg(!tmp, "Realloc error.\n");
        string = tmp;
    }
    string[i] = '\0';

    if (i) return string;

    free(string);
    return NULL;
}