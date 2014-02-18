//
//  StartMenu.cpp
//  MoonWarriorsx
//
//  Created by 磊 王 on 12-12-27.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "StartMenu.h"
#include "Resource.h"
#include "SimpleAudioEngine.h"
#include "Effect.h"
#include "GameLayer.h"
#include "About.h"
#include "Options.h"
#include "ExplosionFragment.h"
#include "BossBullet.h"
#include "util.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* StartMenu::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    StartMenu *layer = StartMenu::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool StartMenu::init()
{
    //////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    CCTexture2D *shipTexture = CCTextureCache::sharedTextureCache()->addImage(s_ship01);
    // 添加Android的按键处理，这里加了全局代理
    this->setKeypadEnabled(true);
    CostomeKeyPadDelegae *keyDelegate = new CostomeKeyPadDelegae();
    keyDelegate->autorelease();
    CCDirector::sharedDirector()->getKeypadDispatcher()->addDelegate(keyDelegate);
    
    //////////////////
    // 2. add main menu ,logo, loading, and ships
    winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite* sl = CCSprite::create(s_loading);
    sl->setAnchorPoint(ccp(0, 0));
    this->addChild(sl,0,1);
    
    CCSprite* logo = CCSprite::create(s_logo);
    logo->setAnchorPoint(ccp(0.5, 1));
    logo->setPosition(ccp(winSize.width/2, winSize.height-100));
    this->addChild(logo, 10, 1);
    
    CCSprite* newGameNormal = CCSprite::create(s_menu, CCRectMake(0, 0, 126, 33));
    CCSprite* newGameSelected = CCSprite::create(s_menu, CCRectMake(0, 33, 126, 33));
    CCSprite* newGameDisabled = CCSprite::create(s_menu, CCRectMake(0, 33*2, 126, 33));
    
    CCSprite* gameSettingNormal = CCSprite::create(s_menu, CCRectMake(126, 0, 126, 33));
    CCSprite* gameSettingNSelected = CCSprite::create(s_menu, CCRectMake(126, 33, 126, 33));
    CCSprite* gameSettingDesabled = CCSprite::create(s_menu, CCRectMake(126, 33*2, 126, 33));
    
    CCSprite* aboutNormal = CCSprite::create(s_menu, CCRectMake(252, 0, 126, 33));
    CCSprite* aboutSelected = CCSprite::create(s_menu, CCRectMake(252, 33, 126, 33));
    CCSprite* aboutDesabled = CCSprite::create(s_menu, CCRectMake(252, 33*2, 126, 33));
    
    
    CCMenuItemSprite* newGame = CCMenuItemSprite::create(newGameNormal, newGameSelected, newGameDisabled, this, menu_selector(StartMenu::flareEffect));
	newGame->setScale(1.5f);
    CCMenuItemSprite* gameSetting = CCMenuItemSprite::create(gameSettingNormal, gameSettingNSelected, gameSettingDesabled, this, menu_selector(StartMenu::menuCallback));
	gameSetting->setScale(1.5f);
    gameSetting->setTag(20);
    CCMenuItemSprite* about = CCMenuItemSprite::create(aboutNormal, aboutSelected, aboutDesabled, this, menu_selector(StartMenu::menuCallback));
	about->setScale(1.5f);
	about->setTag(21);

    // 最后一个参数要是NULL
    CCMenu* menu = CCMenu::create(newGame, gameSetting, about, NULL);
    menu->alignItemsVerticallyWithPadding(30);
    this->addChild(menu, 1, 2);
    menu->setPosition(ccp(winSize.width / 2, winSize.height / 2 - 80));
    this->schedule(schedule_selector(StartMenu::update), 0.1);
    
    // ships
    CCTexture2D *textCache = CCTextureCache::sharedTextureCache()->textureForKey(s_ship01);
    m_ship = CCSprite::createWithTexture(textCache, CCRectMake(0, 45, 60, 38));
    this->addChild(m_ship, 0, 4);
    CCPoint position = ccp(CCRANDOM_0_1() * winSize.width, 0);
    m_ship->setPosition(position);
    m_ship->runAction(CCMoveBy::create(2, ccp(CCRANDOM_0_1() * winSize.width, position.y + winSize.height + 100)));

    
	if (Config::sharedConfig()->getAudioState()&&!Config::sharedConfig()->isOptions()&&!Config::sharedConfig()->isAbout()) {
        SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.7);
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic(s_mainMainMusic, true);

    }
    Config::sharedConfig()->setOptions(false);
	Config::sharedConfig()->setAbout(false);

	CCSprite* fragment = CCSprite::create();
	fragment->initWithFile(s_fragment);
	this->addChild(fragment,1000);
	fragment->setPosition(ccp(240,300));
	CCOrbitCamera * orbit = CCOrbitCamera::create(0.3, 1, 0, 0, 360, 0, 0);//(8, 1, 0, 0, 360, 90, -45)
	fragment->runAction(CCRepeatForever::create(orbit));

	/////////////////////////////////////////////////////////////////////////////
	CCSprite *sprite = CCSprite::create();
	sprite->initWithFile(s_fire);
	this->addChild(sprite,10);
	sprite->setPosition(ccp(100,200));

	CCMyParticleSun *system = CCMyParticleSun::create();
	sprite->addChild(system,10);
	CCSize size = sprite->getContentSize();
	system->setPosition(ccp(size.width/2,size.height/2));

	CCMoveBy *by = CCMoveBy::create(5,ccp(400,100));
	CCFiniteTimeAction *seq = CCSequence::create(by,  by->reverse(), NULL);
	sprite->runAction(CCRepeatForever::create((CCActionInterval*)seq));
	
/*
	CCTexture2D *temp = CCTextureCache::sharedTextureCache()->addImage(s_boss_one);
	CCTexture2D* texture = Util::getGrayTexture(temp);
	// create a new CCSprite to return 
	CCSprite* s = CCSprite::create();  
    s->initWithTexture(texture);
	addChild(s,1000);
	s->setPosition(ccp(200,100));
	*/
	
    return true;
}

void StartMenu::onButtonEffect()
{
    if (Config::sharedConfig()->getAudioState()) {
         SimpleAudioEngine::sharedEngine()->playEffect(s_buttonEffect);
    }
   
}

void StartMenu::flareEffect(CCObject *node)
{
    onButtonEffect();
    Effect* flareEffect = Effect::create();
    CCCallFunc *callback =  CCCallFunc::create(this, callfunc_selector(StartMenu::newGame));
    flareEffect->flareEffect(this, callback);
}

void StartMenu::newGame()
{

	CCScene *scene = GameLayer::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.2, scene));

}

void StartMenu::menuCallback(CCObject* pSender)
{
    onButtonEffect();
    int tag = dynamic_cast<CCNode*>(pSender)->getTag();
    if (tag == 20) {
        CCScene *scene = Options::scene();
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.2, scene));

    }
    else if(tag == 21)
    {
        CCScene *scene = About::scene();
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.2, scene));
    }
    
}

void StartMenu::update(float dt)
{
    if (m_ship->getPosition().y > winSize.height) {
        CCPoint pos = ccp(CCRANDOM_0_1() * winSize.width, 10);
        m_ship->setPosition(pos);
        m_ship->runAction(CCMoveBy::create(floor(5 * CCRANDOM_0_1()), ccp(CCRANDOM_0_1() * winSize.width, pos.y + winSize.height)));
        
    }
}


