#include "BossBullet.h"
#include "Resource.h"
#include "ExplosionFragment.h"

BossBullet::BossBullet(int speed, const char* weapon,CCPoint thisPosition,CCPoint targetPosition):Bullet(5),m_count(0),c(0){
	m_speed = speed;
	m_isrota = true;
	m_targetPosition = targetPosition;
	m_thisPosition = thisPosition;
	this->initWithFile(weapon);
	/*
	CCMyParticleSun *system = CCMyParticleSun::create();
	this->addChild(system,10);
	CCSize size = this->getContentSize();
	system->setPosition(ccp(size.width/2,size.height/2));
	*/
}

BossBullet::~BossBullet(){

}

void BossBullet::update(float dt){
	if(m_count == 0){
		if(m_targetPosition.x == 1000){
		     
		}else{
			c = (atan2(m_thisPosition.x - m_targetPosition.x,m_thisPosition.y - m_targetPosition.y)*180/M_PI);
		}
		if(m_isrota){
		   this->setRotation(c);
		}
	}
	targetMove(c);
	if (m_Hp <= 0) {
        m_active = false;
    }
	m_count++;
}

void BossBullet::targetMove(int c){
	float x2 = this->getPosition().x-sin(c*M_PI/180)*m_speed;
	float y2 = this->getPosition().y-cos(c*M_PI/180)*m_speed;
	//CCLog("%s%d%s%d%s%d","=======================update=>",x2,":",y2," b=",b);
	this->setPosition(ccp(x2,y2));
}

void BossBullet::setAngle(float c){
	this->c = c;
}

float BossBullet::getAngle(){
	return this->c;
}