#include "IA.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "IA.h"


#define IA_VISION 32*6
#define ATTACK_RANGE 32



enum PlayerAnims
{
	STAND_RIGHT, STAND_LEFT,
	MOVE_RIGHT, MOVE_LEFT,
	BLOCK_RIGHT, BLOCK_LEFT,
	ATTACK_RIGHT, ATTACK_LEFT,
	DYING_RIGHT, DYING_LEFT,
	DEAD_LEFT, DEAD_RIGHT
};

void IA::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Player* player, int minPos, int maxPos)
{
	// Configuring the spritesheet
	spritesheet.setWrapS(GL_MIRRORED_REPEAT);
	if (tileMapPos.y/64 < 9) spritesheet.loadFromFile("images/spriteSheetEnemy1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	else spritesheet.loadFromFile("images/spriteSheetEnemy2.png", TEXTURE_PIXEL_FORMAT_RGBA);

	// Configuring a single sprite
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.1f, 0.1f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(12);

	//KeyFrames
	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.6f, 0.0f));

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(-0.7f, 0.0f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.2f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.3f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.4f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.0f));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.3f, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.4f, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.5f, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.6f, 0.0f));

	sprite->setAnimationSpeed(BLOCK_RIGHT, 8);
	sprite->addKeyframe(BLOCK_RIGHT, glm::vec2(0.6f, 0.0f));
	sprite->addKeyframe(BLOCK_RIGHT, glm::vec2(0.7f, 0.0f));
	sprite->addKeyframe(BLOCK_RIGHT, glm::vec2(0.8f, 0.0f));
	sprite->addKeyframe(BLOCK_RIGHT, glm::vec2(0.9f, 0.0f));

	sprite->setAnimationSpeed(BLOCK_LEFT, 8);
	sprite->addKeyframe(BLOCK_LEFT, glm::vec2(-0.7f, 0.0f));
	sprite->addKeyframe(BLOCK_LEFT, glm::vec2(-0.8f, 0.0f));
	sprite->addKeyframe(BLOCK_LEFT, glm::vec2(-0.9f, 0.0f));
	sprite->addKeyframe(BLOCK_LEFT, glm::vec2(-1.0f, 0.0f));

	sprite->setAnimationSpeed(ATTACK_RIGHT, 6);
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.0f, 0.1f));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.1f, 0.1f));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.2f, 0.1f));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.3f, 0.1f));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.4f, 0.1f));

	sprite->setAnimationSpeed(ATTACK_LEFT, 6);
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(-0.1f, 0.1f));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(-0.2f, 0.1f));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(-0.3f, 0.1f));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(-0.4f, 0.1f));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(-0.5f, 0.1f));

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
	tileMapDispl = glm::ivec2(0, 0);
	sprite->setPosition(glm::vec2(float(0 + posIA.x), float(0 + posIA.y )));
	
	healthPoints = 3;
	playerBlocking = false;
	playerAttacking = false;
	this->player = player;
	this->minPos = minPos;
	this->maxPos = maxPos;
}


void IA::reset() {

	healthPoints = 3;
	playerBlocking = false;
	playerAttacking = false;
	tileMapDispl = glm::ivec2(0, 0);
	this->setPosition(posIAInit);
	sprite->changeAnimation(STAND_RIGHT);

	}

void IA::update(int deltaTime)
{

	bool finishAction = sprite->update(deltaTime);


	int playerX = player->getPosition().x;
	int playerY = player->getPosition().y;
	int sultanX = posIA.x;
	int sultanY = posIA.y;
	bool visionRange = (playerY - sultanY == 0) && (abs(playerX - sultanX) <= IA_VISION);
	bool fightRange = (playerY - sultanY == 0) && (abs(playerX - sultanX) <= ATTACK_RANGE);
	playerBlocking = player->isAttacking();
	playerAttacking = player->isAttacking();


	if (finishAction) {

		switch (sprite->animation()) {

		case STAND_LEFT:
				
			if (healthPoints == 0) sprite->changeAnimation(DYING_LEFT);
			else if (playerHealth == 0) sprite->changeAnimation(STAND_LEFT);
			else if (playerX > sultanX)	sprite->changeAnimation(STAND_RIGHT);
			else if (fightRange && playerBlocking) sprite->changeAnimation(BLOCK_LEFT);
			else if (fightRange) sprite->changeAnimation(ATTACK_RIGHT);
			else if (visionRange) sprite->changeAnimation(MOVE_LEFT);
			
			break;

		case STAND_RIGHT:

			if (healthPoints == 0) sprite->changeAnimation(DYING_RIGHT);
			else if (playerHealth == 0) sprite->changeAnimation(STAND_RIGHT);
			else if (playerX < sultanX)	sprite->changeAnimation(STAND_LEFT);
			else if (fightRange) sprite->changeAnimation(ATTACK_RIGHT);
			else if (visionRange) sprite->changeAnimation(MOVE_RIGHT);

			break;

		case MOVE_LEFT:

			if (healthPoints == 0) sprite->changeAnimation(DYING_LEFT);
			else if (playerHealth == 0) sprite->changeAnimation(STAND_LEFT);
			else if (playerX > sultanX)	sprite->changeAnimation(STAND_RIGHT);
			else if (fightRange && playerBlocking) sprite->changeAnimation(BLOCK_LEFT);
			else if (fightRange) sprite->changeAnimation(ATTACK_LEFT);
			else if (visionRange) sprite->changeAnimation(MOVE_LEFT);
		
			break;

		case MOVE_RIGHT:

			if (healthPoints == 0) sprite->changeAnimation(DYING_RIGHT);
			else if (playerHealth == 0) sprite->changeAnimation(STAND_RIGHT);
			else if (playerX < sultanX)	sprite->changeAnimation(STAND_LEFT);
			else if (fightRange && playerBlocking) sprite->changeAnimation(BLOCK_RIGHT);
			else if (fightRange) sprite->changeAnimation(ATTACK_RIGHT);
			else if (visionRange) sprite->changeAnimation(MOVE_RIGHT);
			
			break;

		case ATTACK_LEFT:

			if (!playerBlocking) player->dealDamage(1, "enemy");
			if (healthPoints == 0) sprite->changeAnimation(DYING_LEFT);
			else if (playerHealth == 0) sprite->changeAnimation(STAND_LEFT);
			else if (playerX > sultanX)	sprite->changeAnimation(STAND_RIGHT);
			else if (fightRange && playerBlocking) sprite->changeAnimation(BLOCK_LEFT);
			else if (fightRange) sprite->changeAnimation(ATTACK_LEFT);
			else if (visionRange) sprite->changeAnimation(MOVE_LEFT);


			break;

		case ATTACK_RIGHT:

			if (!playerBlocking) player->dealDamage(1,"enemy");
			if (healthPoints == 0) sprite->changeAnimation(DYING_RIGHT);
			else if (playerHealth == 0) sprite->changeAnimation(STAND_RIGHT);
			else if (playerX < sultanX)	sprite->changeAnimation(STAND_LEFT);
			else if (fightRange) sprite->changeAnimation(ATTACK_RIGHT);
			else if (visionRange) sprite->changeAnimation(MOVE_RIGHT);



			break;

		case BLOCK_LEFT:

			if (healthPoints == 0) sprite->changeAnimation(DYING_LEFT);
			else sprite->changeAnimation(ATTACK_LEFT);

			break;

		case BLOCK_RIGHT:

			if (healthPoints == 0) sprite->changeAnimation(DYING_RIGHT);
			else sprite->changeAnimation(ATTACK_RIGHT);

			break;
			
		case DYING_LEFT:

			sprite->changeAnimation(DEAD_LEFT);

			break;

		case DYING_RIGHT:

			sprite->changeAnimation(DEAD_RIGHT);

			break;


		}
	}
	if (sprite->animation() == MOVE_LEFT && sprite->getFrame() == 0 && posIA.x - 1 >= minPos) posIA.x -= 1;

	if (sprite->animation() == MOVE_RIGHT && sprite->getFrame() == 0 && posIA.x + 1 <= maxPos) posIA.x += 1;
	//else if (sprite->animation() == MOVE_RIGHT && posIA.x + 2 >= maxPos) sprite->changeAnimation(STAND_RIGHT);
	if (sprite->animation() == BLOCK_RIGHT && sprite->getFrame() % 2) posIA.x -= 1;
	if (sprite->animation() == BLOCK_LEFT && sprite->getFrame() %  2) posIA.x += 1;

	if (player->getHealth() == 0) sprite->changeAnimation(STAND_LEFT);

	
	//if (posIA.x - 2 == minPos && player->getPosition().x < posIA.x) sprite->changeAnimation(STAND_LEFT);
	//else if (posIA.x + 2 == maxPos && player->getPosition().x > posIA.x) sprite->changeAnimation(STAND_RIGHT);


	if (healthPoints == 0 && sprite->animation() != DYING_LEFT && sprite->animation() != DYING_RIGHT  && sprite->animation() != DEAD_LEFT && sprite->animation() != DEAD_RIGHT) sprite->changeAnimation(DYING_LEFT);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posIA.x), float(tileMapDispl.y + posIA.y - 6 )));
}

void IA::render()
{
	sprite->render();
}


void IA::setPosition(const glm::vec2 &pos)
{
	posIA = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posIA.x), float(tileMapDispl.y + posIA.y)));
	posIAInit = glm::vec2(float(tileMapDispl.x + posIA.x), float(tileMapDispl.y + posIA.y));

}


void IA::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void IA::dealDamageEnemy(int damage) {
	
	if (sprite->animation() != BLOCK_RIGHT && sprite->animation() != BLOCK_LEFT  ) {
		if (healthPoints - 1 >= 0)healthPoints -= 1;
		else healthPoints = 0;
			
	}



}

bool IA::sultanIsBlockking() {

	if (sprite->animation() != BLOCK_RIGHT && sprite->animation() != BLOCK_LEFT) {
		return false;

	}
	else return true;
	



}

bool IA::sultanIsAttacking() {

	if (sprite->animation() != ATTACK_RIGHT && sprite->animation() != ATTACK_LEFT) {
		return false;

	}
	else return true;




}

void IA::setPlayerStats(Player *player) {

	this->player = player;

}