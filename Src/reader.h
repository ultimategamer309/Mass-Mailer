#pragma once
#include "SFML/Main.hpp"
#include "mailman.h"
#include "SFML/Graphics.hpp"
#include <filesystem>
#include <string>
#include <filesystem>
#include <algorithm>

class file 
{
public:
	file(std::string fileLoc, std::string name);
	file();
	std::fstream get();
	std::fstream getTrunc();
	void open(std::string fileLoc, std::string name);
	void writeVec(std::vector<std::string> strVec);
	void write(std::string s);
	void trunc();
	bool isEmpty();
	std::vector<std::string> getlines();
	std::string getName();
private:
	std::string name;
	std::string path;
	std::string loc;
	bool emptyFlair = false;
};

class reader
{
public:
	reader();
	void open();

	//mailman
	void save();
	void read();
	void get();

	//actions
	void validate();
	void send();
	void exportList(std::string loc);

	//change vars
	void addRecipiant(std::string r);
	void removeRecipiant(std::string r);
	void wipeRecipiants();
	void replaceRecipiants(std::string loc);
	void changeSenderName(std::string senderName);
	void changeFromAdress(std::string adr);
	void changeSubject(std::string subject);
	void changeIsHtml(bool isHtml);
	void changeMessage(std::string fileLoc);
	void changeKey(std::string key);
	void changeThreads(int thr);
	void changeServer(std::string server);
	void defaultServer();

	//get vars
	std::vector<std::string> getRecipiants();
	int getRecipiantsSize();
	std::string getName();
	std::string getFrom();
	std::string getSubject();
	std::vector<std::string> getMessage();
	std::string getKey();
	int getThreads();
	bool getIsHtml();
	std::string getServer();
	bool isDefaultServer();
	//first is completed second is total

	//window settings
	sf::Vector2f readWindow();
	void saveWindow();
private:
	//bool
	bool isOpen = false;

	//file paths
	const std::string settingsPathS = "Settings/";
	const std::string serverS = "server.txt";
	const std::string messageS = "message.txt";
	const std::string toS = "recipiants.txt";
	const std::string fromS = "from.txt";
	const std::string subjectS = "subject.txt";
	const std::string nameS = "name.txt";
	const std::string keyS = "key.txt";
	const std::string threadsS = "threads.txt";
	const std::string windowS = "gui.txt";

	//files
	file settingsF;
	file serverF;
	file messageF;
	file toF;
	file fromF;
	file subjectF;
	file nameF;
	file keyF;
	file threadF;
	file windowF;

	//current vars
	std::vector<std::string> recipiants;
	std::string name;
	std::string from;
	std::string subject;
	std::vector<std::string> message;
	std::string key;
	int threads;
	bool isHtml;
	std::string server;

	//mailman
	mailman mailer;

	//data
	sf::Vector2f dataSave;
};

