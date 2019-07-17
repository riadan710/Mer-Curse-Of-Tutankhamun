#include "Background.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include <ctime>
#include <cmath>

USING_NS_CC;

bool Background::init()
{
	if (!Layer::init())
	{
		return false;
	}
}

void Background::start()
{
	background = Sprite::create("background.png");
	background->setPosition(Point(960, 540));
	background->setScale(1.2);
	this->addChild(background, 0);
}