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

Setup::Background::Background(const std::string& texture_file_path) : spritesheet_size(10, 25) {

	this->texture_path = texture_file_path;
	this->texture.loadFromFile(this->texture_path);
	this->texture.setSmooth(true);

	//we have to divide the image Length and Width by the corrosponding row and column number so we can get
	//the actual width and length of each slide insie the spritesheet
	this->frame_size.x = this->texture.getSize().x / this->spritesheet_size.x;
	this->frame_size.y = this->texture.getSize().y / this->spritesheet_size.y;

	this->sprite_scale.x = sf::VideoMode::getDesktopMode().width / (float)this->frame_size.x;
	this->sprite_scale.y = sf::VideoMode::getDesktopMode().height / (float)this->frame_size.y;

};

std::wstring Setup::Game::create_executable_path(const std::string& game_path) {

	std::wstring exe_path = std::wstring(game_path.begin(), game_path.end());

	return exe_path;

};

Setup::Game::Game(const std::string& game_title, const std::string& game_path, const std::string& texture_file_path) : 
	
	title(game_title), 
	game_path(create_executable_path(game_path)), 
	background(texture_file_path) {
	
};

void Setup::add_game(const std::string& game_title, const std::string& game_path, const std::string& texture_file_path) {

	this->game_library.emplace_back(std::make_unique<Game>(game_title, game_path, texture_file_path));

	std::cout << this->game_library[this->game_library.size() - 1]->title << " was added from PATH: ";

	std::wcout << this->game_library[this->game_library.size() - 1]->game_path << std::endl;

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

void Setup::get_game_information(const fs::path& games_folder, std::vector<fs::path>& folder_paths, std::vector<std::string>& game_names, std::vector<std::string>& image_paths) {

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

				image_paths.emplace_back(file_path.path().string());

				std::cout << file_path.path().string() << std::endl;

				break;

			};

		};

		if (!background_found) {

			image_paths.emplace_back(this->stock_background_path);

			std::cout << "No Background found in: " << game_paths.parent_path().string() << " => Stock Background image loaded" << std::endl;

		};//if no background image was found, we load the standard image from stock_background_path

	};//searches the same directory as the chosen game executable for a .png file to use it as background

};

void Setup::initialize_library() {

	std::vector<fs::path> folder_paths;

	std::vector<std::string> game_names;

	std::vector<std::string> image_paths;

	fs::path games_folder(this->games_directory);

	get_game_information(games_folder, folder_paths, game_names, image_paths);

	std::string executable_path;
	for (int i = 0; i < folder_paths.size(); ++i) {	

		for (char c : folder_paths[i].string()) {

			if (c == '\\') {

				executable_path += "\\\\";
			}
			else {

				executable_path += c;

			};

		};

		add_game(game_names[i], executable_path, image_paths[i]);
		executable_path.clear();

	};

};

std::wstring Setup::find_game_path(const std::string& game_title) {

	for (auto& game : this->game_library) {

		if (make_lowercase(game_title) == make_lowercase(game->title)) {

			return game->game_path;

		};

	};

	return NULL;

};//find_game_path brackets

void Setup::start_game(const std::string& game_title) {

	LPCTSTR game_path = _wcsdup(find_game_path(game_title).c_str());

	HWND main_window = HWND();

	if (game_path != NULL) {

		LPTSTR command_line = NULL;

		LPSECURITY_ATTRIBUTES process_attributes = NULL;

		LPSECURITY_ATTRIBUTES thread_attributes = NULL;

		BOOL inherit_handles = FALSE;

		DWORD creation_flags = 0;

		LPVOID environment = NULL;

		LPCTSTR current_directory = NULL;

		PROCESS_INFORMATION process_information;

		STARTUPINFO startup_information;

		ZeroMemory(&process_information, sizeof(process_information));

		ZeroMemory(&startup_information, sizeof(startup_information));
		startup_information.cb = sizeof(startup_information);

		bool success = CreateProcess(

			game_path,

			command_line,

			process_attributes,

			thread_attributes,

			inherit_handles,

			creation_flags,

			environment,

			current_directory,

			&startup_information,

			&process_information

		);

		assert(success);

		CloseHandle(process_information.hThread);
		//WaitForSingleObject(process_information.hProcess, INFINITE);
		CloseHandle(process_information.hProcess);

		SetForegroundWindow(main_window);

		delete game_path;

	}
	else {

		std::cout << "ERROR: couldnt find game\n";

	};//game_path check bracket

};

Setup::Setup() : splash_screen("resources\\ancient_ruins.png"), main_background("resources\\The Witcher 3.png") {

	this->games_directory = fs::current_path().root_path().string() + "\Games";
	this->stock_background_path = "resources\\Geralt_Wine.png";

	initialize_library();

};


