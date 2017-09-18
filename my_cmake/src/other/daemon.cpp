/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   daemon.cpp
 * Author: uj6
 *
 * Created on 2017年9月18日, 下午2:13
 */

#include <stdlib.h>
#include <iostream>
#include <signal.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

bool g_run = true;

void daemonize() {
	// clear file creation mask
	umask(0);

	// fork child
	pid_t pid = fork();

	if (pid < 0) {
		exit(-1);
	} else if (pid > 0) {
		exit(0);
	}

	setsid();
	close(0);
	close(1);
	close(2);

	int fd0 = open("/dev/null", O_RDWR);
	int fd1 = dup(fd0);
	int fd2 = dup(fd0);
}

int system_shutdown(void) {
	g_run = false;
	return 0;
}

static void signal_handler(int sig) {

	switch (sig) {
		case SIGTERM:
			std::cout << "catch SIGTERM...\n";
			system_shutdown();
			break;
			case SIGHUP:
				std::cout << "catch SIGHUP...\n";
			system_shutdown();
			break;
	}
}

static void printHelpInfo() {
	std::cout << "-h                             help\n"
			"-d                             daemon\n"
			<< std::endl;
}

int main(int argc, char** argv) {
	const char* short_options = "hd";
	const struct option long_options[] = {
		{ "help", 0, NULL, 'h'},
		{ "daemon", 0, NULL, 'd'},
		{ NULL, 0, NULL, 0}
	};

	int c;
	bool dm = false;
	while ((c = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
		switch (c) {
			case 'h':
				printHelpInfo();
				return 0;
			case 'd':
				dm = true;
				break;
		}
	}

	signal(SIGCHLD, SIG_IGN);
	signal(SIGTERM, signal_handler);

	if (dm) {
		daemonize();
	}

	while (g_run) {
		std::cout << "running...\n";
		sleep(8);
	}

	return 0;
}

