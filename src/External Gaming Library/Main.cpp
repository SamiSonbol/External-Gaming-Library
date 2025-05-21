#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <Windows.h>
#include <iostream>
#include <string>
#include <crtdbg.h>

#include "External Gaming Library/Game_Library.h"
#include "External Gaming Library/User_Interface.h"

int main() {
	
	int window_length = sf::VideoMode::getDesktopMode().size.x;
	int window_height = sf::VideoMode::getDesktopMode().size.y;

	float dt;

	sf::Clock dt_clock;

	sf::Vector2u screen_size(window_length, window_height);
	sf::RenderWindow window(sf::VideoMode(screen_size), PROJECT_NAME, sf::Style::Default);//RENDERER

	window.setFramerateLimit(60);

	sf::Font font;
	font.openFromFile(RESOURCES_DIR"/fonts/Montserrat-SemiBold.otf");
	Game_Library Library;
	User_Interface UI(Library, &window, font);

	//window loop
	while (window.isOpen()) {

		dt = dt_clock.restart().asSeconds();

		while (const std::optional event = window.pollEvent()) {

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape) || event->is<sf::Event::Closed>()) {

				window.close();

			};

			UI.UPDATE(event);

		};//poll event closing brackets

		//window drawing logic
		window.clear(sf::Color::Black);

		UI.RENDER(dt, &window);

		window.display();

	};//window closing bracket

	//_CrtDumpMemoryLeaks();

};