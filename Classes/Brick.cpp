#include "Brick.h"
#include <string>
#include<iostream>
#include "windows.h"
#include "time.h"
using namespace std;
USING_NS_CC;

Brick::Brick(cocos2d::Layer *layer, cocos2d::Vec2 position) {
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	wallTop = Sprite::create("bricktopbot.png");
	wallBot = Sprite::create("bricktopbot.png");
	wallLeft = Sprite::create("brickside.png");
	wallRight = Sprite::create("brickside.png");
	body = Sprite::create("brick.png");
	body->setPosition(position);
	cocos2d::Size bodySize = body->getContentSize();
	wallTop->setPosition(Vec2(position.x, position.y + bodySize.height/2 + 1));
	wallBot->setPosition(Vec2(position.x, position.y - bodySize.height/2));
	wallLeft->setPosition(Vec2(position.x - bodySize.width / 2, position.y));
	wallRight->setPosition(Vec2(position.x + bodySize.width / 2, position.y));
	layer->addChild(body, 20);
	layer->addChild(wallTop, 20);
	layer->addChild(wallBot, 20);
	layer->addChild(wallLeft, 20);
	layer->addChild(wallRight, 20);
}

Brick::~Brick() {
	wallTop->removeFromParentAndCleanup(true);
	wallBot->removeFromParentAndCleanup(true);
	wallLeft->removeFromParentAndCleanup(true);
	wallRight->removeFromParentAndCleanup(true);
	body->removeFromParentAndCleanup(true);
}


int Brick::checkWallsHit(cocos2d::Rect ball) {
	auto top = wallTop->getBoundingBox();
	auto bot = wallBot->getBoundingBox();
	auto left = wallLeft->getBoundingBox();
	auto right = wallRight->getBoundingBox();
	if (ball.intersectsRect(right))
		return 0;
	if (ball.intersectsRect(left))
		return 1;
	if (ball.intersectsRect(top))
		return 2;
	if (ball.intersectsRect(bot))
		return 3;
	
	return 4;
}



	/*std::ostringstream buffer;
	buffer << " X: " << randomX
		<< ", Y: " << randomY;
	std::wstring waw = s2ws(buffer.str());
	LPCWSTR result = waw.c_str();
	OutputDebugString(result);*/


std::wstring Brick::s2ws(const std::string& s)
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
