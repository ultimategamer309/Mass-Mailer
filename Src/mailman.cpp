#include "mailman.h"
//init
mailman::mailman(std::vector<std::string> &recipiants, int threads) {
	set(recipiants, threads);
}
//init
void mailman::set(std::vector<std::string> &recipiants, int threads) {
	//set values
	this->recipiants = &recipiants;
	this->total = recipiants.size();
	this->completed = 0;
	this->remaining = this->total;
	this->threads = threads;
	this->deffined = true;
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
	for (std::string &i : *recipiants) {
		boost::asio::post(pool, boost::bind(&mailman::isValid, i, &recipiantsValidateTemp));
	}
	recipiants1 = recipiantsValidateTemp;
	recipiantsValidateTemp.clear();
	recipiants = &recipiants1;

	return recipiantsValidateTemp.size();
}

bool mailman::isValid(std::string to,
	std::vector<std::string> *validStrings) {
	mail m(to);

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
	for (std::string& i : *recipiants) {
		boost::asio::post(pool, boost::bind(&mailman::send, i));

		++completed;
		--remaining;
	}
	pool.join();
	return true;
}

void mailman::send(std::string to){
	mail m(to);
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
std::vector<std::string>* mailman::getRecipiants() {
	return recipiants;
}
