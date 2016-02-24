#include "SplashScene.h"
#include "stdlib.h"
#include "HelloWorldScene.h"
USING_NS_CC;
using namespace std;

Scene* SplashScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	auto layer = SplashScene::create();
	scene->addChild(layer);
	return scene;
}

bool SplashScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playEffect("1up.wav", false);
	startLabel = Label::createWithSystemFont("by Meio", "Minecraft", 32);
	startLabel->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height/2 + origin.y));
	this->addChild(startLabel);
	this->schedule(schedule_selector(SplashScene::transitionWithDelay), 2);
	return true;
}

void SplashScene::transitionWithDelay(float delta) {
	auto scene = HelloWorld::createScene();
	auto director = Director::getInstance();
	director->replaceScene(scene);
}
