#ifndef __BOSS_H__
#define __BOSS_H__

#include "Enemy.h"
#include "Ship.h"

class Boss :public Enemy{
private:
	Enemy *m_bomb;
	Ship *m_ship_target;
	int m_count;
public :
	Boss();
	~Boss();
	void setShipTarget(Ship *ship);
	void shoot(float dt);
	void explore(CCNode *pSender);
	virtual void update(float dt);
	virtual void hurt();
	virtual void destroy();
	virtual void hurt(int power);
	virtual bool init();
	void calRoat(float dt);
	void killSprite(CCNode *pSender);
	void destroy(CCNode *pSender);
	CREATE_FUNC(Boss);
};

#endif