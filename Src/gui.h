#pragma once
#include "SFML/Graphics.hpp"
#include <functional>
#include <time.h>
#include <stdlib.h>
#include "reader.h";
#include "mailman.h"
#include "guiElems.h"
#include "SFML/Main.hpp"
#include "SFML/Window.hpp"

class gui
{
public:
	gui();
	~gui();

	/**********************/
	/******call backs******/
	/**********************/

	//mail list callbacks
	static void veiwListCallback(reader* r);
	static void exportListCallback(reader* r);
	static void loadListCallback(reader* r);

	static void addRecipiantCallback(reader* r);
	static void removeRecipiantCallback(reader* r);
	static void wipeRecipiantCallback(reader* r);

	//send callbacks
	static void validateCallback(reader* r);
	static void sendCallback(reader* r);

	//progress bar
	static void progBarCallback(reader* r);

	/*settings*/
	//sender info
	static void changeSenderCallback(reader* r);
	static void changeSendEmailCallback(reader* r);
	//message info
	static void changeSubjectCallback(reader* r);
	static void changeMessageCallback(reader* r);
	//threading
	static void changeThreadsCallback(reader* r);
	//server
	static void changeServerCallback(reader* r);
	static void changeKeyCallback(reader* r);
	//save
	static void saveDataCallback(reader* r);
	/*settings*/

	/**********************/
	/******call backs******/
	/**********************/

private:
	void draw();
	void events();
	void update();

	//Texts
	std::string windowName = "Mass Mailer";

	//window
	sf::RenderWindow window;

	//colors
	sf::Color titleColor;
	sf::Color backround;
	sf::Color titleBar;
	sf::Color secondary;
	sf::Color textColor;

	//Vectors
	std::vector<sf::Drawable*> toDraw;
	std::vector<std::unique_ptr<button>> buttonsVec;
	std::vector<checkbox*> checkboxVec;

	//fonts
	sf::Font titleFont;
	sf::Font font;

	//text sises
	int titleSize = 48;
	int settingSize = 42;
	int subtitleSize = 36;
	int bigTextSize = 24;
	int medTextSize = 20;
	int textSize = 17;
	int outline = 5;
	int outline2 = 2;

	//reader
	reader read;

	//vars
	int loadedEmailsInt;
	sf::Text loadedEmails;

	//texts
	sf::Text* prog;
	button* progB;
	sf::Text* threadsT;
	button* threadsB;
	int threadCurr;
	sf::Text serverT;

	//mailman
	bool running = false;
	mailman mailman;

	//bools
	bool isDefaultServer;
	bool isDebugging;

	//checkboxes
	checkbox isHtmlB;
	checkbox isDefaultServerC;
	checkbox isDegugC;

	//console comands
	HWND myConsole = GetConsoleWindow();
	void hideConsole();
	void showConsole();
	bool isConsoleVisable();

	//builder
	builder builder;
};
