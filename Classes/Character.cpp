#include "Character.h"
#include "SimpleAudioEngine.h"
#include <ctime>
#include <cmath>

USING_NS_CC;

bool Character::init()
{
	if (!Layer::init())
	{
		return false;
	}
}

void Character::start()
{
	sprCharacter = Sprite::create("Character_StandLeft.png");
	sprCharacter->setPosition(Point(960, 540));
	sprCharacter->setScale(0.15);
	this->addChild(sprCharacter);

	sprWeapon = Sprite::create("gun.png");
	sprWeapon->setAnchorPoint(Point(0.4, 0.45));
	sprWeapon->setPosition(Point(960, 500));
	//sprWeapon->setScale(0.7);
	this->addChild(sprWeapon);

	sprhpoutline = Sprite::create("hpoutline.png");
	sprhpoutline->setAnchorPoint(Point(0, 0.5));
	sprhpoutline->setPosition(Point(70, 980));
	this->addChild(sprhpoutline);

	sprhpbar = Sprite::create("hpbar.png");
	sprhpbar->setAnchorPoint(Point(0.07, 0.5));
	sprhpbar->setPosition(Point(105, 980));
	this->addChild(sprhpbar);

	degreelabel = Label::createWithSystemFont("Degree", "Arial", 50);
	degreelabel->setPosition(Point(1700, 800));
	this->addChild(degreelabel);

	background = Background::create();
	helloworld = Director::getInstance()->getRunningScene();


	/////////////////////////////////////////////////////////////////////////////
	wire_joystick = Sprite::create("boxEmpty.png");
	wire_joystick->setPosition(Point(1700, 500));
	wire_joystick->setAnchorPoint(Point(0.5, 0.5));
	this->addChild(wire_joystick);

	for (int i = 0; i < 30; i++) {
		ground[i] = Sprite::create("bardak.png");
		ground[i]->setPosition(Point(i * 100, 0));
		ground[i]->setAnchorPoint(Point(0, 0));
		this->addChild(ground[i]);
	}


	joystick = Sprite::create("cur.png");
	joystick->setPosition(Point(1400, 200));
	joystick->setAnchorPoint(Point(0.5, 0.5));
	joystick->setScale(0.5);
	joystick->setVisible(false);
	this->addChild(joystick);

	weapon_joystick = Sprite::create("boxItemAlt.png");
	weapon_joystick->setPosition(Point(1400, 200));
	weapon_joystick->setAnchorPoint(Point(0.5, 0.5));
	this->addChild(weapon_joystick);

	for (int i = 0; i < 10; i++) {
		wire_chook[i] = Sprite::create("boxCoinAlt.png");
		wire_chook[i]->setPosition(Point(i * 200 + 50, 500 + i * 100));
		wire_chook[i]->setAnchorPoint(Point(0.5, 0.5));
		this->addChild(wire_chook[i]);
	}

	joysticksize = Sprite::create("joystick.png");
	joysticksize->setPosition(Point(1700, 500));
	joysticksize->setAnchorPoint(Point(0.5, 0.5));
	this->addChild(joysticksize);



	wire_spike = Sprite::create("spikes.png");
	wire_spike->setPosition(Point(960, 540));
	wire_spike->setAnchorPoint(Point(0.5, 0));

	this->addChild(wire_spike);
	wire_spike->setVisible(false);


	wire = Sprite::create("wire.png");
	wire->setPosition(Point(960, 500));
	wire->setAnchorPoint(Point(0, 0.5));
	this->addChild(wire);
	wire->setScaleX(mWire);

	schedule(schedule_selector(Character::object_follow_player), 0.001f);
	schedule(schedule_selector(Character::spike_follow_wire), 0.001f);
	schedule(schedule_selector(Character::player_gravity), 0.01f);

	/////////////////////////////////////////////////////////////////////////////

	int randX = rand() % 1500 + 100;

	sprWalkMonster = Sprite::create("monster1_1.png");
	sprWalkMonster->setPosition(Point(randX, 225));
	sprWalkMonster->setScale(0.25);
	sprWalkMonster->setScaleX(0.28);
	this->addChild(sprWalkMonster, 1);

	randX = rand() % 1500 + 100;
	int randY = rand() % 300 + 700;

	sprFlyMonster = Sprite::create("monster2_1.png");
	sprFlyMonster->setPosition(Point(randX, randY));
	sprFlyMonster->setScale(0.4);
	this->addChild(sprFlyMonster, 2);

	this->schedule(schedule_selector(Character::Set_WalkMonster), 0.1f);
	this->schedule(schedule_selector(Character::Set_FlyMonster), 0.1f);
	this->schedule(schedule_selector(Character::FlyMon_FlyChange), 0.1f);
}

void Character::weapon()
{
	switch (weapontype)
	{
		case 0:
		{
			schedule(schedule_selector(Character::shootBullet_rifle), 0.1f);
			break;
		}
		case 1:
		{
			shotgun_ready = true;
			break;
		}
		case 2:
		{
			bow_ready = true;
			mBow_Bulletspeed = 100;
			schedule(schedule_selector(Character::charge_bow), 0);
			break;
		}
		case 3:
		{
			charging = 1;
			schedule(schedule_selector(Character::charge_fireball), 0);
			break;
		}
	}
}

void Character::unweapon()
{
	switch (weapontype)
	{
		case 0:
		{
			unschedule(schedule_selector(Character::shootBullet_rifle));
			break;
		}
		case 1:
		{
			if (shotgun_ready)
			{
				scheduleOnce(schedule_selector(Character::shootBullet_shotgun), 0);
				shotgun_ready = false;
			}
			break;
		}
		case 2:
		{
			if (bow_ready)
			{
				bow_ready = false;
			}
			break;
		}
		case 3:
		{
			scheduleOnce(schedule_selector(Character::shootBullet_fireball), 0.1f);
			unschedule(schedule_selector(Character::charge_fireball));
			sprWeapon->setScale(0);
			break;
		}
	}
}


void Character::shootBullet_rifle(float dt)
{
	Sprite * sprBullet = Sprite::create("bullet.png");
	sprBullet->setPosition(Point(sprWeapon->getPositionX(), sprWeapon->getPositionY()));
	sprBullet->setRotation(Degree);
	sprBullet->setScale(0.5);
	this->addChild(sprBullet);

	int mbulletrandrot = rand() % bulletrotate_rifle - bulletrotate_rifle / 2;

	mMoveX = mBulletspeed * cos((-Degree + mbulletrandrot) * 3.1416 / 180);
	mMoveY = mBulletspeed * sin((-Degree + mbulletrandrot) * 3.1416 / 180);

	MoveBy * bulletmvb = MoveBy::create(1, Point(mMoveX, mMoveY));
	sprBullet->runAction(RepeatForever::create(bulletmvb));

	bullet->pushBack(sprBullet);

	sprWeapon->stopAllActions(); 

	Vec2 char_pos = weapon_joystick->getPosition();

	sprWeapon->setPosition(Point(sprWeapon->getPositionX() - mMoveX / 40, sprWeapon->getPositionY() - mMoveY / 40));
	MoveTo * knife_mvt = MoveTo::create(0.4, Point(char_pos.x, char_pos.y));
	sprWeapon->runAction(knife_mvt);


	/*
	sprCharacter->setPosition(Point(sprCharacter->getPositionX() - mMoveX / 40, sprCharacter->getPositionY() - mMoveY / 40));
	MoveTo * character_mvt = MoveTo::create(0.09, Point(960, 540));
	sprCharacter->runAction(character_mvt);
	*/

	Character::minushp(0.005);
}

void Character::shootBullet_shotgun(float dt) // 샷건 ( 탄속:mShotgun_Bulletspeed, 랜덤 각도: Random_degree1,Random_degree2)
{
   for (int i = 1; i <= 8; i++)
   {
      Sprite * sprBullet = Sprite::create("shotgun_bullet.png");
      int RD = (4 - i) * 2 + (rand() % 8 - 4);
      sprBullet->setPosition(Point(sprCharacter->getPositionX(), sprCharacter->getPositionY() + RD));
      sprBullet->setRotation(Degree + RD);
      sprBullet->setScale(0.6);
      this->addChild(sprBullet);

      mMoveX = cos((-Degree + RD) * 3.1416 / 180);
      mMoveY = sin((-Degree + RD) * 3.1416 / 180);

      MoveBy * bulletmvb = MoveBy::create(1 / (mShotgun_Bulletspeed + rand() % 250), Point(mMoveX, mMoveY));
      sprBullet->runAction(Sequence::create(Repeat::create(bulletmvb, 600), RemoveSelf::create(), NULL));
      bullet->pushBack(sprBullet);
   }
   Character::minushp(0.05);
}


void Character::rotateWeapon(Touch * touch)
{
	mtouch_place = touch->getLocation();

	Degree = 180 - atan2f((double)weapon_joystick->getPositionY() - mtouch_place.y,
		(double)weapon_joystick->getPositionX() - mtouch_place.x) * 180 / 3.1415f;

	degreelabel->setString(std::to_string(Degree));

	sprWeapon->setRotation(Degree);

	if (Degree >= 90 && Degree <= 270)
		sprWeapon->setFlippedY(true);
	else
		sprWeapon->setFlippedY(false);


	Character::move_character_if();
}

void Character::minushp(double minushp)
{
	if (sprhpbar->getScaleX() - minushp > 0)
		sprhpbar->setScaleX(sprhpbar->getScaleX() - minushp);
	else
		sprhpbar->setScaleX(0);
}

void Character::move_character_if()
{
	mCharX = mCharspeed * cos((-Degree) * 3.1416 / 180);
	mCharY = mCharspeed * sin((-Degree) * 3.1416 / 180);
}
void Character::move_character_else(Touch * touch)
{
	
}
/*
void Character::move_character(float dt)
{
	sprCharacter->setPosition(Point(sprCharacter->getPositionX() + mCharX, sprCharacter->getPositionY() + mCharY));
	Director::getInstance()->getRunningScene()->getDefaultCamera()->setPosition(Point(sprCharacter->getPositionX(), sprCharacter->getPositionY()));
}
*/

void Character::move_start()
{
	//schedule(schedule_selector(Character::move_character), 0.01f);
}
void Character::move_end()
{
	//unschedule(schedule_selector( acter::move_character));
}


void Character::itemapply(int tempitemcode) // 아이템 코드에 따라 적용 효과는 후에 추가
{
	switch (tempitemcode)
	{
	case 1:
		minushp(50);
		break;
	case 2:
		break;


	default:
		break;
	}
}

void Character::update(float dt) // 아이템 떨구면 실행 스케줄러 아이뎀 코드는 tempitemcode에 저장 아이템은 맵당1만
{
	/*SprItem = Sprite::create("아이템 이름");
	SprItem->setPosition(몬스터 위치);
	SprItem->setAnchorPoint(Point(0.5, 0));
	this->addChild(SprItem);*///몬스터 죽을떄 확률적으로 아이템 랜덤생성
	if (sprCharacter->getBoundingBox().intersectsRect(SprItem->getBoundingBox()))
	{
		itemapply(tempitemcode);
		SprItem->runAction(RemoveSelf::create());
		unschedule(schedule_selector(Character::update));

	}

}


void Character::charge_bow(float dt)
{
	if (mBow_Bulletspeed < 2000) mBow_Bulletspeed += 100;
	if (bow_ready == false)
	{
		scheduleOnce(schedule_selector(Character::shootBullet_bow), 0.1f);
		unschedule(schedule_selector(Character::charge_bow));
	}
}

void Character::flying_bow(float dt)
{

}

void Character::shootBullet_bow(float dt)
{
	Sprite * sprBullet = Sprite::create("arrow.png");
	sprBullet->setPosition(Point(sprCharacter->getPositionX(), sprCharacter->getPositionY()));
	sprBullet->setRotation(Degree);
	sprBullet->setScale(1);
	mMoveX = cos((-Degree) * 3.1416 / 180);
	mMoveY = sin((-Degree) * 3.1416 / 180);
	MoveBy * bulletmvb = MoveBy::create(1 / mBow_Bulletspeed, Point(mMoveX, mMoveY));
	sprBullet->runAction(Sequence::create(Repeat::create(bulletmvb, 750), RemoveSelf::create(), NULL));

	this->addChild(sprBullet);

	//sprBullet->runAction(Sequence::create(Repeat::create(bulletmvb, 750), RemoveSelf::create(), NULL));
	bullet->pushBack(sprBullet);
}

void Character::charge_fireball(float dt)
{
	if (charging < 10)
	{
		charging += 0.05;
		sprWeapon->setScale(charging / 5);
	}
}

void Character::shootBullet_fireball(float dt)
{
	Sprite * sprBullet = Sprite::create("fireball.png");
	sprBullet->setPosition(Point(sprCharacter->getPositionX(), sprCharacter->getPositionY()));
	sprBullet->setRotation(Degree);
	sprBullet->setScale(charging);
	this->addChild(sprBullet);

	mMoveX = cos((-Degree) * 3.1416 / 180);
	mMoveY = sin((-Degree) * 3.1416 / 180);
	MoveBy * bulletmvb = MoveBy::create(1 / (mfireball_Bulletspeed / charging), Point(mMoveX, mMoveY));
	sprBullet->runAction(Sequence::create(Repeat::create(bulletmvb, 750), RemoveSelf::create(), NULL));
}

void Character::changeweapon(Touch * touch)
{
	mtouch_place = touch->getLocation();
	auto hprect = sprhpoutline->getBoundingBox();
	if (hprect.containsPoint(mtouch_place))
	{
		Character::unweapon();	
		weapontype++;
		if (weapontype == 4) weapontype = 0;
		switch (weapontype)
		{
		case 0: case 1:
			{
				sprWeapon->setScale(0.7);
				sprWeapon->setVisible(true);
				sprWeapon->setTexture("gun.png");
				break;
			}
		case 2:
			{
				sprWeapon->setScale(0.5);
				sprWeapon->setTexture("bow.png");
				break;
			}
		case 3:
			{
				sprWeapon->setTexture("fireball.png");
				sprWeapon->setScale(0);
				break;
			}
		}
	}
}




void Character::player_gravity(float dt)
{
	for (int i = 0; i < 30; i++) {
		auto ground_rect = ground[i]->getBoundingBox();
		mPlayer_gravity -= 0.002;


		if (ground_rect.containsPoint(Point(sprCharacter->getPosition().x, sprCharacter->getPosition().y + mPlayer_gravity - 60)))
		{
			unschedule(schedule_selector(Character::wire_gwansung_ready));
			mPlayer_gravity = 0;
			break;
		}
	}

	sprCharacter->setPositionY(sprCharacter->getPosition().y + mPlayer_gravity);


}

void Character::wire_shoot(Touch * touch)
{
	if (wire_already_shoot == 0) {
		wire_degree = 180 - atan2f((double)wire_joystick->getPositionY() - mtouch_place.y, (double)wire_joystick->getPositionX() - mtouch_place.x) * 180 / 3.1415f;
		wire->setRotation(wire_degree);
		if (wire_touch == 1 && touch_over == 0 && wire_already_shoot == 0)
		{
			mWire = 0;
			wire_touch = 0;
			wire_already_shoot = 1;
			schedule(schedule_selector(Character::wire_ready), 0.005f);
			schedule(schedule_selector(Character::wire_to_wire), 0.001f);
		}
	}

}
void Character::object_follow_player(float dt)
{
	sprWeapon->setPosition(Point(sprCharacter->getPosition().x, sprCharacter->getPosition().y));
	wire->setPosition(Point(sprCharacter->getPosition().x, sprCharacter->getPosition().y));;

}

void Character::spike_follow_wire(float dt) {
	wire_spike->setRotation(wire_degree + 90);
	if (wire_already_shoot == 1 && wire_spike_already == 0) {

		if (wire_degree >= 270 && wire_degree <= 360 || wire_degree >= 0 && wire_degree <= 89.999) {
			wire_spike->setPositionX(wire->getBoundingBox().getMaxX());
		}
		else wire_spike->setPositionX(wire->getBoundingBox().getMinX());
		if (wire_degree >= 180 && wire_degree <= 359.99999) {
			wire_spike->setPositionY(wire->getBoundingBox().getMaxY());
		}
		else wire_spike->setPositionY(wire->getBoundingBox().getMinY());
	}
}


void Character::wire_to_wire(float dt)
{
	for (int i = 0; i < 10; i++) {
		auto wire_rect = wire_chook[i]->getBoundingBox();
		if (wire_rect.containsPoint(Point(wire_spike->getPosition().x, wire_spike->getPosition().y)) && wire_spike->isVisible())
		{
			unschedule(schedule_selector(Character::wire_gwansung_ready));
			mWire_chook_i = i;
			unschedule(schedule_selector(Character::spike_follow_wire));
			mWire_move_time = mWire;
			unschedule(schedule_selector(Character::player_gravity));
			mPlayer_gravity = 0;
			middle_wire_riding = wire_chook[i]->getPosition();
			sprCharacter->runAction(Sequence::create(MoveTo::create(mWire_move_time / 2, Point(wire_spike->getPosition().x, wire_spike->getPosition().y)), CallFunc::create(CC_CALLBACK_0(Character::wire_gwansung, this)), NULL));
			schedule(schedule_selector(Character::wire_finish), 0.005f);
			unschedule(schedule_selector(Character::wire_to_wire));
			unschedule(schedule_selector(Character::wire_ready));
		}
	}
}


void Character::wire_gwansung()
{

	after_wire_riding.x = (3 * (sprCharacter->getPosition().x) - before_wire_riding.x) / 2 - middle_wire_riding.x;
	after_wire_riding.y = (3 * (sprCharacter->getPosition().y) - before_wire_riding.y) / 2 - middle_wire_riding.y;
	sprCharacter->runAction(Sequence::create(/*EaseOut::create(MoveBy::create(mWire_move_time/2, Point(after_wire_riding.x, after_wire_riding.y )),1)
		,*/ CallFunc::create(CC_CALLBACK_0(Character::abcd, this)), NULL));
}

void Character::abcd()
{
	schedule(schedule_selector(Character::wire_gwansung_ready), 0.01f);
	schedule(schedule_selector(Character::player_gravity), 0.01f);
}

void Character::wire_gwansung_ready(float dt) {


	sprCharacter->setPosition(Point(sprCharacter->getPosition().x + after_wire_riding.x / (mWire_move_time * 100)
		, sprCharacter->getPosition().y + 1.3*(after_wire_riding.y / (mWire_move_time * 100))));
	int seeballlseex = sprCharacter->getPosition().x;
	int seeballlseey = sprCharacter->getPosition().y;
	int a = 0;
}

void Character::wire_ready(float dt)
{

	if (mWire > 0.05)wire_spike->setVisible(true);
	if (mWire < 1.0) {
		mWire += 0.01;

	}
	else {
		mWire = 0.0;
		wire->setScaleX(mWire);
		unschedule(schedule_selector(Character::wire_ready));
		wire_already_shoot = 0;
		unschedule(schedule_selector(Character::wire_to_wire));
		wire_spike->setVisible(false);
	}

	wire->setScaleX(mWire);
}


void Character::wire_finish(float dt)
{
	wire->setAnchorPoint(Point(0, 1));
	wire_spike_already = 1;
	if (mWire > 0.0) {
		mWire -= 0.01;
	}
	else {
		mWire = 0.0;
		wire->setScaleX(mWire);
		unschedule(schedule_selector(Character::wire_finish));
		wire_already_shoot = 0;
		schedule(schedule_selector(Character::spike_follow_wire), 0.01f);

		wire_spike->setVisible(false);
		wire_spike_already = 0;
	}

	wire->setScaleX(mWire);
}








///////////////// woo shin ontouch////////////////////////

void Character::touchbegin(Touch * touch)
{
	touch_over = 1;
	mtouch_place = touch->getLocation();

	if ((mtouch_place.x - 1400)*(mtouch_place.x - 1400) + (mtouch_place.y - 200)*(mtouch_place.y - 200) <= 40000) {

		Character::weapon();
		Character::rotateWeapon(touch);
	}
	Character::changeweapon(touch);

	if (wire_already_shoot == 0 && (mtouch_place.x - 1700)*(mtouch_place.x - 1700) + (mtouch_place.y - 500)*(mtouch_place.y - 500) <= 40000)
	{
		wire_touch = 1;
		wire->setScaleX(1.0);
		joystick->setVisible(true);
		joystick->setPosition(mtouch_place);
	}

	Vec2 location = touch->getLocation();
	Vec2 CharLocation = sprCharacter->getPosition();
	if (location.x < CharLocation.x) //����
	{
		sprCharacter->setFlippedX(true);
		sprWeapon->setFlippedY(false);
		sprWeapon->setPosition(945, 395);
		sprWeapon->setRotation(180);
		auto actact1 = RotateTo::create(0.3, 270);
		auto actact2 = RotateTo::create(0.2, 90);
		auto actact3 = Sequence::create(actact1, actact2, NULL);
		sprWeapon->runAction(actact3);
	}
	else //������
	{
		sprCharacter->setFlippedX(false);
		sprWeapon->setFlippedY(true);
		sprWeapon->setPosition(1015, 395);
		sprWeapon->setRotation(0);
		auto actact1 = RotateTo::create(0.3, 270);
		auto actact2 = RotateTo::create(0.2, 90);
		auto actact3 = Sequence::create(actact1, actact2, NULL);
		sprWeapon->runAction(actact3);
	}

}

void Character::touchmoved(Touch * touch)
{
	mtouch_place = touch->getLocation();
	if ((mtouch_place.x - 1400)*(mtouch_place.x - 1400) + (mtouch_place.y - 200)*(mtouch_place.y - 200) <= 40000)
	{
		Character::rotateWeapon(touch);
	}
	if (wire_already_shoot == 0 && wire_touch == 1)
	{
		Character::wire_shoot(touch);
		if ((mtouch_place.x - 1700)*(mtouch_place.x - 1700) + (mtouch_place.y - 500)*(mtouch_place.y - 500) <= 40000)
			joystick->setPosition(mtouch_place);
		else
		{
			t = ((mtouch_place.y - 500)*(mtouch_place.y - 500)) / ((mtouch_place.x - 1700)*(mtouch_place.x - 1700)) + 1;
			u = sqrt(t*t * 3400 * 3400 - 4 * t*(1700 * 1700 * t - 40000)) / (2 * t);
			if (mtouch_place.x > 1700)
			{
				x = 1700 + u;
				y = ((mtouch_place.y - 500) / (mtouch_place.x - 1700))*(x - 1700) + 500;
			}
			else
			{
				x = 1700 - u;
				y = ((mtouch_place.y - 500) / (mtouch_place.x - 1700))*(x - 1700) + 500;
			}
			joystick->setPosition(Point(x, y));
		}
	}
}

void Character::touchended(Touch * touch)
{
	touch_over = 0;

	if (wire_already_shoot == 0) {
		Character::wire_shoot(touch);
		before_wire_riding = sprCharacter->getPosition();
	}
	Character::unweapon();
	joystick->setVisible(false);
}


void Character::Set_WalkMonster(float dt)
{
	sprLocation = sprCharacter->getPosition();
	WalkMonLocation = sprWalkMonster->getPosition();
	int gap = abs(sprLocation.y - WalkMonLocation.y);

	if (sprLocation.x > WalkMonLocation.x)
	{
		sprWalkMonster->setFlippedX(true);
		if (sprLocation.x - WalkMonLocation.x < 130 && gap < 100)
		{
			unschedule(schedule_selector(Character::Set_WalkMonster));
			schedule(schedule_selector(Character::WalkMon_AttackChange), 0.2f);
		}
		else
		{
			auto act1 = MoveBy::create(0.1, Point(20, 0));
			sprWalkMonster->runAction(act1);
		}
	}
	else
	{
		sprWalkMonster->setFlippedX(false);
		if (WalkMonLocation.x - sprLocation.x < 130 && gap < 100)
		{
			unschedule(schedule_selector(Character::Set_WalkMonster));
			schedule(schedule_selector(Character::WalkMon_AttackChange), 0.2f);
		}
		else
		{
			auto act1 = MoveBy::create(0.1, Point(-20, 0));
			sprWalkMonster->runAction(act1);
		}
	}
}

void Character::WalkMon_AttackChange(float dt)
{
	sprLocation = sprCharacter->getPosition();
	WalkMonLocation = sprWalkMonster->getPosition();
	int gap = abs(sprLocation.y - WalkMonLocation.y);
	if (gap > 100)
	{
		unschedule(schedule_selector(Character::WalkMon_AttackChange));
		schedule(schedule_selector(Character::Set_WalkMonster), 0.1f);
	}

	if (walk_cnt == 1)
		sprWalkMonster->setTexture("monster1_4.png");
	else if (walk_cnt == 2)
		sprWalkMonster->setTexture("monster1_5.png");
	else if (walk_cnt == 3)
		sprWalkMonster->setTexture("monster1_6.png");
	else if (walk_cnt == 4)
		sprWalkMonster->setTexture("monster1_5.png");

	walk_cnt++;

	if (walk_cnt > 4)
		walk_cnt = 1;
}

void Character::Set_FlyMonster(float dt)
{
	sprLocation = sprCharacter->getPosition();
	FlyMonLocation = sprFlyMonster->getPosition();
	int gap = sprLocation.x - FlyMonLocation.x;
	if (gap > 0)
		sprFlyMonster->setFlippedX(true);

	if (sprLocation.x > FlyMonLocation.x)
	{
		if (sprLocation.y > FlyMonLocation.y)
			fly_rotate = atan2f((double)sprLocation.y - FlyMonLocation.y, (double)sprLocation.x - FlyMonLocation.x) * 180 / 3.1415f + 270;
		else
			fly_rotate = atan2f((double)FlyMonLocation.y - sprLocation.y, (double)sprLocation.x - FlyMonLocation.x) * 180 / 3.1415f;
	}
	else
	{
		if (sprLocation.y > FlyMonLocation.y)
			fly_rotate = atan2f((double)sprLocation.y - FlyMonLocation.y, (double)sprLocation.x - FlyMonLocation.x) * 180 / 3.1415f + 90;
		else
			fly_rotate = atan2f((double)FlyMonLocation.y - sprLocation.y, (double)sprLocation.x - FlyMonLocation.x) * 180 / 3.1415f;
	}

	fly_moveX = 15 * cos((-fly_rotate) * 3.1416 / 180);
	fly_moveY = 15 * sin((-fly_rotate) * 3.1416 / 180);

	distanceTwoPoint = sprLocation.getDistance(FlyMonLocation);

	auto act2 = MoveBy::create(0.1, Point(fly_moveX, fly_moveY));
	sprFlyMonster->runAction(act2);
	if (distanceTwoPoint < 400)
	{
		this->unschedule(schedule_selector(Character::Set_FlyMonster));
		this->unschedule(schedule_selector(Character::FlyMon_FlyChange));
		this->schedule(schedule_selector(Character::FlyMon_AttackChange), 0.2f);
	}
}

void Character::FlyMon_FlyChange(float dt)
{
	switch (fly_cnt)
	{
	case 1:
		sprFlyMonster->setTexture("Monster2_1.png");
		break;
	case 2:
		sprFlyMonster->setTexture("Monster2_2.png");
		break;
	case 3:
		sprFlyMonster->setTexture("Monster2_3.png");
		break;
	case 4:
		sprFlyMonster->setTexture("Monster2_4.png");
		break;
	case 5:
		sprFlyMonster->setTexture("Monster2_5.png");
		break;
	case 6:
		sprFlyMonster->setTexture("Monster2_4.png");
		break;
	case 7:
		sprFlyMonster->setTexture("Monster2_3.png");
		break;
	case 8:
		sprFlyMonster->setTexture("Monster2_2.png");
		break;
	default:
		fly_cnt = 0;
	}
	fly_cnt++;
}

void Character::FlyMon_AttackChange(float dt)
{
	distanceTwoPoint = sprLocation.getDistance(FlyMonLocation);

	if (distanceTwoPoint > 400)
	{
		this->schedule(schedule_selector(Character::Set_FlyMonster), 0.1f);
		this->schedule(schedule_selector(Character::FlyMon_FlyChange), 0.1f);
		this->unschedule(schedule_selector(Character::FlyMon_AttackChange));
	}
	switch (fly_cnt2)
	{
	case 1:
		sprFlyMonster->setTexture("monster2_6.png");
		break;
	case 2:
		sprFlyMonster->setTexture("monster2_7.png");
		break;
	case 3:
		sprFlyMonster->setTexture("monster2_8.png");
		break;
	case 4:
		sprFlyMonster->setTexture("monster2_6.png");
		break;
	default:
		fly_cnt2 = 0;
		Sprite * sprBullet = Sprite::create("fireball.png");
		sprBullet->setPosition(Point(FlyMonLocation.x, FlyMonLocation.y));
		Degree = atan2f(FlyMonLocation.y - (double)sprLocation.y, (double)sprLocation.x - FlyMonLocation.x) * 180 / 3.1415f;
		sprBullet->setRotation(Degree);
		sprBullet->setScale(2);
		this->addChild(sprBullet);

		mMoveX = cos((-Degree) * 3.1416 / 180);
		mMoveY = sin((-Degree) * 3.1416 / 180);
		MoveBy * bulletmvb = MoveBy::create(1 / (mfireball_Bulletspeed / charging), Point(mMoveX, mMoveY));
		sprBullet->runAction(Sequence::create(Repeat::create(bulletmvb, 750), RemoveSelf::create(), NULL));
	}
	fly_cnt2++;
}