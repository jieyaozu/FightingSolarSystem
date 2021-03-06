#ifndef __BOSSBULLET_H__
#define __BOSSBULLET_H__

#include "cocos2d.h"
#include "Bullet.h"
USING_NS_CC;

class BossBullet:public Bullet{
private:
	float m_speed;
	int m_count;
	float c;
	CCPoint m_targetPosition;
	CCPoint m_thisPosition;
	bool m_isrota;
public:
	BossBullet(int speed, const char* weapon,CCPoint thisPosition,CCPoint targetPosition);
	~BossBullet();
	virtual void update(float dt);
	void targetMove(int c);
	void setAngle(float c);
	float getAngle();
	void setIsRota(bool is){m_isrota = is;}
};
#endif