#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <pthread.h>
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
//用于每一个fd的接收字符串
char *recv_msg[NUM];
int total_len[NUM];
int used_size[NUM];
bool finish_flag[NUM];

int epoll_fd = -1;

char * adjust_buf(char *buf, int index) {
	//剩余一个字节，则重新分配
	char *new_buf = buf;
	if(total_len[index] - used_size[index] <= 1)
	{
		total_len[index] *= 2;
		new_buf = new char[total_len[index]];
		memset(new_buf,0x0,total_len[index]);
		::memcpy(new_buf,buf,strlen(buf));
		cout<<"before-del-0 msg:"<<buf<<endl;
		delete[] buf;
		cout<<"after-del-0 msg:"<<buf<<endl;
		buf = NULL;
	}
	return new_buf;
}

void set_fd_nonblock(int fd) {

	int flags = fcntl(fd,F_GETFL,0);
	if (fcntl(fd,F_SETFL,flags | O_NONBLOCK) != 0 ) {
		perror("F_SETFL ERROR");
	}
}

void *onepoll(void *args) {
	int listen_fd = (int)(*((int*)args));
	epoll_fd = epoll_create(FD_SIZE);
	struct epoll_event ev;
	struct epoll_event epoll_events[MAX_EVENT];
	struct sockaddr_in cli_addr;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	//char msg[8];
	ev.data.fd = listen_fd;
	ev.events = EPOLLET|EPOLLIN;
	int ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev);
	if ( ret != 0 ) {
		pthread_exit(NULL);
	}

	for (; ;) {
		//block indefinitely
		int count = epoll_wait(epoll_fd, epoll_events, MAX_EVENT, -1);
		//nonblock
		//int count = epoll_wait(epoll_fd, epoll_events, MAX_EVENT, 0);
		switch (count) {
			case 0:
				cout << "nothing happen...\n";
				break;
			case -1:
				cout << "error event happen error:"<<strerror(errno)<<endl;
				break;
			default:
				//cout << "the event we expect happen count="<<count<<endl;
				break;

		}
		for (int index = 0; index < count; ++index) {
			//accept
			if (epoll_events[index].data.fd == listen_fd) {
				int conn_fd = accept(listen_fd,(struct sockaddr *)&cli_addr,&addrlen);
				set_fd_nonblock(conn_fd);
				ev.data.fd = conn_fd;
				ev.events = EPOLLET|EPOLLIN;
				int ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, conn_fd, &ev);
				if ( ret != 0 ) {
					perror("epoll_ctl error");
					continue;
				}
				cout<<"a new connection from client fd="<<conn_fd<<endl;
				//readable
			} else if (epoll_events[index].events & EPOLLIN) {
				cout<<"readable event happen fd="<<epoll_events[index].data.fd<<endl;
				int conn_fd = epoll_events[index].data.fd;
				finish_flag[conn_fd] = false;
				if (recv_msg[conn_fd] == NULL) {
					recv_msg[conn_fd] = new char[INIT_BUF_SIZE];
					//cout <<"new char\n";
					used_size[conn_fd] = 0;
					total_len[conn_fd] = INIT_BUF_SIZE;
				}
				while(1) {
					char *p = adjust_buf(recv_msg[conn_fd],conn_fd);
					cout <<"conn_fd="<<conn_fd<<",total_len="<<total_len[conn_fd]<<",used_size="<<used_size[conn_fd]<<endl;
					int recv_len = read(conn_fd,p+used_size[conn_fd],total_len[conn_fd]-used_size[conn_fd]-1);
					recv_msg[conn_fd] = p;
					//-1- 异常
					if (recv_len < 0) {
						//non-block use
						if (errno == EAGAIN) {
							cout << "EAGAIN fd="<<conn_fd<<endl;
							ev.data.fd = conn_fd;
							ev.events = EPOLLET|EPOLLIN;
							int ret = epoll_ctl(epoll_fd, EPOLL_CTL_MOD, conn_fd, &ev);
							if (ret != 0) {
								perror("epoll ctl abc error");
								break;
							}
							break;
						}
						cout << "recvlen < 0 fd="<<conn_fd<<endl;
						ev.data.fd = conn_fd;
						int ret = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, conn_fd, &ev);
						if (ret != 0) {
							perror("epoll ctl error");
							break;
						}
						shutdown(conn_fd, SHUT_RDWR);//0 succ
						close(conn_fd);
						break;
					} else if (recv_len == 0) {
						//-2- 对端关闭
						cout<<"peer dis-connect fd="<<conn_fd<<endl;
						ev.data.fd = conn_fd;
						int ret = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, conn_fd, &ev);
						if (ret != 0) {
							perror("epoll ctl error");
							break;
						}
						shutdown(conn_fd, SHUT_RDWR);//0 succ
						close(conn_fd);
						break;
					}

					ev.data.fd = conn_fd;
					//-3- 判断是否读完
					cout<<"judge end:"<<(p+used_size[conn_fd]+recv_len-2)<<endl;
					if (strncmp(p+used_size[conn_fd]+recv_len-2,"nn",2) == 0) {
						p[used_size[conn_fd]+recv_len-2] = '\0';
						finish_flag[conn_fd] = true;
					}
					if (finish_flag[conn_fd]) {
						cout<<"recv msg finish(fd="<<conn_fd<<"):whole-msg["<<recv_msg[conn_fd]<<"],recv_len["<<strlen(recv_msg[conn_fd])<<"]"<<endl;
						ev.events = EPOLLET|EPOLLOUT;
						int ret = epoll_ctl(epoll_fd, EPOLL_CTL_MOD, conn_fd, &ev);
						if (ret != 0) {
							perror("epoll ctl error read");
							break;
						}
						finish_flag[conn_fd] = false;

						//接收完数据释放
						cout<<"before-del-1 msg:"<<recv_msg[conn_fd]<<endl;
						delete[] recv_msg[conn_fd];
						cout<<"after-del-1 msg:"<<recv_msg[conn_fd]<<endl;
						recv_msg[conn_fd] = NULL;
						break;
					} else {
						cout<<"recv unfinish(fd="<<conn_fd<<"),[part-msg="<<(p+used_size[conn_fd])<<",recv_len="<<recv_len<<"],continue to read!"<<endl;
						used_size[conn_fd] += recv_len;
						ev.events = EPOLLET|EPOLLIN;
						int ret = epoll_ctl(epoll_fd, EPOLL_CTL_MOD, conn_fd, &ev);
						if ( ret != 0 ) {
							perror("epoll ctl error read");
							break;
						}
					}
				}//while
				//writeable
			} else if (epoll_events[index].events & EPOLLOUT) {
				cout<<"writeable event happen fd="<<epoll_events[index].data.fd<<endl;
				int conn_fd = epoll_events[index].data.fd;
				char msg[16];
				char tmp[16];
				memset(msg,0x0,sizeof(msg));
				strcpy(msg,"OK:1234567890");
				int write_len = -1;
				int left_len = strlen(msg);
				int cur_pos = 0;
				do {
					write_len = write(conn_fd,msg+cur_pos,8);
					if (write_len == -1)
					{
						cout <<"write error fd="<<conn_fd<<":"<<errno<<"-"<<strerror(errno)<<endl;
						perror("write error:");
						break;
					} else {
						strncpy(tmp,msg+cur_pos,write_len);
						cout <<"write msg succ to fd="<<conn_fd<<"[msg="<<tmp<<",len="<<write_len<<"]"<<endl;
					}
					cur_pos += write_len;
					left_len -= write_len;

				} while(left_len > 0);

				ev.events = EPOLLET|EPOLLIN;
				ev.data.fd = conn_fd;
				int ret = epoll_ctl(epoll_fd, EPOLL_CTL_MOD, conn_fd, &ev);
				if (ret != 0) {
					perror("epoll ctl error write");
					continue;
				}
				//假定了一个链接要发完了一次请求才会发第二个,用完一次释放
				//-1-不释放
				//memset(recv_msg[conn_fd],0x0,sizeof(recv_msg[conn_fd]));
				//used_size[conn_fd] = 0;
				//-2-释放
				//delete[] recv_msg[conn_fd];
				//recv_msg[conn_fd] = NULL;
			} else {
				cout<<"other events happen..."<<endl;
			}
		}
		usleep(100);
	}
}

int main(int argc,char *argv[]) 
{
	cout<<"##welcome to server##"<<endl;
	pthread_attr_t attr;
	pthread_t p_id;
	//--/usr/include/netinet/in.h
	struct sockaddr_in *myaddr = new sockaddr_in;
	myaddr->sin_family=AF_INET;
	myaddr->sin_port=htons(SERVPORT);
	myaddr->sin_addr.s_addr = INADDR_ANY;

	socklen_t s_len = sizeof(struct sockaddr_in);
	//tcp
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		cout<<"create socket error!"<<endl;
		return -1;
	}

	int ret = bind(fd, (struct sockaddr *)myaddr, s_len);
	if (ret != 0) {
		cout<<"bind error!"<<endl;
		return -1;
	}

	ret = listen(fd, 10);
	if (ret != 0) {
		cout<<"listen error!"<<endl;
		return -1;
	}
	for (int k=0; k < NUM; k++) {
		finish_flag[k] = false;
	}
	pthread_t pid_1;
	pthread_create(&pid_1, NULL, &onepoll, &fd);

	//wait for thread to return
	void *s1 = NULL;
	pthread_join(pid_1,&s1);
	cout<<"Thread 1 returns:"<<(char *)s1<<endl;

	//release resource
	close(fd);
	close(epoll_fd);
	delete myaddr;
	return 0;
}
