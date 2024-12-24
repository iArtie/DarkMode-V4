#include "TexturePackCell.hpp"
#include "Geode/binding/CCMenuItemSpriteExtra.hpp"

using namespace geode::prelude;



    TexturePackCell* TexturePackCell::create(const CreateArg& arg) {
        TexturePackCell* ret = new TexturePackCell();

    if (ret && ret->init(arg)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

bool TexturePackCell::init(const CreateArg& arg) {
    if (!CCLayer::init()) {
        return false;
    }

    setContentSize(arg.size);
   
    CCNode* icon = createIcon(arg.iconFrame);
    CCNode* title = createTitle(arg.titleFrame);
    CCNode* version = createVersion(arg.version);

    CCNode* playButton = createViewButton(
        arg.target,
        arg.callback
    );

    addChild(icon);
    addChild(title);
    addChild(version);
    addChild(playButton);

    return true;
}

CCNode* TexturePackCell::createIcon(char const* frame) {
    const CCSize container = getContentSize();
    auto* sprite = CCSprite::createWithSpriteFrameName(frame);

    sprite->setScale(0.925f);
    sprite->setPosition({
        container.height / 2.0f,
        container.height / 2.0f,
    });

    return sprite;
}

CCNode* TexturePackCell::createTitle(char const* frame) {
    const CCSize container = getContentSize();
   /* auto* sprite = CCSprite::createWithSpriteFrameName(frame);*/
    auto TexturePackName = CCLabelBMFont::create(frame, "bigFont.fnt");
    TexturePackName->setAnchorPoint({ 0.0f, 0.5f });
    TexturePackName->setPosition({
        (container.height / 2.0f) + 35.0f,
        container.height / 2.0f,
    });

    TexturePackName->setPositionY(TexturePackName->getPositionY() + 20);
    TexturePackName->setScale(0.6f);

    return TexturePackName;
}


CCNode* TexturePackCell::createVersion(char const* frame) {
    const CCSize container = getContentSize();
    /* auto* sprite = CCSprite::createWithSpriteFrameName(frame);*/
    auto TexturePackName = CCLabelBMFont::create(frame, "goldFont.fnt");

    TexturePackName->setString(frame);
    TexturePackName->setAnchorPoint({ 0.0f, 0.5f });
    TexturePackName->setPosition({
        (container.height / 2.0f) + 35.0f,
        (container.height / 2.0f) - 25,
        });

    TexturePackName->setPositionY(TexturePackName->getPositionY() + 20);
    TexturePackName->setScale(0.5f);

    return TexturePackName;
}
//CCNode* TexturePackCell::createTitle(char const* frame) {
//    const CCSize container = getContentSize();
//    /* auto* sprite = CCSprite::createWithSpriteFrameName(frame);*/
//    auto TexturePackName = CCLabelBMFont::create(frame, "bigFont.fnt");
//    TexturePackName->setAnchorPoint({ 0.0f, 0.5f });
//    TexturePackName->setPosition({
//        (container.height / 2.0f) + 25.0f,
//        container.height / 2.0f,
//        });
//
//    TexturePackName->setPositionY(TexturePackName->getPositionY() + 20);
//    TexturePackName->setScale(0.6f);
//
//    return TexturePackName;
//}
CCNode* TexturePackCell::createViewButton(
    CCObject* target,
    SEL_MenuHandler callback
) {
    const CCSize container = getContentSize();

    auto* menu = CCMenu::create();
    menu->setAnchorPoint({ 0.0f, 0.0f });
    menu->setPosition({
        container.width - (container.height / 2.0f) - 5.0f,
        container.height / 2.0f
    });
    menu->setScale(0.6f);
    auto viewButtonSpr = ButtonSprite::create("Get",2);
    viewButtonSpr->setScale(1.7);
   /* auto* sprite = CCSprite::createWithSpriteFrameName("GJ_playBtn2_001.png");*/
    button = CCMenuItemSpriteExtra::create(
        viewButtonSpr,
        nullptr,
        target,
        callback
    );
    button->setPosition({ -35.0f, 0.0f });
    menu->addChild(button);

    return menu;
}
