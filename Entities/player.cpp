#include <iostream>
#include "player.h"

// Constructor
Player::Player()
{
	position = sf::Vector2f(100.f, 100.f);

	// Physics
	speed = 300.f;
	jumpSpeed = 200.f;
	gravity = 1500.f;
	grounded = false;

	// Input
	moveLeft = false;
	moveRight = false;
	moveDown = false;
	moveJump = false;
}

Player::~Player()
{

}

bool Player::Init()
{
	// Loading Player Textures
	if (!idleTexture.loadFromFile("Content/Sprites/Player/Idle.png")) {
		std::cout << "Error - Player Idle.png failed to load" << std::endl;
		return false;
	}
	if (!runTexture.loadFromFile("Content/Sprites/Player/Run.png")) {
		std::cout << "Error - Player Run.png failed to load" << std::endl;
		return false;
	}

	if (!jumpIdleTexture.loadFromFile("Content/Sprites/Player/JumpIdle.png")) {
		std::cout << "Error - Player JumpIdle.png failed to load" << std::endl;
		return false;
	}
	if (!jumpRunTexture.loadFromFile("Content/Sprites/Player/JumpRun.png")) {
		std::cout << "Error - Player JumpRun.png failed to load" << std::endl;
		return false;
	}

	sprite.setTexture(idleTexture);
	sprite.setPosition(100.f, 100.f);
	sprite.setScale(2.f, 2.f);

	size.x = sprite.getGlobalBounds().width;
	size.y = sprite.getGlobalBounds().height;

	return true;
}

void Player::Update(float dt)
{
	// Checking for grounded state for applying gravity (may be able to just remove this tbh)
	if (grounded)
		velocity.y = 0.f;
	else
		velocity.y += gravity * dt;

	// Basic animation
	if (moveLeft == moveRight)
		sprite.setTexture(grounded ? idleTexture : jumpIdleTexture);
	else if (moveLeft)
	{
		sprite.setTexture(grounded ? runTexture : jumpRunTexture);
		sprite.setTextureRect(sf::IntRect(
			sprite.getLocalBounds().width,
			0,
			-sprite.getLocalBounds().width,
			sprite.getLocalBounds().height));
	}
	else if (moveRight)
	{
		sprite.setTexture(grounded ? runTexture : jumpRunTexture);
		sprite.setTextureRect(sf::IntRect(
			0, 0, sprite.getLocalBounds().width,
			sprite.getLocalBounds().height));
	}

	// Updating position from velocity
	position += velocity * dt;
	sprite.setPosition(position);
}

void Player::Collision(sf::Vector2f newPosition, bool ground)
{
	grounded = ground;
	position = newPosition;
	sprite.setPosition(newPosition);
}

void Player::Render(sf::RenderWindow& window)
{
	window.draw(sprite);
}

void Player::KeyboardInput(sf::Event event)
{
	// Checking if event is 'KeyPressed' or 'KeyReleased'
	bool keyPressed = event.type == sf::Event::KeyPressed;
	// Input cases
	switch (event.key.scancode)
	{
	case sf::Keyboard::Scancode::D:
		velocity.x += keyPressed && !moveRight ? speed : 0;
		velocity.x += !keyPressed && moveRight ? -speed : 0;
		moveRight = keyPressed ? true : false;
		break;
	case sf::Keyboard::Scancode::A:
		velocity.x += keyPressed && !moveLeft ? -speed : 0;
		velocity.x += !keyPressed && moveLeft ? speed : 0;
		moveLeft = keyPressed ? true : false;
		break;

	case sf::Keyboard::Scancode::Space:
		if (grounded && keyPressed)
		{
			velocity.y = -jumpSpeed*3;
			grounded = false;
		}
		break;
	}
}

void Player::MouseInput(sf::Event event)
{
	// NOTE: Currently unused
}