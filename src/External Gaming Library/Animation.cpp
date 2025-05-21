#include "External Gaming Library/Animation.h"

float Animator::calculate_frame_duration(int fps, int n_frames) {

	if (fps <= 0) {

		std::cerr << "FPS must be greater than zero." << std::endl;
		return 0.0f;

	};

	return static_cast<float>(1) / fps;

};

void Animator::update(sf::Sprite& sprite, const sf::Texture& texture, const sf::Vector2u& spritesheet_size, const sf::Vector2f& frame_size, const sf::Vector2f& sprite_scale, sf::Vector2u& current_frame, float& elapsed_time, const float& delta_time, const int& fps) {

	elapsed_time += delta_time;

	this->frame_duration = calculate_frame_duration(fps, spritesheet_size.x * spritesheet_size.y);

	if (elapsed_time >= this->frame_duration) {

		elapsed_time -= this->frame_duration;
		current_frame.x++;

		if (current_frame.x >= spritesheet_size.x) {

			current_frame.x = 0;
			current_frame.y++;

		};

		if (current_frame.y >= spritesheet_size.y) {

			current_frame.y = 0;

		};

	};

	this->frame_rect.size.x = frame_size.x;
	this->frame_rect.size.y = frame_size.y;

	this->frame_rect.position.y = current_frame.y * this->frame_rect.size.y;
	this->frame_rect.position.x = current_frame.x * this->frame_rect.size.x;

	sprite.setTexture(texture);
	sprite.setTextureRect(this->frame_rect);
	sprite.setScale(sprite_scale);

};


Spritesheet::Spritesheet(const std::filesystem::path& texture_file_path, const sf::Vector2u& size) : size(size) {

	if (!std::filesystem::exists(texture_file_path)) { std::cout << "Spritesheet path doesnt exist => dead Spritesheet object created!\n"; return; };

	this->texture.loadFromFile(texture_file_path);
	//this->texture.setSmooth(true);

	//we have to divide the image Length and Width by the corrosponding row and column number so we can get
	//the actual width and length of each slide insie the spritesheet
	this->frame_size.x = this->texture.getSize().x / this->size.x;
	this->frame_size.y = this->texture.getSize().y / this->size.y;

	//VIP NOTE: if for some reason the type for frame_size was int and not float, then we have to cast frame_size to float first, otherwise we will get big miscalculations
	this->sprite_scale.x = sf::VideoMode::getDesktopMode().size.x / this->frame_size.x;
	this->sprite_scale.y = sf::VideoMode::getDesktopMode().size.y / this->frame_size.y;

	this->elapsed_time = 0.0f;
	this->current_frame = { 1, 1 };

};