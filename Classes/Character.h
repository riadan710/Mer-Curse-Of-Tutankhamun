#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "cocos2d.h"
#include "classes.h"
#include "Background.h"
#include "HelloWorldScene.h"

USING_NS_CC;

class Character : public Layer
{
public:

	virtual bool init();

	Background * background;
	Scene * helloworld;

	/////////  캐릭터  /////////
	Sprite * sprCharacter;

	int Characterhp = 100;
	int Charactermaxhp = 100;
	int tempitemcode;
	void itemapply(int tempitemcode);
	void update(float dt);
	////////////////////////////

	Sprite * SprItem;
	Sprite * sprWeapon;
	Sprite * sprhpoutline;
	Sprite * sprhpbar;

	int bulletrotate_pistol = 1;
	int bulletrotate_rifle = 10;

	int weapontype = 0;

	
	bool shotgun_ready = false; 
	bool bow_ready = false;

	Label * degreelabel;

	Vector<Sprite*> * bullet = new Vector<Sprite*>;

	Vec2 mtouch_place;

	double Degree;

	double mMoveX, mMoveY;
	double mBulletspeed = 1000;
	double mShotgun_Bulletspeed = 1500;
	double mBow_Bulletspeed = 100;
	double mfireball_Bulletspeed = 1000;
	
	double mCharX, mCharY;
	double mCharspeed = 5;


	void start();


	void weapon();
	void unweapon();
	void shootBullet_rifle(float dt);
	void shootBullet_shotgun(float dt);
	void shootBullet_bow(float dt);
	void shootBullet_fireball(float dt);

	void charge_fireball(float dt);
	void charge_bow(float dt);

	double charging = 1;

	void flying_bow(float dt);


	void remove_bullet(Ref * spr);
	void rotateWeapon(Touch * touch);
	void changeweapon(Touch * touch);
	void move_character_if();
	void move_character_else(Touch * touch);
	//void move_character(float dt);
	void move_start();
	void move_end();
	void minushp(double minushp);

	///////////////////////////////////////////////


	Sprite * sprWalkMonster;
	Sprite * sprFlyMonster;

	void Set_WalkMonster(float dt);
	void WalkMon_AttackChange(float dt);
	int walk_cnt = 1;

	void Set_FlyMonster(float dt);
	void FlyMon_AttackChange(float dt);
	void FlyMon_FlyChange(float dt);
	int fly_cnt = 1;
	int fly_cnt2 = 1;

	Vec2 sprLocation;
	Vec2 WalkMonLocation;
	Vec2 FlyMonLocation;

	float fly_rotate;
	float distanceTwoPoint;
	float fly_moveX;
	float fly_moveY;




	////////////////////////////////////////////////

	Sprite * wire;
	Sprite * hook;
	Sprite * weapon_joystick;
	Sprite * wire_chook[10];
	Sprite * ground[30];
	Sprite * joystick;
	Sprite * joysticksize;
	Sprite * wire_spike;
	int yap;
	float mWire_move_time;
	int mWire_chook_i;

	double mPlayer_gravity = 0;

	int wire_spike_already = 0;
	void abcd();

	int wire_touch = 0;
	float mWire = 0;
	void spike_follow_wire(float dt);
	void wire_ready(float dt);
	void wire_shoot(Touch * touch);
	void wire_to_wire(float dt);
	void wire_finish(float dt);
	void wire_gwansung();

	Vec2 before_wire_riding;
	Vec2 after_wire_riding;
	Vec2 middle_wire_riding;

	double wire_degree;

	Sprite * wire_joystick;

	int wire_touch_on = 0;
	int touch_over = 1;
	int wire_already_shoot = 0;

	float t;
	float x;
	float u;
	float y;



	void object_follow_player(float dt);



	void player_gravity(float dt);
	void wire_gwansung_ready(float dt);
	float aaaa = 0;
	float bbbb = 0;

	void touchbegin(Touch * touch);
	void touchmoved(Touch * touch);
	void touchended(Touch * touch);

	CREATE_FUNC(Character);
};

#endif // __CHARACTER_H__
