/* 
 * File:   CMysqlWrapper.h
 * Author: Administrator
 *
 * Created on 2017年7月1日, 上午10:42
 */

#ifndef MYSQL_WRAPPER_H
#define MYSQL_WRAPPER_H

#include "mysql++.h"
#include "mysql++/mysql++.h"
#include <vector>

class CMysqlWrapper {
public:
	CMysqlWrapper();
	//CMysqlWrapper(const CMysqlWrapper& orig);
	virtual ~CMysqlWrapper();
	void Query(const std::string &sql);
	void Query(const std::string &sTable, const std::string &sCondition,
		const std::string &sSelects, const std::string &order_by = "", int limit = 1);
	void PrintInfo() const;
	void Update(const std::string & sql);
	void Delete(const std::string & sql);
	void Insert(const std::string & sql);
private:
	/*std::string passwd;
	std::string user;
	std::string db;
	std::string server;*/
	std::string select;
	mysqlpp::Connection conn;
	//mysqlpp::Query query;
	mysqlpp::StoreQueryResult res;
	std::vector<std::string> v_fields;
};

#endif /* MYSQL_WRAPPER_H */

