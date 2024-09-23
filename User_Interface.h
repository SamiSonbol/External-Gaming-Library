#pragma once
#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>
#include <memory>
#include <iostream>
#include <functional>
#include "Setup.h"
#include "Animation.h"
#include "Executable.h"

class User_Interface
{
public:

	struct Title {

		sf::Text text;

		bool is_hovered_over = false;
		bool is_selected = false;

		int index = 0;

		Title(const std::string& text, const sf::Vector2f& text_scale, const sf::Font& font);
		
	};

	class Button {
		
	public:

		Title title;
		sf::ConvexShape frame;
		Executable executable;
		Spritesheet image;
		
		sf::Vector2f size;//size of the Button in pixels => x = length, y = width
		void set_position(const sf::Vector2f& position);
		sf::Vector2f get_position();

		//these functions are responsible to set the visual effects when hovering over a game title or any button basically
		void turn_on(const sf::Color& fill_color, const sf::Color& outline_color);
		void turn_off();

		//checks if the frame contains the parameter point in its global bounds (it basically calls the sf::getGlobalBounds().contains())
		bool contains(const sf::Vector2f& point);

		//this function sets the position of the owner to exactly the right side of the Button in the parameter
		void stick_to(Button& target);

		Button(const std::string& text, const sf::Vector2f& text_scale, const sf::Font& font, const int& frame_type, const std::string& exe_path = "", const std::string& image_path = "", const sf::Vector2u& spritesheet_size = {1, 1});
	
	};

	struct Page {

		std::vector<std::shared_ptr<Button>> games;

	};

	struct Searchbar {

		Page search_results_page;

		Button search_box;
		Button return_button;
		Button play_button;

		std::string inputed_text;

		bool search_selected = false;
		bool return_selected = false;
		bool play_selected = false;

		bool searching = false;
		bool search_complete = false;
		bool mouse_wheel_locked = false;//locks the mousewheel so if user is in search page and scrolled, he wont also scroll the main page

		void set_searching();
		void reset_searching();
		void take_search_input(const sf::Event& my_event);
		bool search(std::vector<std::unique_ptr<Page>>& pages);

		void update_page(Page& page);
		void return_to_page(Page& page);

		void update_buttons(const sf::Color& fill_color, const sf::Color& outline_color, const sf::Vector2f& mouse_pos);

		Searchbar(const sf::Font& font, const sf::Color& color);

	};

	sf::Font font;
	sf::Color box_color = sf::Color(0, 0, 0, 50);
	sf::Color chosen_outline_color = sf::Color(255, 215, 0, 255);

	std::vector<std::shared_ptr<Button>> games_collection;
	std::vector<std::unique_ptr<Page>> pages;

	Searchbar searchbar;
	Animator animator;

	Spritesheet menu_background;
	Spritesheet loading_screen;
	sf::Sprite rendered_sprite;

	std::atomic<bool> display_game_background = false;
	sf::Vector2f last_mouse_pos = { 0, 0 };
	std::atomic<int> page_number = 0;
	int last_selected_title = 0;

	std::mutex thread_lock;

	void set_cursor(const sf::RenderWindow* window, const std::string& cursor_icon_path);

	void create_buttons(const Setup& setup_wizard);
	void initialize_buttons();
	void initialize_pages();

	void update_buttons(Page& page, const sf::Vector2f& mouse_pos);
	//hover_over_buttons simply calls both update_buttons functions from Searchbar and User_Interface based off of a mouse position check. HOWEVER, i added the functionality to let it run in a 
	//thread on its own IF NEED BE(if performance took a hit due to the loops inside of update_buttons). The run_permenantly function just traps whatever function parameter it has in a while loop
	//hence, calling it permenantly the whole time. The run_as_thread function runs whatever function in its parameter in a thread and detaches it. THUS, FOR A PERMANENT THREAD THAT RUNS WITHOUT END,
	//call the run_as_thread function with its parameter being the run_permenantly function, in which the actual function you want to run would be the parameter of run_permenantly. The implementation
	//would look like this "run_as_thread([this]() { this->run_permenantly([this]() { this->your_function_here(); }); });" NOTE: the parameter must be a function without a parameter, otherwise parameter
	//of run_permenantly and run_as_thread must change from (std::function<void()> function) to (std::function<void(parameter_type_here)> function).
	void hover_over_buttons();
	void run_permenantly(const std::function<void()> function);
	void run_as_thread(const std::function<void()>& function);

	void unselect_games(Page& page);

	void select_game(Button& game);

	void clicking_on_game(Page& page);

	void select();

	void UPDATE(const sf::Event& my_event);

	void render_background(Spritesheet& background, const float delta_time, sf::RenderWindow* window);
	void render_buttons(const Page& page, sf::RenderWindow* window);
	void RENDER(const float delta_time, sf::RenderWindow* window);

	User_Interface(sf::RenderWindow* window);

};

