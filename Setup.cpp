#include "Setup.h"

std::string Setup::make_lowercase(const std::string& game_title) {

	std::string new_title;

	for (auto& character : game_title) {

		if (!isdigit(character) && !isspace(character)) {

			new_title = tolower(character);

		}
		else {

			new_title += character;

		};

	};

	return new_title;

};

Setup::Game::Game(const std::string& game_title, const std::string& game_path, const std::string& image_path, const sf::Vector2u& image_size) : 
	
	title(game_title), 
	game_path(game_path), 
	image_path(image_path),
    image_size(image_size) {

};

void Setup::add_game(const std::string& game_title, const std::string& game_path, const std::string& image_path, const sf::Vector2u& image_size) {

	this->game_library.emplace_back(Game(game_title, game_path, image_path, image_size));

	std::cout << this->game_library[this->game_library.size() - 1].title << " was added from PATH: ";

	std::cout << this->game_library[this->game_library.size() - 1].game_path << std::endl;

};	

void Setup::search_directories_for_executables(const fs::path& main_directory, std::vector<fs::path>& executable_paths) {

	//using recursive_directory_iterator so it automatically iterates through all folders inside my directory instead of having to manually call 
	//search_directories_for_executables function everytime in the loop when i encounter a directory
	for (const auto& sub_directory : fs::recursive_directory_iterator(main_directory)) {

		 if (sub_directory.path().extension() == ".exe" && sub_directory.path().filename() != "unins000.exe" && sub_directory.path().filename() != "UnityCrashHandler64.exe" && sub_directory.path().filename() != "UnityCrashHandler32.exe") {

			executable_paths.emplace_back(sub_directory.path());

		 };//checks if the iterator is currently looking at an .exe and then stores it in the executable_paths vector

	};

};

void Setup::load_information(const fs::path& games_folder, std::vector<fs::path>& folder_paths, std::vector<std::string>& game_names, std::vector<std::string>& image_paths, std::vector<sf::Vector2u>& image_sizes) {

	int i = 0;
	std::string name;
	for (auto& main_directory : fs::directory_iterator(games_folder)) {

		folder_paths.emplace_back(main_directory.path());

		int j = 0;
		for (auto& c : folder_paths[i].string()) {

			if (j > 8) {

				name += c;

			};

			++j;

		};

		game_names.emplace_back(name);
		name.clear();

		++i;

	};//this loop here adds all directories from the games folder into our folder_paths vector and uses the name of each directory added as the game's name

	i = 0;
	std::vector<fs::path> executable_paths;
	for (auto& main_path : folder_paths) {//main_path is here is basically the game directory from folder_paths 

		search_directories_for_executables(main_path, executable_paths);

		std::sort(executable_paths.begin(), executable_paths.end(), [](const fs::path& path1, const fs::path& path2) {

			return fs::file_size(path1) > fs::file_size(path2);

			});

		folder_paths[i] = executable_paths[0];

		executable_paths.clear();

		++i;

	};//this loop compares all .exe files inside each game directory and chooses tha largest one in size to take as the actual game exe. The chosen .exes are then inserted in folder_paths.

	for (auto& game_paths : folder_paths) {

		bool background_found = false;
		for (const auto& file_path : fs::directory_iterator(game_paths.parent_path())) {

			if (file_path.path().extension() == ".png") {

				background_found = true;

				std::string image_file = file_path.path().string();
				image_paths.emplace_back(image_file);
				std::cout << image_file << std::endl;

				//looking if there exists a .txt file with the exact name as the .png but with the .txt file extension
				std::string text_file = image_file.substr(0, image_file.size() - 4) + ".txt";
				sf::Vector2u image_size(1, 1);
				if (std::filesystem::exists(text_file)) {

					std::ifstream input(text_file);

					text_file.clear();//reusing "text_file" as a string instead of creating a new string instance
					while (std::getline(input, text_file)) {

						if (text_file.substr(0, 9) == "n_rows = ") {//checks if this line in the .txt is for rows or columns

							//saving the the rest of the string after the " = " part which is actually a number and turning it into a type int
							image_size.y = std::stoi(text_file.substr(9, text_file.size() - 1));
							
						}
						else if (text_file.substr(0, 12) == "n_columns = ") {//checks if this line in the .txt is for rows or columns

							//saving the the rest of the string after the " = " part which is actually a number and turning it into a type int
							image_size.x = std::stoi(text_file.substr(12, text_file.size() - 1));

						};

					};
					
					input.close();//closing the file to free up resources

				};
				image_sizes.emplace_back(image_size);

				break;
				
			};

		};

		if (!background_found) {

			image_paths.emplace_back(this->stock_background_path);
			image_sizes.emplace_back(sf::Vector2u(10, 25));

			std::cout << "No Background found in: " << game_paths.parent_path().string() << " => Stock Background image loaded" << std::endl;

		};//if no background image was found, we load the standard image from stock_background_path

	};//searches the same directory as the chosen game executable for a .png file to use it as background

};

void Setup::initialize_library() {

	std::vector<fs::path> folder_paths;
	std::vector<std::string> game_names;
	std::vector<std::string> image_paths;
	std::vector<sf::Vector2u> image_sizes;

	fs::path games_folder(this->games_directory);

	load_information(games_folder, folder_paths, game_names, image_paths, image_sizes);

	for (int i = 0; i < folder_paths.size(); ++i) {	

		add_game(game_names[i], folder_paths[i].string(), image_paths[i], image_sizes[i]);

	};

};

Setup::Setup() : splash_screen_path("resources\\ancient_ruins.png"), menu_background_path("resources\\The Witcher 3.png") {

	this->games_directory = fs::current_path().root_path().string() + "\Games";
	this->stock_background_path = "resources\\Geralt_Wine.png";

	initialize_library();

};


