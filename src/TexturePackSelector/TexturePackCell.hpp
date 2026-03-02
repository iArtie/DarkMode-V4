#pragma once
#include <Geode/Bindings.hpp>

class TexturePackCell : public CCNode {
public:
    CCMenuItemSpriteExtra* button = nullptr;

    struct CreateArg {
        CCSize size;
        std::string_view iconFrame;
        std::string_view titleFrame;
        std::string_view version;
        int downloadsCounter;
        CCObject* target;
        SEL_MenuHandler callback;
    };

    static TexturePackCell* create(const CreateArg& arg);

private:
    bool init(const CreateArg& arg);

    CCNode* createIcon(std::string_view frame);
    CCNode* createTitle(std::string_view frame);
    CCNode* createVersion(std::string_view frame);
    CCNode* createDownloadsCounter(int downloads);
    CCNode* createViewButton(CCObject* target, SEL_MenuHandler callback);
    void setupBackground(const std::string& version);

    CCScale9Sprite* m_cellBG = nullptr;
    float CELL_WIDTH = 395.f;
    float CELL_HEIGHT = 70.f;
    float ICON_SCALE = 0.925f;
    float BUTTON_SCALE = 0.6f;
};