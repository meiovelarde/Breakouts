#pragma once
#include "cocos2d.h"
#include <string>
using namespace std;

class  Brick {

public:
	Brick(cocos2d::Layer *layer, cocos2d::Vec2 position);
	~Brick();
	std::wstring s2ws(const std::string& s);
	int checkWallsHit(cocos2d::Rect ball);
private:
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
	cocos2d::Sprite *wallTop;
	cocos2d::Sprite *wallBot;
	cocos2d::Sprite *wallLeft;
	cocos2d::Sprite *wallRight;
	cocos2d::Sprite *body;
};