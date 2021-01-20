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
	inline static void setButton(sf::Color border, sf::Color fill, int outline) {
		buttonBorder = border;
		buttonFill = fill;
		buttonOutline = outline;
		buttonDeffined = true;
	}
	inline static void setCheckbox(sf::Color border,
		sf::Color fill,
		int boxSize,
		int textSeperation,
		bool textRightOfCheckbox) {
		checkboxBorder = border;
		checkboxFill = fill;
		checkboxBoxSize = boxSize;
		checkboxTextSeperation = textSeperation;
		checkboxisLeft = textRightOfCheckbox;
	}
	inline static void setReader(reader* r) {
		read = r;
	}
private:
	//freinds
	friend class button;
	friend class checkbox;
	//Buttons
	inline static sf::Color buttonBorder;
	inline static sf::Color buttonFill;
	inline static int buttonOutline;
	inline static bool buttonDeffined = false;
	//checkbox
	inline static sf::Color checkboxBorder;
	inline static sf::Color checkboxFill;
	inline static int checkboxBoxSize;
	inline static int checkboxTextSeperation;
	inline static bool checkboxisLeft;
	inline static bool checkboxDeffined = false;
	//reader
	inline static reader* read;
};

class setPos {
public:
	inline static sf::Vector2f middle(sf::RectangleShape& back, sf::Text text) {
		return sf::Vector2f(
			(back.getGlobalBounds().width / 2) - (text.getGlobalBounds().width / 2) + back.getGlobalBounds().left,
			(back.getGlobalBounds().height / 2) - (text.getGlobalBounds().height / 1.25) + back.getGlobalBounds().top
		);
	}
	inline static float middleX(sf::RectangleShape& back, sf::Text text) {
		return (back.getGlobalBounds().width / 2) - (text.getGlobalBounds().width / 2) + back.getGlobalBounds().left;
	}
	inline static float middleY(sf::RectangleShape& back, sf::Text text) {
		return (back.getGlobalBounds().height / 2) - (text.getGlobalBounds().height / 1.25) + back.getGlobalBounds().top;
	}
	inline static sf::Vector2f setTop(sf::RenderWindow &window, sf::RectangleShape &rect) {
		return sf::Vector2f(window.getSize().x - rect.getGlobalBounds().width, window.getSize().y - rect.getGlobalBounds().height);
	}
	inline static float getBottom(sf::RectangleShape& rect) {
		return rect.getGlobalBounds().top + rect.getGlobalBounds().height;
	}
	inline static float getBottom(sf::Text& t) {
		return t.getGlobalBounds().top + t.getGlobalBounds().height;
	}
	inline static float getBottom(sf::RectangleShape& rect, sf::Text& t) {
		return rect.getGlobalBounds().top + t.getGlobalBounds().height;
	}
	inline static float getRight(sf::RectangleShape& rect) {
		return rect.getGlobalBounds().left + rect.getGlobalBounds().width;
	}
	inline static float getRight(sf::Text& t) {
		return t.getGlobalBounds().left + t.getGlobalBounds().width;
	}
	inline static float widthDifference(sf::RectangleShape &r, sf::Text &t) {
		return r.getGlobalBounds().width - t.getGlobalBounds().width;
	}
	inline static float heightDifference(sf::RenderWindow &r, sf::RectangleShape& rect) {
		return r.getSize().y - rect.getGlobalBounds().height;
	}
	inline static float heightTotal(sf::RectangleShape& rect, button &b) {
		return rect.getGlobalBounds().height - b.getHight();
	}
};

class builder {
public:
	void setTextDefaults(sf::Font font, int size, sf::Color fill);
	sf::Text text(std::string str,
		int *size = nullptr,
		sf::Font *font = nullptr,
		sf::Color *fill = nullptr);
	void setRectDefaults(sf::Color fill,
		sf::Color borderColor,
		int outlineThickness);
	sf::RectangleShape rect(sf::Vector2f size);
	sf::RectangleShape rect(sf::Vector2f size, sf::Vector2f pos);
private:
	//text
	sf::Font textFont;
	int textSize;
	sf::Color textColor;
	//rect
	sf::Color rectFill;
	sf::Color rectOutlineColor;
	int rectOutlineThickness;
};

class button {
public:
	bool isClicked(sf::Vector2i& clickPos);
	button();
	button(sf::Vector2f leftTop,
		sf::Vector2f widthHeight,
		std::function<void(reader*)> onClick,
		sf::Text& text,
		reader* r = universal::read);
	button(int left,
		int top,
		int width,
		int hight,
		std::function<void(reader*)> onClick,
		sf::Text& text,
		reader* r = universal::read);
	void setup(sf::Vector2f leftTop,
		sf::Vector2f widthHeight,
		std::function<void(reader*)> onClick,
		sf::Text& text,
		reader* r = universal::read);
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
