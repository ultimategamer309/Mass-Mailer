#include "reader.h"

file::file(std::string fileLoc, std::string name) {
	this->name = name;
	loc = fileLoc;
	get();
}
file::file() {}
std::fstream file::get() {
	//needs existing file
	if (!std::filesystem::exists(loc))
		std::ofstream ofs(loc);

	std::fstream f(loc, std::ios::out | std::ios::in);
	if (f.peek() == std::fstream::traits_type::eof()) {
		emptyFlair = true;
	}
	return f;
}
std::fstream file::getTrunc() {
	std::fstream f(loc, std::ios::out | std::ios::in | std::ios::trunc);
	if (!f.peek() == std::ifstream::traits_type::eof()) {
		emptyFlair = true;
	}
	return f;
}
void file::open(std::string fileLoc, std::string name) {
	this->name = name;
	loc = fileLoc;
	get();
}
void file::trunc() {
	std::fstream f = get();
	f.close();
}
std::vector<std::string> file::getlines() {
	std::fstream f = get();
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
	std::fstream f = getTrunc();
	for (int i = 0; i < strVec.size(); i++) {
		f << strVec[i] << std::endl;
	}
	f.close();
}
void file::write(std::string s) {
	std::fstream f = getTrunc();
	f << s << std::endl;
	f.close();
}
bool file::isEmpty() {
	return emptyFlair;
}

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
//else is what variables are set to if settings files are empty
void reader::read() {
	//get isHtml from first line of message file, delete the first line from vector, and save the rest to message vector
	if (!messageF.isEmpty()) {
		std::vector<std::string> mesTemp = messageF.getlines();
		isHtml = std::stoi(mesTemp[0]);
		mesTemp.erase(mesTemp.begin());
		message = mesTemp;
	}
	else
		isHtml = false;
	//get recipiants
	if (!toF.isEmpty())
		recipiants = toF.getlines();
	else
		recipiants = { "Default Message!" };
	//get name
	if (!nameF.isEmpty())
		name = nameF.getlines()[0];
	else
		name = "Sender";
	//get from
	if (!fromF.isEmpty())
		from = fromF.getlines()[0];
	else
		from = "MassMailer@yourcomputer.com";
	//get subject
	if (!subjectF.isEmpty())
		subject = subjectF.getlines()[0];
	else
		subject = "Subject Goes Here";
	//get key
	if (!keyF.isEmpty())
		key = keyF.getlines()[0];
	else
		key = "Key is Undeffined";
	//get threads
	if (!threadF.isEmpty())
		threads = std::stoi(threadF.getlines()[0]);
	else
		threads = 6;
	//get server
	try {
		if (!serverF.isEmpty())
			server = serverF.getlines()[0];
		else
			server = "0";
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

sf::Vector2f reader::readWindow() {
	file f(settingsPathS + windowS, "window");
	std::vector<std::string> line;
	if (!f.isEmpty())
		line = f.getlines();
	else
		line = { "800", "600" };
	dataSave = sf::Vector2f(std::stoi(line[0]), std::stoi(line[1]));
	return dataSave;
}

void reader::saveWindow() {
	file f(settingsPathS + windowS, "window");
	f.write(std::to_string(dataSave.x));
	f.write(std::to_string(dataSave.y));
}
