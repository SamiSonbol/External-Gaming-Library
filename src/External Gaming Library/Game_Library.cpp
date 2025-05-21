#include "External Gaming Library/Game_Library.h"

std::string Game_Library::make_lowercase(const std::string& game_title) {

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

Game_Library::Game::Game(const sf::Font& text_font, const std::string& game_title, const std::filesystem::path& executable_path, const std::filesystem::path& image_path, const sf::Vector2u& image_size) :
	
	title(text_font, game_title),
	executable(executable_path),
    background(image_path, image_size) {

	this->title.setStyle(sf::Text::Bold);
	this->title.setOutlineThickness(1.5);
	this->title.setFillColor(sf::Color::Black);
	this->title.setOutlineColor(sf::Color::White);
	this->title.setScale(sf::Vector2f(0.9, 0.9));

};

void Game_Library::add_game(const std::string& game_title, const std::filesystem::path& executable_path, const std::filesystem::path& image_path, const sf::Vector2u& image_size) {

	this->games.emplace_back(this->text_font, game_title, executable_path, image_path, image_size);

	//std::cout << game_title << " was added from PATH: " << executable_path << "\n";

};	

void Game_Library::search_directory_recursively_for_game_executable(const std::filesystem::path& directory, std::filesystem::path& game_executable_path) {

	//using recursive_directory_iterator so it automatically iterates through all folders inside the directory. If it encounters a file, it checks if its an executable and that its not Unity engine executables.
	//If said conditions are valid, it checks if *game_executable_path* is empty(still not set). If its empty then it sets the *game_executable_path* to the path of the found executable directly; however, if
	//*game_executable_path* is not empty(we already have another path set to it), we compare the size of the executable at *game_executable_path* and the current found executable, we then assign the path of
	//the larger executable size to the *game_path_executable* since an actual game's executable must be the largest executable found its root directory (common sense).
	game_executable_path.clear();
	for (const auto& sub_directory : std::filesystem::recursive_directory_iterator(directory)) {

		if (sub_directory.is_regular_file()) {

			if (sub_directory.path().extension() == ".exe" && sub_directory.path().filename() != "unins000.exe" && sub_directory.path().filename() != "UnityCrashHandler64.exe" && sub_directory.path().filename() != "UnityCrashHandler32.exe") {

				if (game_executable_path.empty()) { game_executable_path = sub_directory.path(); }
				else { if (std::filesystem::file_size(sub_directory.path()) > std::filesystem::file_size(game_executable_path)) { game_executable_path = sub_directory.path(); }; };

			};

		};

	};

	if (game_executable_path.empty()) { std::cerr << "WARNING: no executable was found in directory: " << directory << ". Empty variable returned!\n"; };
	
};

void Game_Library::search_game_executable_directory_for_game_image(const std::filesystem::path& game_executable_path, std::filesystem::path& game_image_path) {
	
	if (!std::filesystem::exists(game_executable_path)) { std::cerr << "WARNING: couldnt find " << game_executable_path << ". Empty variable returned!\n"; return; };

	std::filesystem::path game_image_path_png(game_executable_path.parent_path()/this->game_background_file_name);
	if (std::filesystem::exists(game_image_path_png)) {

		game_image_path = game_image_path_png;
		return;

	};

	std::cerr << "WARNING: no image was found in directory: " << game_executable_path.parent_path() << ". Stock Background image loaded!\n";
	game_image_path = RESOURCES_DIR"/default_images/Geralt_Wine.png";

};

void Game_Library::search_for_game_txt_and_extract_game_image_size(const std::filesystem::path& game_executable_path, sf::Vector2u& game_image_size) {

	if (!std::filesystem::exists(game_executable_path)) { std::cerr << "WARNING: couldnt find " << game_executable_path << ". Empty variable returned!\n"; return; };

	std::filesystem::path game_txt_file_path(game_executable_path.parent_path()/this->game_txt_file_name);
	if (!std::filesystem::exists(game_txt_file_path)) {

		std::cerr << "WARNING: no txt was found in directory: " << game_executable_path.parent_path() << ". Stock image size will be used!\n";
		game_image_size = sf::Vector2u(10, 25);
		return;

	};
	
	std::ifstream txt_file(game_txt_file_path);
	std::string buffer;

	while (std::getline(txt_file, buffer)) {

		std::string_view line = buffer;
		if (line.starts_with("n_rows = ")) {

			game_image_size.y = std::stoi(std::string(line.substr(9)));

		}
		else if (line.starts_with("n_columns = ")) {

			game_image_size.x = std::stoi(std::string(line.substr(12)));

		};

	};

	txt_file.close();//closing the file to free up resources

};

void Game_Library::scrape_directory(const std::filesystem::path& Games_folder) {

	if (!std::filesystem::exists(Games_folder)) { std::cerr << "WARNING: Games directory " << Games_folder << " doesnt exist!\n"; return; };

	std::filesystem::path executable_path;
	std::filesystem::path image_path;
	sf::Vector2u image_size;
	for (auto& game_directory : std::filesystem::directory_iterator(Games_folder)) {

		if (game_directory.is_directory()) {

			this->search_directory_recursively_for_game_executable(game_directory, executable_path);
			this->search_game_executable_directory_for_game_image(executable_path, image_path);
			this->search_for_game_txt_and_extract_game_image_size(executable_path, image_size);
			this->add_game(game_directory.path().stem().string(), executable_path, image_path, image_size);

		};

	};

};

Game_Library::Game_Library() : 
	
	splash_screen_path(RESOURCES_DIR"/default_images/ancient_ruins.png"), 
	menu_background_path(RESOURCES_DIR"/default_images/The Witcher 3.png"),
    game_background_file_name("background.png"),
    game_txt_file_name("background_size.txt") {

	this->text_font.openFromFile(RESOURCES_DIR"/fonts/Montserrat-SemiBold.otf");
	scrape_directory(std::filesystem::current_path().root_path() / "Games");

};


