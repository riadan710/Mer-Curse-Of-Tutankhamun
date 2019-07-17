#include "HelloWorldScene.h"
#include "Character.h"
#include "SimpleAudioEngine.h"
#include <time.h>
#include <math.h>

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}


bool HelloWorld::init()
{
	if (!Scene::init())
	{
		return false;
	}

	background = Background::create();
	this->addChild(background);

	background->start();

	character = Character::create();
	this->addChild(character);

	character->start();


	srand(time(NULL));


	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(HelloWorld::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(HelloWorld::onTouchesMoved, this);
	//listener->onTouchesCancelled = CC_CALLBACK_2(HelloWorld::onTouchesCancelled, this);
	listener->onTouchesEnded = CC_CALLBACK_2(HelloWorld::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);;

	 
    return true;
}

bool HelloWorld::onTouchesBegan(const std::vector<Touch*>&touches, Event* unused_event)
{
	for (auto iter = touches.begin(); iter != touches.end(); iter++) {

		Point touch = (*iter)->getLocation();

		IsTouched = true;

		//character->weapon();
		//character->rotateWeapon(touch);

		//character->changeweapon(touch);

		//character->move_character_if(touch);
		character->move_start();

		character->touchbegin(*iter);
	}

	return true;
}
void HelloWorld::onTouchesMoved(const std::vector<Touch*>&touches, Event* unused_event)
{
	for (auto iter = touches.begin(); iter != touches.end(); iter++) {

		Point touch = (*iter)->getLocation();

		//character->rotateWeapon(touch);

		character->touchmoved(*iter);
		//character->move_character_if(touch);
	}
	
}
/*void onTouchesCancelled(const std::vector<Touch*>&touches, Event* unused_event)
{

}*/
void HelloWorld::onTouchesEnded(const std::vector<Touch*>&touches, Event* unused_event)
{
	for (auto iter = touches.begin(); iter != touches.end(); iter++) {

		Point touch = (*iter)->getLocation();
		
		IsTouched = false;

		character->unweapon();

		character->move_end();

		character->touchended(*iter);
	}
}

void HelloWorld::Move_Camera()
{
	
}