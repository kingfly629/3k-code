#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	char   *ptr, **pptr;
	struct hostent *hptr;
	char   str[32];
	char hostname[64];
	//ptr = argv[1];
	if (gethostname(hostname,sizeof(hostname)) != 0) {
		printf(" gethostname error for host:%s\n", ptr);
		return -1; 
	}

	printf("hostname=%s\n",hostname);
	if((hptr = gethostbyname(hostname)) == NULL)
	{
		printf(" gethostbyname error for hostname:%s\n", hostname);
		return -2; 
	}

	printf("official hostname:%s\n",hptr->h_name);
	for(pptr = hptr->h_aliases; *pptr != NULL; pptr++)
		printf(" alias:%s\n",*pptr);

	switch(hptr->h_addrtype)
	{
		case AF_INET:
		case AF_INET6:
			pptr=hptr->h_addr_list;
			for(; *pptr!=NULL; pptr++)
				printf(" address:%s\n", 
						inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
			printf(" first address: %s\n", 
					inet_ntop(hptr->h_addrtype, hptr->h_addr, str, sizeof(str)));
			break;
		default:
			printf("unknown address type\n");
			break;
	}

	return 0;
}
