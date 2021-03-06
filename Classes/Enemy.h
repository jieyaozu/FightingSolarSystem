//
//  Enemy.h
//  MoonWarriorsx
//
//  Created by 王 磊 on 13-1-14.
//
//

#ifndef __MoonWarriorsx__Enemy__
#define __MoonWarriorsx__Enemy__

#include "cocos2d.h"
#include "Config.h"
#include "UnitSprite.h"

USING_NS_CC;

class Enemy : public UnitSprite {
private:
    int m_speed;
    int m_bulletSpeed;
    int m_HP;
    int m_bulletPowerValure;
    int m_moveType;
    int m_zOrder;
    float m_delayTime;
    int m_attackMode;
    bool m_ishurt;
	int m_hurtcount;
	bool m_shader;
	CCSprite *m_glint;
protected:
	bool m_active;
	int m_scoreValue;
public:
    Enemy();
	Enemy(int hp,int score);
    ~Enemy();
    bool enemyInit(EnemyType type);
    virtual void update(float dt);
    void shoot(float f);
    virtual void hurt();
	virtual void hurt(int power);
    virtual void destroy();
    virtual CCRect collideRect();
    virtual bool isActive();
    
    static void sharedEnemy();
    static Enemy* create(EnemyType type);
    int getMoveType();

	//设置着色器
	void setShader(int what);

	void childrenInit();
    
};
#endif /* defined(__MoonWarriorsx__Enemy__) */
