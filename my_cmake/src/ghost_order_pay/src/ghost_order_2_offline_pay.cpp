/* 
 * File:   ghost_order_2_offline_pay.cpp
 * Author: Administrator
 *
 * Created on 2017年7月1日, 上午10:42
 */

#include <stdlib.h>
#include <iostream>
#include <signal.h>
#include <getopt.h>
#include "CMysqlWrapper.h"
#include "http_client_syn.h"
#include "plconfig.h"
using namespace std;
using namespace kkk;

int g_run = true;

static int system_shutdown(void) {
	g_run = false;
	return 0;
}

static void signal_handler(int sig) {
	switch (sig) {
		case SIGHUP:
		case SIGTERM:
			system_shutdown();
			break;
	}
}

static void printHelpInfo() {
	std::cout << "-h                          help\n"
			"-d                             daemon\n"
			"-c     <config>                config\n"
			<< std::endl;
}

static int loadCfgData(PLConfig &conf, const std::string &cfg_file) {
	return conf.init(cfg_file);
}

int main(int argc, char** argv) {
	//异常处理机制
	try {
		const char* short_options = "hdc";

		const struct option long_options[] = {
			{ "help", 0, NULL, 'h'},
			{ "daemon", 0, NULL, 'd'},
			{ "config", 1, NULL, 'c'},
			{ NULL, 0, NULL, 0}
		};

		int c;
		std::string type;
		std::string config;
		std::string time;
		bool daemon = false;

		//参数处理
		while ((c = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
			switch (c) {
				case 'h':
					printHelpInfo();
					return 0;
				case 'd':
					daemon = true;
					break;
				case 'c':
					config = optarg;
					break;
				case 't':
					time = optarg;
					break;
				default:
					break;
			}
		}

		if (!config.size()) {
			printHelpInfo();
			return -1;
		}


		//信号处理机制
		signal(SIGPIPE, SIG_IGN);
		signal(SIGCHLD, SIG_IGN);
		signal(SIGTSTP, SIG_IGN);
		signal(SIGTTOU, SIG_IGN);
		signal(SIGTTIN, SIG_IGN);
		signal(SIGHUP, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGURG, SIG_IGN);
		signal(SIGTERM, signal_handler);

//		if (daemon) {
//			ef::daemonize();
//		}

		//加载配置文件：订单接口地址等
		PLConfig conf;
		if ((int ret = conf.init(config)) < 0) {
			goto RET;
		}

		//业务逻辑处理 todo here
		std::auto_ptr<CMysqlWrapper> mysql = std::auto_ptr<CMysqlWrapper>
				(new CMysqlWrapper(conf.server, conf.user, conf.passwd, conf.db, conf.charset));
		std::string sTable = "orders";
		std::string sSelects = "order_id,order_status";
		std::string sCondition = "channel_id = 'ghost' and order_status = 100 and substring(order_time,1,10) = ";
		std::string order_by = "appoint_time";
		std::string limit = "2";
		sCondition.append("'");
		sCondition.append(time);
		sCondition.append("'");
		mysql->Query(sTable, sCondition, sSelects, order_by, limit);
		mysql->PrintInfo();
		//2-循环每一笔订单，调用订单接口完成线下支付
		//		while (g_run) {
		//			
		//		}

	} catch (const std::exception &ex) {
		std::cerr << "catch Exception: " << ex.what() << std::endl;
		fprintf(stderr, "catch Exception : %s", ex.what());
	}

	//finish
RET:
	return 0;
}

