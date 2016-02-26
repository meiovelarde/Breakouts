#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"  
class MenuScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	void scrollBg(float);
	void menuCommand(cocos2d::EventKeyboard::KeyCode);
	void sel();
	void startGame(float);
	void exitGame(float);
	CREATE_FUNC(MenuScene);

private:
	cocos2d::Sprite* logo;
	cocos2d::Label* startLabel;
	cocos2d::Label* exitLabel;
	int selected;
	boolean selecting;
};