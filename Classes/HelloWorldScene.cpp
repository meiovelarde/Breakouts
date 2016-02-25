#include "HelloWorldScene.h"
#include "math.h"
#include "stdlib.h"
#include "Brick.h"
USING_NS_CC;
using namespace std;

Scene* HelloWorld::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	auto layer = HelloWorld::create();
	scene->addChild(layer);
	return scene;
}

bool HelloWorld::init()
{
	auto bg = cocos2d::LayerColor::create(Color4B(146, 174, 38, 255));
	this->addChild(bg);
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	angle = 45;
	brekduler = schedule_selector(HelloWorld::update);
	restarting = schedule_selector(HelloWorld::restart);
	gamespeed = .05;
	this->schedule(brekduler, gamespeed); 
	auto eventListener = EventListenerKeyboard::create();

	eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		// If a key already exists, do nothing as it will already have a time stamp
		// Otherwise, set's the timestamp to now
		if (keys.find(keyCode) == keys.end()) {
			keys[keyCode] = std::chrono::high_resolution_clock::now();
		}
	};
	eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		// remove the key.  std::map.erase() doesn't care if the key doesnt exist
		keys.erase(keyCode);
	};

	paddle = CCSprite::create("paddle.png");
	paddle->setPosition(Vec2(visibleSize.width/2 - origin.x, 
							 visibleSize.height/6 - origin.y - (2.8*paddle->getContentSize().height)));
	
	ball = CCSprite::create("ball.png");
	ball->setPosition(Vec2(visibleSize.width / 2 - origin.x,
		visibleSize.height / 6 - origin.y - (2.8*ball->getContentSize().height)));
	updateXY();
	bricks.push_back(new Brick(this, Vec2(ball->getPosition().x, ball->getPosition().y + 50)));
	wall = CCSprite::create("wall.png");
	wall->setPosition(Vec2(0, 0));
	wall->setAnchorPoint(Vec2(0, 0));
	wallTops = CCSprite::create("walltop.png");
	wallTops ->setPosition(Vec2(0, 455));
	wallTops->setAnchorPoint(Vec2(0, 0));
	wallLeft = Rect(0,0, 18, 475);
	wallTop = wallTops->getBoundingBox();
	wallRight = Rect(618, 0, 18, 475);
	this->addChild(wall);
	this->addChild(ball);
	this->addChild(paddle);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);
	return true;
}

void HelloWorld::updateXY() {
	bmX = ball->getContentSize().width * cos(angle * M_PI/180);
	bmY = ball->getContentSize().height * sin(angle * M_PI/180);
}

bool HelloWorld::isKeyPressed(EventKeyboard::KeyCode code) {
	// Check if the key is currently pressed by seeing it it's in the std::map keys
	// In retrospect, keys is a terrible name for a key/value paried datatype isnt it?
	if (keys.find(code) != keys.end())
		return true;
	return false;
}

void HelloWorld::restart(float delta) {
	auto newScene = HelloWorld::createScene(); 
	Director::getInstance()->replaceScene(newScene);
}

void HelloWorld::move() {
	auto position = paddle->getPosition();
	auto ballPos = ball->getPosition();
	
		if (isKeyPressed(EventKeyboard::KeyCode::KEY_LEFT_ARROW) || isKeyPressed(EventKeyboard::KeyCode::KEY_A)) {
			if (!isKeyPressed(EventKeyboard::KeyCode::KEY_RIGHT_ARROW) && !isKeyPressed(EventKeyboard::KeyCode::KEY_D)) {
				if (checkWallHit() != 1)
				position.x -= paddle->getContentSize().width / 6;
				dir = -1;
			} 
		}
		else if (isKeyPressed(EventKeyboard::KeyCode::KEY_RIGHT_ARROW) || isKeyPressed(EventKeyboard::KeyCode::KEY_D)) {
			if (!isKeyPressed(EventKeyboard::KeyCode::KEY_LEFT_ARROW) && !isKeyPressed(EventKeyboard::KeyCode::KEY_A)) {
				if (checkWallHit() != 2)
				position.x += paddle->getContentSize().width / 6;
				dir = 1;
			}
		}
		ballPos.x += bmX;
		ballPos.y += bmY;

		paddle->setPosition(position);
		ball->setPosition(ballPos);
}

void HelloWorld::checkBallHit() {
	auto ballBox = ball->getBoundingBox();
	auto padPos = paddle->getPosition();
	auto padSize = paddle->getContentSize();
	/*auto padBox1 = Rect(padPos.x,
						padPos.y,
						padSize.width / 3,
						padSize.height
						);*/
	auto padBox2 = paddle->getBoundingBox();/* Rect(padPos.x+padSize.width / 3,
						padPos.y,
						padSize.width / 3,
						padSize.height
						);
	auto padBox3 = Rect(padPos.x + (2 * (padSize.width / 3)),
						padPos.y,
						padSize.width / 3,
						padSize.height
						); */

	/*if (ballBox.intersectsRect(padBox1)) {
		switch (dir) {
			OutputDebugString(L" HitBox1 ");
		case 1://right
			if ((angle >= 270 && angle <= 360))
				angle += 125;
			else if ((angle < 270 && angle >= 180))
				angle += 145;
			break;
		case -1: //left
			if ((angle >= 270 && angle <= 360))
				angle -= 125;
			else if ((angle < 270 && angle >= 180))
				angle -= 145;
			break;
		}
	}*/

	if (ballBox.intersectsRect(padBox2)) {
		switch (dir) {
			OutputDebugString(L" HitBox2 ");
		case 1://right
			if ((angle >= 270 && angle <= 360))
				angle += 180;
			else if ((angle < 270 && angle >= 180))
				angle -= 180;
			break;
		case -1: //left
			if ((angle >= 270 && angle <= 360))
				angle += 90;
			else if ((angle < 270 && angle >= 180))
				angle -= 90;
			break;
		}
	}

	/*if (ballBox.intersectsRect(padBox3)) {
		OutputDebugString(L" HitBox3 ");
		switch (dir) {
		case 1://right
			if ((angle >= 270 && angle <= 360))
				angle -= 285;
			else if ((angle < 270 && angle >= 180))
				angle -= 215;
			break;
		case -1: //left
			if ((angle >= 270 && angle <= 360))
				angle += 145;
			else if ((angle < 270 && angle >= 180))
				angle -= 125;
			break;
		}

	}
		*/			
	if (ballBox.intersectsRect(wallLeft)) {
		if (angle >= 90 && angle <= 180)
			angle -= 90;
		else if (angle > 180 && angle < 270)
			angle += 90;
		OutputDebugString(L" HitLeft ");
	}

	else if (ballBox.intersectsRect(wallTop)) {
		if (angle>=90 && angle < 180)
		angle += 90;
		else if (angle >= 0 && angle < 90)
		angle -= 90;
		OutputDebugString(L" HitTop ");
	}

	else if (ballBox.intersectsRect(wallRight)) {
		if(angle>=0 && angle < 90)
		angle += 90;
		else if (angle >= 270 && angle < 360) angle -= 90;
		OutputDebugString(L" HitRight ");
	}
}

int HelloWorld::checkWallHit() {
	auto padBox = paddle->getBoundingBox();
	if (padBox.intersectsRect(wallLeft)) {
		return 1;
	}
	else if (padBox.intersectsRect(wallRight))
		return 2;
	else return 3;
}

void HelloWorld::update(float delta) {
	// Register an update function that checks to see if the CTRL key is pressed
	// and if it is displays how long, otherwise tell the user to press it
	Node::update(delta);
	checkBallHit();
	if (angle >= 360) angle -= 360;
	if (angle < 0) angle += 360;
	updateXY();
	move();

}
std::map<cocos2d::EventKeyboard::KeyCode,
	std::chrono::high_resolution_clock::time_point> HelloWorld::keys;