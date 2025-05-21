#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <memory>
#include <iostream>
#include <filesystem>
#include <functional>

#include "External Gaming Library/Game_Library.h"
#include "External Gaming Library/Animation.h"
#include "External Gaming Library/Executable.h"

namespace Widgets {

	class Button_Base {

	 private:

		 sf::Vector2f size;

	 public:

		sf::ConvexShape frame;

		//these pairs contains the colors of the button when its off *first* and when its on *second*
		std::pair<sf::Color, sf::Color> fill_color;
		std::pair<sf::Color, sf::Color> outline_color;

		bool is_hovered_over = false;
		bool is_selected = false;
		int index = 0;

		void set_size(const sf::Vector2f& size);
		sf::Vector2f get_size();

		virtual void set_fill_color(const sf::Color& color_off, const sf::Color& color_on);
		virtual void set_outline_color(const sf::Color& color_off, const sf::Color& color_on);

		virtual void set_position(const sf::Vector2f& position) = 0;
		sf::Vector2f get_position();

		//this function sets the position of the owner to exactly the right side of the Button in the parameter
		void stick_to(Button_Base& target);

		//checks if the frame contains the parameter point in its global bounds (it basically calls the sf::getGlobalBounds().contains())
		bool contains(const sf::Vector2f& point);
		bool is_clicked(const sf::Vector2f& mouse_position, const sf::Mouse::Button& mouse_button);

		//these functions are responsible to set the visual effects when hovering over a game title or any button basically
		virtual void turn_on();
		virtual void turn_off();

		std::function<void()> behaviour;
		void set_behaviour(std::function<void()> lambda);
		void activate_behaviour();

		virtual void render(sf::RenderWindow* window);

		void make_frame(const int& frame_type);
		Button_Base(const int& frame_type = 2, std::function<void()> behaviour = nullptr);
		virtual ~Button_Base() = default;

	};

	class Button : public Button_Base {

	 public:

		 sf::Text text;

		 void set_fill_color(const sf::Color& color_off, const sf::Color& color_on) override;
		 void set_outline_color(const sf::Color& color_off, const sf::Color& color_on) override;

		 void set_position(const sf::Vector2f& position) override;

		 void turn_on() override;
		 void turn_off() override;

		 void render(sf::RenderWindow* window) override;

		 Button(const sf::Font& font, const std::string& text, const int& frame_type = 2, std::function<void()> behaviour = nullptr);

	};

	class Button_Game : public Button_Base {

	 public:

		Game_Library::Game* game;

		void set_fill_color(const sf::Color& color_off, const sf::Color& color_on) override;
		void set_outline_color(const sf::Color& color_off, const sf::Color& color_on) override;

		void set_position(const sf::Vector2f& position) override;

		void turn_on() override;
		void turn_off() override;

		void render(sf::RenderWindow* window) override;

		Button_Game(Game_Library::Game& game, const int& frame_type = 2, std::function<void()> behaviour = nullptr);
		~Button_Game();

	};

	class Page {

	 private:

		std::vector<Button_Game> buttons;

	 public:

		void update_buttons_positions();
		void update_buttons_states(const sf::Vector2f& mouse_position);

		void add_button(Button_Game&& button);
		Button_Game& get_reference_to_button(const size_t& index);

		size_t get_size() const;
		bool is_empty() const;
		void clear();

	};

	class Searchbar {

	 public:

		Widgets::Page search_results_page;

		Widgets::Button search_box;
		Widgets::Button return_button;
		Widgets::Button play_button;

		std::string inputed_text;

		bool search_selected = false;
		bool return_selected = false;
		bool play_selected = false;

		bool searching = false;
		bool search_complete = false;
		bool mouse_wheel_locked = false;//locks the mousewheel so if user is in search page and scrolled, he wont also scroll the main page

		void set_searching();
		void reset_searching();
		void take_search_input(const sf::Event::TextEntered& my_event);
		bool search(std::vector<Widgets::Page>& pages);

		void return_to_page(Widgets::Page& page);

		void update_searching_state(const std::optional<sf::Event>& entering_text_event, const sf::Vector2f& mouse_pos, std::vector<Widgets::Page>& pages);
		void update(const std::optional<sf::Event>& entering_text_event, const sf::Vector2f& mouse_pos, Widgets::Page current_page, std::vector<Widgets::Page>& all_pages);

		Searchbar(const sf::Color& color, const sf::Font& font);

	};

};