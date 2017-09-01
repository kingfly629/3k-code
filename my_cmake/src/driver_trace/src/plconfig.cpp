#include "plconfig.h"

namespace kkk {

	int PLConfig::init(const std::string& f) {
		int ret = 0;
		std::fstream ifs;
		ifs.open(f.data(), std::ios::in);
		if (!ifs.is_open()) {
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

	int PLConfig::parseFromJson(const Json::Value& root) {
		int ret = 0;
		if (root["host"].isString()) {
			host = root["host"].asString();
		}
		
		if (root["user"].isString()) {
			user = root["user"].asString();
		}
		
		if (root["password"].isString()) {
			password = root["password"].asString();
		}
		
		if (root["database"].isString()) {
			database = root["database"].asString();
		}
		
		if (root["port"].isInt()) {
			port = root["port"].asInt();
		}
		
		if (root["charset"].isString()) {
			charset = root["charset"].asString();
		}

		return ret;
	}

	int PLConfig::serializeToJson(Json::Value& v) const {
		v["host"] = host;
		v["user"] = user;
		v["database"] = database;
		v["password"] = password;
		v["charset"] = charset;

		return 0;
	}

	std::string PLConfig::toStyledString() const {
		Json::Value v;
		serializeToJson(v);
		return v.toStyledString();
	}

};
