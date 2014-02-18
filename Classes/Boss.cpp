#include "Boss.h"
#include "Resource.h"
#include "BossBullet.h"
#include "Effect.h"
#include "SimpleAudioEngine.h"
#include "ExplosionFragment.h"

using namespace CocosDenshion;

Boss::Boss():Enemy(3000,5000),m_bomb(NULL),m_ship_target(NULL),m_count(0){
	
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

	childrenInit();
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

void Boss::destroy(){
	if(m_count == 0){
		this->unschedule(schedule_selector(Boss::calRoat));
		// 敌机爆炸，从敌机数组删除
        enemy_items->removeObject(this);
		//stop the all actions
		this->stopAllActions();
		//start new action
		CCMoveBy *mv = NULL;
		if(this->getPosition().x<240){
		   mv = CCMoveBy::create(9, ccp(100, -450));
		}else{
		   mv = CCMoveBy::create(9, ccp(-100, -450));
		}
		
		this->runAction(mv);
		CCCallFuncN *callfun = CCCallFuncN::create(this,callfuncN_selector(Boss::explore));
		CCCallFuncN *kill = CCCallFuncN::create(this,callfuncN_selector(Boss::killSprite));
		CCCallFuncN *destroy = CCCallFuncN::create(this,callfuncN_selector(Boss::destroy));
		CCDelayTime *delay =  CCDelayTime::create(1);
		CCDelayTime *delay2 =  CCDelayTime::create(0.5);
		CCDelayTime *delay3 =  CCDelayTime::create(0.3);
		CCDelayTime *delay4 =  CCDelayTime::create(0.15);
		CCSequence *sque = CCSequence::create(callfun,delay,callfun,delay2,callfun,delay2,callfun,delay2,
			                                  callfun,delay3,callfun,delay3,callfun,delay3,callfun,delay3,callfun,
											  callfun,delay4,callfun,delay4,callfun,delay4,callfun,delay4,callfun,
											  callfun,delay4,callfun,delay4,callfun,delay4,callfun,delay4,callfun,
											  callfun,delay4,callfun,delay4,callfun,delay4,callfun,delay4,callfun,
											  callfun,delay4,callfun,delay4,callfun,delay4,callfun,delay4,callfun,
											  callfun,delay4,callfun,delay4,callfun,delay4,callfun,delay4,callfun,
											  callfun,delay4,callfun,delay4,callfun,delay4,callfun,delay4,callfun,
											  callfun,delay4,callfun,delay4,callfun,delay4,callfun,delay4,callfun,
											  callfun,delay4,callfun,delay4,kill,callfun,delay4,callfun,delay4,callfun,delay,destroy,NULL);
		this->runAction(sque);
	}
	m_count++;
	//Enemy::destroy();
}

void Boss::explore(CCNode *pSender){
	Effect *effect = Effect::create();
	CCPoint pos = getPosition();
	effect->explodeboss(this->getParent(), ccp(pos.x-100+CCRANDOM_0_1()*200,pos.y-50+CCRANDOM_0_1()*100));
	 // 声音
    if (Config::sharedConfig()->getEffectState()) {
            SimpleAudioEngine::sharedEngine()->playEffect(s_explodeEffect);
			//SimpleAudioEngine::sharedEngine()->playEffect(s_shipDestroyEffect);
    }
}

void Boss::killSprite(CCNode *pSender){
	m_active = false;  
    // 删除精灵
	//this->setShader(2);
	this->setOpacity(0);

	Config::sharedConfig()->setScoreValue(m_scoreValue );

    // 爆炸特效和闪光特效
    Effect *effect = Effect::create();
    effect->explode(this->getParent(), getPosition());
    effect->spark(this->getPosition(),this->getParent(), 1.2, 0.7);

	ExplosionFragment *fragments = new ExplosionFragment();
	this->getParent()->addChild(fragments,10001);
	fragments->setPosition(this->getPosition());
	fragments->initFragments(150);
	fragments->release();
}

void Boss::destroy(CCNode *pSender){
    m_active = false;
    // 更新分数
    
    //粒子特效
	/*
	CCParticleSystem *m_emitter = new CCParticleSystemQuad();
    m_emitter->initWithFile(s_explodingring);
	this->getParent()->addChild(m_emitter, 1000);
	m_emitter->setPosition(this->getPosition());
	m_emitter->release();
	*/
	//--------------------------------------------------------------

    // 删除精灵
    this->removeFromParent();
    // 声音
    if (Config::sharedConfig()->getEffectState()) {
            SimpleAudioEngine::sharedEngine()->playEffect(s_explodeEffect);
			//SimpleAudioEngine::sharedEngine()->playEffect(s_shipDestroyEffect);
    }
}