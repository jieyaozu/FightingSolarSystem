#ifndef __MoonWarriorsx__BOOM__
#define __MoonWarriorsx__BOOM__

#include "cocos2d.h"
#include "UnitSprite.h"
USING_NS_CC;

class Bomb:public UnitSprite{
private:
	bool m_active;
	int count;
	 // ������������
public:
	Bomb();
	~Bomb();
	// ����
    virtual void update(float dt);

    virtual void hurt();
    // ��ײ����
    virtual CCRect collideRect();
    //���ٷɴ�
    virtual void destroy();
    //�û�״̬
	void setActivity(bool isactive);
    // ��ȡ����״̬
    virtual bool isActive();

	// ������
    CREATE_FUNC(Bomb);
};
#endif