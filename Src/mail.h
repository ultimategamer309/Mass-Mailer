#pragma once
#include <curl/curl.h>
#include <string>
#include <vector>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/thread.hpp>
#include <fstream>

class mailConsts {
public:
	inline static std::string from;
	inline static std::string name;
	inline static std::string subject;
	inline static std::vector<std::string> message;
	inline static std::string secret;
	inline static bool html;
	inline static std::string server;
};

class mail
{
public:
	mail(std::string to);
	void set(std::string to);

	mail();

	bool vaidate();

	bool send();
private:
	void log(std::string str);

	struct upload_status {
		int lines_read;
		std::vector<const char*> message;
	};

	std::vector<std::string> getPayload();

	static size_t payload_source(void* ptr, size_t size, size_t nmemb, void* userp);

	//send info
	std::string to = "";
	std::string from = "";
	std::string name = "";

	//message info
	std::vector<std::string> message;
	std::string subject = "";
	bool html;

	//login info
	std::string secret;

	//validating
	static int writer(void* contents, size_t size, size_t nmemb, void* userp);
	static std::string DownloadedResponse;

	//log info
	boost::mutex logMutex;

	//defaults
	std::string server;
};
