#pragma once

#include <Geode/Geode.hpp>

class CustomRenderTexture : public cocos2d::CCRenderTexture { // stolen from matthew >:D
public:
    unsigned char* getData() {
        const auto size = m_pTexture->getContentSizeInPixels();
        auto savedBufferWidth = (int)size.width;
        auto savedBufferHeight = (int)size.height;
        GLubyte* pTempData = nullptr;
        pTempData = new GLubyte[savedBufferWidth * savedBufferWidth * 4];
        this->begin();
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glReadPixels(0, 0, savedBufferWidth, savedBufferHeight, GL_RGBA, GL_UNSIGNED_BYTE, pTempData);
        this->end();
        return pTempData;
    }
    const auto& getSizeInPixels() {
        return m_pTexture->getContentSizeInPixels();
    }
};

class Screenshot {
    public:
        Screenshot() {};
        static Screenshot create(bool hidePlayer, bool hideUI);
        void saveImage(bool toClipboard);

    protected:
        unsigned char* imageData;
        cocos2d::CCSize contentSize;

        std::string getFileName();
};