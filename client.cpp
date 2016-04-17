//--client.cpp
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>//用于结构struct hostent
#include <pthread.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using std::cout;
using std::cin;
using std::endl;
using std::string;

#define SERVER_PORT 4444

int getIpbyHostName(char *ip) {
    char **pptr;
    struct hostent *hptr;
    char str[32];

    char hostname[64];
    if (gethostname(hostname, sizeof (hostname)) != 0) {
        printf(" gethostname error for host:%s\n", hostname);
        return -1;
    }

    printf("hostname:%s\n", hostname);
    if ((hptr = gethostbyname(hostname)) == NULL) {
        printf(" gethostbyname error for host:%s\n", hostname);
        return -2;
    }

    printf("official hostname:%s\n", hptr->h_name);
    for (pptr = hptr->h_aliases; *pptr != NULL; pptr++)
        printf(" alias:%s\n", *pptr);

    switch (hptr->h_addrtype) {
    case AF_INET:
    case AF_INET6:
        pptr = hptr->h_addr_list;
        for (; *pptr != NULL; pptr++)
            printf(" address:%s\n",
                   inet_ntop(hptr->h_addrtype, *pptr, str, sizeof (str)));
        ip = (char *) inet_ntop(hptr->h_addrtype, hptr->h_addr, str, sizeof (str));
        //应该需要拷贝？？
        if (ip == NULL) {
            return -3;
        }
        printf(" first address: %s\n", ip);
        break;
    default:
        printf("unknown address type\n");
        break;
    }

    return 0;
}

bool judge_recv(const int recv_len,const int conn_fd) {
    if (recv_len < 0) {
        //-1- 异常
        //non-block use
        if(errno == EAGAIN) {
            cout << "EAGAIN fd=" << conn_fd << endl;
            return 1;
        }
        cout << "recv_len<0, fd=" << conn_fd  << endl;
        shutdown(conn_fd, SHUT_RDWR); //0 succ
        close(conn_fd);
        perror("recv_len<0");
        return -1;
    } else if (recv_len == 0) {
        //-2- 对端关闭
        cout << "peer dis-connnect(error="<<strerror(errno)<<"), fd=" << conn_fd << endl;
        shutdown(conn_fd, SHUT_RDWR); //0 succ
        close(conn_fd);
        return -2;
    }
    return 0;
}

void set_fd_nonblock(int fd) {

    int flags = fcntl(fd, F_GETFL, 0);
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) != 0) {
        perror("F_SETFL ERROR");
    }
}

void *onread(void *args) {
    int conn_fd = (int) (*((int*) args));
    char msg[8];
    string recv_msg;

    while (true) {
        recv_msg.clear();
        //包头：记录包体的长度
        memset(msg, 0x0, sizeof(msg));
        int recv_len = read(conn_fd, msg, sizeof(msg));
        if (judge_recv(recv_len,conn_fd) < 0) {
            sleep(5);
            exit(-1);
        }

        int body_len = atoi(msg);
        if (body_len < 0) {
            perror("head length < 0");
            exit(-3);
        } else {
            cout << "recv head succ(fd=" << conn_fd << ") body_len:" << body_len << endl;
        }

        //包体
        while (1) {
            memset(msg, 0x0, sizeof (msg));
            recv_len = read(conn_fd, msg, sizeof (msg));
            if (judge_recv(recv_len,conn_fd) < 0) {
                exit(-2);
            } else if(judge_recv(recv_len,conn_fd) == 1) {
                sleep(3);//fortest
                continue;
            }

            //-3- 判断是否读完
            body_len -= recv_len;
            if (recv_len != sizeof(msg) || body_len <= 0) {
                cout << "recv last msg(fd=" << conn_fd << "):" << msg << endl;
                recv_msg += msg;
                cout << "recv msg finish(fd=" << conn_fd << "):" << recv_msg << endl;
                break;
            } else {
                cout << "recv unfinish(fd=" << conn_fd << "),[msg=" << msg << ",recv_len=" << recv_len << "],continue to read!" << endl;
                recv_msg += msg;
            }
        }//while(1))
        usleep(5);
    }//while(true)
}

void *onwrite(void *args) {
    int conn_fd = (int) (*((int*) args));
    char head_len[8];
    while (1) {
        char msg[1024];
        cout << "input the msg to send(1024 bytes max):\n";
        cin>>msg;

        int ret = send(conn_fd, msg, strlen(msg), 0);
        if (ret <= 0) {
            cout << "write write error" << endl;
            exit(3);
        }
        cout << "write msg body success(" << conn_fd << ") msg=" << msg << ",len=" << ret << endl;

    }
}

int onconnect() {
    //--/usr/include/netinet/in.h
    struct sockaddr_in *myaddr = new sockaddr_in;
    myaddr->sin_family = AF_INET;
    myaddr->sin_port = htons(SERVER_PORT);
    myaddr->sin_addr.s_addr = INADDR_ANY;
    char server_ip[32] = {'\0'};
    if (getIpbyHostName(server_ip) != 0) {
        cout << "getIpbyHostName error!" << endl;
        return -1;
    }
    inet_pton(AF_INET, server_ip, (void *) &(myaddr->sin_addr));

    //tcp
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        cout << "create socket error!" << endl;
        return -2;
    }
    socklen_t s_len = sizeof (struct sockaddr_in);
    while (true) {
        int ret = connect(fd, (struct sockaddr *) myaddr, s_len);
        if (ret != 0) {
            cout << "connect fail:" << strerror(errno) << endl;
            sleep(10);
            cout << "reconnect..." << endl;
            continue;
        }

        cout << "connect succ on fd=" << fd << endl;
        //非阻塞模式
        set_fd_nonblock(fd);
        break;
    }

    pthread_t pid_1, pid_2;
    pthread_create(&pid_1, NULL, onwrite, &fd);
    pthread_create(&pid_2, NULL, onread, &fd);

    //--等待线程结束
    void *s1 = NULL;
    void *s2 = NULL;
    pthread_join(pid_2, &s1);
    cout << "Thread 2 returns:" << (char *) s1 << endl;
    //pthread_cancel(pid_1);
    pthread_join(pid_1, &s2);
    cout << "Thread 1 returns:" << (char *) s2 << endl;

    //close
    close(fd);
    delete myaddr;

    return 0;
}

int main(int argc, char *argv[]) {
    cout << "##I am Client##" << endl;

    pid_t pid;
    if ((pid = fork()) > 0) {
        cout << "in parent ...\n";
    } else if (pid == 0) {
        cout << "in child ...\n";
        onconnect();
    } else {
        cout << "fork error ...\n";
        return -1;
    }

    //signal(SIGCHLD, sig_handle);

    int status = -1;
    while ((pid = waitpid(-1, &status, 0)) >= 0) {
        if (WIFEXITED(status)) {
            WEXITSTATUS(status);
        }
        cout << "catch signal SIGCHLD pid=" << pid << ",exit_status=" << status << endl;
        pid_t a;
        a = fork();
        if (a == 0) {
            cout << "fork a new child..." << endl;
            onconnect();
            sleep(5);
        }

    }

    return 0;
}
