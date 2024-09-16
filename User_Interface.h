#pragma once
#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>
#include <memory>
#include <iostream>
#include <functional>
#include "Setup.h"

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

	struct Button {
		
		Title title;
		sf::ConvexShape frame;

		//size of the Button in pixels => x = length, y = width
		sf::Vector2f size;

		void set_position(const sf::Vector2f& position);
		sf::Vector2f get_position();

		//these functions are responsible to set the visual effects when hovering over a game title or any button basically
		void turn_on(const sf::Color& fill_color, const sf::Color& outline_color, int& last_hovered_over_title);
		void turn_off();

		//checks if the frame contains the parameter point in its global bounds (it basically calls the sf::getGlobalBounds().contains())
		bool contains(const sf::Vector2f& point);

		//this function sets the position of the owner to exactly the right side of the Button in the parameter
		void stick_to(Button& target);

		Button(const std::string& text, const sf::Vector2f& text_scale, const sf::Font& font, const int& frame_type);
	
	};

	struct Page {

		std::vector<std::shared_ptr<Button>> games;

	};

	struct Animator {

		sf::IntRect frame_rect;

		//position of the current frame on the spritesheet in row and column => x = column, y = row
		sf::Vector2u current_frame;

		float elapsed_time;
		float frame_duration;		

		float calculate_frame_duration(int fps, int n_frames);

		//this update function can certainly be programmed to take in less data and parameters; however, it will have to do more calculations. Hence, i opted to pre-calculate the data inside Setup
		//and use them as parameters here 
		void update(sf::Sprite& sprite, sf::Texture& texture, const sf::Vector2u& spritesheet_size, const sf::Vector2f& frame_size, const sf::Vector2f& sprite_scale, const float& delta_time);

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
		bool mouse_wheel_locked = false;//locks the mousewheel so if user is in produced search page and scrolled, he wont also scroll the main page

		void set_searching();
		void reset_searching();
		void take_search_input(const sf::Event& my_event);

		void update_page(Page& page);
		void return_to_page(Page& page);

		bool search(const std::vector<Page>& pages);

		void update_buttons(sf::Color& fill_color, sf::Color& outline_color, const sf::Vector2f& mouse_pos);

		Searchbar(const sf::Font& font, const sf::Color& color);

	};

	sf::Color box_color = sf::Color(0, 0, 0, 50);
	sf::Color chosen_outline_color = sf::Color(255, 215, 0, 255);

	std::vector<std::shared_ptr<Button>> games_collection;
	std::vector<Page> pages;

	std::mutex thread_lock;

	sf::Vector2f last_mouse_pos = { 0, 0 };

	std::atomic<int> page_number = 0;
	std::atomic<bool> display_game_background = false;

	std::unique_ptr<sf::RenderWindow> window;

	sf::Font font;

	Searchbar searchbar;

	int last_hovered_over_title = 0;

	int last_selected_title = 0;

	void set_window(sf::RenderWindow* window);

	void draw_splash_screen(const sf::Sprite& image);

	void create_titles();
	void calculate_n_titles_on_screen();
	void initialize_pages();

	void update_buttons(Page& page, const sf::Vector2f& mouse_pos);
	//hover_over_buttons simply calls both update_buttons functions from Searchbar and User_Interface based off of a mouse position check. HOWEVER, i added the functionality to let it run in a 
	//thread on its own IF NEED BE(if performance took a hit due to the loops inside of update_buttons). The run_permenantly function just traps whatever function parameter it has in a while loop
	//hence, calling it permenantly the whole time. The run_as_thread function runs whatever function in its parameter in a thread and detaches it. THUS, FOR A PERMANENT THREAD THAT RUNS WITHOUT END,
	//call the run_as_thread function with its parameter being the run_permenantly function, in which the actual function you want to run would be the parameter of run_permenantly. The implementation
	//would look like this "run_as_thread([this]() { this->run_permenantly([this]() { this->your_function_here(); }); });" NOTE: the parameter must be a function without a parameter, otherwise parameter
	//of run_permenantly and run_as_thread must change from (std::function<void()> function) to (std::function<void(parameter_type_here)> function).
	void hover_over_buttons();
	void run_permenantly(std::function<void()> function);
	void run_as_thread(const std::function<void()>& function);

	void unselect_games();

	void select_game(Button& game);

	void launch_game(const Button& game);

	void clicking_on_game(Page& page);

	void select();

	void UPDATE(const sf::Event& my_event);

	void render_background(Setup::Background& background, const float delta_time);
	void render_buttons(Page& page);
	void RENDER(const float delta_time);

	Setup setup_wizard = Setup();

	Animator animator;

	sf::Sprite rendered_sprite;

	User_Interface(sf::RenderWindow* window);

};

