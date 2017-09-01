#ifndef __MAIN_CONFIG_H__
#define __MAIN_CONFIG_H__

#include "json/json.h"
#include <iostream>
#include <fstream>
#include <vector>

namespace kkk {
	struct PLConfig {
		std::string host;
		std::string user;
		std::string password;
		std::string database;
		int port;
		std::string charset;

		int init(const std::string& f);

		int serializeToJson(Json::Value& v) const;
		int parseFromJson(const Json::Value& v);

		std::string toStyledString() const;
	};
}

#endif/*__LOGIC_CONFIG_H__*/
