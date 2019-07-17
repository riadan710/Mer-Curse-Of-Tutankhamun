#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__

#include "cocos2d.h"

#include "classes.h"

USING_NS_CC;

class Background : public Layer
{
public:
	
	virtual bool init();

	void start();

	Sprite * background;

	double bgmovedx;
	double bgmovedy;

	void rm_background();


	CREATE_FUNC(Background);
};

#endif // __BACKGROUND_H__
