#include "util.h"

CCTexture2D* Util::getGrayTexture(const char* filename){
		typedef enum {  
			RED = 0,  
			GREEN = 1,  
			BLUE = 2,  
			ALPHA = 3  
		} PIXELS;  
		CCImage *image = new CCImage();
		image->initWithImageFile(filename);
		unsigned char* pixels = image->getData();
		int len = image->getDataLen();
		uint32_t *p = (uint32_t *)pixels;
		for(int i=0;i<len;i++){
			uint8_t *rgbaPixel = (uint8_t *) &p[i];
			//uint32_t gray = 0.3 * rgbaPixel[RED] + 0.59 * rgbaPixel[GREEN] + 0.11 * rgbaPixel[BLUE];
			uint32_t gray = 1.2*rgbaPixel[RED] + 1.0*rgbaPixel[GREEN] + 0.0*rgbaPixel[BLUE];
			// set the pixels to gray  
			rgbaPixel[RED] = gray;  
			rgbaPixel[GREEN] = gray;  
			rgbaPixel[BLUE] = gray;  
		} 

		CCTexture2D* texture = new CCTexture2D();  
		texture->autorelease();
		texture->initWithImage(image);
		image->release();

		return texture;
}

CCTexture2D* Util::getGrayTexture(CCTexture2D* texture){
		typedef enum {  
			RED = 0,  
			GREEN = 1,  
			BLUE = 2,  
			ALPHA = 3  
		} PIXELS;  
		CCSize textSize = texture->getContentSize();
		CCSprite *temporarySprite = CCSprite::createWithTexture(texture);
		//就这么一小句搞了我半天了，不过能出来也值了
		temporarySprite->setAnchorPoint(ccp(0,0));
		CCRenderTexture *rt = CCRenderTexture::create(textSize.width, textSize.height);
		rt->begin();
		temporarySprite->visit();
		rt->end();
		
		CCImage *image = rt->newCCImage();
		unsigned char* pixels = image->getData();
		int len = image->getDataLen();
		uint32_t *p = (uint32_t *)pixels;
		for(int i=0;i<len;i++){
			uint8_t *rgbaPixel = (uint8_t *) &p[i];
			//uint32_t gray = 0.3 * rgbaPixel[RED] + 0.59 * rgbaPixel[GREEN] + 0.11 * rgbaPixel[BLUE];
			uint32_t gray = 1.2*rgbaPixel[RED] + 1.0*rgbaPixel[GREEN] + 0.0*rgbaPixel[BLUE];
			// set the pixels to gray  
			rgbaPixel[RED] = gray;  
			rgbaPixel[GREEN] = gray;  
			rgbaPixel[BLUE] = gray;  
		}
		CCTexture2D* newtexture = new CCTexture2D();  
		newtexture->autorelease();
		newtexture->initWithImage(image);

		image->release();
		return newtexture;
}