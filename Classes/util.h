#ifndef _UTIL_H_
#define _UTIL_H_

#include "cocos2d.h"
USING_NS_CC;

class Util{
public:
	//得到灰色的图片纹理
	static CCTexture2D* getGrayTexture(const char* filename);
	static CCTexture2D* Util::getGrayTexture(CCTexture2D* texture);
};

#endif