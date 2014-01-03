//
//  GameLayer.cpp
//  MoonWarriorsx
//
//  Created by 王 磊 on 12-12-29.
//
//

#include "GameLayer.h"
#include "SimpleAudioEngine.h"
#include "Bullet.h"
#include "Resource.h"
#include "Config.h"
#include "Enemy.h"
#include "Effect.h"
#include "GameOver.h"
#include "PauseLayer.h"
#include "Boss.h"

using namespace cocos2d;
using namespace CocosDenshion;


bool isPaused = false;

CCScene* GameLayer::scene(){
	CCScene *scene = CCScene::create();
	GameLayer* gameLayer = GameLayer::create();
	scene->addChild(gameLayer);
	return scene;
}

GameLayer::GameLayer():m_state(statePlaying),m_time(0),m_ship(NULL),m_boss(NULL),m_backSky(NULL),m_backSkyHeight(0),m_backSkyRe(NULL),m_backTileMap(NULL),m_backTileMapHeight(0),m_backTileMapRe(NULL),m_isBackSkyReload(false),m_isBackTileReload(false),m_lbScore(NULL),m_lifeCount(NULL),
    m_tempScore(0)
{
    
}
GameLayer::~GameLayer()
{
    if (m_levelManager) {
        delete m_levelManager;
    }
    play_bullet->release();
    enemy_bullet->release();
    enemy_items->release();
}

bool GameLayer::init()
{
    if (!CCLayer::init()) {
        return false;
    }
    // 开启触摸
    this->setTouchEnabled(true);
    
    // 创建数组，需要retain一下
    play_bullet = CCArray::create();
    play_bullet->retain();

    enemy_bullet = CCArray::create();
    enemy_bullet->retain();
    
    enemy_items = CCArray::create();
    enemy_items->retain();
    
    m_state = statePlaying;
    Enemy::sharedEnemy();
    Effect::sharedExplosion();
    
    Config::sharedConfig()->resetConfig();
    
    winSize = CCDirector::sharedDirector()->getWinSize();
    m_levelManager = new LevelManager(this);
    
    //初始化背景
    initBackground();

    m_screenRec = CCRectMake(0, 0,  winSize.width, winSize.height + 10);
    
    // score
    m_lbScore = CCLabelBMFont::create("Score:0", s_arial14_fnt);
    m_lbScore->setAnchorPoint(ccp(1, 0));
    m_lbScore->setAlignment(kCCTextAlignmentRight);
    addChild(m_lbScore, 1000);
    m_lbScore->setPosition(winSize.width - 5, winSize.height - 30);
    
    // ship life
    CCTexture2D *shipTexture = CCTextureCache::sharedTextureCache()->addImage(s_ship01);
    CCSprite *life = CCSprite::createWithTexture(shipTexture, CCRectMake(0, 0, 60, 38));
    life->setScale(0.6);
    life->setPosition(ccp(30,winSize.height-23));
    addChild(life, 1, 5);
    
    // ship life count
    char lifecount[2];
    sprintf_s(lifecount, "%d",Config::sharedConfig()->getLifeCount());
    m_lifeCount = CCLabelTTF::create(lifecount, "Arial", 20);
    m_lifeCount->setPosition(ccp(60, winSize.height-20));
    m_lifeCount->setColor(ccc3(255,0, 0));
    addChild(m_lifeCount, 1000);
    
    // ship
    m_ship = Ship::create();
	//m_ship->setTarget(target);
    addChild(m_ship, m_ship->getZoder(), 1001);
    
    CCMenuItemImage *pause = CCMenuItemImage::create(s_pause, s_pause, this, menu_selector(GameLayer::doPause));
    pause->setAnchorPoint(ccp(1, 0));
    pause->setPosition(ccp(winSize.width, 0));
	CCMenuItemImage *boom = CCMenuItemImage::create(s_pause,s_pause,this,menu_selector(GameLayer::shootBoom));
	boom->setAnchorPoint(ccp(0,0));
    CCMenu *menu = CCMenu::create(pause,boom, NULL);
    menu->setAnchorPoint(ccp(0, 0));
    addChild(menu, 1, 10);
    menu->setPosition(CCPointZero);
    
    // 调 update函数
    scheduleUpdate();
    
    // 每秒调一次 scoreCounter函数
    schedule(schedule_selector(GameLayer::scoreCounter), 1);
    
	m_boss = Boss::create();
	addChild(m_boss,1000,1000);
	enemy_items->addObject(m_boss);
	m_boss->setPosition(ccp(100,650));
	m_boss->setShipTarget(m_ship);

    if (Config::sharedConfig()->getAudioState()) {
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic(s_bgMusic, true);
    }

    return true;
}



void GameLayer::update(float dt)
{

    if (m_state == statePlaying) {
        checkIsCollide();
        removeInactiveUnit(dt);
        checkIsReborn();
        updateUI();
		//导弹跟踪1
		if(m_ship){
			if(!m_ship->getTarget() || !m_ship->getTarget()->isActive()){
				if((enemy_items->count())>0){
				   Enemy *targetc = dynamic_cast<Enemy*>(enemy_items->objectAtIndex(0));
				   m_ship->setTarget(targetc,NULL);
				}
			}

			//导弹跟踪2
			if(!m_ship->getTargetTwo() || !m_ship->getTargetTwo()->isActive()){
				if((enemy_items->count())>1){
				   Enemy *targetc = dynamic_cast<Enemy*>(enemy_items->objectAtIndex(enemy_items->count()-1));
				   m_ship->setTargetTwo(targetc,NULL);
				}
			}
		}

		if(m_ship){
			if(m_ship->getBomb()){
			   checkBoomCollide();
		    }
		}
    }
    
}

void GameLayer::scoreCounter(float f)
{
    if (m_state == statePlaying) {
        m_time++;
        //m_levelManager->loadLevelResource(m_time);
    }
}

void GameLayer::checkIsCollide()
{
	for(int i=enemy_items->count()-1;i>=0;i--){

		Enemy *enemy = dynamic_cast<Enemy*>(enemy_items->objectAtIndex(i));

		for(int j=play_bullet->count()-1;j>=0;j--){
			Bullet *bullet = dynamic_cast<Bullet*>(play_bullet->objectAtIndex(j));
            if (this->collide(enemy, bullet)) {
				enemy->hurt(bullet->getPower());
                bullet->hurt();
            }
            if (!(m_screenRec.intersectsRect(bullet->boundingBox()))) {
                bullet->destroy();
            }
		}
		if (collide(enemy, m_ship)) {
            if (m_ship->isActive()) {
                enemy->hurt();
                m_ship->hurt();
            }
            
        }
        if (!(m_screenRec.intersectsRect(enemy->boundingBox()))) {
            enemy->destroy();
        }

	}

	for(int k=enemy_bullet->count()-1;k>=0;k--){
		Bullet *enemyb = dynamic_cast<Bullet*>(enemy_bullet->objectAtIndex(k));
        if (enemyb) {
            if (collide(enemyb, m_ship)) {
                if (m_ship->isActive()) {
					enemyb->hurt();
                    m_ship->hurt(enemyb->getPower());
                }
            }

            if (!m_screenRec.intersectsRect(enemyb->boundingBox())) {
                enemyb->destroy();
            }
        }
	}
}

void GameLayer::checkBoomCollide(){
	for(int i=enemy_items->count()-1;i>=0;i--){
		Enemy *enemy = dynamic_cast<Enemy*>(enemy_items->objectAtIndex(i));
		if(this->collide(enemy,m_ship->getBomb())){
			enemy->hurt();
		}
	}

	for(int k=enemy_bullet->count()-1;k>=0;k--){
		UnitSprite *enemyb = dynamic_cast<UnitSprite*>(enemy_bullet->objectAtIndex(k));
		if (enemyb) {
			if (collide(enemyb, m_ship->getBomb())) {
			    enemyb->hurt();
			}
		}
	}
}

void GameLayer::removeInactiveUnit(float dt)
{
        
    CCArray *children = this->getChildren();
    
	for (int i = children->count()-1; i>=0; i--) {
        UnitSprite *selChild =  dynamic_cast<UnitSprite*>(children->objectAtIndex(i));
        if (selChild) {
            selChild->update(dt);
            int tag = selChild->getTag();
            if (( tag == 900) || (tag == 901 )|| (tag == 1000)) {
                if (!(selChild->isActive())) {
				     //CCLog("destory----------------------------------");
                    selChild->destroy();
                }
            }
        }
    }

    if (m_ship) {
        if (!m_ship->isActive()) {
            m_ship->destroy();
            m_ship = NULL;
        }
    }
	//delete boss when it have be destoryed
	//这里面会出现未知的错误，那就算了不release了
	//
	/*
	if(m_boss){
		if(!m_boss->isActive()){
			m_boss->release();
			m_boss = NULL;
		}
	}
    */
}

void GameLayer::checkIsReborn()
{    
    if (Config::sharedConfig()->getLifeCount() > 0) {
        if (!m_ship) {
                m_ship = Ship::create();
				//m_ship->setTarget(target);
				if(m_boss){
					if(m_boss->isActive()){
						m_boss->setShipTarget(m_ship);
					}		   
				}
                this->addChild(m_ship, m_ship->getZoder(), 1001);
        }
        
    }
    else if (Config::sharedConfig()->getLifeCount() <= 0 ) {
        m_state = stateGameOver;
        m_ship = NULL;
        CCCallFunc *gameOver = CCCallFunc::create(this, callfunc_selector(GameLayer::gameOver));
        this->runAction(CCSequence::create(CCDelayTime::create(0.2), gameOver, NULL));
    }
    
}

bool GameLayer::collide(UnitSprite *a, UnitSprite *b)
{
    if(!a || !b)
    {
        return false;
    }
    CCRect aRect = a->collideRect();
    CCRect bRect = b->collideRect();
    if (aRect.intersectsRect(bRect)) {
        return true;
    }
    return false;
}

void GameLayer::updateUI()
{
    if (m_tempScore < Config::sharedConfig()->getScoreValue()) {
        m_tempScore += 5;
    }
    char score[20];
    char s[100] = "Score:";
    sprintf_s(score, "%d", m_tempScore);
	strcat_s(s,sizeof(s), score);
    m_lbScore->setString(s);
    char lifecount[2];
    sprintf_s(lifecount, "%d",Config::sharedConfig()->getLifeCount());
    m_lifeCount->setString(lifecount);
}

void GameLayer::onEnter()
{
	CCLog("onEnter");
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    CCLayer::onEnter();
}

void GameLayer::onExit()
{
	CCLog("onExit");
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}


bool GameLayer::ccTouchBegan(CCTouch *touch, CCEvent *event)
{
//    CCLog("touch began!");
    return true;
}

void GameLayer::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{

	if ((m_state == statePlaying) && m_ship&&m_ship->isCanBeMove()) {
    CCPoint pos = touch->getDelta();
    CCPoint currentPos = m_ship->getPosition();
    currentPos = ccpAdd(currentPos, pos);
    currentPos = ccpClamp(currentPos, CCPointZero, ccp(winSize.width, winSize.height));
    m_ship->setPosition(currentPos);
    }
    
}

void GameLayer::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
//    CCLog("touch end!");
}

// 无限滚动地图，采用两张图循环加载滚动
void GameLayer::initBackground()
{
    m_backSky = CCSprite::create(s_bg01);
    //星空闪烁
	m_backSky->runAction(CCRepeatForever::create(this->getBackAnimate()));
    m_backSky->setAnchorPoint(ccp(0, 0));
    m_backSkyHeight = m_backSky->getContentSize().height;
    addChild(m_backSky, -10);
    // Tile map
    m_backTileMap = CCTMXTiledMap::create(s_level01);
    addChild(m_backTileMap, -9);
    m_backTileMapHeight = m_backTileMap->getMapSize().height * m_backTileMap->getTileSize().height;

	CCSprite* test = CCSprite::create(s_play);
	test->setPosition(ccp(415,900));
	m_backTileMap->addChild(test,1);


    m_backSkyHeight -= 50;
    m_backTileMapHeight -= 200;
    m_backSky->runAction(CCMoveBy::create(3, ccp(0, -50)));
    m_backTileMap->runAction(CCMoveBy::create(3, ccp(0, -200)));
    
    schedule(schedule_selector(GameLayer:: movingBackground),3);
    
	if (m_backSkyHeight <= winSize.height) {
        if (!m_isBackSkyReload) {
            
            // 如果另一张图还没加载则create一个
            m_backSkyRe = CCSprite::create(s_bg01);
			 //星空闪烁
			m_backSkyRe->runAction(CCRepeatForever::create(this->getBackAnimate()));
            m_backSkyRe->setAnchorPoint(ccp(0, 0));
            addChild(m_backSkyRe, -10);
			m_backSkyRe->setPosition(ccp(0, winSize.height));
            // 反转标志位
            m_isBackSkyReload = true;
        }
        // 第二张图紧接着第一张图滚动
        m_backSkyRe->runAction(CCMoveBy::create(3, ccp(0, -50)));
    }
}

// 这里就是视差背景了
void GameLayer::movingBackground(float f)
{
    m_backSky->runAction(CCMoveBy::create(3, ccp(0, -50)));
    m_backTileMap->runAction(CCMoveBy::create(3, ccp(0, -200)));
    
    // 每次移动48
    m_backSkyHeight -= 50;
    
    // 每次移动200
    m_backTileMapHeight -= 200;
    

    // 图的顶部到达屏幕顶部时
    if (m_backSkyHeight <= winSize.height) {
        if (!m_isBackSkyReload) {
            
            // 如果另一张图还没加载则create一个
            m_backSkyRe = CCSprite::create(s_bg01);
			 //星空闪烁
			m_backSkyRe->runAction(CCRepeatForever::create(this->getBackAnimate()));
            m_backSkyRe->setAnchorPoint(ccp(0, 0));
            addChild(m_backSkyRe, -10);
            m_backSkyRe->setPosition(ccp(0, winSize.height));
            
            // 反转标志位
            m_isBackSkyReload = true;
        }
        // 第二张图紧接着第一张图滚动
        m_backSkyRe->runAction(CCMoveBy::create(3, ccp(0, -50)));
    }
    
    // 第一张图完全经过屏幕
    if (m_backSkyHeight <= 0) {
        m_backSkyHeight = m_backSky->getContentSize().height;
        // 移除第一张的精灵
        this->removeChild(m_backSky, true);
        
        // 指向第二张图的精灵
        m_backSky = m_backSkyRe;
        
        // 第二张的精灵指针置空
        m_backSkyRe = NULL;
        
        // 反转标志位
        m_isBackSkyReload = false;
    }
    
    if (m_backTileMapHeight <= winSize.height) {
        if (!m_isBackTileReload) {
            m_backTileMapRe = CCTMXTiledMap::create(s_level01);
            this->addChild(m_backTileMapRe, -9);
            m_backTileMapRe->setPosition(0, winSize.height);
            m_isBackTileReload = true;
        }
        m_backTileMapRe->runAction(CCMoveBy::create(3, ccp(0, -200)));
    }
    
    if (m_backTileMapHeight <= 0) {
        m_backTileMapHeight = m_backTileMap->getMapSize().height * m_backTileMap->getTileSize().height;
        this->removeChild(m_backTileMap, true);
        m_backTileMap = m_backTileMapRe;
        m_backTileMapRe = NULL;
        m_isBackTileReload = false;
    }
}

void GameLayer::gameOver()
{
    CCScene * scene = GameOver::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.2, scene));
}

CCAnimate* GameLayer::getBackAnimate(){
	        CCTexture2D * shipTextureCache1 = CCTextureCache::sharedTextureCache()->addImage(s_bg01);
			CCTexture2D * shipTextureCache2 = CCTextureCache::sharedTextureCache()->addImage(s_bg02);

			CCSpriteFrame *frame0 = CCSpriteFrame::createWithTexture(shipTextureCache1, CCRectMake(0, 0, 480, 800));
			CCSpriteFrame *frame1 = CCSpriteFrame::createWithTexture(shipTextureCache2, CCRectMake(0, 0, 480, 800));

			CCArray *animFrames = CCArray::create();
			animFrames->addObject(frame0);
			animFrames->addObject(frame1);
			CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames, 0.3);
			CCAnimate *animate = CCAnimate::create(animation);
			return animate;
}

void GameLayer::doPause(CCObject *pSender)
{
    CCDirector::sharedDirector()->pause();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
    PauseLayer *pauseLayer = PauseLayer::create();
    addChild(pauseLayer,9999);
}

Ship* GameLayer::getShip()
{
    return m_ship;
}

void GameLayer::shootBoom(CCObject *pSender){
	m_ship->addBombChild();
}