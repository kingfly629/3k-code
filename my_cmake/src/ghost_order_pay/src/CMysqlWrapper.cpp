/* 
 * File:   CMysqlWrapper.cpp
 * Author: Administrator
 * 
 * Created on 2017年7月1日, 上午10:42
 */

#include "CMysqlWrapper.h"

CMysqlWrapper::CMysqlWrapper(const std::string &server, const std::string &user,
		const std::string &passwd, const std::string &db, const std::string charset)
: conn(false), res(false) {
	if (conn.connect(db, server, user, password)) {
		cout << "connect db succeed. " << endl;
	} else {
		cout << "connect db fail. " << endl;
	}
	v_fields.clear();
}

CMysqlWrapper::~CMysqlWrapper() {
}

void CMysqlWrapper::Query(const std::string & sql) {
	mysqlpp::Query query = conn.query($sql);
	res = query.store();
}

void CMysqlWrapper::Query(const std::string &sTable, const std::string &sCondition,
		const std::string &sSelects, const std::string &order_by, int limit) {
	select.swap(sSelects);
	std::string sql = "select " + select + " from " + sTable + " where " + sCondition;
	if (!order_by.length()) {
		sql += " order by " + order_by;
	}
	sql += " limit " + limit;
	mysqlpp::Query query = conn.query($sql);
	res = query.store();
}

void CMysqlWrapper::Insert(const std::string & sql) {
	mysqlpp::Query query = conn.query($sql);
	res = query.store();
}

void CMysqlWrapper::Update(const std::string & sql) {
	mysqlpp::Query query = conn.query($sql);
	res = query.store();
}

void CMysqlWrapper::Delete(const std::string & sql) {
	mysqlpp::Query query = conn.query($sql);
	res = query.store();
}

void CMysqlWrapper::PrintInfo() const {
	if (res) {
		if (0 != select.compare("*")) {
			cout.setf(ios::left);
			char delims[] = "#";
			char *result = NULL;
			result = strtok(select.c_str(), delims);
			while (result != NULL) {
				v_fields.push_back(result);
				cout << setw(10) << result;
				result = strtok(NULL, delims);
			}
			cout << endl;

			mysqlpp::StoreQueryResult::const_iterator it;
			for (it = res.begin(); it != res.end(); ++it) {
				mysqlpp::Row row = *it;
				for (int k = 0; k < v_fields.size(); ++k) {
					cout << setw(9) << row[v_fields[k]] << ' ';
				}
				cout << endl;
			}

			/*for (size_t i = 0; i < res.num_rows(); ++i) {
				for (int k = 0; k < v_fields.size(); ++k) {
					cout << setw(9) << res[i][v_fields[k]] << ' ';
				}
			    cout << endl;
			}*/
			
		}
	}

