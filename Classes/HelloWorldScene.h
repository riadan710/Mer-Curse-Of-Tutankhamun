#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "classes.h"
#include "Character.h"
#include "Background.h"

USING_NS_CC;

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();


	bool IsTouched = false;


	virtual bool onTouchesBegan(const std::vector<Touch*>&touches, Event* unused_event);
	virtual void onTouchesMoved(const std::vector<Touch*>&touches, Event* unused_event);
	//virtual void onTouchesCancelled(const std::vector<Touch*>&touches, Event* unused_event);
	virtual void onTouchesEnded(const std::vector<Touch*>&touches, Event *unused_event);


	void Move_Camera();

	void runschedule();
	void stopschedule();

	Character * character;
	Background * background;

	
	CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
