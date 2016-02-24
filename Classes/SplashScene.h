#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"  
class SplashScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	void transitionWithDelay(float delta);
	CREATE_FUNC(SplashScene);

private:
	cocos2d::Label* startLabel;
	cocos2d::SEL_SCHEDULE bgduler;
	int playing;
};