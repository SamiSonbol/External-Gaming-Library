#pragma once
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <iostream>
#include <filesystem>
#include <vector>
#include <memory>
#include <assert.h>

namespace fs = std::filesystem;

class Setup
{
public:

	struct Background {

		//location of the png. file on computer
		std::string texture_path;

		sf::Texture texture;

		//size of spritesheet grid in columns and rows => x = n_columns, y = n_rows
		sf::Vector2u spritesheet_size;

		//size of one actual frame on the spritesheet in pixels => x = length, y = width 
		sf::Vector2f frame_size;

		//the size to which the sprite should be scaled inorder to envelop the whole screen
		sf::Vector2f sprite_scale;

		Background(const std::string& texture_file_path);

	};

	struct Game {

		std::string title;

		Background background;

		//game_path is the location of the .exe on computer but in std::wstring type. Since launching the game requires the .exe path to be of type LPCTSTR and LPCTSTR is a pointer; to preserve
		//memory safety, i opted to use type std::wstring which can be transformed to type LPCTSTR right before we search for the game and then deleted right after we launch the game (type std::string cannot be transformed to type LPCTSTR).
		//Hence, by using create_executable_path(string s) we transform the found game path from std::string to std::wstring and later in start_game(string s) to LPCTSTR. 
		std::wstring game_path;
		std::wstring create_executable_path(const std::string& game_path);

		Game(const std::string& game_title, const std::string& game_path, const std::string& texture_file_path);

	};

	std::string games_directory;//location of the folder containing the games on computer (folder should be named "Games")
	std::string stock_background_path;//location of the .png file on computer (used as a substitute to "image_path" inside Background if no .png was found for Background object)
	std::vector<std::unique_ptr<Game>> game_library;//stores all instances of Game object to be used in the User_Interface class

	Background splash_screen;
	Background main_background;

	static std::string make_lowercase(const std::string& game_title);

	void add_game(const std::string& game_title, const std::string& game_path, const std::string& texture_file_path);

	void get_game_information(const fs::path& games_folder, std::vector<fs::path>& folder_paths, std::vector<std::string>& game_names, std::vector<std::string>& image_paths);

	void search_directories_for_executables(const fs::path& main_directory, std::vector<fs::path>& executable_paths);

	void initialize_library();

	std::wstring find_game_path(const std::string& game_title);

	void start_game(const std::string& game_title);

	Setup();

};

