#include "MenuScene.h"
#include "stdlib.h"
#include "HelloWorldScene.h"
USING_NS_CC;
using namespace std;

Scene* MenuScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	auto layer = MenuScene::create();
	scene->addChild(layer);
	return scene;
}

bool MenuScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("pinball.wav", true);
	selecting = true;

	auto bg = cocos2d::LayerColor::create(Color4B(146, 174, 38, 255));
	this->addChild(bg);
	
	auto wall = cocos2d::CCSprite::create("wall.png");
	wall->setPosition(Vec2(0, 0));
	wall->setAnchorPoint(Vec2(0, 0));
	this->addChild(wall);
	
	startLabel = Label::createWithSystemFont("Start", "Minecraft", 32);
	startLabel->setColor(ccc3(100, 119, 30));
	startLabel->setPosition(Vec2((visibleSize.width / 2 + 16), visibleSize.height / 3.5));
	

	exitLabel = Label::createWithSystemFont("Exit", "Minecraft", 32);
	exitLabel->setColor(ccc3(76, 81, 19));
	exitLabel->setPosition(Vec2((visibleSize.width / 2 + 16), visibleSize.height / 3.5 - (startLabel->getContentSize().height * 1.5)));

	logo = CCSprite::create("logo.png");
	logo->setPosition(Vec2(visibleSize.width / 2 + logo->getContentSize().width*.030,
		visibleSize.height / 2 + logo->getContentSize().height));
	
	this->addChild(startLabel, 36);
	this->addChild(exitLabel, 36);
	this->addChild(logo);

	selected = 0;
	auto eventListener = EventListenerKeyboard::create();
	eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		 if(selecting) menuCommand(keyCode); 
	};
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);


	return true;
}

void MenuScene::menuCommand(EventKeyboard::KeyCode code) {
	switch (code) {
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
	case EventKeyboard::KeyCode::KEY_W:
		if (selected) {
			sel();
		}
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	case EventKeyboard::KeyCode::KEY_S:
		if (!selected) {
			sel();
		}
		break;
	case EventKeyboard::KeyCode::KEY_ENTER:
		selecting = false;
		auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
		audio->playEffect("menu3.wav", false);
		
		if (selected) {
			this->schedule(schedule_selector(MenuScene::exitGame), 1.5);
		}
		else {
			this->schedule(schedule_selector(MenuScene::startGame), 1.5);
		}
		break;
	}
}

void MenuScene::sel() {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playEffect("menu2.wav", false);

	if (selected) {
		selected = 0;
		startLabel->setColor(ccc3(100, 119, 30));
		exitLabel->setColor(ccc3(76, 81, 19));
		
	}
	else {
		exitLabel->setColor(ccc3(100, 119, 30));
		startLabel->setColor(ccc3(76, 81, 19));
		selected = 1;
		
	}
}

void MenuScene::startGame(float dt) {
	auto director = Director::getInstance();
	auto scene2 = HelloWorld::createScene();
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->stopBackgroundMusic();
	director->replaceScene(scene2);

}

void MenuScene::exitGame(float dt) {
	auto director = Director::getInstance();
	director->end();
}

