#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
using std::cout;
using std::endl;
using std::string;

#define MAX_EVENT 16
#define INIT_BUF_SIZE 10
#define FD_SIZE MAX_EVENT+1
#define SERVPORT 4444
#define NUM 10 
#define REPLY_MSG "OK:1234567890"

//对应fd的接收字符串
char *recv_msg[NUM];
int total_len[NUM];
int used_size[NUM];
bool finish_flag[NUM];
int epoll_fd = -1;

char *adjust_buf(char **buf, int index) {
    //剩余一个字节，则重新分配
    char *new_buf = *buf;
    if (total_len[index] - used_size[index] <= 1) {
        total_len[index] *= 2;
        new_buf = new char[total_len[index]];
        memset(new_buf, 0x0, total_len[index]);
        ::memcpy(new_buf, *buf, strlen(*buf));
        delete[] *buf;
        *buf = NULL;
        *buf = new_buf;
    }
    return new_buf;
}

void set_fd_nonblock(int fd) {

    int flags = fcntl(fd, F_GETFL, 0);
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) != 0) {
        perror("F_SETFL ERROR");
    }
}

int onaccept(const int listen_fd) {
    struct sockaddr_in cli_addr;
    socklen_t addrlen = sizeof (struct sockaddr_in);
    int conn_fd = accept(listen_fd, (struct sockaddr *) &cli_addr, &addrlen);\

    //非阻塞模式
    set_fd_nonblock(conn_fd);

    struct epoll_event ev;
    ev.data.fd = conn_fd;
    ev.events = EPOLLET | EPOLLIN;
    int ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, conn_fd, &ev);
    if (ret != 0) {
        perror("epoll_ctl error");
        return -1;
    }
    char tmp[256] = {'\0'};
    sprintf(tmp, "a new connection from client:fd=%d;client-ip=%s;client-port=%d", conn_fd, inet_ntoa(cli_addr.sin_addr), cli_addr.sin_port);
    cout << tmp << endl;
    return conn_fd;
}

int onread(const int conn_fd) {
    int flag = 0;
    struct epoll_event ev;
    finish_flag[conn_fd] = false;
    if (recv_msg[conn_fd] == NULL) {
        recv_msg[conn_fd] = new char[INIT_BUF_SIZE];
        used_size[conn_fd] = 0;
        total_len[conn_fd] = INIT_BUF_SIZE;
    }
    while (1) {
        char *p = adjust_buf(&recv_msg[conn_fd], conn_fd);
        cout << "conn_fd=" << conn_fd << ",total_len=" << total_len[conn_fd] << ",used_size=" << used_size[conn_fd] << endl;
        int recv_len = read(conn_fd, p + used_size[conn_fd], total_len[conn_fd] - used_size[conn_fd] - 1);
        //recv_msg[conn_fd] = p;
        //-1- 异常
        if (recv_len < 0) {
            //non-block use
            if (errno == EAGAIN) {
                cout << "EAGAIN fd=" << conn_fd << endl;
                ev.data.fd = conn_fd;
                ev.events = EPOLLET | EPOLLIN;
                int ret = epoll_ctl(epoll_fd, EPOLL_CTL_MOD, conn_fd, &ev);
                if (ret != 0) {
                    perror("epoll ctl error");
                    return -1;
                }
                return 0;
            }
            cout << "recvlen < 0 fd=" << conn_fd << endl;
            ev.data.fd = conn_fd;
            int ret = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, conn_fd, &ev);
            if (ret != 0) {
                perror("epoll ctl error");
                return -2;
            }
            shutdown(conn_fd, SHUT_RDWR); //0 succ
            close(conn_fd);
            return -3;
        } else if (recv_len == 0) {
            //-2- 对端关闭
            cout << "peer dis-connect fd=" << conn_fd << endl;
            ev.data.fd = conn_fd;
            int ret = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, conn_fd, &ev);
            if (ret != 0) {
                perror("epoll ctl error");
                return -4;
            }
            shutdown(conn_fd, SHUT_RDWR); //0 succ
            close(conn_fd);
            return -5;
        }

        ev.data.fd = conn_fd;
        //-3- 判断是否读完
        cout << "judge end:" << (p + used_size[conn_fd] + recv_len - 2) << endl;
        if (strncmp(p + used_size[conn_fd] + recv_len - 2, "nn", 2) == 0) {
            p[used_size[conn_fd] + recv_len - 2] = '\0';
            finish_flag[conn_fd] = true;
        }
        if (finish_flag[conn_fd]) {
            cout << "recv msg finish(fd=" << conn_fd << "):whole-msg[" << recv_msg[conn_fd] << "],total_len[" << strlen(recv_msg[conn_fd]) << "]" << endl;
            ev.events = EPOLLET | EPOLLOUT;
            int ret = epoll_ctl(epoll_fd, EPOLL_CTL_MOD, conn_fd, &ev);
            if (ret != 0) {
                perror("epoll ctl error read");
                return -6;
            }
            finish_flag[conn_fd] = false;

            //接收完数据释放
            delete[] recv_msg[conn_fd];
            recv_msg[conn_fd] = NULL;
            return 0;
        } else {
            cout << "recv unfinish(fd=" << conn_fd << "),[part-msg=" << (p + used_size[conn_fd]) << ",recv_len=" << recv_len << "],continue to read!" << endl;
            used_size[conn_fd] += recv_len;
            ev.events = EPOLLET | EPOLLIN;
            int ret = epoll_ctl(epoll_fd, EPOLL_CTL_MOD, conn_fd, &ev);
            if (ret != 0) {
                perror("epoll ctl error read");
                return -7;
            }
        }
    }//while

    return -8;
}

int onwrite(const char *msg, const int fd) {
    assert(msg != NULL);
    //head
    char head_len[8];
    sprintf(head_len, "%d", strlen(msg));
    int ret = send(fd, head_len, strlen(head_len), 0);
    if (ret <= 0) {
        cout << "write head error" << endl;
        return -1;
    }
    cout << "write msg head success(" << fd << ") head=" << head_len << ",len=" << ret << endl;

    //body
    char tmp[16];
    int write_len = -1;
    int left_len = strlen(msg);
    int cur_pos = 0;
    do {
        write_len = write(fd, msg + cur_pos, 8);
        if (write_len == -1) {
            //cout << "write error fd=" << fd << ":" << errno << "-" << strerror(errno) << endl;
            perror("write error:");
            return -2;
        } else {
            strncpy(tmp, msg + cur_pos, write_len);
            cout << "write msg succ to fd=" << fd << "[msg=" << tmp << ",len=" << write_len << "]" << endl;
        }
        cur_pos += write_len;
        left_len -= write_len;

    } while (left_len > 0);

    struct epoll_event ev;
    ev.events = EPOLLET | EPOLLIN;
    ev.data.fd = fd;
    int ret2 = epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &ev);
    if (ret2 != 0) {
        perror("epoll ctl error write");
        return -3;
    }
    return 0;
}

void *onepoll(void *args) {
    int listen_fd = (int) (*((int*) args));
    epoll_fd = epoll_create(FD_SIZE);
    struct epoll_event ev;
    struct epoll_event epoll_events[MAX_EVENT];

    ev.data.fd = listen_fd;
    ev.events = EPOLLET | EPOLLIN;
    int ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev);
    if (ret != 0) {
        pthread_exit(NULL);
    }

    for (;;) {
        //--block indefinitely
        //int count = epoll_wait(epoll_fd, epoll_events, MAX_EVENT, -1);

        //--nonblock
        int count = epoll_wait(epoll_fd, epoll_events, MAX_EVENT, 0);
        switch (count) {
        case 0:
            //cout << "nothing happen...\n";
            break;
        case -1:
            cout << "error event happen error:" << strerror(errno) << endl;
            break;
        default:
            //cout << "the event we expect happen count="<<count<<endl;
            break;

        }
        for (int index = 0; index < count; ++index) {
            if (epoll_events[index].data.fd == listen_fd) {
                //accept
                cout << "connect event happen fd=" << listen_fd << endl;
                if (onaccept(listen_fd) < 0) {
                    perror("onaccept fail");
                    continue;
                }
            } else if (epoll_events[index].events & EPOLLIN) {
                //readable
                cout << "readable event happen fd=" << epoll_events[index].data.fd << endl;
                int conn_fd = epoll_events[index].data.fd;
                if (onread(conn_fd) != 0) {
                    perror("onread fail");
                    continue;
                }
            } else if (epoll_events[index].events & EPOLLOUT) {
                //writeable
                cout << "writeable event happen fd=" << epoll_events[index].data.fd << endl;
                int conn_fd = epoll_events[index].data.fd;
                if (onwrite(REPLY_MSG, conn_fd) != 0) {
                    perror("onwrite fail");
                    continue;
                }
            } else {
                cout << "other events happen..." << endl;
            }
        }
        usleep(100);
    }
}

int main(int argc, char *argv[]) {
    cout << "##welcome to server##" << endl;
    pthread_attr_t attr;
    pthread_t p_id;
    //--/usr/include/netinet/in.h
    struct sockaddr_in *myaddr = new sockaddr_in;
    myaddr->sin_family = AF_INET;
    myaddr->sin_port = htons(SERVPORT);
    myaddr->sin_addr.s_addr = INADDR_ANY;
    //myaddr->sin_addr.s_addr=inet_addr("192.168.0.1");

    socklen_t s_len = sizeof (struct sockaddr_in);
    //tcp
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        cout << "create socket error!" << endl;
        return -1;
    }

    int ret = bind(fd, (struct sockaddr *) myaddr, s_len);
    if (ret != 0) {
        cout << "bind error!" << endl;
        return -1;
    }

    ret = listen(fd, 10);
    if (ret != 0) {
        cout << "listen error!" << endl;
        return -1;
    }
    for (int k = 0; k < NUM; k++) {
        finish_flag[k] = false;
    }
    pthread_t pid_1;
    pthread_create(&pid_1, NULL, &onepoll, &fd);

    //wait for thread to return
    void *s1 = NULL;
    pthread_join(pid_1, &s1);
    cout << "Thread 1 returns:" << (char *) s1 << endl;

    //release resource
    close(fd);
    close(epoll_fd);
    delete myaddr;
    return 0;
}
