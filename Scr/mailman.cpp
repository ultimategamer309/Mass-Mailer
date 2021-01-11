#include "mailman.h"
//init
mailman::mailman(std::vector<std::string> recipiants,
	std::string from,
	std::string senderName,
	std::string subject,
	std::vector<std::string> message,
	std::string key,
	int threads,
	bool isHtml,
	std::string server) {
	set(recipiants,
		from,
		senderName,
		subject,
		message,
		key,
		threads,
		isHtml,
		server);
}
//init
void mailman::set(std::vector<std::string> recipiants,
	std::string from,
	std::string senderName,
	std::string subject,
	std::vector<std::string> message,
	std::string key,
	int threads,
	bool isHtml,
	std::string server) {
	//set values
	this->recipiants = recipiants;
	this->from = from;
	this->senderName = senderName;
	this->message = message;
	this->secret = key;
	this->total = recipiants.size();
	this->completed = 0;
	this->remaining = this->total;
	this->threads = threads;
	this->isHtml = isHtml;
	this->server = server;
	this->deffined = true;
	this->subject = subject;
}

//init latter
mailman::mailman() {
	this->deffined = false;
}

//deconstruct
mailman::~mailman() {
}

/*removes invalid emails from list *
 *returns the number of remaining  *
 *emails in the list unless mailman*
 *is undefined then returns -1     */
int mailman::validate() {
	if (!deffined)
		return -1;

	recipiantsValidateTemp.clear();
	boost::asio::thread_pool pool(threads);
	for (std::string i : recipiants) {
		boost::asio::post(pool, boost::bind(&mailman::isValid,
			i,
			from,
			senderName,
			subject,
			message,
			secret,
			&recipiantsValidateTemp,
			server));
	}
	recipiants = recipiantsValidateTemp;
	recipiantsValidateTemp.clear();

	return recipiantsValidateTemp.size();
}

bool mailman::isValid(std::string to,
	std::string from,
	std::string senderName,
	std::string subject,
	std::vector<std::string> message,
	std::string secret,
	std::vector<std::string> *validStrings,
	std::string server) {
	mail m;

	if (server == "")
		m.set(to, from, senderName, subject, message, secret);
	else
		m.set(to, from, senderName, subject, message, secret, false, server);

	bool res = m.vaidate();
	if (res) {
		validStrings->emplace_back(to);
		return true;
	}
	return false;
}

//if returns false then mailman is undeffined;
bool mailman::start()
{
	if (!deffined)
		return false;

	boost::asio::thread_pool pool(threads);
	for (int i = 0; i < total; i++) {
		boost::asio::post(pool, boost::bind(&mailman::send,
			recipiants[0],
			from,
			senderName,
			subject,
			message,
			secret,
			isHtml,
			server));

		++completed;
		--remaining;
	}
	pool.join();
	return true;
}

void mailman::send(std::string to,
	std::string from,
	std::string senderName,
	std::string subject,
	std::vector<std::string> message,
	std::string secret,
	bool html,
	std::string server){
	mail m;
	if (server == "0")
		m.set(to, from, senderName, subject, message, secret, html);
	else
		m.set(to, from, senderName, subject, message, secret, html, server);
	m.send();
}

/*Returns the percent of progress as a double*/
double mailman::getProgressAsPercent() {
	return double(completed / total);
}
/*Returns the number of completed emails sent*/
int mailman::getCompleted() {
	return completed;
}
/*Returns the number of remaining emails to send*/
int mailman::getRemaining() {
	return remaining;
}
/*Returns the mailing list*/
std::vector<std::string> mailman::getRecipiants() {
	return recipiants;
}