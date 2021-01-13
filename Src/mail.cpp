#include "mail.h"
#include <SFML/System/String.hpp>
#pragma warning(disable:4996)

//declair params
mail::mail(std::string to) {
	set(to);
}
void mail::set(std::string to) {
	this->to = to;
	this->from = from;
	this->name = name;
	this->subject = subject;
	this->message = message;
	this->secret = secret;
	this->html = html;
	this->server = server;
}

mail::mail() {}

bool mail::send() {
	std::string message = "";
	for (std::string &i : mailConsts::message) {
		message += i;
		message += '\n';
	}

	CURL* curl;
	CURLcode res;
	std::string readBuffer;
	std::string user = "api:" + mailConsts::secret;
	std::string f = "from=" + mailConsts::name + "%3C" + mailConsts::from + "%3E";
	std::string t = "to=" + to;
	std::string sub1 = "subject=";
	std::string mes;
	if (html)
		mes = "html=" + message;
	else
		mes = "text=" + message;

	curl = curl_easy_init();
	if (curl) {
		std::string sub2 = curl_easy_escape(curl, subject.c_str(), subject.length());
		std::string sub = sub1 + sub2;
		std::string data = f + '&' + t + '&' + sub + '&' + mes + '&' + "o:tracking=True";

		curl_easy_setopt(curl, CURLOPT_URL, server.c_str());
		curl_easy_setopt(curl, CURLOPT_USERPWD, user.c_str());
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, data.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

	//log it to console
	boost::property_tree::ptree pt;
	std::istringstream is(readBuffer);
	if (readBuffer == "") {
		log("failed: Read Buffer Empty");
		return false;
	}
		
	boost::property_tree::json_parser::read_json(is, pt);
	log(readBuffer);
	if (pt.get<std::string>("message") == "Queued. Thank you.") {
		log("sucess id: " + pt.get<std::string>("id"));
		return true;
	}
}

bool mail::vaidate() {
	CURL* curl;
	CURLcode res;
	std::string readBuffer;
	std::string user = "api:" + mailConsts::secret;
	std::string data =  "adress=" + to;

	std::string json;

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "https://api.mailgun.net/v4/address/validate");
		curl_easy_setopt(curl, CURLOPT_USERPWD, user.c_str());
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, data.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		json = readBuffer;

		boost::property_tree::ptree pt;
		std::istringstream is(json);
		boost::property_tree::json_parser::read_json(is, pt);

		//make logs file
		try {
			if (pt.get<bool>("is_valid") && (!pt.get<bool>("is_disposable_address"))) {
				log(std::to_string(true));
				return true;
			}
		}
		catch(int err) {
			log("failed json response: " + readBuffer);
		}
	}
	return false;
}

void mail::log(std::string str) {
	logMutex.lock();
	std::cout << str << std::endl;
	logMutex.unlock();
}

int mail::writer(void* contents, size_t size, size_t nmemb, void* userp) {
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}
