#pragma once

#include <Geode/Geode.hpp>

class CustomRenderTexture : public cocos2d::CCRenderTexture { // stolen from matthew >:D
public:
    const auto& getSizeInPixels() {
        return m_pTexture->getContentSizeInPixels();
    }

    unsigned char* getData() {
        const auto size = getSizeInPixels();
        auto savedBufferWidth = (int)size.width;
        auto savedBufferHeight = (int)size.height;
        GLubyte* pTempData = nullptr;
        pTempData = new GLubyte[savedBufferWidth * savedBufferWidth * 4];
        this->begin();
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glReadPixels(0, 0, savedBufferWidth, savedBufferHeight, GL_RGBA, GL_UNSIGNED_BYTE, pTempData);
        this->end();
        GLubyte* pData = nullptr;
        pData = new GLubyte[savedBufferWidth * savedBufferWidth * 4];
        for (int i = 0; i < savedBufferHeight; ++i){
            memcpy(&pData[i * savedBufferWidth * 4], 
                    &pTempData[(savedBufferHeight - i - 1) * savedBufferWidth * 4], 
                    savedBufferWidth * 4);
        }
        return pData;
    }
};

class Screenshot {
    public:
        Screenshot() {};
        static Screenshot create(bool hidePlayer, bool hideUI);
        void saveImage(bool toClipboard);

        unsigned char* imageData;
        cocos2d::CCSize contentSize;

    protected:

        std::string getFileName();
};