#pragma once
#include <iostream>
#include <filesystem>
#include <fstream>

#include "External Gaming Library/Executable.h"
#include "External Gaming Library/Animation.h"

class Game_Library
{
public:
	
	class Game {

	 public:

		sf::Text title;
		Spritesheet background;
		Executable executable;

		Game(const sf::Font& text_font, const std::string& game_title, const std::filesystem::path& executable_path, const std::filesystem::path& image_path, const sf::Vector2u& image_size);
	
	};
	
	std::vector<Game> games;//stores all instances of Game object to be used in the User_Interface class

	std::filesystem::path splash_screen_path;
	std::filesystem::path menu_background_path;

	static std::string make_lowercase(const std::string& game_title);

	void add_game(const std::string& game_title, const std::filesystem::path& executable_path, const std::filesystem::path& image_path, const sf::Vector2u& image_size);

	void search_directory_recursively_for_game_executable(const std::filesystem::path& directory, std::filesystem::path& game_executable_path);
	void search_game_executable_directory_for_game_image(const std::filesystem::path& game_executable_path, std::filesystem::path& game_image_path);
	void search_for_game_txt_and_extract_game_image_size(const std::filesystem::path& game_executable_path, sf::Vector2u& game_image_size);

	void scrape_directory(const std::filesystem::path& Games_folder);

	Game_Library();

public:

	sf::Font text_font;

private:

	std::string game_background_file_name;
	std::string game_txt_file_name;

};

