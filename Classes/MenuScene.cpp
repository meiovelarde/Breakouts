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
	audio->playBackgroundMusic("ddune.mp3", true);
	selecting = true;
	
	startLabel = Label::createWithSystemFont("Start", "Minecraft", 32);
	startLabel->setColor(ccc3(132, 174, 174));
	startLabel->setPosition(Vec2((visibleSize.width / 2 + 16), visibleSize.height / 3.5));
	

	exitLabel = Label::createWithSystemFont("Exit", "Minecraft", 32);
	exitLabel->setColor(ccc3(60, 38, 15));
	exitLabel->setPosition(Vec2((visibleSize.width / 2 + 16), visibleSize.height / 3.5 - (startLabel->getContentSize().height * 1.5)));

	
	this->addChild(startLabel, 36);
	this->addChild(exitLabel, 36);

	this->schedule(bgduler, 0.05f);

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
		unschedule(bgduler);
		
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
		startLabel->setColor(ccc3(132, 174, 174));
		exitLabel->setColor(ccc3(0, 0, 0));
		
	}
	else {
		exitLabel->setColor(ccc3(132, 174, 174));
		startLabel->setColor(ccc3(0, 0, 0));
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

