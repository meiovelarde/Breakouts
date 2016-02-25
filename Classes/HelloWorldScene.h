#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Brick.h"
#include <map>
using namespace std;

class HelloWorld : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	void update(float);
	CREATE_FUNC(HelloWorld);
	void restart(float);
	bool isKeyPressed(cocos2d::EventKeyboard::KeyCode);
	std::wstring s2ws(const std::string& s);

private:
	float gamespeed;
	cocos2d::SEL_SCHEDULE brekduler;
	cocos2d::SEL_SCHEDULE restarting;
	cocos2d::Sprite* wall;
	cocos2d::Sprite* wallTops;
	cocos2d::Sprite* paddle;
	cocos2d::Sprite* ball;
	std::vector<Brick*> bricks;
	static std::map<cocos2d::EventKeyboard::KeyCode,
		std::chrono::high_resolution_clock::time_point> keys;
	cocos2d::Label * label;
	cocos2d::Rect wallLeft;
	cocos2d::Rect wallTop;
	cocos2d::Rect wallRight;
	float bmX;
	float bmY;
	float angle;
	void move();
	int dir;
	int HelloWorld::checkWallHit();
	void HelloWorld::checkBallHit();
	void HelloWorld::paddleCD(float);
	void updateXY();
	boolean canHit;
};