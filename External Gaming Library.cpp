#define _CRTDBG_MAP_ALLOC_
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <Windows.h>
#include <iostream>
#include <string>
#include <crtdbg.h>
#include "User_Interface.h"

int main() {
	
	int window_length = sf::VideoMode::getDesktopMode().width;
	int window_height = sf::VideoMode::getDesktopMode().height;

	float dt;

	sf::Clock dt_clock;

	sf::RenderWindow window(sf::VideoMode(window_length, window_height), "Game Library", sf::Style::Fullscreen);//RENDERER

	window.setFramerateLimit(60);

	User_Interface UI(&window);

	//////------------------------------------------------------------------------------------------------------------------------------------------
	/////-------------------------------------------------------------------------------------------------------------------------------------------
	//////------------------------------------------------------------------------------------------------------------------------------------------
	/////-------------------------------------------------------------------------------------------------------------------------------------------

	//window loop
	while (window.isOpen()) {

		dt = dt_clock.restart().asSeconds();

		sf::Event my_event;

		while (window.pollEvent(my_event)) {

			if (my_event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {

				window.close();

			};

			UI.UPDATE(my_event);

		};//poll event closing brackets

		//window drawing logic
		window.clear(sf::Color::Black);

		UI.RENDER(dt, &window);

		window.display();

	};//window closing bracket

	_CrtDumpMemoryLeaks();

};