#include "ShadowSprite.h"

void ShadowSprite::setShader(int what){
	do{ 

        const GLchar* pszFragSource;
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
			"float grey = dot(texture2D(u_texture, v_texCoord).rgb, vec3(0.0, 0.0, 0.0)); \n "  
			"gl_FragColor = vec4(grey, grey, grey, 0.0); \n " 
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