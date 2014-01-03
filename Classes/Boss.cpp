#include "Boss.h"
#include "Resource.h"
#include "BossBullet.h"

Boss::Boss():Enemy(1000),m_bomb(NULL),m_ship_target(NULL){
	
}

Boss::~Boss(){
	CCLog("=============================================>Boss finish()");
	if(m_ship_target)
	m_ship_target->release();
}

bool Boss::init(){
	if ( !CCSprite::init() )
    {
        return false;
    }
	this->initWithFile(s_boss_one);

	m_bomb = new Enemy();
	m_bomb->autorelease();
	m_bomb->initWithFile(s_boss_one_bomb);
	addChild(m_bomb,1);
	CCSize size = this->getContentSize();
	m_bomb->setAnchorPoint(ccp(0.5,0.8));
	m_bomb->setPosition(ccp(size.width/2,size.height/2));


	CCDelayTime *delay =  CCDelayTime::create(1);
    CCMoveBy *mv = CCMoveBy::create(6, ccp(300, 0));
    CCFiniteTimeAction *seq = CCSequence::create(delay, mv, delay->copy(), mv->reverse(), NULL);
    this->runAction(CCRepeatForever::create((CCActionInterval*)seq));

	this->schedule(schedule_selector(Boss::calRoat),1);
}

void Boss::update(float dt){
	Enemy::update(dt);
	m_bomb->update(dt);
}

void Boss::hurt()
{
	Enemy::hurt();
	m_bomb->hurt();
}

void Boss::hurt(int power){
	Enemy::hurt(power);
	m_bomb->hurt(power);
}

void Boss::calRoat(float dt){
	if(m_ship_target){
		if(m_ship_target->isActive()){
		  CCPoint bossp = this->getPosition();
		  CCPoint gameLayerPoint = ccp(bossp.x,bossp.y);
		  CCPoint ship_target = m_ship_target->getPosition();
		  int c = (int)(atan2(gameLayerPoint.x - ship_target.x,gameLayerPoint.y - ship_target.y)*180/M_PI);
		  m_bomb->setRotation(c);
      
		  shoot(dt);
		}
	}
}

void Boss::setShipTarget(Ship *ship){
	m_ship_target = ship;
	ship->retain();
}

void Boss::shoot(float dt){
	 CCPoint position = this->getPosition();
	 BossBullet *bullet = new BossBullet(12,s_boss_bullet,position,m_ship_target->getPosition());
	 if (bullet) {
		 bullet->autorelease();
		 enemy_bullet->addObject(bullet);
		 getParent()->addChild(bullet, 1000, 900);
		 bullet->setPosition(position);
	 }
}