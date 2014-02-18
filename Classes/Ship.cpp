//
//  Ship.cpp
//  MoonWarriorsx
//
//  Created by 王 磊 on 12-12-29.
//
//

#include "Ship.h"
#include "Resource.h"
#include "Bullet.h"
#include "FollowBullet.h"
#include "Config.h"
#include "Effect.h"
#include "SimpleAudioEngine.h"
#include "ExplosionFragment.h"

using namespace CocosDenshion;
using namespace cocos2d;
using namespace std;

Ship::Ship():m_speed(220),m_bulletSpeed(900),m_HP(5),m_bulletTypeValue(1),m_bulletPowerValue(1),m_throwBombing(false),m_canBeAttack(true),m_isThrowBomb(false),m_zOder(3000),m_maxBulletPowerValue(4),m_appearPosition(ccp(0, 0)),m_hurtColorLife(0),m_active(true)
{
     //ccBlendFunc cbl = {GL_ONE_MINUS_DST_COLOR, GL_ONE};
	 ccBlendFunc cbl = {GL_ZERO, GL_ONE};
     this->setBlendFunc(cbl);
}

Ship::~Ship()
{
}

bool Ship::init()
{
    // super init first
    if ( !CCSprite::init() )
    {
        return false;
    }
    
    // init life
    CCTexture2D * shipTextureCache = CCTextureCache::sharedTextureCache()->textureForKey(s_ship01);
    CCRect rec = CCRectMake(0, 0, 60, 38);
    this->initWithTexture(shipTextureCache,  rec);
    
    this->setPosition(m_appearPosition);

    //shadow
	CCSprite* shadow = CCSprite::create();
	shadow->initWithFile(s_shadow);
	this->addChild(shadow);
	shadow->setPosition(ccp(90,-30));
	shadow->setScale(0.8f);
    
    // set frame
    CCSpriteFrame *frame0 = CCSpriteFrame::createWithTexture(shipTextureCache, CCRectMake(0, 0, 60, 38));
    CCSpriteFrame *frame1 = CCSpriteFrame::createWithTexture(shipTextureCache, CCRectMake(60, 0, 60, 38));

    CCArray *animFrames = CCArray::create();
    animFrames->addObject(frame0);
    animFrames->addObject(frame1);
    
    // ship animate
    // 这个方法有差异
    CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames, 0.1);
    CCAnimate *animate = CCAnimate::create(animation);
    this->runAction(CCRepeatForever::create(animate));
	//设置透明
	this->setOpacity(0);
    
    // 子弹发射
    //this->schedule(schedule_selector(Ship::shoot), 0.10);
    
    // revive effect
    this->m_canBeAttack = false;
	this->m_canBeMove = false;
   // CCSprite *ghostSprite = CCSprite::createWithTexture(shipTextureCache, CCRectMake(0, 45, 60, 38));
   // ccBlendFunc cbl = {GL_SRC_ALPHA, GL_ONE};
   // ghostSprite->setBlendFunc(cbl);
    //ghostSprite->setScale(8);
   // ghostSprite->setPosition(ccp(this->getContentSize().width / 2, 12));
   // this->addChild(ghostSprite, 3000, 99999);
   // ghostSprite->runAction(CCScaleTo::create(0.5, 1, 1));
	

    winSize = CCDirector::sharedDirector()->getWinSize();
	
	//开始移动出场
	CCCallFuncN *moveByAndShootAndCanbeAttack = CCCallFuncN::create(this, callfuncN_selector(Ship::startMove));
	//设置为可见
	CCCallFuncN *visible = CCCallFuncN::create(this, callfuncN_selector(Ship::setMyOpacity));

    this->runAction(CCSequence::create(CCDelayTime::create(1.5),visible,moveByAndShootAndCanbeAttack, NULL));

    return true;
}


void Ship::update(float dt)
{

    if (m_HP <= 0) {
        m_active = false;
    }
	if(m_boom){
		if(!m_boom->isActive()){
			m_boom->destroy();
		    m_boom = NULL;
		}else{
			m_boom->update(dt);
		}
	}
    
}

void Ship::setMyOpacity(CCNode *pSender){
	this->setOpacity(255);
}

void Ship::setTarget(Enemy *targett,CCSprite *bz){
	target = targett;
	if(target)
	target->retain();
	biaozi = bz;
}

Enemy* Ship::getTarget(){
	return target;
}

void Ship::setTargetTwo(Enemy *targett,CCSprite *bz){
	targetTwo = targett;
	if(targetTwo)
	targetTwo->retain();
	biaoziTwo = bz;
}

Enemy* Ship::getTargetTwo(){
	return targetTwo;
}


void Ship::startMove(CCNode *pSender){
	CCMoveBy *moveBy = CCMoveBy::create(1.0f,ccp(winSize.width/ 2,60));
	//动作由快到慢
	CCActionInterval* move_ease = CCEaseSineOut::create((CCActionInterval*)(moveBy) );
	
	//开始攻击
	CCCallFuncN *startSht = CCCallFuncN::create(this, callfuncN_selector(Ship::startShoot));
	//开始导弹攻击
	CCCallFuncN *startFollowSht = CCCallFuncN::create(this, callfuncN_selector(Ship::startFollowShoot));
	// 闪烁动画
    CCBlink *blinks = CCBlink::create(3, 60);
	
	// 回调，攻击使能
    // 带执行者回调，谁执行Action传谁。这里是this执行的动作，所以传的就是this
    CCCallFuncN *makeBeAttack = CCCallFuncN::create(this, callfuncN_selector(Ship::makeAttack));
	this->runAction(CCSequence::create(move_ease,startSht,startFollowSht,blinks,makeBeAttack,NULL));

	//if (Config::sharedConfig()->getEffectState()){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(s_moveout);
   // }
}

void Ship::startShoot(CCNode *pSender){
	this->schedule(schedule_selector(Ship::shoot), 0.10);
	this->m_canBeMove = true;
}

void Ship::startFollowShoot(CCNode *pSender){
	this->schedule(schedule_selector(Ship::followShoot), 0.6);
}

void Ship::makeAttack(CCNode * pSender)
{
//    CCLog("pSend tag %d",pSender->getTag());
    this->m_canBeAttack = true;
    this->setVisible(true);
    pSender->removeChildByTag(99999);
//    CCLog("make Attack !");
    
}

bool Ship::isCanBeMove(){
     return this->m_canBeMove;
}

void Ship::shoot(float dt)
{

    int offset = 13;
    CCPoint position = this->getPosition();
    CCSize contentSize = this->getContentSize();
    Bullet *bullet_a = new Bullet(m_bulletSpeed, "W1.png", 1);
    if (bullet_a) {
        bullet_a->autorelease();
        play_bullet->addObject(bullet_a);
        this->getParent()->addChild(bullet_a, bullet_a->m_zorder, 901);
        bullet_a->setPosition(ccp(position.x + offset, position.y + 3 + contentSize.height * 0.3));

    }else{
        delete bullet_a;
        bullet_a = 0;
    }
    
    
    Bullet *bullet_b = new Bullet(m_bulletSpeed, "W1.png", 1);
    if (bullet_b) {
        bullet_b->autorelease();
        play_bullet->addObject(bullet_b);
        this->getParent()->addChild(bullet_b, bullet_b->m_zorder, 901);
        bullet_b->setPosition(ccp(position.x - offset, position.y + 3 + contentSize.height * 0.3));
    }else{
        delete bullet_b;
        bullet_a = 0;
    }

	if (Config::sharedConfig()->getEffectState()){
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(s_fireEffect);
    }

}

void Ship::followShoot(float dt){
	    int offset = 13;
		CCPoint position = this->getPosition();
		CCSize contentSize = this->getContentSize();
	if(target && target->isActive()){
		FollowBullet *follow_bullet = new FollowBullet(10, "W3.png",target);
		if(follow_bullet){
		   follow_bullet->autorelease();
		   play_bullet->addObject(follow_bullet);
		   this->getParent()->addChild(follow_bullet, follow_bullet->m_zorder, 901);
		   follow_bullet->setPosition(ccp(position.x + offset, position.y + 3 + contentSize.height * 0.3));
		}
     }
	if(targetTwo && targetTwo->isActive()){
	    FollowBullet *follow_bullet_b = new FollowBullet(10, "W3.png",targetTwo);
		if(follow_bullet_b){
		   follow_bullet_b->autorelease();
		   play_bullet->addObject(follow_bullet_b);
		   this->getParent()->addChild(follow_bullet_b, follow_bullet_b->m_zorder, 901);
		   follow_bullet_b->setPosition(ccp(position.x - offset, position.y + 3 + contentSize.height * 0.3));
		}
	}
	
}

void Ship::hurt()
{
    if (m_canBeAttack) {
        CCLog("under fire!");
        m_HP--;
		this->setColor(ccWHITE);
		//this->setShader(1);
		//this->setColor(ccc3(0, 0, 255));
    }
    
}

void Ship::hurt(int power)
{
    if (m_canBeAttack) {
        CCLog("under fire!");
        m_HP = m_HP - power;
		this->setColor(ccc3(0,255,0));
		//this->setShader(1);
		//this->setColor(ccc3(0, 0, 255));
    }
    
}

void Ship::destroy()
{
    CCLOG("destroy one ship");
    Config::sharedConfig()->updateLifeCount();

    CCLOG("life count is %d",Config::sharedConfig()->getLifeCount());
    
    Effect *effect = Effect::create();
    
    effect->explode(this->getParent(), this->getPosition());

    this->removeFromParent();
    
    if (Config::sharedConfig()->getAudioState()){
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(s_shipDestroyEffect);
        
    }

}

CCRect Ship::collideRect()
{
    CCPoint pos = getPosition();
    CCSize cs = getContentSize();
    
    return CCRectMake(pos.x - cs.width / 2 , pos.y - cs.height / 2, cs.width, cs.height / 2);
}

bool Ship::isActive()
{
    return m_active;
}

int Ship::getZoder()
{
    return m_zOder;
}

void Ship::addBombChild(){
	m_boom = Bomb::create();
	m_boom->initWithFile(s_boom);
	this->addChild(m_boom,100);
	m_boom->setAnchorPoint(ccp(0.5,0.05));
	m_boom->setPosition(ccp(30,19));

	// 大小渐变
    CCActionInterval *bigAnim = CCScaleBy::create(0.4, 2.5, 2);
	CCEaseSineOut *biggerEase = CCEaseSineOut::create(bigAnim);
	CCDelayTime *delay =  CCDelayTime::create(1.5);
	CCActionInterval *smallAnim = CCScaleBy::create(1.1, 0.2, 1);
	CCCallFuncN *removeFunc =  CCCallFuncN::create(m_boom, callfuncN_selector(Ship::killSprite));
	m_boom->runAction(CCSequence::create(biggerEase,delay,smallAnim,removeFunc,NULL));
    
	//闪烁
	//CCBlink* blink = CCBlink::create(2.5,40);
	//boom->runAction(blink);

	//音效
	CCCallFuncN *sound1 =  CCCallFuncN::create(m_boom, callfuncN_selector(Ship::boomOneSound));
	CCDelayTime *delays =  CCDelayTime::create(0.8);
	CCCallFuncN *sound2 =  CCCallFuncN::create(m_boom, callfuncN_selector(Ship::boomTwoSound));
	m_boom->runAction(CCSequence::create(sound1,delays,sound2,NULL));
}
//这是两个音效文件，用两个方法合成起来的
void Ship::boomOneSound(CCNode *pSender){
	if (Config::sharedConfig()->getEffectState()){
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(s_boom_sound_one);
    }
}

void Ship::boomTwoSound(CCNode *pSender){
	if (Config::sharedConfig()->getEffectState()){
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(s_boom_sound_two);
    }
}

void Ship::killSprite(CCNode *pSender)
{
	((Bomb*)pSender)->setActivity(false);
}

UnitSprite* Ship::getBomb(){
    return m_boom;
}

void Ship::setShader(int what){
	do{ 

        GLchar* pszFragSource;
		if(what == 0){
			pszFragSource =  
		   "#ifdef GL_ES \n "
			"precision mediump float; \n "  
			"#endif \n "  
			"uniform sampler2D u_texture; \n " 
			"varying vec2 v_texCoord; \n "  
			"varying vec4 v_fragmentColor; \n " 
			"void main(void) \n "  
			"{ \n "  
			"// Convert to greyscale using NTSC weightings \n "  
			"vec4 col = texture2D(u_texture, v_texCoord); \n "  
			"gl_FragColor = vec4(col.r, col.g, col.b, col.a); \n " 
			"}";
		}else{
			pszFragSource =  
		   "#ifdef GL_ES \n "
			"precision mediump float; \n "  
			"#endif \n "  
			"uniform sampler2D u_texture; \n " 
			"varying vec2 v_texCoord; \n "  
			"varying vec4 v_fragmentColor; \n " 
			"void main(void) \n "  
			"{ \n "  
			"// Convert to greyscale using NTSC weightings \n "  
			"float grey = dot(texture2D(u_texture, v_texCoord).rgba, vec4(0.5, 0.0, 0.0,0.7)); \n "  
			"gl_FragColor = vec4(grey, 0.0, 0.0, 0.0); \n " 
			"}";
		}
		
        
        CCGLProgram* pProgram = new CCGLProgram();  
        pProgram->initWithVertexShaderByteArray(ccPositionTextureColor_vert, pszFragSource);  
        this->setShaderProgram(pProgram);  
        pProgram->release();  
        CHECK_GL_ERROR_DEBUG();  
          
        this->getShaderProgram()->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);  
        this->getShaderProgram()->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);  
        this->getShaderProgram()->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);  
        CHECK_GL_ERROR_DEBUG();  
          
        this->getShaderProgram()->link();  
        CHECK_GL_ERROR_DEBUG();  
          
        this->getShaderProgram()->updateUniforms();  
        CHECK_GL_ERROR_DEBUG();   
    } while (0);  
}

void Ship::shootBomb(){
	CCSprite *bomb = CCSprite::create();
	bomb->initWithFile(s_fire2);
	//bomb->setScale(0.15f);
	this->getParent()->addChild(bomb,10086);
	CCPoint pos = this->getPosition();
	bomb->setPosition(pos);
	//加入粒子效果
	CCMyParticleSun *sun = CCMyParticleSun::create();
	bomb->addChild(sun);
	CCSize size = bomb->getContentSize();
	sun->setPosition(ccp(size.width/2,size.height/2));
	//加入动作
	CCMoveBy *moveby = CCMoveBy::create(0.5,ccp(0,160));
	CCCallFuncN *callRemove = CCCallFuncN::create(bomb,callfuncN_selector(Ship::removeBomb));
	//动作由快到慢
	CCActionInterval* move_ease = CCEaseSineOut::create((CCActionInterval*)(moveby));
	bomb->runAction(CCSequence::create(moveby,callRemove,NULL));
}

void Ship::removeBomb(CCNode *pSender){
	CCParticleSystem *m_emitter = new CCParticleSystemQuad();
    m_emitter->initWithFile(s_explodingring);
	this->getParent()->addChild(m_emitter, 1000);
	m_emitter->setPosition(pSender->getPosition());
	m_emitter->release();

	ExplosionFragment *fragments = new ExplosionFragment();
	this->getParent()->addChild(fragments,10001);
	fragments->setPosition(pSender->getPosition());
	fragments->initFragments(150);
	fragments->release();

	pSender->removeFromParent();

	// 声音
    if (Config::sharedConfig()->getEffectState()) {
            SimpleAudioEngine::sharedEngine()->playEffect(s_explodeEffect);
			//SimpleAudioEngine::sharedEngine()->playEffect(s_shipDestroyEffect);
    }
}