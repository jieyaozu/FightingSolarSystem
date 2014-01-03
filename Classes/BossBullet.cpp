#include "BossBullet.h"
#include "Resource.h"

BossBullet::BossBullet(int speed, const char* weapon,CCPoint thisPosition,CCPoint targetPosition):Bullet(5),m_count(0),c(0){
	m_speed = speed;
	m_targetPosition = targetPosition;
	m_thisPosition = thisPosition;
	this->initWithFile(weapon);
}

BossBullet::~BossBullet(){

}

void BossBullet::update(float dt){
	if(m_count == 0){
	    c = (atan2(m_thisPosition.x - m_targetPosition.x,m_thisPosition.y - m_targetPosition.y)*180/M_PI);
		this->setRotation(c);
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