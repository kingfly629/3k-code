#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <vector>
#include <list>

using std::cout;
using std::endl;
using std::list;
using std::vector;

#define SERVPORT 4444

int conn_fd = -1;
int select_fd = -1;
list<int> list_accept_fd;
vector<int> vec_close_fd;

void onHeartBeatFromServer (int fd) {
	char msg[] = "connection keep alive......";
	int ret = write(fd,msg,sizeof(msg));
	if (ret < 0 ) {
		cout <<"write error,fd="<<fd<<endl;
	} else {
		//cout <<"send msg to fd="<<fd<<endl;
	}

}
void onReplyFromServer(int fd) {
	//todo add your business here
	char msg[] = "hello,world:200!";
	int ret = write(fd,msg,strlen(msg));
	if (ret < 0 ) {
		cout <<"write error,fd="<<fd<<endl;
	} else {
		cout <<"send msg succ to client_fd="<<fd<<endl;
	}
}

void *onselect(void *args) {
	int select_fd= (int)(*((int*)args));
	fd_set write_fd_set;
	fd_set read_fd_set;
	struct sockaddr_in cli_addr;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	char msg[10];
	list<int>::iterator iter;

	for (; ;) {
		int max_fd = -1;
		FD_ZERO(&write_fd_set);
		FD_ZERO(&read_fd_set);
		for(iter = list_accept_fd.begin(); iter != list_accept_fd.end(); ++iter) {
			FD_SET(*iter,&read_fd_set);
			//FD_SET(*iter,&write_fd_set);
			if (max_fd < *iter) {
				max_fd = *iter;
			}
		}
		FD_SET(select_fd,&read_fd_set);
		if (max_fd < select_fd) {
			max_fd = select_fd;
		} 
		//--non_block
		/*struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 0;
		int retval = select(max_fd + 1, &read_fd_set, NULL, NULL, &tv);
		*/
		//--block 0 == NULL
		int retval = select(max_fd + 1, &read_fd_set, &write_fd_set, NULL, NULL);
		switch (retval) {
			case 0:
				cout << "nothing happen...\n";
				break;
			case -1:
				cout<<"error occur:"<<strerror(errno)<<endl;
				break;
			default:
				cout<<"event we expect happen...\n";
				break;
		}

		//accept
		if(FD_ISSET(select_fd,&read_fd_set)){
			int conn_fd = accept(select_fd,(struct sockaddr *)&cli_addr,&addrlen);
			if (conn_fd < 0) {
				cout<<"accept error..."<<endl;
				continue;
			} else {
				cout<<"a new connection client_fd="<<conn_fd<<endl;
				list_accept_fd.push_back(conn_fd);
			}
		}

		for(iter = list_accept_fd.begin(); iter != list_accept_fd.end(); ++iter) {
			//read
			if(FD_ISSET(*iter,&read_fd_set)){
				std::string recv_finish_msg;
				while(1) {
					memset(msg,0x0,sizeof(msg));
					int recv_len= read(*iter,msg,sizeof(msg));
					//cout<<"recv from client_fd="<<*iter<<",len="<<recv_len<<endl;
					//-1- 异常
					if (recv_len < 0) {
						cout<<"recv error,fd="<<*iter<<endl;
						if(errno == EAGAIN) {
							//continue;
							break;
						}
						pthread_exit(NULL);
					} else if (recv_len == 0) {
						//-2- 对端关闭
						cout<<"peer dis-connect client_fd="<<*iter<<endl;
						close(*iter);
						FD_CLR(*iter,&read_fd_set);
						vec_close_fd.push_back(*iter);
						//list_accept_fd.remove(*iter);
						break;
					}

					//-3- 判断是否读完
					if (recv_len != sizeof(msg)) {	
						recv_finish_msg += msg;
						cout<<"recv msg succ from (client_fd="<<*iter<<"):"<<msg<<endl;
						cout<<"recv msg finish from (client_fd="<<*iter<<"):"<<recv_finish_msg<<endl;
						onReplyFromServer(*iter);
						break;
					} else if (recv_len == sizeof(msg)){
						cout<<"recv unfinish[recv_len="<<recv_len<<",recv_msg="<<msg<<";client_fd="<<*iter<<"],continue to read!"<<endl;
						recv_finish_msg += msg;
					}	
				}
			}

			//write
			if(FD_ISSET(*iter,&write_fd_set)){
				//onHeartBeatFromServer(*iter);
			}
		}
		for (int index = 0; index < vec_close_fd.size(); ++index) {
			list_accept_fd.remove(vec_close_fd[index]);		
		}
		if (vec_close_fd.size() > 0) { 
			vec_close_fd.clear();
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
	//myaddr = (sockaddr_in *)malloc(sizeof(struct sockaddr_in));
	myaddr = new sockaddr_in;
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
		cout<<"bind error:"<<strerror(errno)<<endl;
		return -1;
	}

	ret = listen(fd, 10);
	if (ret != 0) {
		cout<<"listen error!"<<endl;
		return -1;
	}
	cout << "server fd= " << fd <<endl;
	pthread_t pid_1;
	pthread_create(&pid_1, NULL, &onselect, &fd);

	//wait for thread to return
	void *s1 = NULL;
	pthread_join(pid_1,&s1);
	cout << "Thread 1 returns: " << (char *)s1 <<endl;

	//release resource
	close(fd);
	close(conn_fd);
	close(select_fd);
	delete myaddr;
	return 0;
}
