#include "plconfig.h"
#include "base/ef_log.h"
#include <fstream>

namespace gim{


using namespace std;
using namespace ef;

int PLConfig::init(const std::string& f){

	int ret = 0;
	fstream ifs;

	ifs.open(f.data(), ios::in);

	if(!ifs.is_open()){
		return -1;
	}

	Json::Reader reader;
	Json::Value root;

	if (!reader.parse(ifs, root, false)) {
		std::cout << "parse fail" << std::endl;
		return -2;
	}

	ret = parseFromJson(root);

	return ret;
}


int PLConfig::parseFromJson(const Json::Value& root){

	int ret = 0;

	if (root["ID"].isInt()) {
		ID = root["ID"].asInt();
	}

	if (root["ZkUrl"].isString()) {
		ZkUrl = root["ZkUrl"].asString();
	}

	if (root["RPCConfig"].isArray()) {
		RPCConfig = root["RPCConfig"];
	}
	
	if (root["OrderQueues"].isArray()) {
		for (int i = 0; i < root["OrderQueues"].size(); i++) {
			struct hostport hp;
			hp.host = root["OrderQueues"][i]["host"].asString();
			hp.port = root["OrderQueues"][i]["port"].asInt();
                        hp.password = root["OrderQueues"][i]["password"].asString();
			OrderQueues.push_back(hp);
		}
	}

	if (root["DistConfig"].isArray()) {
		DistConfig = root["DistConfig"];
	}
	
	if (root["round"].isInt()) {
		round = root["round"].asInt();
	}
	
	if (root["next_round_interval"].isInt()) {
		next_round_interval = root["next_round_interval"].asInt();
	}
	
	if (root["push_threshold"].isInt()) {
		push_threshold = root["push_threshold"].asUInt();
	}

	if (root["NetLogLevel"].isString()) {
		NetLogLevel = getStrLevel(root["NetLogLevel"].asString().data());
	}

	if (root["KeyPrefix"].isString()) {
		KeyPrefix = root["KeyPrefix"].asString();
	}
	
	if (root["LineID"].isString()) {
		LineID = root["LineID"].asString();
	}

	if (root["NetLogPath"].isString()) {
		NetLogPath = root["NetLogPath"].asString();
	}

	if (root["LogLevel"].isString()) {
		LogLevel = getStrLevel(root["LogLevel"].asString().data());
	}

	if (root["LogPath"].isString()) {
		LogPath = root["LogPath"].asString();
	}


	if (root["StatisLogPath"].isString()) {
		StatisLogPath = root["StatisLogPath"].asString();
	}

        if (root["PushDestFlag"].isInt()) {
		PushDestFlag = root["PushDestFlag"].asInt();
	}
        
	return ret;
}

int PLConfig::serializeToJson(Json::Value& v) const{
	v["ID"] = ID;
	v["ZkUrl"] = ZkUrl;
	v["RPCConfig"] = RPCConfig;
	vector<hostport>::const_iterator it = OrderQueues.begin();
	for (int i = 0; it != OrderQueues.end(); it++) {
		v["OrderQueues"][i]["host"] = it->host;
		v["OrderQueues"][i]["port"] = it->port;
	}
	v["DistConfig"] = DistConfig;
	v["round"] = round;
	v["next_round_interval"] = next_round_interval;
	v["NetLogLevel"] = NetLogLevel;
	v["NetLogPath"] = NetLogPath;
	v["LogLevel"] = LogLevel;
	v["LogPath"] = LogPath;
	v["StatisLogPath"] = StatisLogPath;
        v["PushDestFlag"] = PushDestFlag;

	return 0;
}

string PLConfig::toStyledString() const{
	Json::Value v;
	serializeToJson(v);
	return v.toStyledString();
}

};
