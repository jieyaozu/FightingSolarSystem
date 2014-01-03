#ifndef __MoonWarriorsx__FollowBullet__
#define __MoonWarriorsx__FollowBullet__

#include "cocos2d.h"
#include "UnitSprite.h"
#include "Bullet.h"
#include "Enemy.h"
USING_NS_CC;
class FollowBullet : public Bullet{
private:
	float m_speed;
	float omega;
	int b;
	Enemy *target;
	CCParticleSystem*    m_emitter;
public:
	FollowBullet(int speed, const char* weapon,Enemy *pSender);
	~FollowBullet();
	virtual void update(float dt);
	void targetMove(CCPoint position,CCPoint targetPosition);
};
#endif