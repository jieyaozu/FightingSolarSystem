#ifndef __SHADOWSPRITE__
#define __SHADOWSPRITE__

#include "cocos2d.h"
USING_NS_CC;

class ShadowSprite : public CCSprite {
public:
	void setShader(int what);
	CREATE_FUNC(ShadowSprite);
};

#endif