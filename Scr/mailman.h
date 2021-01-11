#pragma once
#include <curl/curl.h>
#include <string>
#include <vector>
#include <iostream>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include "mail.h"
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>
#include <fstream>

class mailman
{
public:
	mailman(std::vector<std::string> recipiants,
		std::string from,
		std::string senderName,
		std::string subject,
		std::vector<std::string> message,
		std::string key,
		int threads,
		bool isHtml = false,
		std::string server = "0");

	mailman();

	mailman(const mailman&);

	void set(std::vector<std::string> recipiants,
		std::string from,
		std::string senderName,
		std::string subject,
		std::vector<std::string> message,
		std::string key,
		int threads,
		bool isHtml = false,
		std::string server = "0");

	~mailman();

	int validate();
	bool start();

	//gets
	double getProgressAsPercent();
	int getCompleted();
	int getRemaining();
	std::vector<std::string> getRecipiants();

private:
	static bool isValid(std::string to,
		std::string from,
		std::string senderName,
		std::string subject,
		std::vector<std::string> message,
		std::string secret,
		std::vector<std::string> *validStrings,
		std::string server = "");

	static void send(std::string to,
		std::string from,
		std::string senderName,
		std::string subject,
		std::vector<std::string> message,
		std::string secret,
		bool html = false,
		std::string server = "");

	std::vector<std::string> recipiants;
	std::string from;
	std::string senderName;

	//vaidating
	std::vector<std::string> recipiantsValidateTemp;

	//message info
	std::vector<std::string> message;
	std::string subject = "";

	//login info
	std::string secret;

	//progress
	int total;
	int completed;
	int remaining;

	//multithreading
	int threads;

	//defaults
	bool isHtml;
	std::string server;

	//don't run when undeffined
	bool deffined;
};

