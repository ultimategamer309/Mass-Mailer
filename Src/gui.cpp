#include "gui.h"

gui::gui() {
	std::cout << "logs will be displayed here" << std::endl;
	read.open();
	sf::Vector2f d = read.readWindow();
	read.read();
	window.create(sf::VideoMode(d.x, d.y, 32), windowName);
	backround = sf::Color(48, 48, 48);
	titleBar = sf::Color(33, 33, 33);
	secondary = sf::Color(66, 66, 66);
	textColor = sf::Color::Black;
	titleColor = sf::Color::White;
	font.loadFromFile("Assets/Fonts/NimbusRom.otf");
	titleFont.loadFromFile("Assets/Fonts/title.ttf");

	//set universals
	//Buttons
	universal::setButton(backround, titleBar, 5);
	//checkbox
	universal::setCheckbox(textColor, titleBar, 25, 10, true);
	//reader
	universal::setReader(&read);

	//builder set defaults
	builder.setTextDefaults(font, textSize, titleColor);
	builder.setRectDefaults(secondary, sf::Color::Black, outline);

	sf::RectangleShape titleBack;
	titleBack.setFillColor(titleBar);
	titleBack.setPosition(0, 0);
	titleBack.setSize(sf::Vector2f((float)window.getSize().x, (float)window.getSize().x / 10));
	toDraw.emplace_back(&titleBack);


	sf::Text titleText = builder.text(windowName,
		&titleSize,
		&titleFont);
	titleText.setPosition(setPos::middle(titleBack, titleText));
	toDraw.emplace_back(&titleText);

	//send and validate box
	sf::RectangleShape sendBox = builder.rect(sf::Vector2f((window.getSize().x / 3), window.getSize().x / 10));
	sendBox.setPosition(setPos::setTop(window, sendBox));
	toDraw.emplace_back(&sendBox);

	sf::Text sendTitle = builder.text("Send", &subtitleSize);
	sendTitle.setPosition(setPos::middleX(sendBox, sendTitle),
		(sendTitle.getGlobalBounds().height * .1) + sendBox.getGlobalBounds().top);
	toDraw.emplace_back(&sendTitle);

	sf::Text validateT = builder.text("Validate List");
	button validateB(sf::Vector2f(sendBox.getGlobalBounds().left + 25, setPos::getBottom(sendTitle) + 10),
		sf::Vector2f((sendBox.getGlobalBounds().width - 65) / 2, sendBox.getGlobalBounds().height / 4),
		validateCallback,
		validateT);
	validateB.getDrawables(toDraw);
	buttonsVec.emplace_back(std::make_unique<button>(validateB));

	sf::Text sendT = builder.text("Start Sending");
	button sendB(sf::Vector2f(validateB.getLeft() + validateB.getWidth() + 15, validateB.getTop()),
		sf::Vector2f(validateB.getWidth(), validateB.getHight()),
		sendCallback,
		sendT);
	sendB.getDrawables(toDraw);
	buttonsVec.emplace_back(std::make_unique<button>(sendB));

	//mailing list box
	sf::RectangleShape ListBox;
	ListBox = builder.rect(sf::Vector2f((window.getSize().x / 1.5 - (outline * 7)), sendBox.getGlobalBounds().height - (outline * 2)),
		sf::Vector2f(outline * 2, sendBox.getGlobalBounds().top + outline));
	toDraw.emplace_back(&ListBox);

	sf::Text listTitle = builder.text("Mailing List", &subtitleSize);
	listTitle.setPosition(setPos::middleX(ListBox, listTitle),
		(listTitle.getGlobalBounds().height * .1) + ListBox.getGlobalBounds().top);
	toDraw.emplace_back(&listTitle);

	loadedEmails = builder.text("Emails: " + std::to_string(loadedEmailsInt), &bigTextSize);
	loadedEmails.setPosition((loadedEmails.getGlobalBounds().width / 2) + ListBox.getGlobalBounds().left - 25,
		(listTitle.getGlobalBounds().height * .1) + setPos::getBottom(ListBox, listTitle) + 10);
	toDraw.emplace_back(&loadedEmails);

	sf::Text veiwListT = builder.text("Veiw Sample");
	button veiwListB(sf::Vector2f(loadedEmails.getGlobalBounds().left, listTitle.getGlobalBounds().top + 5),
		sf::Vector2f((setPos::widthDifference(ListBox, loadedEmails) - 10) / 4,	loadedEmails.getGlobalBounds().height),
		veiwListCallback,
		veiwListT);
	veiwListB.getDrawables(toDraw);
	buttonsVec.emplace_back(std::make_unique<button>(veiwListB));

	sf::Text loadListT = builder.text("Load");
	button loadListB(sf::Vector2f(setPos::getRight(loadedEmails) + 25, loadedEmails.getGlobalBounds().top),
		sf::Vector2f((setPos::widthDifference(ListBox, loadedEmails) - 125) / 5, loadedEmails.getGlobalBounds().height),
		loadListCallback,
		loadListT);
	loadListB.getDrawables(toDraw);
	buttonsVec.emplace_back(std::make_unique<button>(loadListB));

	sf::Text exportListT = builder.text("Export");
	button exportListB(sf::Vector2f(loadListB.getLeft() + loadListB.getWidth() + 15, loadListB.getTop()),
		sf::Vector2f(loadListB.getWidth(), loadListB.getHight()),
		exportListCallback,
		exportListT);
	exportListB.getDrawables(toDraw);
	buttonsVec.emplace_back(std::make_unique<button>(exportListB));

	sf::Text addListT = builder.text("Add");
	button addListB(sf::Vector2f(exportListB.getLeft() + exportListB.getWidth() + 15, exportListB.getTop()),
		sf::Vector2f(exportListB.getWidth(), exportListB.getHight()),
		addRecipiantCallback,
		addListT);
	addListB.getDrawables(toDraw);
	buttonsVec.emplace_back(std::make_unique<button>(addListB));

	sf::Text removeListT = builder.text("Remove");
	button removeListB(sf::Vector2f(addListB.getLeft() + addListB.getWidth() + 15, addListB.getTop()),
		sf::Vector2f(addListB.getWidth(), addListB.getHight()),
		removeRecipiantCallback,
		removeListT);
	removeListB.getDrawables(toDraw);
	buttonsVec.emplace_back(std::make_unique<button>(removeListB));

	sf::Text wipeListT = builder.text("Wipe");
	button wipeListB(sf::Vector2f(removeListB.getLeft() + removeListB.getWidth() + 15, removeListB.getTop()),
		sf::Vector2f(removeListB.getWidth(), removeListB.getHight()),
		wipeRecipiantCallback,
		wipeListT);
	wipeListB.getDrawables(toDraw);
	buttonsVec.emplace_back(std::make_unique<button>(wipeListB));

	//progress bar
	sf::Text progbarT("Progress: NULL", font, bigTextSize);
	button progbarB(sf::Vector2f(10,	ListBox.getGlobalBounds().top - 60),
		sf::Vector2f(window.getSize().x - 20, 50),
		validateCallback,
		progbarT);
	prog = &progbarT;
	progB = &progbarB;
	progbarB.getDrawables(toDraw);

	//settings
	sf::RectangleShape settingBox = builder.rect(
		sf::Vector2f(window.getSize().x - 20, setPos::heightDifference(window, ListBox) - setPos::heightTotal(titleBack, progbarB) - 80),
		sf::Vector2f(10, titleBack.getGlobalBounds().height + 30));
	toDraw.emplace_back(&settingBox);
	toDraw.emplace_back(&settingBox);

	//change rect defaults
	builder.setRectDefaults(sf::Color::Transparent, sf::Color::Black, outline2);

	//sender info settings
	sf::Text senderNameT = builder.text("Change Sender Name");
	button senderNameB(sf::Vector2f(settingBox.getGlobalBounds().left + 20,	settingBox.getGlobalBounds().top + 20),
		sf::Vector2f(senderNameT.getGlobalBounds().width + 20, senderNameT.getGlobalBounds().height + 20),
		changeSenderCallback,
		senderNameT);
	senderNameB.getDrawables(toDraw);
	buttonsVec.emplace_back(std::make_unique<button>(senderNameB));

	sf::Text senderEmailT = builder.text("Change Sender Email");
	button senderEmailB(sf::Vector2f(senderNameB.getLeft(), senderNameB.getTop() + senderNameB.getHight() + 20),
		sf::Vector2f(senderEmailT.getGlobalBounds().width + 20,	senderEmailT.getGlobalBounds().height + 20),
		changeSendEmailCallback,
		senderEmailT);
	senderEmailB.getDrawables(toDraw);
	buttonsVec.emplace_back(std::make_unique<button>(senderEmailB));

	sf::RectangleShape senderSettings = builder.rect(
		sf::Vector2f(senderEmailB.getWidth() + 20, senderEmailB.getHight() + senderNameB.getHight() + 40),
		sf::Vector2f(senderNameB.getLeft() - 10, senderNameB.getTop() - 10)
	);
	toDraw.emplace_back(&senderSettings);

	//setting title
	sf::Text settingsT = builder.text("Settings", &settingSize);

	settingsT.setPosition(setPos::middleX(settingBox, settingsT),
		(senderSettings.getGlobalBounds().height / 2) - (settingsT.getGlobalBounds().height / 2) + settingBox.getGlobalBounds().top);
	toDraw.emplace_back(&settingsT);

	//message settings
	sf::Text isHtmlT = builder.text("Message is in html");
	isHtmlB.set(senderEmailB.getLeft(),
		(senderNameB.getHight() / 2) - (25 / 2) + senderEmailB.getTop() + senderEmailB.getHight() + 20,
		isHtmlT,
		read.getIsHtml());
	isHtmlB.getDrawables(toDraw);
	checkboxVec.emplace_back(&isHtmlB);

	sf::Text mesSubjectT = builder.text("Change Email Subject");
	button mesSubjectB(sf::Vector2f(isHtmlB.getLeft(), senderNameB.getTop() + (3 * senderNameB.getHight()) + 60),
		sf::Vector2f(senderEmailB.getWidth(), senderEmailB.getHight()),
		changeSubjectCallback,
		mesSubjectT);
	mesSubjectB.getDrawables(toDraw);
	buttonsVec.emplace_back(std::make_unique<button>(mesSubjectB));

	sf::Text mesT = builder.text("Change Email Message");
	button mesB(sf::Vector2f(isHtmlB.getLeft(),	mesSubjectB.getTop() + senderEmailB.getHight() + 20),
		sf::Vector2f(senderEmailB.getWidth(), senderEmailB.getHight()),
		changeMessageCallback,
		mesT);
	mesB.getDrawables(toDraw);
	buttonsVec.emplace_back(std::make_unique<button>(mesB));

	sf::RectangleShape mesSettings = builder.rect(sf::Vector2f(mesSubjectB.getWidth() + 20, (senderEmailB.getHight() * 3) + 55),
		sf::Vector2f(senderSettings.getGlobalBounds().left + outline2, isHtmlB.getTop() - 10));
	toDraw.emplace_back(&mesSettings);

	//threads
	threadCurr = read.getThreads();
	sf::Text threadsAllotedT = builder.text("Maximum Threads: " + std::to_string(threadCurr));
	button threadsAlloted(sf::Vector2f(mesB.getLeft() + mesB.getWidth() + 30, mesB.getTop()),
		sf::Vector2f(mesB.getWidth() * 2, mesB.getHight()),
		changeThreadsCallback,
		threadsAllotedT);
	threadsT = &threadsAllotedT;
	threadsB = &threadsAlloted;
	threadsAlloted.getDrawables(toDraw);
	buttonsVec.emplace_back(std::make_unique<button>(threadsAlloted));

	sf::Text changeThreadsT = builder.text("Change Max Threads");
	button changeThreadsB(sf::Vector2f((threadsAlloted.getLeft() / 2) - (mesSubjectB.getWidth() / 1.333) + threadsAlloted.getWidth(), mesSubjectB.getTop()),
		sf::Vector2f(mesSubjectB.getWidth(), mesSubjectB.getHight()),
		changeThreadsCallback,
		changeThreadsT);
	changeThreadsB.getDrawables(toDraw);
	buttonsVec.emplace_back(std::make_unique<button>(changeThreadsB));

	sf::RectangleShape threadsBox = builder.rect(sf::Vector2f(threadsAlloted.getWidth() + 20, (threadsAlloted.getHight() * 2) + 40),
		sf::Vector2f(threadsAlloted.getLeft() - 10, changeThreadsB.getTop() - 10));
	toDraw.emplace_back(&threadsBox);

	//server
	sf::Text isDefaultServerT = builder.text("Use Default Server");
	isDefaultServerC.set(threadsAlloted.getLeft() + threadsAlloted.getWidth() + 30,
		(senderNameB.getHight() / 2) + senderNameB.getTop() - (25 / 2) - outline,
		isDefaultServerT,
		read.isDefaultServer());
	isDefaultServerC.getDrawables(toDraw);
	checkboxVec.emplace_back(&isDefaultServerC);

	sf::Text changeServerT = builder.text("Change Server");
	button changeServerB(sf::Vector2f(isDefaultServerC.getLeft(), senderEmailB.getTop() - 10),
		sf::Vector2f(senderEmailB.getWidth(), senderEmailB.getHight()),
		changeServerCallback,
		changeServerT);
	changeServerB.getDrawables(toDraw);
	buttonsVec.emplace_back(std::make_unique<button>(changeServerB));

	sf::Text changeKeyT = builder.text("Change Server Key");
	button changeKeyB(sf::Vector2f(isDefaultServerC.getLeft(), changeServerB.getTop() + changeServerB.getHight() + 20),
		sf::Vector2f(senderEmailB.getWidth(), senderEmailB.getHight()),
		changeKeyCallback,
		changeKeyT);
	changeKeyB.getDrawables(toDraw);
	buttonsVec.emplace_back(std::make_unique<button>(changeKeyB));

	sf::RectangleShape serverBox = builder.rect(sf::Vector2f(changeKeyB.getWidth() + 20, (threadsAlloted.getHight() * 3) + 50),
		sf::Vector2f(changeKeyB.getLeft() - 10, senderSettings.getGlobalBounds().top + outline2));
	toDraw.emplace_back(&serverBox);

	//debugg
	sf::Text isdebuggingT = builder.text("Show Debugger");
	isDegugC.set(changeKeyB.getLeft(),
		(changeThreadsB.getHight() / 2) + changeThreadsB.getTop() - (25 / 2) - outline,
		isdebuggingT,
		false);
	isDegugC.getDrawables(toDraw);
	checkboxVec.emplace_back(&isDegugC);

	//save
	sf::Text saveDataT = builder.text("Save Settings");
	button saveDataB(sf::Vector2f(changeKeyB.getLeft(), threadsAlloted.getTop()),
		sf::Vector2f(senderEmailB.getWidth(), senderEmailB.getHight()),
		saveDataCallback,
		saveDataT);
	saveDataB.getDrawables(toDraw);
	buttonsVec.emplace_back(std::make_unique<button>(saveDataB));

	sf::RectangleShape saveBox = builder.rect(sf::Vector2f(saveDataB.getWidth() + 20, (saveDataB.getHight() * 2) + 40),
		sf::Vector2f(changeKeyB.getLeft() - 10, changeThreadsB.getTop() - 10));
	toDraw.emplace_back(&saveBox);

	//display server
	serverT = builder.text("Server: Default", &medTextSize);
	serverT.setPosition(setPos::middleX(settingBox, serverT),
		settingsT.getGlobalBounds().top + settingsT.getGlobalBounds().height);
	toDraw.emplace_back(&serverT);

	while (window.isOpen()) {
		update();
		draw();
		events();
	}
}

void gui::update() {
	//check for newly loaded emails
	loadedEmails.setString("Emails: " + std::to_string(read.getRecipiantsSize()));
	for (int i = 0; i < checkboxVec.size(); i++) {
		checkboxVec[i]->getBool();
	}
	read.changeIsHtml(isHtmlB.getBool());
	isDefaultServer = isDefaultServerC.getBool();
	isDebugging = isDegugC.getBool();
	if (running) {
		prog->setString("Progress: " + mailman.getCompleted() + '/' + mailman.getCompleted() + mailman.getRemaining());
		progB->updateTextPos();
	}
	if (threadCurr != read.getThreads()) {
		threadCurr = read.getThreads();
		threadsT->setString("Maximum Threads: " + std::to_string(threadCurr));
		threadsB->updateTextPos();
	}
	if (isDebugging != isConsoleVisable()) {
		if (isConsoleVisable())
			hideConsole();
		else
			showConsole();
	}
	if (isDefaultServer || read.getServer() == "default") {
		read.changeServer("0");
		serverT.setString("Server: Default");
	}
	else
		serverT.setString("Server: " + read.getServer());

}
gui::~gui() {
	window.close();
}

void gui::draw() {
	//clear
	window.clear(backround);

	//draw everyting else
	if (toDraw.size() >= 0) {
		for (int i = 0; i < toDraw.size(); i++) {
			window.draw(*toDraw[i]);
		}
	}

	//display
	window.display();
}

void gui::events() {
	sf::Event event;
	while (window.pollEvent(event)) {
		//if close button pressed close window
		if (event.type == sf::Event::Closed) {
			window.close();
		}
		//if mouse input
		else if (event.type == sf::Event::MouseButtonPressed) {
			//if left click
			if (event.mouseButton.button == sf::Mouse::Left) {
				sf::Vector2i position = sf::Mouse::getPosition();
				for (int i = 0; i < buttonsVec.size(); i++) {
					sf::Vector2i position = sf::Mouse::getPosition(window);
					button b = *buttonsVec[i];
					b.isClicked(position);
				}
				for (int i = 0; i < checkboxVec.size(); i++) {
					sf::Vector2i position = sf::Mouse::getPosition(window);
					checkboxVec[i]->isClicked(position);
				}
			}
		}
	}
}

void gui::hideConsole() {
	ShowWindow(myConsole, SW_HIDE);
}
void gui::showConsole() {
	ShowWindow(myConsole, SW_SHOW);
}
bool gui::isConsoleVisable() {
	return IsWindowVisible(myConsole);
}

//callbacks
void gui::validateCallback(reader* r) {
	alert("Validating", "Starting Validation");
	r->validate();
}
void gui::sendCallback(reader* r) {
	alert("Sending", "Sending Emails");
	r->send();
}

/*show first 5 emails. So you know emails loaded correctly*/
void gui::veiwListCallback(reader* r) {
	std::vector<std::string> vec = r->getRecipiants();
	srand(time(NULL));
	int i = rand() % vec.size();
	int j = rand() % vec.size();
	int k = rand() % vec.size();
	alert("Veiw Sample", vec[i], vec[j], vec[k]);
}
void gui::exportListCallback(reader* r) {
	prompt p("File Path to Save to");
	r->exportList(p.response());
}
void gui::loadListCallback(reader* r) {
	prompt p("File Path to Load From");
	r->replaceRecipiants(p.response());
}
void gui::addRecipiantCallback(reader* r) {
	prompt p("Email to Add");
	r->addRecipiant(p.response());
}
void gui::removeRecipiantCallback(reader* r) {
	prompt p("Email to Remove");
	yesOrNo q("Are You Sure", "You Are About to Remove " + p.response(), "Are you sure you want to remove " + p.response());
	if (q.getResponse())
		r->removeRecipiant(p.response());
}
void gui::wipeRecipiantCallback(reader* r) {
	yesOrNo q("Are You Sure", "You Are About to Wipe the Email List", "Are you sure you want to do this?");
	if (q.getResponse())
		r->wipeRecipiants();
}

//settings
void gui::changeSenderCallback(reader* r) {
	yesOrNo q("Sender Name", "The Current Sender is: " + r->getName(), "Do you want to change it?");
	if (q.getResponse()) {
		prompt p("New Sender Name");
		r->changeSenderName(p.response());
	}
}
void gui::changeSendEmailCallback(reader* r) {
	yesOrNo q("Sender Email", "The Current From Address is: " + r->getFrom(), "Do you want to change it?");
	if (q.getResponse()) {
		prompt p("New Sender Email");
		std::cout << p.response();
		r->changeFromAdress(p.response());
	}
}
void gui::changeSubjectCallback(reader* r) {
	yesOrNo q("Subject", "The Current Email Subject is: " + r->getSubject(), "Do you want to change it?");
	if (q.getResponse()) {
		prompt p("New Subject");
		r->changeSubject(p.response());;
	}
}
void gui::changeMessageCallback(reader* r) {
	prompt p("Load Message File Path");
	r->changeMessage(p.response());
}
void gui::changeThreadsCallback(reader* r) {
	yesOrNo q("Threads", "The Current Maximum Threads are: " + r->getThreads(), "Do you want to change it?");
	if (q.getResponse()) {
		bool isInt = false;
		int myInt;
		while (!isInt) {
			try {
				prompt p("New Maximum Threads");
				myInt = std::stoi(p.response());
				if (myInt <= 0) {
					alert("Must Have At Least 1 Thread", "Please Allot At Least One Thread");
				}
				else
					isInt = true;
			}
			catch (int err) {
				alert("Use a Integer", "Type Integers Only", "No Charecters");
			}
		}
		r->changeThreads(myInt);
	}
}
void gui::changeServerCallback(reader* r) {
	yesOrNo q("Server", "The Current Server is: " + r->getServer(), "Do you want to change it?");
	if (q.getResponse()) {
		if (!r->isDefaultServer()) {
			prompt p("New Server");
			r->changeServer(p.response());
		}
		else {
			alert("Unselect \"Use Default Server\"", "Unselect \"Use Default Server\" in order to change the server");
		}
	}
}
void gui::changeKeyCallback(reader* r) {
	yesOrNo q("Server Key", "The Current Server Key is: " + r->getKey(), "Do you want to change it?");
	if (q.getResponse()) {
		prompt p("New Server Key");
		r->changeKey(p.response());
	}
}
void gui::saveDataCallback(reader* r) {
	yesOrNo q("Save", "This Will Overide the Previous Save", "Do you want to continue?");
	if (q.getResponse()) {
		r->save();
		alert("Saved", "Settings Saved");
	}
}
