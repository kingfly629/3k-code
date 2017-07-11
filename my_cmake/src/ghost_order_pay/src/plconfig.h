#ifndef __MAIN_CONFIG_H__
#define __MAIN_CONFIG_H__

#include "json/json.h"
#include <vector>

namespace gim{

struct hostport {
	std::string host;
	int port;
        std::string password;
};

struct PLConfig{
	int ID;
	std::string ZkUrl;
	Json::Value RPCConfig;

	std::vector<hostport> OrderQueues;
	Json::Value DistConfig;;//maxdistances used to query for every round
	int round;
	int next_round_interval;//ms

	int push_threshold;
	int NetLogLevel;
	std::string KeyPrefix;
	std::string LineID;
	std::string NetLogPath;

	int LogLevel;
	std::string LogPath;

	std::string StatisLogPath;

        int PushDestFlag;
        
	int init(const std::string& f);

	int serializeToJson(Json::Value& v) const;
	int parseFromJson(const Json::Value& v);

	std::string toStyledString() const;
};


}


#endif/*__LOGIC_CONFIG_H__*/
