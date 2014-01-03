#ifndef __MoonWarriorsx__BOOM__
#define __MoonWarriorsx__BOOM__

#include "cocos2d.h"
#include "UnitSprite.h"
USING_NS_CC;

class Bomb:public UnitSprite{
private:
	bool m_active;
	int count;
	 // 被攻击，受伤
public:
	Bomb();
	~Bomb();
	// 更新
    virtual void update(float dt);

    virtual void hurt();
    // 碰撞矩形
    virtual CCRect collideRect();
    //销毁飞船
    virtual void destroy();
    //置换状态
	void setActivity(bool isactive);
    // 获取生存状态
    virtual bool isActive();

	// 构造器
    CREATE_FUNC(Bomb);
};
#endif