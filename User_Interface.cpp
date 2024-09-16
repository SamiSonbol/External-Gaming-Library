#include "User_Interface.h"

void User_Interface::set_window(sf::RenderWindow* window) {

	this->window.reset(window);

	sf::WindowHandle wHandle;
	wHandle = window->getSystemHandle();
	HCURSOR Cursor = LoadCursorFromFile(L"resources\\Kunai.ani");
	SetCursor(Cursor);
	SetClassLongPtr(wHandle, GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(Cursor));

};

void User_Interface::draw_splash_screen(const sf::Sprite& image) {

	this->window->draw(image);
	this->window->display();

};

User_Interface::Title::Title(const std::string& text, const sf::Vector2f& text_scale, const sf::Font& font) : text(text, font){

	this->text.setStyle(sf::Text::Bold);

	this->text.setOutlineThickness(1);

	this->text.setFillColor(sf::Color::Black);

	this->text.setOutlineColor(sf::Color::White);

	this->text.setScale(text_scale);

};

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void User_Interface::Button::set_position(const sf::Vector2f& position) {

	int offset = (this->title.text.getCharacterSize() / 2) / 2;
	offset -= 1;

	this->frame.setPosition(position);
	this->title.text.setPosition(position.x + offset, position.y + 1);

};

sf::Vector2f User_Interface::Button::get_position() {//this function exists only so the function naming would stay on paar with the set_position function

	return this->frame.getPosition();

};

void User_Interface::Button::turn_on(const sf::Color& fill_color, const sf::Color& outline_color, int& last_hovered_over_title) {

	this->frame.setFillColor(fill_color);

	this->frame.setOutlineColor(outline_color);

	this->title.text.setOutlineColor(outline_color);

	this->title.is_hovered_over = true;

	last_hovered_over_title = this->title.index;

};

void User_Interface::Button::turn_off() {

	this->frame.setFillColor(sf::Color::Transparent);

	this->frame.setOutlineColor(sf::Color::Transparent);

	this->title.text.setOutlineColor(sf::Color::White);

	this->title.is_hovered_over = false;

};

bool User_Interface::Button::contains(const sf::Vector2f& point) {

	if (this->frame.getGlobalBounds().contains(point)) {

		return true;

	}
	else {

		return false;

	};

};

void User_Interface::Button::stick_to(Button& target) {

	float offset = target.size.x * 0.03;
	set_position({ target.get_position().x + target.size.x + offset, target.get_position().y });

};

User_Interface::Button::Button(const std::string& text, const sf::Vector2f& text_scale, const sf::Font& font, const int& frame_type) : title(text, text_scale, font) {

	switch (frame_type) {

	case 1://draws circle

		this->frame.setPointCount(100);

		for (int i = 0; i < 100; ++i) {

			float angle = 2 * 3.14 * i;

			float x = 1 * cos(angle);

			float y = 1 * sin(angle);

			this->frame.setPoint(i, { x, y });

		};

		break;

	case 2://draws rect

		this->frame.setPointCount(4);

		this->frame.setPoint(0, { 0, 0 });

		this->frame.setPoint(1, { 1, 0 });

		this->frame.setPoint(2, { 1, 1 });

		this->frame.setPoint(3, { 0, 1 });

		break;

	case 3://draws triangle

		this->frame.setPointCount(3);

		this->frame.setPoint(0, { 0, 0 });

		this->frame.setPoint(1, { 1, 1 });

		this->frame.setPoint(2, { - 1, 1 });

		break;

	};

	this->frame.setFillColor(sf::Color::Transparent);
	this->frame.setOutlineThickness(0.03);

	int offset = this->title.text.getCharacterSize() / 2;
	this->size.x = this->title.text.getGlobalBounds().width + offset;
	this->size.y = this->title.text.getGlobalBounds().height + offset;
	this->frame.setScale(this->size);
	
	set_position({0,0});

};

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

float User_Interface::Animator::calculate_frame_duration(int fps, int n_frames) {

	if (fps <= 0) {

		std::cerr << "FPS must be greater than zero." << std::endl;
		return 0.0f;

	};

	return static_cast<float>(1) / fps;

};

void User_Interface::Animator::update(sf::Sprite& sprite, sf::Texture& texture, const sf::Vector2u& spritesheet_size, const sf::Vector2f& frame_size, const sf::Vector2f& sprite_scale, const float& delta_time) {

	this->elapsed_time += delta_time;

	if (this->elapsed_time >= this->frame_duration) {

		this->elapsed_time -= this->frame_duration;
		this->current_frame.x++;

		if (this->current_frame.x >= spritesheet_size.x) {

			this->current_frame.x = 0;
			this->current_frame.y++;

		};

		if (this->current_frame.y >= spritesheet_size.y) {

			this->current_frame.y = 0;

		};

	};

	this->frame_rect.width = frame_size.x;
	this->frame_rect.height = frame_size.y;

	this->frame_rect.top = this->current_frame.y * this->frame_rect.height;
	this->frame_rect.left = this->current_frame.x * this->frame_rect.width;

	sprite.setTexture(texture);
	sprite.setTextureRect(this->frame_rect);
	sprite.setScale(sprite_scale);

};

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void User_Interface::Searchbar::set_searching() {

	this->searching = true;

	this->search_box.title.text.setString("|");

	this->inputed_text.clear();

}//setting the searchbar active to take input

void User_Interface::Searchbar::reset_searching() {

	this->searching = false;

	this->search_box.title.text.setString("Search");

	this->inputed_text.clear();

}//reseting the searchbar if the search wasnt started after taking in the input

void User_Interface::Searchbar::take_search_input(const sf::Event& my_event) {

	if (this->searching) {

		this->inputed_text += static_cast<char>(my_event.text.unicode);

		this->search_box.title.text.setString(this->inputed_text);

	};

};

void User_Interface::Searchbar::update_page(Page& page) {

	bool first_title_is_set = false;

	float width_offset = page.games[0]->size.y * 2;

	for (int i = 0; i < page.games.size(); ++i) {

		int last_index = (i - 1) % page.games.size();

		if (!first_title_is_set && !page.games.empty()) {

			page.games[i]->set_position({ page.games[i]->get_position().x, width_offset });

			first_title_is_set = true;

		}
		else if (first_title_is_set && !page.games.empty()) {

			page.games[i]->set_position({ page.games[last_index]->get_position().x, page.games[last_index]->get_position().y + width_offset });

		};

	};

};

void User_Interface::Searchbar::return_to_page(Page& page) {

	this->search_complete = false;

	reset_searching();

	update_page(page);

	this->mouse_wheel_locked = false;

};

bool User_Interface::Searchbar::search(const std::vector<Page>& pages) {

	this->search_results_page.games.clear();

	this->inputed_text = Setup::make_lowercase(this->inputed_text);//lowering all alphabet cases of the input search to streamline the search

	for (auto& page : pages) {

		for (auto& game : page.games) {

			if (Setup::make_lowercase(game->title.text.getString()) == this->inputed_text) {

				this->search_results_page.games.emplace_back(game);/////

				update_page(this->search_results_page);

				this->search_complete = true;

				return true;

			};

		};

	};//searching for a match between the input search and a game title

	for (auto& page : pages) {

		for (auto& game : page.games) {

			if (tolower(game->title.text.getString()[0]) == this->inputed_text[0]) {

				this->search_results_page.games.emplace_back(game);

			};

		};

	};//if no match was found then this loop will search again with a criteria of taking in the same first letter instead of the whole name

	if (!this->search_results_page.games.empty()) {

		update_page(this->search_results_page);
		this->search_complete = true;	
		return true;      
	
	}                                        
	else {                                         
	
		this->search_box.title.text.setString("no results found");
		return false;  

	};

};

void User_Interface::Searchbar::update_buttons(sf::Color& fill_color, sf::Color& outline_color, const sf::Vector2f& mouse_pos) {

	if (this->search_box.contains(mouse_pos)) {

		this->search_selected = true;

		this->search_box.frame.setOutlineColor(outline_color);

	}
	else if (!this->search_box.contains(mouse_pos)) {

		this->search_selected = false;

		this->search_box.frame.setOutlineColor(fill_color);

	}//checks if the mouse is hovering over the search bar or not

	if (this->return_button.contains(mouse_pos)) {

		this->return_selected = true;

		this->return_button.frame.setOutlineColor(outline_color);

	}
	else if (!this->return_button.contains(mouse_pos)) {

		this->return_selected = false;

		this->return_button.frame.setOutlineColor(fill_color);

	}//checks if the mouse is hovering over the return button or not

	if (this->play_button.contains(mouse_pos)) {

		this->play_selected = true;

		this->play_button.frame.setOutlineColor(outline_color);

	}
	else if (!this->play_button.contains(mouse_pos)) {

		this->play_selected = false;

		this->play_button.frame.setOutlineColor(fill_color);

	}//checks if the mouse is hovering over the play button or not

};

User_Interface::Searchbar::Searchbar(const sf::Font& font, const sf::Color& color) :  search_box("Search", { 0.6, 0.6 }, font, 2), return_button("<<", { 0.6, 0.6 }, font, 2), play_button("PLAY", { 0.6, 0.6 }, font, 2) {

	this->search_box.frame.setOutlineThickness(0.1);
	this->return_button.frame.setOutlineThickness(0.1);        

	this->search_box.frame.setFillColor(color);
	this->return_button.frame.setFillColor(color);
	this->play_button.frame.setFillColor(color);

	this->search_box.frame.setOutlineColor(color);
	this->return_button.frame.setOutlineColor(color);       
	this->play_button.frame.setOutlineColor(color);

	this->search_box.title.text.setFillColor(sf::Color::White);
	this->return_button.title.text.setFillColor(sf::Color::White);
	this->play_button.title.text.setFillColor(sf::Color::White);

	float part_of_screen_width = sf::VideoMode::getDesktopMode().width * 0.25;
	this->search_box.size.x = part_of_screen_width;
	this->search_box.frame.setScale(this->search_box.size);

	this->return_button.size = { this->search_box.size.y, this->search_box.size.y };
	this->return_button.frame.setScale(this->return_button.size);

	this->search_box.set_position({ 10, 14 });
	//due to a problem with SFML outlineThickness, the stick_to function doesnt work on the return_button with combination of search_box; hence, am using this logic with hard coded numbers instead
	this->return_button.set_position({ this->search_box.get_position().x + this->search_box.size.x + this->return_button.size.x * 1.7f, this->search_box.get_position().y });

};

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void User_Interface::create_titles() {

	sf::Vector2f text_scale = { 0.9, 0.9 };

	int i = 0;
	for (auto& game : this->setup_wizard.game_library) {

		this->games_collection.emplace_back(std::make_shared<Button>(game->title, text_scale, this->font, 2));
		this->games_collection[this->games_collection.size() - 1]->title.index = i;

		++i;

	};

};

void User_Interface::calculate_n_titles_on_screen() {

	//this title_area takes the space needed for each title as double its width inorder to makeup for the bounding box and spacing between titles, and the length as the whole
	//horizontal space where this title exists since we want our titles to be listed as one column and not stuffed in every available space on the screen.
	float title_area = (this->games_collection[0]->size.y * 2) * this->window->getSize().x;
	float window_area = this->window->getSize().y * this->window->getSize().x;

	//here the actual formula we use is number_of_objects = area_of_whole_space / area_of_one_object. However, since in our title_area doesnt take the space of our searchbar
	//into consederation, the screen_capacity will be +1 of the actual capacity that the screen can hold; hence, we subtract 1 from it.
	int screen_capacity = (window_area / title_area) - 1;

	int n_titles = this->games_collection.size();
	
	int index = 0;
	while (true) {

		Page page;

		int page_size;

		int remaining_titles = n_titles - screen_capacity;

		if (remaining_titles >= 0) {

			page_size = screen_capacity;

		}
		else if (remaining_titles < 0) {

			page_size = n_titles;

		};

		for (int i = index; i < index + page_size; i++) {

			this->games_collection[i]->set_position({ 10, 0});

			page.games.emplace_back(this->games_collection[i]);////

		};

		this->pages.emplace_back(page);///

		index += page_size;

		n_titles = n_titles - screen_capacity;

		if (n_titles <= 0) {

			break;

		};

	};//while loop bracket

};

void User_Interface::initialize_pages() {

	for (auto& page : this->pages) {

		this->searchbar.update_page(this->pages[this->page_number]);

		this->page_number += 1;

	};

	this->page_number = 0;

	this->searchbar.update_page(this->pages[this->page_number]);

};

void User_Interface::update_buttons(Page& page, const sf::Vector2f& mouse_pos) {

	for (auto& game : page.games) {

		if (game->contains(mouse_pos)) {

			game->turn_on(this->box_color, this->chosen_outline_color, this->last_hovered_over_title);

			break;

		}
		else if (!game->contains(mouse_pos) && !game->title.is_selected && game->title.is_hovered_over == true) {

			game->turn_off();

			break;

		};

	};//for loop bracket

};

void User_Interface::hover_over_buttons() {

	sf::Vector2f mouse_pos (sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);

	if (last_mouse_pos != mouse_pos) {

		last_mouse_pos = mouse_pos;

		if (mouse_pos.x >= 0 && mouse_pos.x <= this->window->getSize().x && mouse_pos.y >= 0 && mouse_pos.y <= this->window->getSize().y) {

			this->searchbar.update_buttons(this->box_color, this->chosen_outline_color, mouse_pos);

			thread_lock.lock();//locking to use page or search_complete

			if (this->searchbar.search_complete) {

				update_buttons(this->searchbar.search_results_page, mouse_pos);

			}//mouse hovering function on search results page

			else {

				update_buttons(this->pages[this->page_number], mouse_pos);

			}//mouse hovering function on all standard pages

			thread_lock.unlock();//unlocking the previous lock

		};//checks if mouse is inside window

	};//checks if mouse has moved
	
};

void User_Interface::run_permenantly(std::function<void()> function)
{

	while (true) {

		function();

	};

};

void User_Interface::run_as_thread(const std::function<void()>& function)
{

	std::thread title_selection([function]() {

		function();

		});

	title_selection.detach();

};

void User_Interface::select_game(Button& game) {

	game.title.is_selected = true;

	this->last_selected_title = game.title.index;

	game.turn_on(this->box_color, this->chosen_outline_color, this->last_hovered_over_title);

	this->searchbar.play_button.frame.setScale(this->searchbar.play_button.size.x, game.size.y);
	this->searchbar.play_button.stick_to(game);

	this->display_game_background.store(true);

};

void User_Interface::unselect_games() {

	for (auto& page : this->pages) {

		for (auto& game : page.games) {

			if (game->title.is_selected) {

				game->turn_off();
				this->display_game_background.store(false);
				game->title.is_selected = false;

				break;

			};

		};//games on page loop

	};//pages loop

};

void User_Interface::launch_game(const Button& game) {

	std::string game_title = game.title.text.getString();

	this->setup_wizard.start_game(game_title);

};

void User_Interface::clicking_on_game(Page& page) {

	for (auto& game : page.games) {

		if (game->title.is_selected && this->searchbar.play_selected) {

			launch_game(*game);

			return;

		}//launches the game after clicking on play
		else if (!game->title.is_selected && game->title.is_hovered_over) {

			unselect_games();

			select_game(*game);

			return;

		}//selects the game if the mouse was clicked over this game and said game was not already selected

		//else if (game.title.is_selected && game.title.is_hovered_over) {

		//    return;

		//};//checks if game is already selected and mouse is over it so i wont do anything, this is incase of useless clicks over an already selected game

	};

	unselect_games();//unselects everything if the mouse was clicked one the screen but not on any game title

};

void User_Interface::select() {

	if (this->searchbar.return_selected) {

		this->searchbar.return_to_page(this->pages[this->page_number]);

		return;

	}//returning back to the main page after a search if the "return_button" was clicked

	if (this->searchbar.search_selected && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {

		if (this->searchbar.search(this->pages)) {

			return;

		}//seraching through titles and producing a search result

	}//if no results were found we go forward to check other logic
	else {

		if (this->searchbar.search_selected) {

			this->searchbar.set_searching();

			return;

		}//directly returning from this function if the searchbar was set to take input
		else {

			this->searchbar.reset_searching();

		}

	}//moving forward with the logic if the searchbar was reset

	if (this->searchbar.search_complete) {      
	
		clicking_on_game(this->searchbar.search_results_page);   
	                                     
	}                                             
	else {

		clicking_on_game(this->pages[this->page_number]);

	};

};

void User_Interface::UPDATE(const sf::Event& my_event) {

	if (my_event.type == sf::Event::MouseWheelScrolled && !this->searchbar.mouse_wheel_locked) {

		if (my_event.mouseWheelScroll.delta >= 0 && this->page_number > 0) {

			this->page_number -= 1;

		}
		else if (my_event.mouseWheelScroll.delta < 0 && this->page_number < this->pages.size() - 1) {

			this->page_number += 1;

		};

		this->searchbar.update_page(this->pages[this->page_number]);

	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {

		this->select();

	}
	else if (my_event.type == sf::Event::TextEntered) {

		this->searchbar.take_search_input(my_event);

	};

	hover_over_buttons();

};

void User_Interface::render_background(Setup::Background& background, const float delta_time) {

	this->animator.update(this->rendered_sprite, background.texture, background.spritesheet_size, background.frame_size, background.sprite_scale, delta_time);
	this->window->draw(this->rendered_sprite);

};

void User_Interface::render_buttons(Page& page) {

	for (auto& game : page.games) {

		this->window->draw(game->title.text);
		if (game->title.is_selected && this->display_game_background.load()) {

			this->window->draw(this->searchbar.play_button.frame);
			this->window->draw(this->searchbar.play_button.title.text);
			this->window->draw(game->frame);

		}//checks if the game is selected and then draws the play button
		else if (game->frame.getFillColor() == this->box_color) {

			this->window->draw(game->frame);

		};//checks if the game is being hovered over and then draws the frame

	};

};

void User_Interface::RENDER(const float delta_time) {

	if (this->display_game_background) {

		render_background(this->setup_wizard.game_library[this->last_selected_title]->background, delta_time);

	}
	else {

		render_background(this->setup_wizard.main_background, delta_time);

	};
	
	thread_lock.lock();

	if (this->searchbar.search_complete) {

		if (!this->searchbar.search_results_page.games.empty()) {

			render_buttons(this->searchbar.search_results_page);

		};

	}
	else {

		render_buttons(this->pages[this->page_number]);

	};

	thread_lock.unlock();

	this->window->draw(this->searchbar.search_box.frame);
	this->window->draw(this->searchbar.search_box.title.text);

	this->window->draw(this->searchbar.return_button.frame);
	this->window->draw(this->searchbar.return_button.title.text);

};

User_Interface::User_Interface(sf::RenderWindow* window) : searchbar(searchbar) {

	set_window(window);
	//draw_splash_screen(this->setup_wizard.splash_screen.image);

	this->font.loadFromFile("resources\\Montserrat-SemiBold.otf");
	create_titles();

	calculate_n_titles_on_screen();

	this->searchbar = Searchbar(this->font, this->box_color);
	this->searchbar.play_button.frame.setOutlineThickness(this->games_collection[0]->frame.getOutlineThickness());

	initialize_pages();

};