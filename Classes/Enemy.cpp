//
//  Enemy.cpp
//  MoonWarriorsx
//
//  Created by 王 磊 on 13-1-14.
//
//

#include "Enemy.h"
#include "Resource.h"
#include "Bullet.h"
#include "Config.h"
#include "SimpleAudioEngine.h"
#include "Effect.h"
#include "ShadowSprite.h"
#include "util.h"

using namespace CocosDenshion;

Enemy::Enemy():m_shader(true),m_ishurt(false),m_hurtcount(0),m_active(true),m_speed(200),m_bulletSpeed(-300),m_HP(15),m_bulletPowerValure(1),m_moveType(0),m_scoreValue(200),m_zOrder(1000),m_delayTime(1+1.2*CCRANDOM_0_1()),m_attackMode(1),m_glint(NULL)
{
    
}

Enemy::Enemy(int hp,int score):m_shader(true),m_ishurt(false),m_hurtcount(0),m_active(true),m_speed(200),m_bulletSpeed(-300),m_HP(hp),m_bulletPowerValure(1),m_moveType(0),m_scoreValue(score),m_zOrder(1000),m_delayTime(1+1.2*CCRANDOM_0_1()),m_attackMode(1),m_glint(NULL)
{

}

Enemy::~Enemy()
{
    
}

/**自己的create函数，因为要带参数，那个子弹类也可以这么写*/
Enemy* Enemy::create(EnemyType type)
{
    Enemy *pRet = new Enemy();
    if (pRet && pRet->enemyInit(type)) {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

/**自己的init函数*/
bool Enemy::enemyInit(EnemyType type)
{
    if ( !CCSprite::init() )
    {
        return false;
    }
    
    
    m_HP = type.hp;
    m_moveType = type.moveType;
    m_scoreValue = type.scoreValue;
    
    
    this->initWithSpriteFrameName(type.textureName.c_str());
    this->schedule(schedule_selector(Enemy::shoot),this->m_delayTime);
    
	//shadow
	ShadowSprite* shadow = ShadowSprite::create();
	shadow->initWithSpriteFrameName(type.textureName.c_str());
	this->addChild(shadow);
	shadow->setPosition(ccp(90,-30));
	shadow->setScale(0.8f);
	shadow->setOpacity(100);
	shadow->setColor(ccc3(0,0,0));
	
	CCTexture2D* texture = this->getTexture();
	CCTexture2D* newtexture = Util::getGrayTexture(texture);
	// create a new CCSprite to return 
	/*
	m_glint = CCSprite::create();  
    m_glint->initWithTexture(newtexture);
	addChild(m_glint,1000);
	m_glint->setAnchorPoint(ccp(0,0));
	m_glint->setOpacity(0);
	*/
    return true;
}

void Enemy::childrenInit(){
    CCTexture2D* texture = Util::getGrayTexture(s_boss_one);
	// create a new CCSprite to return 
	m_glint = CCSprite::create();  
    m_glint->initWithTexture(texture);
	addChild(m_glint,1000);
	m_glint->setAnchorPoint(ccp(0,0));
	m_glint->setOpacity(0);
}

void Enemy::update(float dt)
{
    if (m_HP <= 0) {
        m_active = false;
    }

	if(m_ishurt){
		if(m_hurtcount%2==1){
			//this->setShader(0);
			if(m_glint){
				m_glint->setOpacity(0);
			}else{
				this->setColor(ccWHITE);
			}
			if (Config::sharedConfig()->getEffectState()) {
               SimpleAudioEngine::sharedEngine()->playEffect(s_enemy_under_fire);
            }
			m_ishurt = false;
			m_hurtcount = 0;
			return;
		}
		m_hurtcount++;
	}
}

void Enemy::shoot(float f)
{
    CCPoint pos = this->getPosition();
    Bullet *bullet = new Bullet(m_bulletSpeed, "W2.png", m_attackMode);
    bullet->autorelease();
    enemy_bullet->addObject(bullet);
    getParent()->addChild(bullet, m_zOrder, 900);
    bullet->setPosition(ccp(pos.x, pos.y - getContentSize().height * 0.2));
}

void Enemy::hurt()
{
    m_HP--;
	if(m_hurtcount!=0){
	  //this->setShader(1);
		if(m_glint){
			m_glint->setOpacity(255);
		}else{
		   this->setColor(ccRED);
		}
	}
	m_ishurt = true;
}

void Enemy::hurt(int power){
	m_HP = m_HP - power;
	m_ishurt = true;
	//this->setShader(1);
	if(m_glint){
			m_glint->setOpacity(255);
		}else{
		   this->setColor(ccRED);
	}
}

void Enemy::destroy()
{
	m_active = false;
    // 更新分数
    Config::sharedConfig()->setScoreValue(m_scoreValue );

    // 爆炸特效和闪光特效
    Effect *effect = Effect::create();
    
    effect->explode(this->getParent(), getPosition());
    
    effect->spark(this->getPosition(),this->getParent(), 1.2, 0.7);
    
    // 敌机爆炸，从敌机数组删除
    enemy_items->removeObject(this);
    
    // 删除精灵
	this->removeAllChildren();
    this->removeFromParent();
    // 声音
    if (Config::sharedConfig()->getEffectState()) {
            SimpleAudioEngine::sharedEngine()->playEffect(s_explodeEffect);
			//SimpleAudioEngine::sharedEngine()->playEffect(s_shipDestroyEffect);
    }
}

CCRect Enemy::collideRect()
{
    CCSize size = getContentSize();
    CCPoint pos = getPosition();
    
    return CCRectMake(pos.x - size.width / 2, pos.y - size.height / 4, size.width, size.height / 2);
}

int Enemy::getMoveType()
{
    return m_moveType;
}
bool Enemy::isActive()
{
    return  m_active;
}

void Enemy::sharedEnemy()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(s_Enemy_plist, s_enemy);
}

void Enemy::setShader(int what){
	do{ 

        const GLchar* pszFragSource = NULL;
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
		}else if(what == 1){
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
			"gl_FragColor = vec4(grey, grey, grey, 0.0); \n " 
			"}";
		}else if(what == 2){
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
			"gl_FragColor = vec4(0, 0, 0, 0.0); \n " 
			"}";
		}

        CCGLProgram* pProgram = new CCGLProgram();  
        pProgram->initWithVertexShaderByteArray(ccPositionTextureColor_vert, pszFragSource);  
        this->setShaderProgram(pProgram);
		pProgram->autorelease();
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