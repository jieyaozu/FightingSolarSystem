#ifndef _EXPLOSIONFRAGMENT_H_
#define _EXPLOSIONFRAGMENT_H_

#include "cocos2d.h"

USING_NS_CC;

class ExplosionFragment:public CCLayer{
public:
	void initFragments(int numberOffragments);
};

//! @brief A sun particle system
class CCMyParticleSun : public CCParticleSystemQuad
{
public:
    /**
     * @js ctor
     */
    CCMyParticleSun(){}
    /**
     * @js NA
     * @lua NA
     */
    virtual ~CCMyParticleSun(){}
    bool init(){ return initWithTotalParticles(20); }
    virtual bool initWithTotalParticles(unsigned int numberOfParticles);
    
    static CCMyParticleSun* create();
    static CCMyParticleSun* createWithTotalParticles(unsigned int numberOfParticles);
};
#endif