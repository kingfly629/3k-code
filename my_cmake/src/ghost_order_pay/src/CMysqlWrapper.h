/* 
 * File:   CMysqlWrapper.h
 * Author: Administrator
 *
 * Created on 2017年7月1日, 上午10:42
 */

#ifndef MYSQL_WRAPPER_H
#define MYSQL_WRAPPER_H

class CMysqlWrapper {
public:
	CMysqlWrapper();
	//CMysqlWrapper(const CMysqlWrapper& orig);
	virtual ~CMysqlWrapper();
	void Query(const std::string &sql);
	void Query(const std::string &sql);
	void PrintInfo() const;
	void Update();
	void Delete();
	void Insert();
private:
	/*std::string passwd;
	std::string user;
	std::string db;
	std::string server;*/
	std::string select;
	mysqlpp::Connection conn;
	//mysqlpp::Query query;
	mysqlpp::StoreQueryResult res;
	vector<std::string> v_fields;
};

#endif /* MYSQL_WRAPPER_H */

