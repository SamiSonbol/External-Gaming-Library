#pragma once
#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>
#include <memory>
#include <iostream>
#include <functional>
#include <filesystem>
#include <string>

#include "External Gaming Library/Game_Library.h"
#include "External Gaming Library/Animation.h"
#include "External Gaming Library/Executable.h"
#include "External Gaming Library/Widgets.h"

class User_Interface
{
 public:

	void add_page(Widgets::Page&& page);
	Widgets::Page& get_reference_to_page(const size_t& index);

	sf::Color box_color;
	sf::Color chosen_outline_color;

	Widgets::Searchbar searchbar;
	Animator animator;

	Spritesheet menu_background;
	Spritesheet loading_screen;
	sf::Texture rendered_sprite_texture;
	sf::Sprite rendered_sprite;

	std::atomic<bool> display_game_background;
	sf::Vector2f last_mouse_pos;
	std::atomic<int> page_number;
	int last_selected_title;

	std::mutex thread_lock;

	void set_cursor(const sf::RenderWindow* window, const std::filesystem::path& cursor_icon_path);
	void initialize(Game_Library& library);
	
	void run_permenantly(const std::function<void()> function);
	void run_as_thread(const std::function<void()>& function);

	void put_button_in_selected_state(Widgets::Button_Game& game);
	void unselect_button(Widgets::Button_Game& game);
	void unselect_all_buttons(Widgets::Page& page);

	void hover_over_buttons(Widgets::Page& page, const sf::Vector2f& mouse_position);
	void update_mouse_scroll(const std::optional<sf::Event>& event);
	void update_mouse_button_clicking(const sf::Vector2f& mouse_position);
	void UPDATE(const std::optional<sf::Event>& event);

	void render_background(Spritesheet& background, const float& delta_time, sf::RenderWindow* window);
	void render_buttons(Widgets::Page& page, sf::RenderWindow* window);
	void RENDER(const float& delta_time, sf::RenderWindow* window);

	User_Interface(Game_Library& library, sf::RenderWindow* window, sf::Font& font);

	sf::Font game_titles_font;

	

	private:
		std::vector<Widgets::Page> pages;

};

