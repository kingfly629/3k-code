#include "plconfig.h"
#include <fstream>

namespace kkk {

	int PLConfig::init(const std::string& f) {
		int ret = 0;
		fstream ifs;
		ifs.open(f.data(), ios::in);
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
		if (root["server"].isString()) {
			server = root["server"].asString();
		}
		
		if (root["user"].isString()) {
			server = root["user"].asString();
		}
		
		if (root["password"].isString()) {
			server = root["password"].asString();
		}
		
		if (root["db"].isString()) {
			server = root["db"].asString();
		}
		
		if (root["charset"].isString()) {
			server = root["charset"].asString();
		}

		return ret;
	}

	int PLConfig::serializeToJson(Json::Value& v) const {
		v["server"] = server;
		v["user"] = user;
		v["db"] = db;
		v["passwd"] = passwd;
		v["charset"] = charset;

		return 0;
	}

	string PLConfig::toStyledString() const {
		Json::Value v;
		serializeToJson(v);
		return v.toStyledString();
	}

};
