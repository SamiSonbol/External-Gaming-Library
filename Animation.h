#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Animator {

public:

	sf::IntRect frame_rect;

	float frame_duration;

	float calculate_frame_duration(int fps, int n_frames);

	//this update function can certainly be programmed to take in less data and parameters; however, it will have to do more calculations. Hence, i opted to pre-calculate the data
	//and use them as parameters here 
	void update(sf::Sprite& sprite, const sf::Texture& texture, const sf::Vector2u& spritesheet_size, const sf::Vector2f& frame_size, const sf::Vector2f& sprite_scale, sf::Vector2u& current_frame, float& elapsed_time, const float& delta_time);

};

//Note: I opted to put elapsed_time and current_frame inside every respective Spritesheet object, inorder to avoid having an Animator object for every Spritesheet.
//this way one Animator object can animate all sprites using their Spritesheet information; more flexibale in my opinion. 
struct Spritesheet {

	sf::Texture texture;

	//size of spritesheet grid in columns and rows => x = n_columns, y = n_rows
	sf::Vector2u size;

	//size of one actual frame on the spritesheet in pixels => x = length, y = width 
	sf::Vector2f frame_size;

	//the size to which the sprite should be scaled inorder to envelop the whole screen
	sf::Vector2f sprite_scale;

	//every sprite has an elapsed time tracker since if the Animator used the same elapsed time for all sprites it would cause the animations to speed up and be falsely updated
	//Used to track the update status inorder to help the Animator 
	float elapsed_time;

	//position of the current frame on the spritesheet in row and column => x = column, y = row. Used to track the update status inorder to help the Animator 
	sf::Vector2u current_frame;

	Spritesheet(const std::string& texture_file_path = "", const sf::Vector2u& size = {1, 1});

};

