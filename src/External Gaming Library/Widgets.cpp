#include "External Gaming Library/Widgets.h"

void Widgets::Button_Base::set_size(const sf::Vector2f& size) {

	this->frame.setScale(size);
	this->size = size;

};
sf::Vector2f Widgets::Button_Base::get_size() {

	return this->size;

};

void Widgets::Button_Base::set_fill_color(const sf::Color& color_off, const sf::Color& color_on) { 
	
	this->fill_color.first = color_off; 
	this->fill_color.second = color_on; 
	this->frame.setFillColor(color_off);

};
void Widgets::Button_Base::set_outline_color(const sf::Color& color_off, const sf::Color& color_on) { 
	
	this->outline_color.first = color_off; 
	this->outline_color.second = color_on; 
	this->frame.setOutlineColor(color_off);

};

//this function exists only so the function naming would stay on paar with the set_position function
sf::Vector2f Widgets::Button_Base::get_position() { 
	
	return this->frame.getPosition(); 

};

void Widgets::Button_Base::stick_to(Button_Base& target) { 
	
	float offset = target.size.x * 0.03; 
	set_position({ target.get_position().x + target.size.x + offset, target.get_position().y }); 

};

bool Widgets::Button_Base::contains(const sf::Vector2f& point) {

	if (this->frame.getGlobalBounds().contains(point)) {

		return true;

	}
	else {

		return false;

	};

};

bool Widgets::Button_Base::is_clicked(const sf::Vector2f& mouse_position, const sf::Mouse::Button& mouse_button) {

	return this->contains(mouse_position) && sf::Mouse::isButtonPressed(mouse_button);

};

void Widgets::Button_Base::turn_on() { 
	
	this->frame.setFillColor(this->fill_color.second); 
	this->frame.setOutlineColor(this->outline_color.second);

};	

void Widgets::Button_Base::turn_off() { 
	
	this->frame.setFillColor(this->fill_color.first); 
	this->frame.setOutlineColor(this->outline_color.first); 

};

void Widgets::Button_Base::set_behaviour(std::function<void()> lambda) {

	this->behaviour = std::move(lambda);

};

void Widgets::Button_Base::activate_behaviour() {

	if (this->behaviour) {

		this->behaviour();
		return;

	};

	std::cerr << "WARNING: no behaviour exists for Button!\n";
	return;

};

void Widgets::Button_Base::render(sf::RenderWindow* window) {

	window->draw(this->frame);

};

void Widgets::Button_Base::make_frame(const int& frame_type) {

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

		this->frame.setPoint(2, { -1, 1 });

		break;

	};

	this->frame.setOutlineThickness(0.05f);

	this->set_fill_color(sf::Color::Black, sf::Color::Yellow);
	this->set_outline_color(sf::Color::White, sf::Color::Black);

};
Widgets::Button_Base::Button_Base(const int& frame_type, std::function<void()> behaviour) : behaviour(std::move(behaviour)) {

	this->make_frame(frame_type);

};

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void Widgets::Button::set_fill_color(const sf::Color& color_off, const sf::Color& color_on) {

	this->fill_color.first = color_off;
	this->fill_color.second = color_on;
	this->frame.setFillColor(color_off);
	this->text.setFillColor(color_off);

};
void Widgets::Button::set_outline_color(const sf::Color& color_off, const sf::Color& color_on) {

	this->outline_color.first = color_off;
	this->outline_color.second = color_on;
	this->frame.setOutlineColor(color_off);
	this->text.setOutlineColor(color_off);

};

void Widgets::Button::set_position(const sf::Vector2f& position) {

	int offset = (this->text.getCharacterSize() / 2) / 2;
	offset -= 1;

	this->frame.setPosition(position);
	this->text.setPosition({ position.x + offset, position.y + 1 });

};

void Widgets::Button::turn_on() { 
	
	this->frame.setFillColor(this->fill_color.first); 
	this->frame.setOutlineColor(this->fill_color.second);

	this->text.setFillColor(this->fill_color.second);
	this->text.setOutlineColor(this->fill_color.second);

};
void Widgets::Button::turn_off() {

	this->frame.setFillColor(this->fill_color.first);
	this->frame.setOutlineColor(this->outline_color.first);

	this->text.setFillColor(this->outline_color.first);
	this->text.setOutlineColor(this->outline_color.first);

};

void Widgets::Button::render(sf::RenderWindow* window) {

	window->draw(this->frame);
	window->draw(this->text);

};

Widgets::Button::Button(const sf::Font& font, const std::string& text, const int& frame_type, std::function<void()> behaviour) : Button_Base(frame_type, behaviour), text(font, text) {

	int offset = this->text.getCharacterSize() / 2;
	float X = this->text.getGlobalBounds().size.x + offset;
	float Y = this->text.getGlobalBounds().size.y + offset;
	this->set_size({ X, Y });

	set_position({ 10, 0 });

};

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Widgets::Button_Game::set_fill_color(const sf::Color& color_off, const sf::Color& color_on) {

	this->fill_color.first = color_off;
	this->fill_color.second = color_on;
	this->game->title.setFillColor(color_off);
	this->frame.setOutlineColor(color_off);

};
void Widgets::Button_Game::set_outline_color(const sf::Color& color_off, const sf::Color& color_on) {

	this->outline_color.first = color_off;
	this->outline_color.second = color_on;
	this->game->title.setOutlineColor(color_off);

};

void Widgets::Button_Game::set_position(const sf::Vector2f& position) {

	float offset = this->frame.getOutlineThickness();

	this->frame.setPosition(position);
	this->game->title.setPosition({ position.x + offset, position.y + 1.0f });

};

void Widgets::Button_Game::turn_on() { 
	
	this->game->title.setFillColor(this->fill_color.second); 
	this->game->title.setOutlineColor(this->outline_color.second);

};
void Widgets::Button_Game::turn_off() { 
	
	this->game->title.setFillColor(this->fill_color.first); 
	this->game->title.setOutlineColor(this->outline_color.first);

};

void Widgets::Button_Game::render(sf::RenderWindow* window) {

	window->draw(this->frame);
	window->draw(this->game->title);

};

Widgets::Button_Game::Button_Game(Game_Library::Game& game, const int& frame_type, std::function<void()> behaviour) : Button_Base(frame_type, behaviour), game(&game) {

	float offset = this->game->title.getCharacterSize() / 2;
	float X = this->game->title.getGlobalBounds().size.x + offset;
	float Y = this->game->title.getGlobalBounds().size.y + offset;
	this->set_size({ X, Y });

	this->frame.setFillColor(sf::Color::Transparent);
	this->frame.setOutlineColor(sf::Color::Transparent);
	set_position({ 10, 0 });

};

Widgets::Button_Game::~Button_Game() {

	//free(game);

};

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void Widgets::Page::add_button(Button_Game&& button) {

	this->buttons.emplace_back(std::move(button));
	this->buttons[this->buttons.size() - 1].index = this->buttons.size() - 1;

};

Widgets::Button_Game& Widgets::Page::get_reference_to_button(const size_t& index) {

	return this->buttons[index];

};

void Widgets::Page::update_buttons_positions() {

	if (this->buttons.empty()) { return; };

	bool first_title_is_set = false;

	float width_offset = this->buttons[0].get_size().y * 2.0f;
	size_t n_buttons_on_page = this->buttons.size();

	for (size_t i = 0; i < n_buttons_on_page; ++i) {

		size_t last_index = (i - 1) % n_buttons_on_page;

		if (!first_title_is_set) {

			this->buttons[i].set_position({ this->buttons[i].get_position().x, width_offset });

			first_title_is_set = true;

		}
		else if (first_title_is_set) {

			this->buttons[i].set_position({ this->buttons[last_index].get_position().x, this->buttons[last_index].get_position().y + width_offset });

		};

	};

};

void Widgets::Page::update_buttons_states(const sf::Vector2f& mouse_position) {

	for (auto& button : this->buttons) {

		if (button.contains(mouse_position)) {

			button.turn_on();
			button.is_hovered_over = true;
			break;

		}
		else if (!button.contains(mouse_position) && !button.is_selected && button.is_hovered_over) {

			button.turn_off();
			button.is_hovered_over = false;
			break;

		};

	};//for loop bracket

};

size_t Widgets::Page::get_size() const { 
	
	return this->buttons.size(); 

};

bool Widgets::Page::is_empty() const { 
	
	return this->buttons.empty(); 

};

void Widgets::Page::clear() { 
	
	this->buttons.clear(); 

};

//==================================================================================================================================================================
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Widgets::Searchbar::set_searching() {

	this->searching = true;

	this->search_box.text.setString("|");

	this->inputed_text.clear();

}//setting the searchbar active to take input

void Widgets::Searchbar::reset_searching() {

	this->searching = false;

	this->search_box.text.setString("Search");

	this->inputed_text.clear();

}//reseting the searchbar if the search wasnt started after taking in the input

void Widgets::Searchbar::take_search_input(const sf::Event::TextEntered& my_event) {

	if (this->searching) {

		this->inputed_text += static_cast<char>(my_event.unicode);

		this->search_box.text.setString(this->inputed_text);

	};

};

void Widgets::Searchbar::return_to_page(Widgets::Page& page) {

	this->search_results_page.clear();

	this->search_complete = false;

	reset_searching();

	page.update_buttons_positions();

	this->mouse_wheel_locked = false;

};

bool Widgets::Searchbar::search(std::vector<Widgets::Page>& pages) {

	this->search_results_page.clear();
	this->inputed_text = Game_Library::make_lowercase(this->inputed_text);//lowering all alphabet cases of the input search to streamline the search

	for (auto& page : pages) {

		for (size_t i = 0; i < page.get_size(); i++) {

			if (Game_Library::make_lowercase(page.get_reference_to_button(i).game->title.getString()) == this->inputed_text) {

				Widgets::Button_Game current_button_copy(page.get_reference_to_button(i));
				this->search_results_page.add_button(std::move(current_button_copy));

				this->search_results_page.update_buttons_positions();
				this->mouse_wheel_locked = true;
				this->search_complete = true;

				return true;

			};

		};

	};//searching for a match between the input search and a game title

	for (auto& page : pages) {

		for (size_t i = 0; i < page.get_size(); i++) {

			if (tolower(page.get_reference_to_button(i).game->title.getString()[0]) == this->inputed_text[0]) {

				Widgets::Button_Game current_button_copy(page.get_reference_to_button(i));
				this->search_results_page.add_button(std::move(current_button_copy));

			};

		};

	};//if no match was found then this loop will search again with a criteria of taking in the same first letter instead of the whole name

	if (!this->search_results_page.is_empty()) {

		this->search_results_page.update_buttons_positions();
		this->mouse_wheel_locked = true;
		this->search_complete = true;

		return true;

	}
	else { this->search_box.text.setString("no results found"); return false; };

};

void Widgets::Searchbar::update_searching_state(const std::optional<sf::Event>& entering_text_event, const sf::Vector2f& mouse_pos, std::vector<Widgets::Page>& pages) {

	sf::Mouse::Button left_mouse_button = sf::Mouse::Button::Left;
	if (this->searching && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {

		if (this->search(pages)) {

			return;

		}//seraching through titles and producing a search result

	}//if no results were found we go forward to check other logic
	else if (sf::Mouse::isButtonPressed(left_mouse_button)) {

		if (this->search_box.is_clicked(mouse_pos, left_mouse_button)) {

			this->set_searching();

			return;

		}
		else {//directly returning from this function if the searchbar was set to take input

			this->reset_searching();

		};//moving forward with the logic if the searchbar was reset

	}
	else if (entering_text_event->is<sf::Event::TextEntered>()) {

		this->take_search_input(*entering_text_event->getIf<sf::Event::TextEntered>());

	};

};

void Widgets::Searchbar::update(const std::optional<sf::Event>& entering_text_event, const sf::Vector2f& mouse_position, Widgets::Page current_page, std::vector<Widgets::Page>& all_pages) {

	if (mouse_position.x >= 0.0f && mouse_position.x <= sf::VideoMode::getDesktopMode().size.x && mouse_position.y >= 0.0f && mouse_position.y <= sf::VideoMode::getDesktopMode().size.y) {

		sf::Mouse::Button left_mouse_button = sf::Mouse::Button::Left;
		if (this->search_box.contains(mouse_position)) {

			this->search_selected = true;
			this->search_box.turn_on();
			this->update_searching_state(entering_text_event, mouse_position, all_pages);

		}
		else if (!this->search_box.contains(mouse_position)) {

			this->search_selected = false;
			this->search_box.turn_off();

		}//checks if the mouse is hovering over the search bar or not

		if (this->return_button.contains(mouse_position)) {

			this->return_selected = true;
			this->return_button.turn_on();
			if (this->return_button.is_clicked(mouse_position, left_mouse_button)) { this->return_to_page(current_page); };

		}
		else if (!this->return_button.contains(mouse_position)) {

			this->return_selected = false;
			this->return_button.turn_off();

		}//checks if the mouse is hovering over the return button or not

		if (this->play_button.contains(mouse_position)) {

			this->play_selected = true;
			this->play_button.turn_on();

		}
		else if (!this->play_button.contains(mouse_position)) {

			this->play_selected = false;
			this->play_button.turn_off();

		}//checks if the mouse is hovering over the play button or not

	};

};

Widgets::Searchbar::Searchbar(const sf::Color& color, const sf::Font& font) : search_box(font, "Search"), return_button(font, "<<"), play_button(font, "PLAY") {

	float part_of_screen_width = sf::VideoMode::getDesktopMode().size.x * 0.25f;
	this->search_box.set_size({ part_of_screen_width, this->search_box.get_size().y});
	this->search_box.set_position({ 10, 14 });

	this->return_button.set_size({ this->search_box.get_size().y + this->return_button.text.getCharacterSize(), this->search_box.get_size().y });
	float return_button_pos_x = this->search_box.get_position().x + this->search_box.get_size().x + this->return_button.get_size().x + this->return_button.frame.getOutlineThickness();
	this->return_button.set_position({ return_button_pos_x, this->search_box.get_position().y });

};
