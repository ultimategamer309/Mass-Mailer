#pragma once
#include <curl/curl.h>
#include <string>
#include <vector>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/thread.hpp>
#include <fstream>

class mail
{
public:
	mail(std::string to,
		std::string from,
		std::string name,
		std::string subject,
		std::vector<std::string> message,
		std::string secret,
		bool html = false,
		std::string server = "https://api.mailgun.net/v3/sandbox7847fafcfb12470f8e94e86efad974b4.mailgun.org/messages");
	void set(std::string to,
		std::string from,
		std::string name,
		std::string subject,
		std::vector<std::string> message,
		std::string secret,
		bool html = false,
		std::string server = "https://api.mailgun.net/v3/sandbox7847fafcfb12470f8e94e86efad974b4.mailgun.org/messages");

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