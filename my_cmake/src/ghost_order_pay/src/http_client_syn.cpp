#include "http_client_syn.h"
#include <iostream>

namespace gim
{

HttpClient::HttpClient()
	: headers_(NULL), m_debug(false)
{
}

HttpClient::~HttpClient()
{
    //dtor

}

void HttpClient::init_header()
{
}

void HttpClient::init_json_header()
{
    headers_ = curl_slist_append(headers_, "Content-type: application/json");
    headers_ = curl_slist_append(headers_, "Accept: application/json");
    headers_ = curl_slist_append(headers_, "Cache-Control: no-cache");
    headers_ = curl_slist_append(headers_, "Pragma: no-cache");
}

void HttpClient::add_header(std::vector<std::string> const& headers)
{
    std::vector<std::string>::const_iterator it = headers.begin();
    for(; it != headers.end(); ++it)
    {
        headers_ = curl_slist_append(headers_, (*it).c_str());
    }
}

static size_t write_data_callback(void* buffer, size_t size, size_t nmemb, void* lpVoid)
{
	//std::cout << "curl write data size=" << size << ", ememb=" << nmemb << std::endl;
    std::string* str = dynamic_cast<std::string*>((std::string *)lpVoid);
    if( NULL == str || NULL == buffer )
    {
        return -1;
    }

    char* pData = (char*)buffer;
    str->append(pData, size * nmemb);


    return nmemb;
}

static int curl_debug_callback(CURL *, curl_infotype itype, char * pData, size_t size, void *)
{
	if (itype == CURLINFO_TEXT)
	{
		printf("[TEXT]%s\n", pData);
	}
	else if (itype == CURLINFO_HEADER_IN)
	{
		printf("[HEADER_IN]%s\n", pData);
	}
	else if (itype == CURLINFO_HEADER_OUT)
	{
		printf("[HEADER_OUT]%s\n", pData);
	}
	else if (itype == CURLINFO_DATA_IN)
	{
		printf("[DATA_IN]%s\n", pData);
	}
	else if (itype == CURLINFO_DATA_OUT)
	{
		printf("[DATA_OUT]%s\n", pData);
	}
	return 0;
}

int HttpClient::post(std::string const& addr, std::string const& request_data, long& code, std::string & response_data)
{
    CURLcode res;

    CURL* curl = curl_easy_init();

    if(NULL == curl)
    {
        return CURLE_FAILED_INIT;
    }

	if (m_debug){
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
		curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, curl_debug_callback);
	}
    curl_easy_setopt(curl, CURLOPT_URL, addr.c_str());
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers_);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response_data);
	curl_easy_setopt(curl, CURLOPT_POST, 1);

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request_data.c_str());
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);

    /**
    * 当多个线程都使用超时处理的时候，同时主线程中有sleep或是wait等操作。
    * 如果不设置这个选项，libcurl将会发信号打断这个wait从而导致程序退出。
    */
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
	curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, 1);

    res =  curl_easy_perform(curl);

	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
    curl_easy_cleanup(curl);

    return res;
}

int HttpClient::get(std::string const& strUrl, long& code, std::string & response_data)
{
    CURLcode res;
    CURL* curl = curl_easy_init();
    if(NULL == curl)
    {
        return CURLE_FAILED_INIT;
    }


	if (m_debug){
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
		curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, curl_debug_callback);
	}

    curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response_data);

    /**
    * 当多个线程都使用超时处理的时候，同时主线程中有sleep或是wait等操作。
    * 如果不设置这个选项，libcurl将会发信号打断这个wait从而导致程序退出。
    */
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
	curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, 1);
    res = curl_easy_perform(curl);

	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
    curl_easy_cleanup(curl);

    return res;
}

}
