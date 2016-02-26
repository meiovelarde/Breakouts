#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "MenuScene.h"
#include "SplashScene.h"
USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching() {
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if (!glview) {
		glview = GLViewImpl::create("Brick Out!");
		glview->setFrameSize(640, 480);
		director->setOpenGLView(glview);
	}

	//auto scene = HelloWorld::createScene();
	//auto scene = MenuScene::createScene();
	//auto scene = IntroScene::createScene();
	auto scene = SplashScene::createScene();
	director->runWithScene(scene);
	return true;
}

void AppDelegate::applicationDidEnterBackground() {
}

void AppDelegate::applicationWillEnterForeground() {
}