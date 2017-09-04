/* 
 * File:   CMysqlWrapper.cpp
 * Author: Administrator
 * 
 * Created on 2017年7月1日, 上午10:42
 */

#include "CMysqlWrapper.h"

namespace kkk {

	CMysqlWrapper::CMysqlWrapper(const std::string &host, const std::string &user,
			const std::string &passwd, const std::string &db, const std::string charset, int port)
	: conn(false) {
		if (conn.connect(db.c_str(), host.c_str(), user.c_str(), passwd.c_str(), port)) {
			std::cout << "connect db succeed. " << std::endl;
		} else {
			std::cout << "connect db fail. " << std::endl;
		}
		v_fields.clear();
	}

	CMysqlWrapper::~CMysqlWrapper() {
	}

	void CMysqlWrapper::Query(const std::string &sql) {
		select = "*";
		mysqlpp::Query query = conn.query(sql);
		res = query.store();
	}

	void CMysqlWrapper::Query(const std::string &sTable, const std::string &sCondition,
			std::string &sSelects, const std::string &order_by, int limit) {
		select.clear();
		select.swap(sSelects);
		std::string sql = "select " + select + " from " + sTable + " where " + sCondition;
		if (!order_by.length()) {
			sql += " order by " + order_by;
		}
		sql += " limit " + limit;
		mysqlpp::Query query = conn.query(sql);
		res = query.store();
	}

	void CMysqlWrapper::Insert(const std::string &sql) {
		mysqlpp::Query query = conn.query(sql);
		res = query.store();
	}

	void CMysqlWrapper::Update(const std::string &sql) {
		mysqlpp::Query query = conn.query(sql);
		res = query.store();
	}

	void CMysqlWrapper::Delete(const std::string &sql) {
		mysqlpp::Query query = conn.query(sql);
		res = query.store();
	}

	void CMysqlWrapper::PrintInfo() {
		if (res) {
			int num_fields = res.num_fields();
			if (0 != select.compare("*")) {
				std::cout.setf(std::ios::left);
				char delims[] = "#";
				char *result = NULL;
				result = strtok((char *) select.c_str(), delims);
				while (result != NULL) {
					v_fields.push_back(result);
					std::cout << std::setw(10) << result;
					result = strtok(NULL, delims);
				}
				std::cout << std::endl;

				mysqlpp::StoreQueryResult::const_iterator it;
				for (it = res.begin(); it != res.end(); ++it) {
					mysqlpp::Row row = *it;
					for (int k = 0; k < v_fields.size(); ++k) {
						std::cout << std::setw(9) << row[v_fields[k].c_str()] << ' ';
					}
					std::cout << std::endl;
				}
			} else {
				for (size_t j = 0; j < res.num_rows(); ++j) {
					for (int k = 0; k < num_fields; ++k) {
						std::cout << '\t' << res[j][k] << ' ';
					}
					std::cout << std::endl;
				}
			}

		}
	}

}
