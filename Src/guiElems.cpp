#include "guiElems.h"

void builder::setTextDefaults(sf::Font font, int size, sf::Color fill) {
	textFont = font;
	textSize = size;
	textColor = fill;
}
sf::Text builder::text(std::string str,
	int* size,
	sf::Font* font,
	sf::Color* fill) {
	sf::Text t;
	t.setString(str);
	if (!size)
		t.setCharacterSize(textSize);
	else
		t.setCharacterSize(*size);
	if (!font)
		t.setFont(textFont);
	else
		t.setFont(*font);
	if (!fill)
		t.setFillColor(textColor);
	else
		t.setFillColor(*fill);
	return t;
}
void builder::setRectDefaults(sf::Color fill,
	sf::Color borderColor,
	int outlineThickness) {
	rectFill = fill;
	rectOutlineColor = borderColor;
	rectOutlineThickness = outlineThickness;
}
sf::RectangleShape builder::rect(sf::Vector2f size, sf::Vector2f pos) {
	sf::RectangleShape r(size);
	r.setPosition(pos);
	r.setFillColor(rectFill);
	r.setOutlineColor(rectOutlineColor);
	r.setOutlineThickness(rectOutlineThickness);
	return r;
}
sf::RectangleShape builder::rect(sf::Vector2f size) {
	sf::RectangleShape r(size);
	r.setFillColor(rectFill);
	r.setOutlineColor(rectOutlineColor);
	r.setOutlineThickness(rectOutlineThickness);
	return r;
}

//button
button::button() {}
button::button(sf::Vector2f leftTop,
	sf::Vector2f widthHeight,
	std::function<void(reader*)> onClick,
	sf::Text& text,
	reader* ) {
	this->setup(leftTop.x,
		leftTop.y,
		widthHeight.x,
		widthHeight.y,
		onClick,
		text);
}
button::button(int left,
	int top,
	int width,
	int hight,
	std::function<void(reader*)> onClick,
	sf::Text& text,
	reader* r) {
	this->setup(left,
		top,
		width,
		hight,
		onClick,
		text,
		r);
}
void button::setup(sf::Vector2f leftTop,
	sf::Vector2f widthHeight,
	std::function<void(reader*)> onClick,
	sf::Text& text,
	reader* r) {
	this->setup(leftTop.x,
		leftTop.y,
		widthHeight.x,
		widthHeight.y,
		onClick,
		text);
}
void button::setup(int left,
	int top,
	int width,
	int hight,
	std::function<void(reader*)> onClick,
	sf::Text& text,
	reader* r) {
	button::left = left;
	button::top = top;
	button::width = width;
	button::hight = hight;
	button::onClick = onClick;
	this->r = r;

	sf::RectangleShape shape;
	if (universal::buttonDeffined) {
		shape.setFillColor(universal::buttonFill);
		shape.setOutlineColor(universal::buttonBorder);
		shape.setOutlineThickness(universal::buttonOutline);
	}
	else {
		shape.setFillColor(sf::Color::White);
		shape.setOutlineColor(sf::Color::Black);
		shape.setOutlineThickness(5);
	}
	shape.setSize(sf::Vector2f(width, hight));
	shape.setPosition(left, top);

	sf::Text t;
	t = text;
	t.setPosition(setPos::middle(shape, text));

	button::shape = shape;
	button::text = t;
}

void button::updateTextPos() {
	text.setPosition(setPos::middle(shape, text));
}

bool button::isClicked(sf::Vector2i& clickPos) {
	sf::FloatRect rect;
	rect.left = button::left;
	rect.top = button::top;
	rect.height = button::hight;
	rect.width = button::width;

	if (rect.contains(float(clickPos.x), float(clickPos.y))) {
		button::onClick(r);
		return true;
	}
	return false;
}

void button::getDrawables(std::vector<sf::Drawable*>& drawVector) {
	drawVector.emplace_back(&shape);
	drawVector.emplace_back(&text);
}

int button::getLeft() {
	const int ret = left;
	return ret;
}
int button::getTop() {
	const int ret = top;
	return ret;
}
int button::getWidth() {
	const int ret = width;
	return ret;
}
int button::getHight() {
	const int ret = hight;
	return ret;
}

//prompt
prompt::prompt(std::string question) {
	q = question;

	sf::RenderWindow window(sf::VideoMode(400, 200), "???", sf::Style::None);

	// Create a text to display
	sf::Font font;
	font.loadFromFile("Assets/Fonts/NimbusRom.otf");
	sf::Text text(q, font, 24);
	text.setPosition(200 - (text.getGlobalBounds().width / 2), 0);
	// Create backer to text
	sf::RectangleShape tBack;
	tBack.setFillColor(sf::Color(33, 33, 33));
	tBack.setSize(sf::Vector2f(400, 40));

	sf::Text res("Response: ", font, 24);
	res.setPosition(0, 50);

	//Create backer to response
	sf::RectangleShape rBack;
	rBack.setFillColor(sf::Color(66, 66, 66));
	rBack.setSize(sf::Vector2f(380, 200 - res.getGlobalBounds().height - res.getGlobalBounds().top - 20));
	rBack.setPosition(10, 45 + 2 * res.getGlobalBounds().height);

	//make response string
	std::string response = "";
	sf::Text responseT("Start Typing", font, 16);
	responseT.setPosition(12, rBack.getGlobalBounds().top);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::TextEntered) {
				//if backspace 
				if (event.text.unicode == 8 && response != "")
					response.erase(response.size() - 1);
				else if (event.text.unicode < 128)
					response += (static_cast<char>(event.text.unicode));
				responseT.setString(response);
			}
			else if (event.type == sf::Event::KeyPressed) {
				//if delete 
				if (event.key.code == sf::Keyboard::Delete) {
					if (response.size() > 0)
						response.erase(response.size() - 1);
					responseT.setString(response);
				}
				//if enter
				if (event.key.code == sf::Keyboard::Enter) {
					r = response;
					window.close();
					return;
				}

			}
			else if (event.type == sf::Event::Closed)
				window.close();

		}
		window.clear(sf::Color(48, 48, 48));
		window.draw(tBack);
		window.draw(text);
		window.draw(res);
		window.draw(rBack);
		window.draw(responseT);
		window.display();
	}
}
std::string prompt::response() {
	return r;
}

//choices
yesOrNo::yesOrNo(std::string title,
	std::string statement,
	std::string prompt) {
	sf::RenderWindow window(sf::VideoMode(400, 200), title);
	std::vector<sf::Drawable*> d;

	// Create a text to display
	sf::Font font;
	font.loadFromFile("Assets/Fonts/NimbusRom.otf");
	sf::Text text(title, font, 24);
	text.setPosition(200 - (text.getGlobalBounds().width / 2), 0);
	// Create backer to text
	sf::RectangleShape tBack;
	tBack.setFillColor(sf::Color(33, 33, 33));
	tBack.setSize(sf::Vector2f(400, 40));
	d.emplace_back(&tBack);

	sf::Text res(statement, font, 24);
	res.setPosition(0, 50);
	d.emplace_back(&res);

	//Create backer to response
	sf::RectangleShape rBack;
	rBack.setFillColor(sf::Color(66, 66, 66));
	rBack.setSize(sf::Vector2f(380, 200 - text.getGlobalBounds().height - text.getGlobalBounds().top - 20));
	rBack.setPosition(10, 45 + 2 * text.getGlobalBounds().height);
	d.emplace_back(&rBack);

	//prompt
	sf::Text promptT(prompt, font, 16);
	promptT.setPosition(12, rBack.getGlobalBounds().top);
	d.emplace_back(&promptT);
	reader r;
	//yes or no buttons
	sf::Text yT("Yes", font, 18);
	button y(sf::Vector2f(rBack.getGlobalBounds().left + rBack.getGlobalBounds().width - yT.getGlobalBounds().width - 30, 200 - 22 - 10),
		sf::Vector2f(yT.getGlobalBounds().width + 10, yT.getGlobalBounds().height + 10),
		yCallback,
		yT,
		&r);
	std::cout << y.getHight();
	sf::Text nT("No", font, 18);
	button n(sf::Vector2f(y.getLeft() - nT.getGlobalBounds().width - 35, y.getTop()),
		sf::Vector2f(y.getWidth(), y.getHight()),
		nCallback,
		nT,
		&r);
	y.getDrawables(d);
	n.getDrawables(d);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::MouseButtonPressed) {
				//if left click
				if (event.mouseButton.button == sf::Mouse::Left) {
					sf::Vector2i position = sf::Mouse::getPosition(window);
					if (y.isClicked(position) || n.isClicked(position))
						window.close();
					ret = r.getIsHtml();
				}
			}
		}
		window.clear(sf::Color(48, 48, 48));
		for (int i = 0; i < d.size(); i++) {
			window.draw(*d[i]);
		}
		window.draw(text);
		window.display();
	}
}
void yesOrNo::yCallback(reader* r) {
	r->changeIsHtml(true);
}
void yesOrNo::nCallback(reader* r) {
	r->changeIsHtml(false);
}
bool yesOrNo::getResponse() {
	return ret;
}

//alert
alert::alert(std::string title, std::string alert, std::string alertLine2, std::string alertLine3) {
	sf::RenderWindow window(sf::VideoMode(400, 200), title);

	// Create a text to display
	sf::Font font;
	font.loadFromFile("Assets/Fonts/NimbusRom.otf");
	sf::Text text(title, font, 24);
	text.setPosition(200 - (text.getGlobalBounds().width / 2), 0);
	// Create backer to text
	sf::RectangleShape tBack;
	tBack.setFillColor(sf::Color(33, 33, 33));
	tBack.setSize(sf::Vector2f(400, 40));

	sf::Text res("Alert: ", font, 24);
	res.setPosition(0, 50);

	//Create backer to alert
	sf::RectangleShape rBack;
	rBack.setFillColor(sf::Color(66, 66, 66));
	rBack.setSize(sf::Vector2f(380, 200 - res.getGlobalBounds().height - res.getGlobalBounds().top - 20));
	rBack.setPosition(10, 45 + 2 * res.getGlobalBounds().height);

	//make response string
	sf::Text responseT(alert, font, 24);
	responseT.setPosition(15, rBack.getGlobalBounds().top);

	sf::Text responseT2(alertLine2, font, 24);
	responseT2.setPosition(15, rBack.getGlobalBounds().top + responseT.getGlobalBounds().height + 10);

	sf::Text responseT3(alertLine3, font, 24);
	responseT3.setPosition(15, rBack.getGlobalBounds().top + responseT2.getGlobalBounds().height + 10);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

		}
		window.clear(sf::Color(48, 48, 48));
		window.draw(tBack);
		window.draw(text);
		window.draw(res);
		window.draw(rBack);
		window.draw(responseT);
		window.draw(responseT2);
		window.draw(responseT3);
		window.display();
	}
	window.close();
}

checkbox::checkbox(int left,
	int top,
	sf::Text title,
	bool isTrue)
{
	set(left,
		top,
		title,
		isTrue);
}
void checkbox::set(int left,
	int top,
	sf::Text title,
	bool isTrue)
{
	checked = isTrue;
	//copy vars to class
	this->left = left;
	this->top = top;
	this->spaceFromCheckboxToText = universal::checkboxTextSeperation;
	t = title;
	this->textRightOfCheckbox = universal::checkboxisLeft;
	this->border = universal::checkboxBorder;
	this->fill = universal::checkboxFill;
	this->boxSize = universal::checkboxBoxSize;
	checkedT.loadFromFile("Assets/Images/checked.png");
	checkedT.setSmooth(true);
	uncheckedT.loadFromFile("Assets/Images/unchecked.png");
	uncheckedT.setSmooth(true);
	if (checked)
		check.setTexture(checkedT);
	else
		check.setTexture(uncheckedT);
	check.setPosition(left, top);
	check.setScale(boxSize / check.getGlobalBounds().width, boxSize / check.getGlobalBounds().height);

	if (!textRightOfCheckbox)
		t.setPosition(check.getGlobalBounds().left - spaceFromCheckboxToText - t.getGlobalBounds().width,
			(check.getGlobalBounds().height / 2) - (t.getGlobalBounds().height / 1.25) + check.getGlobalBounds().top);
	else
		t.setPosition(check.getGlobalBounds().left + check.getGlobalBounds().width + spaceFromCheckboxToText,
			(check.getGlobalBounds().height / 2) - (t.getGlobalBounds().height / 1.25) + check.getGlobalBounds().top);
	r.setSize(sf::Vector2f(left - t.getGlobalBounds().left + t.getGlobalBounds().width,
		std::max(boxSize, (int)t.getGlobalBounds().height)));
	r.setPosition(left, top);
}
checkbox::checkbox() {};
bool checkbox::isClicked(sf::Vector2i clickPos) {
	sf::FloatRect rect;
	auto s = check.getGlobalBounds();
	rect.left = s.left;
	rect.top = s.top;
	rect.height = s.height;
	rect.width = s.width;

	if (rect.contains(float(clickPos.x), float(clickPos.y))) {
		wasClicked();
		return true;
	}
	return false;
}

bool checkbox::getBool() {
	return checked;
}

void checkbox::wasClicked() {
	if (checked)
		checked = false;
	else
		checked = true;

	if (checked)
		check.setTexture(checkedT);
	else if (!checked)
		check.setTexture(uncheckedT);

}

void checkbox::getDrawables(std::vector<sf::Drawable*>& drawVector) {
	drawVector.emplace_back(&t);
	drawVector.emplace_back(&check);
}

int checkbox::getLeft() {
	return r.getGlobalBounds().left;
}
int checkbox::getWidth() {
	return r.getGlobalBounds().width;
}
int checkbox::getHight() {
	return r.getGlobalBounds().height;
}
int checkbox::getTop() {
	return r.getGlobalBounds().top;
}
