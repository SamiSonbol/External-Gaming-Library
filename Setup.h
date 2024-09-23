#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <memory>

namespace fs = std::filesystem;

class Setup
{
public:

	struct Game {

		std::string title;

		//location of the .png file on computer (this image resembles the game, can be used as a background for the game or for anything else)
		std::string image_path;

		//if the image was a spritesheet then the number of columns are the X coordinate and the number of rows are the Y coordinate, if its a still image then it defaults to {1, 1}
		//if its a spritesheet this size is read from a .txt file(if exists) in the same directory as the .png file from "image_path" 
		sf::Vector2u image_size;

        //.exe path on computer
		std::string game_path;

		Game(const std::string& game_title, const std::string& game_path, const std::string& image_path, const sf::Vector2u& image_size);

	};

	std::string games_directory;//location of the folder containing the games on computer (folder should be named "Games")
	std::string stock_background_path;//location of the .png file on computer (used as a substitute to "image_path" inside Background if no .png was found for Background object)
	std::vector<Game> game_library;//stores all instances of Game object to be used in the User_Interface class

	std::string splash_screen_path;
	std::string menu_background_path;

	static std::string make_lowercase(const std::string& game_title);

	void add_game(const std::string& game_title, const std::string& game_path, const std::string& image_path, const sf::Vector2u& image_size);

	void load_information(const fs::path& games_folder, std::vector<fs::path>& folder_paths, std::vector<std::string>& game_names, std::vector<std::string>& image_paths, std::vector<sf::Vector2u>& image_sizes);

	void search_directories_for_executables(const fs::path& main_directory, std::vector<fs::path>& executable_paths);

	void initialize_library();

	Setup();

};

