//--client.cpp
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using std::cout;
using std::cin;
using std::endl;

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 4444

void *onread(void *args) {
    int conn_fd = (int) (*((int*) args));
    //    struct sockaddr_in cli_addr;
    //    socklen_t addrlen = sizeof (struct sockaddr_in);
    char msg[10];
    while (true) {
        bool finish = false;
        std::string msg_from_server;
        while (!finish) {
            memset(msg, 0x0, sizeof (msg));
            //block default
            //non-block
            //when no data EAGAIN occur
            //int recv_len = recv(conn_fd,msg,sizeof(msg),MSG_DONTWAIT);
            //cout<<"recv msg from server len="<<recv_len<<endl;
            //-1- 异常
            if (recv_len < 0) {
                cout << "recv error:" << strerror(errno) << endl;
                //EAGAIN for non-block recv
                /*if(errno == EAGAIN) {
                  break;
                  }*/
                close(conn_fd);
                exit(1);
            } else if (recv_len == 0) {
                //-2- 对端关闭
                cout << "peer dis-connect fd=" << conn_fd << endl;
                //for CLOSE_WAIT
                close(conn_fd);
                exit(2);
            }

            //-3- 判断是否读完
            if (recv_len != sizeof (msg)) {
                cout << "recv last msg from server(" << conn_fd << "):" << msg << endl;
                msg_from_server += msg;
                cout << "recv msg finish from server(" << conn_fd << "):" << msg_from_server << endl;
                finish = true;
            } else {
                cout << "recv unfinish(" << msg << "),continue to read!" << endl;
                msg_from_server += msg;
            }
        }
    }
}

void *onread2(void *args) {
    int conn_fd = (int) (*((int*) args));
    char msg[8];
    string recv_msg;

    while (true) {
        //包头：记录包体的长度
        memset(msg, 0x0, sizeof (msg));
        int recv_len = read(conn_fd, msg, sizeof (msg));
        //-1- 异常
        if (recv_len < 0) {
            cout << "recv error!" << endl;
            perror("recv_len < 0");
            continue;
        } else if (recv_len == 0) {
            //-2- 对端关闭
            cout << "peer dis-connnect fd=" << conn_fd << endl;
            close(conn_fd);
            perror("recv_len = 0");
            continue;
        }
        int body_len = atoi(msg);
        if (body_len < 0) {
            perror("head length < 0");
            usleep(5);
            continue;
        } else {
            cout << "recv head succ(fd=" << conn_fd << ") body_len:" << body_len << endl;
        }

        while (1) {
            //包体
            memset(msg, 0x0, sizeof (msg));
            recv_len = read(conn_fd, msg, sizeof (msg));
            //-1- 异常
            if (recv_len < 0) {
                cout << "recv error!" << endl;
                //non-block use
                /*if(errno == EAGAIN) {
                  break;
                  }*/
                perror("recv_len < 0");
                break;
            } else if (recv_len == 0) {
                //-2- 对端关闭
                cout << "peer dis-connnect fd=" << conn_fd << endl;
                close(conn_fd);
                //pthread_exit(NULL);
                perror("recv_len = 0");
                break;
            }

            ev.data.fd = conn_fd;
            //-3- 判断是否读完
            body_len -= recv_len;
            if (recv_len != sizeof (msg) || body_len <= 0) {
                cout << "recv msg succ(fd=" << conn_fd << "):" << msg << endl;
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

void onconnect() {
    //--/usr/include/netinet/in.h
    struct sockaddr_in *myaddr = new sockaddr_in;
    myaddr->sin_family = AF_INET;
    myaddr->sin_port = htons(SERVER_PORT);
    myaddr->sin_addr.s_addr = INADDR_ANY;
    inet_pton(AF_INET, SERVER_IP, (void *) &(myaddr->sin_addr));

    //tcp
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        cout << "create socket error!" << endl;
        return;
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
        break;
    }

    pthread_t pid_1, pid_2;
    pthread_create(&pid_1, NULL, onwrite, &fd);
    pthread_create(&pid_2, NULL, onread2, &fd);

    //--等待线程结束
    void *s1 = NULL;
    void *s2 = NULL;
    pthread_join(pid_1, &s1);
    cout << "Thread 1 returns:" << (char *) s1 << endl;
    pthread_cancel(pid_2);
    pthread_join(pid_2, &s2);
    cout << "Thread 2 returns:" << (char *) s2 << endl;

    //close
    close(fd);
    delete myaddr;
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
