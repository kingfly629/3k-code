#ifndef CURL_BASE_H
#define CURL_BASE_H

#include <string>
#include <vector>

#include "curl/curl.h"

namespace gim
{

class HttpClient
{
    public:
        HttpClient();
        virtual ~HttpClient();

		int post(std::string const& addr, std::string const& request_data, long& code, std::string & response_data);
		int get(std::string const& addr, long& code, std::string & response_data);

        void add_header(std::vector<std::string> const& headers);
        void init_json_header();
		void debug(){
			m_debug = true;
		}
    protected:
        void init_header();

    private:
        struct curl_slist *headers_;
		bool m_debug;
};

}
#endif // CURL_BASE_H
