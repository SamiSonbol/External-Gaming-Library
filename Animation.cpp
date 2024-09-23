#include "Animation.h"
float Animator::calculate_frame_duration(int fps, int n_frames) {

	if (fps <= 0) {

		std::cerr << "FPS must be greater than zero." << std::endl;
		return 0.0f;

	};

	return static_cast<float>(1) / fps;

};

void Animator::update(sf::Sprite& sprite, const sf::Texture& texture, const sf::Vector2u& spritesheet_size, const sf::Vector2f& frame_size, const sf::Vector2f& sprite_scale, const float& delta_time) {

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


Spritesheet::Spritesheet(const std::string& texture_file_path, const sf::Vector2u& size) {

	if (!texture_file_path.empty()) {

		this->texture.loadFromFile(texture_file_path);
		this->texture.setSmooth(true);

		//we have to divide the image Length and Width by the corrosponding row and column number so we can get
		//the actual width and length of each slide insie the spritesheet
		this->size = size;
		this->frame_size.x = this->texture.getSize().x / this->size.x;
		this->frame_size.y = this->texture.getSize().y / this->size.y;

		//VIP NOTE: if for some reason the type for frame_size was int and not float, then we have to cast frame_size to float first, otherwise we will get big miscalculations
		this->sprite_scale.x = sf::VideoMode::getDesktopMode().width / this->frame_size.x;
		this->sprite_scale.y = sf::VideoMode::getDesktopMode().height / this->frame_size.y;

	}
	else {

		std::cout << "no path detected, dead Spritesheet object created" << std::endl;

	};

};