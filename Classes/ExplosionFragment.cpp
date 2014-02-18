#include "ExplosionFragment.h"
#include "BossBullet.h"
#include "Config.h"
#include "Resource.h"
#include "firePngData.h"

void ExplosionFragment::initFragments(int numberOffragments){
	float angleVar = 360.0f/numberOffragments;
	float offsetAngle = 0;
	int cacheCount = particle_cache->count();
	int particlesNeed = numberOffragments - cacheCount;

	int realCount = cacheCount>numberOffragments?numberOffragments:cacheCount;
	for(int i = 0;i<realCount;i++){
		BossBullet *fragment = dynamic_cast<BossBullet*>(particle_cache->objectAtIndex(i));
		CCNode* parent =  fragment->getParent();
		if(parent)
		{
           fragment = new BossBullet(3,s_fire2,this->getPosition(),ccp(1000,0));
		   fragment->setIsRota(false);
		   particle_cache->addObject(fragment);
		   fragment->release();
		}
		//fragment->removeAllChildren();
		//==============================
		CCMyParticleSun *system = CCMyParticleSun::create();
		fragment->addChild(system,10);
		CCSize size = fragment->getContentSize();
		system->setPosition(ccp(size.width/2,size.height/2));
		//==============================
		this->getParent()->addChild(fragment,1001);
		play_bullet->addObject(fragment);
		//初始位置
		float x2 = this->getPosition().x-sin(offsetAngle*M_PI/180)*80;
	    float y2 = this->getPosition().y-cos(offsetAngle*M_PI/180)*80;
		fragment->setPosition(ccp(x2,y2));
		//运动角度
		fragment->setAngle(offsetAngle);
		offsetAngle = offsetAngle + angleVar;
	}
	if(particlesNeed > 0){
		for(int i = 0;i<particlesNeed;i++){
			BossBullet *fragment = new BossBullet(3,s_fire2,this->getPosition(),ccp(1000,0));
			//==============================
			CCMyParticleSun *system = CCMyParticleSun::create();
			fragment->addChild(system,10);
			CCSize size = fragment->getContentSize();
			system->setPosition(ccp(size.width/2,size.height/2));
			//==============================
			fragment->setIsRota(false);
			play_bullet->addObject(fragment);
			particle_cache->addObject(fragment);
			this->getParent()->addChild(fragment,1001);
			//初始位置
			float x2 = this->getPosition().x-sin(offsetAngle*M_PI/180)*80;
			float y2 = this->getPosition().y-cos(offsetAngle*M_PI/180)*80;
			fragment->setPosition(ccp(x2,y2));
			//运动角度
			fragment->setAngle(offsetAngle);
			offsetAngle = offsetAngle + angleVar;

			fragment->release();
		}
	}
}

CCMyParticleSun* CCMyParticleSun::create()
{
    CCMyParticleSun* pRet = new CCMyParticleSun();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

CCMyParticleSun* CCMyParticleSun::createWithTotalParticles(unsigned int numberOfParticles)
{
    CCMyParticleSun* pRet = new CCMyParticleSun();
    if (pRet && pRet->initWithTotalParticles(numberOfParticles))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

CCTexture2D* getDefaultTexture();
bool CCMyParticleSun::initWithTotalParticles(unsigned int numberOfParticles)
{
    if( CCParticleSystemQuad::initWithTotalParticles(numberOfParticles) )
    {
        // additive
        this->setBlendAdditive(true);

        // duration
        m_fDuration = kCCParticleDurationInfinity;

        // Gravity Mode
        setEmitterMode(kCCParticleModeGravity);

        // Gravity Mode: gravity
        setGravity(ccp(0,0));

        // Gravity mode: radial acceleration
        setRadialAccel(0);
        setRadialAccelVar(0);

        // Gravity mode: speed of particles
        setSpeed(3);
        setSpeedVar(0);


        // angle
        m_fAngle = 90;
        m_fAngleVar = 360;

        // emitter position
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        this->setPosition(ccp(winSize.width/2, winSize.height/2));
        setPosVar(CCPointZero);

        // life of particles
        m_fLife = 0.2f;
        m_fLifeVar = 0.2f;

        // size, in pixels
        m_fStartSize = 0.0f;
        m_fStartSizeVar = 18.0f;
        m_fEndSize = kCCParticleStartSizeEqualToEndSize;

        // emits per seconds
        m_fEmissionRate = m_uTotalParticles/m_fLife;

        // color of particles
        m_tStartColor.r = 0.76f;
        m_tStartColor.g = 0.25f;
        m_tStartColor.b = 0.12f;
        m_tStartColor.a = 1.0f;
        m_tStartColorVar.r = 0.0f;
        m_tStartColorVar.g = 0.0f;
        m_tStartColorVar.b = 0.0f;
        m_tStartColorVar.a = 0.0f;
        m_tEndColor.r = 0.0f;
        m_tEndColor.g = 0.0f;
        m_tEndColor.b = 0.0f;
        m_tEndColor.a = 1.0f;
        m_tEndColorVar.r = 0.0f;
        m_tEndColorVar.g = 0.0f;
        m_tEndColorVar.b = 0.0f;
        m_tEndColorVar.a = 0.0f;

        CCTexture2D* pTexture = getDefaultTexture();
        if (pTexture != NULL)
        {
            setTexture(pTexture);
        }

        return true;
    }
    return false;
}

static CCTexture2D* getDefaultTexture()
{
    CCTexture2D* pTexture = NULL;
    CCImage* pImage = NULL;
    do 
    {
        bool bRet = false;
        const char* key = "__firePngData";
        pTexture = CCTextureCache::sharedTextureCache()->textureForKey(key);
        CC_BREAK_IF(pTexture != NULL);

        pImage = new CCImage();
        CC_BREAK_IF(NULL == pImage);
        bRet = pImage->initWithImageData((void*)__firePngData, sizeof(__firePngData), CCImage::kFmtPng);
        CC_BREAK_IF(!bRet);

        pTexture = CCTextureCache::sharedTextureCache()->addUIImage(pImage, key);
    } while (0);

    CC_SAFE_RELEASE(pImage);

    return pTexture;
}