#include "reader.h"

file::file(std::string fileLoc, std::string name) {
	this->name = name;
	loc = fileLoc;
}
file::file() {}
void file::open(std::string fileLoc, std::string name) {
	this->name = name;
	loc = fileLoc;
}
void file::trunc() {
	std::fstream f(loc, std::ios::trunc);
	f.close();
}
std::vector<std::string> file::getlines() {
	std::fstream f(loc, std::ios::in);
	std::vector<std::string> temp;
	std::string line;
	while (std::getline(f, line)) {
		temp.emplace_back(line);
	}
	f.close();
	return temp;
}
std::string file::getName() {
	return name;
}
void file::writeVec(std::vector<std::string> strVec) {
	std::fstream f(loc, std::fstream::out | std::fstream::trunc);
	for (int i = 0; i < strVec.size(); i++) {
		f << strVec[i] << std::endl;
	}
	f.close();
}
void file::write(std::string s) {
	std::fstream f(loc, std::fstream::trunc);
	f << s << std::endl;
	f.close();
}
file::file(const file&) {}

//open all files
reader::reader() {
	open();
}

void reader::open() {
	if (!isOpen) {
		settingsF.open(settingsPathS + serverS, "settings");
		serverF.open(settingsPathS + messageS, "server");
		messageF.open(settingsPathS + messageS, "message");
		toF.open(settingsPathS + toS, "to");
		fromF.open(settingsPathS + fromS, "from");
		subjectF.open(settingsPathS + subjectS, "subject");
		nameF.open(settingsPathS + nameS, "name");
		keyF.open(settingsPathS + keyS, "key");
		threadF.open(settingsPathS + threadsS, "thread");
		isOpen = true;
	}
}

//save current settings
void reader::save() {
	//get isHtml from first line of message file, delete the first line from vector, and save the rest to message vector
	std::vector<std::string> temp = message;
	temp.insert(temp.begin(), std::to_string(isHtml));
	messageF.writeVec(temp);
	//get recipiants and delete from vector
	toF.writeVec(recipiants);
	//save name to first line of from file and from to the second
	fromF.write(from);
	//save subject
	subjectF.write(subject);
	//save key
	keyF.write(key);
	//save threads
	threadF.write(std::to_string(threads));
	//server
	serverF.write(server);
}

//read settings from files
void reader::read() {
	//get isHtml from first line of message file, delete the first line from vector, and save the rest to message vector
	std::vector<std::string> mesTemp = messageF.getlines();
	isHtml = std::stoi(mesTemp[0]);
	mesTemp.erase(mesTemp.begin());
	message = mesTemp;
	//get recipiants
	recipiants = toF.getlines();
	//get name
	name = nameF.getlines()[0];
	//get from
	from = fromF.getlines()[0];
	//get subject
	subject = subjectF.getlines()[0];
	//get key
	key = keyF.getlines()[0];
	//get threads
	threads = std::stoi(threadF.getlines()[0]);
	//get server
	try {
		server = serverF.getlines()[0];
	}
	catch (int err) {
		server = "0";
	}
	if (server == "1")
		server = "0";
}

//returns mailman with curent settings
void reader::get() {
	mailConsts::from = from;
	mailConsts::name = name;
	mailConsts::from = from;
	mailConsts::subject = subject;
	mailConsts::message = message;
	mailConsts::secret = key;
	mailConsts::html = isHtml;
	if (server == "0")
		mailConsts::server = "https://api.mailgun.net/v3/sandbox7847fafcfb12470f8e94e86efad974b4.mailgun.org/messages";
	else
		mailConsts::server = server;
	
	mailer = mailman(recipiants, threads);
}

//actions
void reader::validate() {
	get();
	mailer.validate();
	recipiants = *mailer.getRecipiants();
}
void reader::send() {
	get();
	mailer.start();
}
void reader::exportList(std::string loc) {
	file f(loc, "temparary file");
	f.trunc();
	f.writeVec(recipiants);
}

//change vars
void reader::addRecipiant(std::string r) {
	recipiants.emplace_back(r);
}
void reader::removeRecipiant(std::string r) {
	recipiants.erase(std::remove(recipiants.begin(), recipiants.end(), r), recipiants.end());
}
void reader::wipeRecipiants() {
	recipiants.clear();
}
void reader::replaceRecipiants(std::string loc) {
	file f(loc, "temparary file");
	recipiants = f.getlines();
}
void reader::changeSenderName(std::string senderName) {
	name = senderName;
}
void reader::changeFromAdress(std::string adr) {
	from = adr;
}
void reader::changeSubject(std::string subject) {
	this->subject = subject;
}
void reader::changeMessage(std::string fileLoc) {
	file f(fileLoc, "temparary file");
	message = f.getlines();
}
void reader::changeKey(std::string key) {
	this->key = key;
}
void reader::changeThreads(int thr) {
	threads = thr;
}
void reader::changeIsHtml(bool isHtml) {
	this->isHtml = isHtml;
}
void reader::changeServer(std::string server) {
	this->server = server;
}
void reader::defaultServer() {
	server = "0";
}

//get vars
std::vector<std::string> reader::getRecipiants() {
	return recipiants;
}
int reader::getRecipiantsSize() {
	return recipiants.size();
}
std::string reader::getName() {
	return name;
}
std::string reader::getFrom() {
	return from;
}
std::string reader::getSubject() {
	return subject;
}
std::vector<std::string> reader::getMessage() {
	return message;
}
std::string reader::getKey() {
	return key;
}
int reader::getThreads() {
	return threads;
}
bool reader::getIsHtml() {
	return isHtml;
}
std::string reader::getServer() {
	if (server == "0")
		return "Default";
	else
		return server;
}
bool reader::isDefaultServer() {
	if (server == "0")
		return true;
	return false;
}

data reader::readWindow() {
	data d;
	file f(settingsPathS + windowS, "window");
	std::vector<std::string> line = f.getlines();

	d.demensionX = std::stoi(line[0]);
	d.demensionY = std::stoi(line[1]);

	return d;
}

void reader::saveWindow() {
	file f(settingsPathS + windowS, "window");
	f.write(std::to_string(dataSave.demensionX));
	f.write(std::to_string(dataSave.demensionY));
}
