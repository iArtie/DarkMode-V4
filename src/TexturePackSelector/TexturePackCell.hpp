#pragma once

#include <Geode/Bindings.hpp>


class TexturePackCell : public CCNode {
public:

    CCMenuItemSpriteExtra* button;
    struct CreateArg {
        cocos2d::CCSize size;
        char const* iconFrame;
        char const* titleFrame;
        char const* version;
		int downloadsCounter;
        cocos2d::CCObject* target;
        cocos2d::SEL_MenuHandler callback;
		
    };
    CCScale9Sprite* cellBG = nullptr;
    static TexturePackCell* create(const CreateArg& arg);

private:
    bool init(const CreateArg& arg);

    cocos2d::CCNode* createIcon(char const* frame);
    cocos2d::CCNode* createTitle(char const* frame);

    cocos2d::CCNode* createVersion(char const* frame);
	cocos2d::CCNode* createDownLoadsCounter(int downloads);
    cocos2d::CCNode* createViewButton(
        cocos2d::CCObject* target,
        cocos2d::SEL_MenuHandler callback
    );
};

