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
	canHit = 1;
	brekduler = schedule_selector(HelloWorld::update);
	restarting = schedule_selector(HelloWorld::restart);
	gamespeed = .05;
	this->schedule(brekduler, gamespeed); 
	auto eventListener = EventListenerKeyboard::create();

	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->preloadEffect("beep.mp3");

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
		visibleSize.height / 6 - origin.y - (2.8*ball->getContentSize().height - 40)));
	updateXY();


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

	for (int j = 0; j <= 3; j++) {
		for (int i = 0; i <= 6; i++) {
			bricks.push_back(new Brick(this, Vec2(visibleSize.width / 2 + i*(46), wallTops->getPosition().y - 80 - j*(26))));
			if (i != 0) {
				bricks.push_back(new Brick(this, Vec2(visibleSize.width / 2 - i*(46), wallTops->getPosition().y - 80 - j*(26))));
			}
		}
	}
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

void HelloWorld::paddleCD(float dt) {
	canHit = 1;
}

void HelloWorld::checkBallHit() {
	auto ballBox = ball->getBoundingBox();
	auto padPos = paddle->getPosition();
	auto padSize = paddle->getContentSize();
	int i;
	for (i = 0; i < bricks.size(); i++) {
		switch (bricks.at(i)->checkWallsHit(ballBox)) {
		case 0:
			OutputDebugString(L" Brick Hit Right ");
			if (angle >= 0 && angle < 90 || angle >= 90 && angle < 180)
				angle -= 90;
			else if (angle >= 270 && angle < 360 || angle > 180 && angle < 270)
				angle += 90;
			delete bricks.at(i);
			bricks.erase(bricks.begin() + i);
			break;
		case 1:
			if (angle >= 0 && angle < 90 || angle >= 90 && angle < 180)
				angle += 90;
			else if (angle >= 270 && angle < 360 || angle>= 180 && angle < 270) angle -= 90;
			OutputDebugString(L" Brick Hit Left ");
			delete bricks.at(i);
			bricks.erase(bricks.begin() + i);
			break;
		case 2:
			OutputDebugString(L" Brick Hit Top ");
			if (angle >= 180 && angle < 270)
				angle = 180 - (angle - 180);
			else if (angle >= 270 && angle < 360)
				angle = 180 - (360 - angle);
			delete bricks.at(i);
			bricks.erase(bricks.begin() + i);
			break;
		case 3:
			OutputDebugString(L" Brick Hit Bot ");
			if (angle >= 90 && angle < 180)
				angle = 180 - angle;
			else if (angle >= 0 && angle < 90)
				angle = 360 - angle;
			delete bricks.at(i);
			bricks.erase(bricks.begin() + i);
			break;
		case 4:
			break;
		}

	}

	auto padBox1 = Rect(padPos.x - padSize.width/2,
						padPos.y,
						padSize.width / 12,
						padSize.height
						);
	auto padBox2 =  Rect(padPos.x - padSize.width / 2 + padSize.width / 12,
						padPos.y,
						padSize.width / 12,
						padSize.height
						);
						//paddle->getBoundingBox();
	auto padBox3 = Rect(padPos.x - padSize.width / 2 + (2 * (padSize.width / 12)),
						padPos.y,
						padSize.width / 12,
						padSize.height
						);
	auto padBox4 = Rect(padPos.x - padSize.width / 2 + (3 * (padSize.width / 12)),
		padPos.y,
		padSize.width / 12,
		padSize.height
		);
	auto padBox5 = Rect(padPos.x - padSize.width / 2 + (4 * (padSize.width / 12)),
		padPos.y,
		padSize.width / 12,
		padSize.height
		);
	auto padBox6 = Rect(padPos.x - padSize.width / 2 + (5 * (padSize.width / 12)),
		padPos.y,
		padSize.width / 12,
		padSize.height
		);
	auto padBox7 = Rect(padPos.x - padSize.width / 2 + (6 * (padSize.width / 12)),
		padPos.y,
		padSize.width / 12,
		padSize.height
		);
	auto padBox8 = Rect(padPos.x - padSize.width / 2 + (7 * (padSize.width / 12)),
		padPos.y,
		padSize.width / 12,
		padSize.height
		);
	auto padBox9 = Rect(padPos.x - padSize.width / 2 + (8 * (padSize.width / 12)),
		padPos.y,
		padSize.width / 12,
		padSize.height
		);
	auto padBox10 = Rect(padPos.x - padSize.width / 2 + (9 * (padSize.width / 12)),
		padPos.y,
		padSize.width / 12,
		padSize.height
		);
	auto padBox11 = Rect(padPos.x - padSize.width / 2 + (10 * (padSize.width / 12)),
		padPos.y,
		padSize.width / 12,
		padSize.height
		);
	auto padBox12 = Rect(padPos.x - padSize.width / 2 + (11 * (padSize.width / 12)),
		padPos.y,
		padSize.width / 12,
		padSize.height
		);
	int hitArea;
	boolean hit = 0;
	if (canHit) {
		if (ballBox.intersectsRect(padBox1)) {
			hitArea = -6; hit = 1;
			OutputDebugString(L" Pad Hit 1 ");
		}
		else if (ballBox.intersectsRect(padBox2)) {
			hitArea = -5; hit = 1;
			OutputDebugString(L" Pad Hit 2 ");
		}
		else if (ballBox.intersectsRect(padBox3)) {
			hitArea = -4; hit = 1;
			OutputDebugString(L" Pad Hit 3 ");
		}
		else if (ballBox.intersectsRect(padBox4)) {
			hitArea = -3; hit = 1;
			OutputDebugString(L" Pad Hit 4 ");
		}
		else if (ballBox.intersectsRect(padBox5)) {
			hitArea = -2; hit = 1;
			OutputDebugString(L" Pad Hit 5 ");
		}
		else if (ballBox.intersectsRect(padBox6)) {
			hitArea = -1; hit = 1;
			OutputDebugString(L" Pad Hit 6 ");
		}
		else if (ballBox.intersectsRect(padBox7)) {
			hitArea = 1; hit = 1;
			OutputDebugString(L" Pad Hit 7 ");
		}
		else if (ballBox.intersectsRect(padBox8)) {
			hitArea = 2; hit = 1;
			OutputDebugString(L" Pad Hit 8 ");
		}
		else if (ballBox.intersectsRect(padBox9)) {
			hitArea = 3; hit = 1;
			OutputDebugString(L" Pad Hit 9 ");
		}
		else if (ballBox.intersectsRect(padBox10)) {
			hitArea = 4; hit = 1;
			OutputDebugString(L" Pad Hit 10 ");
		}
		else if (ballBox.intersectsRect(padBox11)) {
			hitArea = 5; hit = 1;
			OutputDebugString(L" Pad Hit 11 ");
		}
		else if (ballBox.intersectsRect(padBox12)) {
			hitArea = 6; hit = 1;
			OutputDebugString(L" Pad Hit 12 ");
		}
		else hit = 0;
		if (hit) {
			this->schedule(schedule_selector(HelloWorld::paddleCD), 0.5f);
			canHit = 0;
			auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
			audio->playEffect("beep.mp3", false);
			float normalizedHitArea = hitArea / 6.0;
			angle = (normalizedHitArea * 45) + 90;
			/*std::ostringstream buffer;
			buffer << " Angle: " << angle << " normalizedHitArea: " << normalizedHitArea;
			std::wstring waw = s2ws(buffer.str());
			LPCWSTR result = waw.c_str();
			OutputDebugString(result);*/
		}
	} 

	

	if (ballBox.intersectsRect(wallLeft)) {
		if (angle >= 90 && angle <= 180)
			angle = 180 - angle;
		else if (angle > 180 && angle < 270)
			angle = 180 + (360 - angle);
		OutputDebugString(L" HitLeft ");
	}

	else if (ballBox.intersectsRect(wallTop)) {
		if (angle>=90 && angle < 180)
		angle = 180 + angle;
		else if (angle >= 0 && angle < 90)
		angle = 360 - angle;
	}

	else if (ballBox.intersectsRect(wallRight)) {
		if(angle>=0 && angle < 90)
		angle = 180 - angle;
		else if (angle >= 270 && angle < 360) 
		angle = 180 + (360-angle);
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



std::wstring HelloWorld::s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
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