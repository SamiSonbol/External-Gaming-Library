#include "External Gaming Library/User_Interface.h"

void User_Interface::set_cursor(const sf::RenderWindow* window, const std::filesystem::path& cursor_icon_path) {

	sf::WindowHandle wHandle = window->getNativeHandle();

	HCURSOR Cursor = LoadCursorFromFile(cursor_icon_path.string().c_str());
	SetCursor(Cursor);

	SetClassLongPtr(wHandle, GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(Cursor));

};

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void User_Interface::add_page(Widgets::Page&& page) {

	this->pages.emplace_back(std::move(page));

};

Widgets::Page& User_Interface::get_reference_to_page(const size_t& index) {

	return this->pages[index];

};

void User_Interface::initialize(Game_Library& library) {

	Widgets::Button_Game bb(library.games[0], 2);

	float title_area = bb.get_size().y * 2.0f * sf::VideoMode::getDesktopMode().size.x;
	float window_area = sf::VideoMode::getDesktopMode().size.x * sf::VideoMode::getDesktopMode().size.y;

	//here the actual formula we use is number_of_objects = area_of_whole_space / area_of_one_object. However, since in our title_area doesnt take the space of our searchbar
	//into consederation, the screen_capacity will be +1 of the actual capacity that the screen can hold; hence, we subtract 1 from it.
	int screen_capacity = (window_area / title_area) - 2.0f;

	int n_titles = library.games.size();

	int index = 0;
	while (true) {

		int page_size;
		int remaining_titles = n_titles - screen_capacity;

		if (remaining_titles >= 0) {

			page_size = screen_capacity;

		}
		else if (remaining_titles < 0) {

			page_size = n_titles;

		};

		Widgets::Page page;
		for (int i = index; i < index + page_size; i++) {

			std::cout << "ADDED!\n";
			Widgets::Button_Game button(library.games[i], 2);
			//button.set_fill_color(button.fill_color.first, this->box_color);
			//button.set_outline_color(button.outline_color.first, this->chosen_outline_color);
			//button.set_fill_color(sf::Color(0, 0, 0, 0), button.fill_color.second);

			page.add_button(std::move(button));

		};

		page.update_buttons_positions();
		this->add_page(std::move(page));
		std::cout << "n_elements on current_page: " << this->get_reference_to_page(this->pages.size() - 1).get_size() << "\n";

		index += page_size;
		n_titles = n_titles - screen_capacity;
		if (n_titles <= 0) { break; };

	};//while loop bracket

	std::cout << "n_pages: " << this->pages.size() << "\n";

	//bb.~Button_Game();

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

void User_Interface::put_button_in_selected_state(Widgets::Button_Game& button) {

	button.is_selected = true;
	this->last_selected_title = button.index;

	//this->searchbar.play_button.stick_to(button);
	this->searchbar.play_button.set_position({button.get_position().x + button.get_size().x, button.get_position().y});

	this->display_game_background.store(true);

};

void User_Interface::unselect_button(Widgets::Button_Game& game) {

	game.turn_off();
	this->display_game_background.store(false);
	game.is_selected = false;

};

void User_Interface::unselect_all_buttons(Widgets::Page& page) {

	for (size_t i = 0; i < page.get_size(); i++) {

		if (page.get_reference_to_button(i).is_selected) {

			this->unselect_button(page.get_reference_to_button(i));

			//return;

		};

	};//games on page loop

};

void User_Interface::hover_over_buttons(Widgets::Page& page, const sf::Vector2f& mouse_position) {

	if (last_mouse_pos != mouse_position) {

		last_mouse_pos = mouse_position;

		if (mouse_position.x >= 0.0f && mouse_position.x <= sf::VideoMode::getDesktopMode().size.x && mouse_position.y >= 0.0f && mouse_position.y <= sf::VideoMode::getDesktopMode().size.y) {

			thread_lock.lock();
			page.update_buttons_states(mouse_position);//mouse hovering function on all standard pages
			thread_lock.unlock();

		};//checks if mouse is inside window

	};//checks if mouse has moved

};

void User_Interface::update_mouse_scroll(const std::optional<sf::Event>& event) {

	if (event->is<sf::Event::MouseWheelScrolled>() && !this->searchbar.mouse_wheel_locked) {
		
		auto scroll_wheel_event = event->getIf<sf::Event::MouseWheelScrolled>();
		if (scroll_wheel_event->delta >= 0 && this->page_number > 0) {

			this->display_game_background.store(false);
			unselect_all_buttons(this->pages[this->page_number]);

			this->page_number -= 1;
			this->get_reference_to_page(this->page_number).update_buttons_positions();

		}
		else if (scroll_wheel_event->delta < 0 && this->page_number < this->pages.size() - 1) {

			this->display_game_background.store(false);
			unselect_all_buttons(this->pages[this->page_number]);

			this->page_number += 1;
			this->get_reference_to_page(this->page_number).update_buttons_positions();

		};

	};

};

void User_Interface::update_mouse_button_clicking(const sf::Vector2f& mouse_position) {

	sf::Mouse::Button left_mouse_button = sf::Mouse::Button::Left;
	if (sf::Mouse::isButtonPressed(left_mouse_button)) {

		Widgets::Page& page_reference = this->get_reference_to_page(this->page_number);
		for (size_t i = 0; i < page_reference.get_size(); i++) {

			Widgets::Button_Game& button = page_reference.get_reference_to_button(i);
			if (this->searchbar.play_button.is_clicked(mouse_position, left_mouse_button) && button.is_selected) {//launches the game if the *play* button is clicked and if the game button was already clicked before

				button.game->executable.launch();

			}
			else if (button.is_clicked(mouse_position, left_mouse_button)) {//sets this button to be in the selected state to be used as launch executable later on if we click the *play* button and display its background

				this->put_button_in_selected_state(button);

			}
			else if (!button.is_clicked(mouse_position, left_mouse_button) && button.is_selected) {//if button wasnt clicked and it isnt in the selected state, we reset it back to normal state

				this->unselect_button(button);

			};

		};

	};

};

void User_Interface::UPDATE(const std::optional<sf::Event>& event) {
	
	sf::Vector2f mouse_position(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);

	this->searchbar.update(event, mouse_position, this->get_reference_to_page(this->page_number), this->pages);

	this->update_mouse_scroll(event);
	if (this->searchbar.search_complete && !this->searchbar.search_results_page.is_empty()) {

		this->hover_over_buttons(this->searchbar.search_results_page, mouse_position);

	}
	else {

		this->hover_over_buttons(this->get_reference_to_page(this->page_number), mouse_position);

	};

	this->update_mouse_button_clicking(mouse_position);

};

void User_Interface::render_background(Spritesheet& background, const float& delta_time, sf::RenderWindow* window) {

	this->animator.update(this->rendered_sprite, background.texture, background.size, background.frame_size, background.sprite_scale, background.current_frame, background.elapsed_time, delta_time);
	window->draw(this->rendered_sprite);

};

void User_Interface::render_buttons(Widgets::Page& page, sf::RenderWindow* window) {

	for (size_t i = 0; i < page.get_size(); i++) {

		Widgets::Button_Game& button = page.get_reference_to_button(i);
		button.render(window);
		if (button.is_selected && this->display_game_background.load()) {

			this->searchbar.play_button.render(window);

		}//checks if the game is selected and then draws the play button
		
	};

};

void User_Interface::RENDER(const float& delta_time, sf::RenderWindow* window) {

	thread_lock.lock();

	if (this->display_game_background) {

		render_background(this->get_reference_to_page(this->page_number).get_reference_to_button(this->last_selected_title).game->background, delta_time, window);

	}
	else {

		render_background(this->menu_background, delta_time, window);

	};

	if (this->searchbar.search_complete) {

		if (!this->searchbar.search_results_page.is_empty()) {

			render_buttons(this->searchbar.search_results_page, window);

		};

	}
	else {

		render_buttons(this->pages[this->page_number], window);

	};

	thread_lock.unlock();

	this->searchbar.search_box.render(window);
	this->searchbar.return_button.render(window);

};

User_Interface::User_Interface(Game_Library& library, sf::RenderWindow* window, sf::Font& font) : 

	searchbar(this->box_color, font), 
	menu_background(library.menu_background_path, { 10, 25 }),
	loading_screen(library.splash_screen_path),
	rendered_sprite(this->loading_screen.texture),
	display_game_background(false),
	last_mouse_pos({ 0.0f, 0.0f }),
	page_number(0),
	last_selected_title(0),
	box_color(sf::Color::Black),
	chosen_outline_color(sf::Color::Yellow) {

	set_cursor(window, RESOURCES_DIR"/mouse_cursors/Eye.ani");
	this->game_titles_font.openFromFile(RESOURCES_DIR"/fonts/Montserrat-SemiBold.otf");
	
	render_background(loading_screen, 1, window);
    window->display();
	
	this->initialize(library);
	set_cursor(window, RESOURCES_DIR"/mouse_cursors/Kunai.ani");
	
	sf::Color fill = this->searchbar.search_box.fill_color.first;
	fill.a = 150;
	this->searchbar.search_box.set_fill_color(fill, this->searchbar.search_box.fill_color.second);
	this->searchbar.return_button.set_fill_color(fill, this->searchbar.return_button.fill_color.second);
	this->searchbar.play_button.set_fill_color(fill, this->searchbar.play_button.fill_color.second);
	
};