#include "IA.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "IA.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 0
#define FALL_STEP 4


enum PlayerAnims
{
	STAND_RIGHT, STAND_LEFT,
	MOVE_RIGHT, MOVE_LEFT,
	ATTACK_RIGHT, ATTACK_LEFT,
	DYING_RIGHT, DYING_LEFT,
	DEAD_LEFT, DEAD_RIGHT
};

void IA::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{

	// Configuring the spritesheet
	spritesheet.setWrapS(GL_MIRRORED_REPEAT);
	spritesheet.loadFromFile("images/spriteSheetEnemy.png", TEXTURE_PIXEL_FORMAT_RGBA);

	// Configuring a single sprite
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.1f, 0.1f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(10);

	//KeyFrames
	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.0f, 0.0f));

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(-0.1f, 0.0f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.3f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.4f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.0f));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.4f, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.5f, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.6f, 0.0f));

	sprite->setAnimationSpeed(ATTACK_RIGHT, 7);
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.6f, 0.0f));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.7f, 0.0f));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.8f, 0.0f));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.9f, 0.0f));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.1f, 0.1f));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.2f, 0.1f));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.3f, 0.1f));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.4f, 0.1f));

	sprite->setAnimationSpeed(ATTACK_LEFT, 7);
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(-0.5f, 0.0f));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(-0.6f, 0.0f));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(-0.7f, 0.0f));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(-0.8f, 0.0f));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(-0.9f, 0.0f));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(-0.1f, 0.1f));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(-0.2f, 0.1f));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(-0.3f, 0.1f));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(-0.4f, 0.1f));

	sprite->setAnimationSpeed(DYING_RIGHT, 8);
	sprite->addKeyframe(DYING_RIGHT, glm::vec2(0.5f, 0.1f));
	sprite->addKeyframe(DYING_RIGHT, glm::vec2(0.6f, 0.1f));
	sprite->addKeyframe(DYING_RIGHT, glm::vec2(0.7f, 0.1f));
	sprite->addKeyframe(DYING_RIGHT, glm::vec2(0.8f, 0.1f));
	sprite->addKeyframe(DYING_RIGHT, glm::vec2(0.9f, 0.1f));
	sprite->addKeyframe(DYING_RIGHT, glm::vec2(0.0f, 0.2f));


	sprite->setAnimationSpeed(DYING_LEFT, 8);
	sprite->addKeyframe(DYING_LEFT, glm::vec2(-0.6f, 0.1f));
	sprite->addKeyframe(DYING_LEFT, glm::vec2(-0.7f, 0.1f));
	sprite->addKeyframe(DYING_LEFT, glm::vec2(-0.8f, 0.1f));
	sprite->addKeyframe(DYING_LEFT, glm::vec2(-0.9f, 0.1f));
	sprite->addKeyframe(DYING_LEFT, glm::vec2(-1.0f, 0.1f));
	sprite->addKeyframe(DYING_LEFT, glm::vec2(-0.1f, 0.2f));


	sprite->setAnimationSpeed(DEAD_RIGHT, 8);
	sprite->addKeyframe(DEAD_RIGHT, glm::vec2(0.1f, 0.2f));

	sprite->setAnimationSpeed(DEAD_LEFT, 8);
	sprite->addKeyframe(DEAD_LEFT, glm::vec2(-0.2f, 0.2f));

	//Init sprite and position
	sprite->changeAnimation(STAND_LEFT);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posIA.x), float(tileMapDispl.y + posIA.y)));

}

void IA::update(int deltaTime)
{

	bool finishAction = sprite->update(deltaTime);


	int playerX = player->getPosition().x;
	int playerY = player->getPosition().y;
	int sultanX = posIA.x;
	int sultanY = posIA.y;

	if (finishAction) {

		switch (sprite->animation()) {
			//	STAND_RIGHT, STAND_LEFT,
			//		MOVE_RIGHT, MOVE_LEFT,
			//      ATTACK_RIGHT, ATTACK_LEFT,
			//		DYING_RIGHT, DYING_LEFT,
			//		DEAD_LEFT, DEAD_RIGHT

		case STAND_LEFT:


			if (player->getPosition().y - posIA.y <= 0 && posIA.x - player->getPosition().x <= 320 && posIA.x - player->getPosition().x >= 0)
				sprite->changeAnimation(MOVE_LEFT);
			break;

		case STAND_RIGHT:

			if (player->getPosition().y - posIA.y <= 0 && player->getPosition().x - posIA.x <= 320 && player->getPosition().x - posIA.x >= 0)
				sprite->changeAnimation(MOVE_RIGHT);
			break;

		case MOVE_LEFT:
			if (player->getPosition().y - posIA.y <= 0 && posIA.x - player->getPosition().x <= 32)sprite->changeAnimation(ATTACK_LEFT);
			else if (player->getPosition().y - posIA.y <= 0 && posIA.x - player->getPosition().x <= 0)
				sprite->changeAnimation(STAND_RIGHT);
			break;

		case MOVE_RIGHT:
			if (player->getPosition().y - posIA.y <= 0 && posIA.x - player->getPosition().x <= 0)
				sprite->changeAnimation(STAND_LEFT);
			break;

		case ATTACK_LEFT:

			break;

		case ATTACK_RIGHT:

			break;

		case DYING_LEFT:

			break;

		case DYING_RIGHT:

			break;

		case DEAD_LEFT:

			break;

		case DEAD_RIGHT:

			break;
		}
	}
	if (sprite->animation() == MOVE_LEFT) posIA.x -= 1;
	if (sprite->animation() == MOVE_RIGHT) posIA.x += 1;

	if ((sprite->animation() == ATTACK_LEFT || sprite->animation() == ATTACK_RIGHT) && !player->isAttacking()) player->dealDamage(1);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posIA.x), float(tileMapDispl.y + posIA.y)));
}

void IA::render()
{
	sprite->render();
}

void IA::setPlayer(Player *player)
{
	this->player = player;
}

void IA::setPosition(const glm::vec2 &pos)
{
	posIA = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posIA.x), float(tileMapDispl.y + posIA.y)));
}


void IA::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

