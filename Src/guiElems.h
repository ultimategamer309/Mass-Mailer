#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Main.hpp"
#include "SFML/Window.hpp"
#include <functional>
#include <vector>
#include <string>
#include <iostream>
#include "reader.h"

class universal {
public:

	//Buttons
	inline static sf::Color buttonBorder;
	inline static sf::Color buttonFill;
	inline static int buttonOutline;
	//checkbox
	inline static sf::Color checkboxBorder;
	inline static sf::Color checkboxFill;
	inline static int checkboxBoxSize;
	inline static int checkboxTextSeperation;
	inline static bool textRightOfCheckbox;
	//reader
	inline static reader* read;
};

class button {
public:
	bool isClicked(sf::Vector2i& clickPos);
	void setup(int left,
		int top,
		int width,
		int hight,
		std::function<void(reader*)> onClick,
		sf::Text& text,
		reader* r = universal::read);
	void updateTextPos();
	void getDrawables(std::vector<sf::Drawable*>& drawVector);
	int getLeft();
	int getTop();
	int getWidth();
	int getHight();

private:
	//interaction
	int left;
	int top;
	int width;
	int hight;
	std::function<void(reader*)> onClick;

	//reader
	reader* r;

	//drawing
	sf::RectangleShape shape;
	sf::Text text;
};

class prompt {
public:
	prompt(std::string question);
	std::string response();

private:
	std::string q;
	std::string r;
};

/*takes a prompt and two options*/
/*if user selects first option returns true*/
class yesOrNo {
public:
	yesOrNo(std::string title,
		std::string statement,
		std::string prompt);
	bool getResponse();
	static void yCallback(reader *r);
	static void nCallback(reader* r);
private:
	bool ret;
};

class alert {
public:
	alert(std::string title, std::string alert, std::string alertLine2 = "", std::string alertLine3 = "");
};

class checkbox {
public:
	checkbox(int left,
		int top,
		sf::Text title,
		bool isTrue);
	checkbox();
	void set(int left,
		int top,
		sf::Text title,
		bool isTrue);
	bool getBool();
	bool isClicked(sf::Vector2i clickPos);

	void getDrawables(std::vector<sf::Drawable*>& drawVector);

	int getLeft();
	int getTop();
	int getWidth();
	int getHight();
private:
	void wasClicked();
	int left;
	int top;
	int boxSize;
	int spaceFromCheckboxToText;
	bool textRightOfCheckbox;
	sf::Text t;
	sf::Color border;
	sf::Color fill;
	sf::Sprite check;
	
	//sprite textures
	sf::Texture checkedT;
	sf::Texture uncheckedT;

	//bool
	bool checked;

	//rect
	sf::RectangleShape r;
};
