#include "FollowBullet.h"
#include "Resource.h"
#include "Config.h"
#include "Math.h"


FollowBullet::~FollowBullet(){
		target->release();
}

FollowBullet::FollowBullet(int speed, const char* weapon,Enemy *pSender):Bullet(5){
	m_speed = speed;
	omega = 5;
	b = 0;
	target = pSender;
	target->retain();

	this->initWithFile(s_daodan);
	//ccBlendFunc cbl = {GL_SRC_ALPHA, GL_ONE};
    //this->setBlendFunc(cbl);
	this->setScale(0.25);
}

void FollowBullet::update(float dt)
{
    CCPoint position = this->getPosition();
	if(target->isActive()){
		//CCLog("%s%f%s%f","is this ===========> ",target->getPosition().x," :: ",target->getPosition().y);
		targetMove(position,target->getPosition());
	}else{
	    targetMove(position,ccp(-1000,800));
	}
	
	if (m_Hp <= 0) {
        m_active = false;
    }
}

void FollowBullet::targetMove(CCPoint position,CCPoint targetPosition){
	//int c = ((int)(270+atan2(700 - position.y,position.x - 400)*180/M_PI))%360;
	int c = 0;
	int a = 0;
	if(targetPosition.x != -1000){
		if((position.x - targetPosition.x)>0){
			if((position.y - targetPosition.y)>0 && b>0){//第一区间顺时针
				//------------------
				 c = ((int)(270+atan2(targetPosition.y - position.y,position.x - targetPosition.x)*180/M_PI))%360;
				 a = c - b;   //还未旋转的角度
				 if(a!=0 && a>0){
				 int tempb = a<omega?a:omega;
				 b = b + tempb;  //已旋转的角度
				 this->setRotation(b);
			    }
			   //---------------------------
			}else{
				c = ((int)(270+atan2(targetPosition.y - position.y, targetPosition.x - position.x)*180/M_PI))%360;
				c = -c;
				a = c - b;   //还未旋转的角度
				if(a!=0 && a<0){
					int tempb = a>(-omega)?a:(-omega);
					b = b + tempb;  //已旋转的角度
					this->setRotation(b);
				}
			}
		}else{
			if((position.y - targetPosition.y)>0 && b<0){//第二区间逆时针

				c = ((int)(270+atan2(targetPosition.y - position.y, targetPosition.x - position.x)*180/M_PI))%360;
				c = -c;
				a = c - b;   //还未旋转的角度
				if(a!=0 && a<0){
					int tempb = a>(-omega)?a:(-omega);
					b = b + tempb;  //已旋转的角度
					this->setRotation(b);
				}
			   
			}else{
			    c = ((int)(270+atan2(targetPosition.y - position.y,position.x - targetPosition.x)*180/M_PI))%360;
				a = c - b;   //还未旋转的角度
				if(a!=0 && a>0){
				int tempb = a<omega?a:omega;
				b = b + tempb;  //已旋转的角度
				this->setRotation(b);
			   }
			}		
		}
	}
	
	int x2 = position.x+sin(b*M_PI/180)*m_speed;
	int y2 = position.y+cos(b*M_PI/180)*m_speed;
	//CCLog("%s%d%s%d%s%d","=======================update=>",x2,":",y2," b=",b);
	this->setPosition(ccp(x2,y2));
}