#include "TexturePackCell.hpp"
#include "Geode/binding/CCMenuItemSpriteExtra.hpp"
#include <unordered_map>

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
    if (!CCNode::init()) {
        return false;
    }

    setContentSize(arg.size);
   
    CCNode* icon = createIcon(arg.iconFrame);
    CCNode* title = createTitle(arg.titleFrame);
    CCNode* version = createVersion(arg.version);
    CCNode* downloads = createDownLoadsCounter(arg.downloadsCounter);
    CCNode* playButton = createViewButton(
        arg.target,
        arg.callback
    );
    const CCSize container = getContentSize();

    cellBG = CCScale9Sprite::create("square02b_001.png", { 0, 0, 80, 80 }
    );

    cellBG->setOpacity(90);
    cellBG->setContentSize({ 395.f, 70.f });
    cellBG->setPosition({
        container.width / 2.0f,
        container.height / 2.0f
        });

    std::unordered_map<std::string, ccColor3B> colorMap = {
        { "Update!", { 0, 255, 0 } },       // green
        { "New!",     { 0, 224, 255 } },     // lightblue
    };

    ccColor3B color = { 0, 0, 0 }; 
    auto it = colorMap.find(arg.version);
    if (it != colorMap.end()) {
        color = it->second;
        downloads->setPosition({
           downloads->getPositionX() + 10,
           downloads->getPositionY()
            });
    }

    cellBG->setColor(color);

    addChild(cellBG, -1);
    addChild(icon);
    addChild(title);
    addChild(version);
    addChild(downloads);
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

    auto versionLabel = CCLabelBMFont::create(frame, "DMFont.fnt"_spr);
    versionLabel->setAnchorPoint({ 0.0f, 0.5f });
    versionLabel->setScale(0.5f);

    auto infoIcon = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    infoIcon->setAnchorPoint({ 0.0f, 0.5f });
    infoIcon->setScale(0.45f);

    float startX = (container.height / 2.0f) + 35.0f;
    float posY = (container.height / 2.0f) - 5.0f;

    infoIcon->setPosition({ startX, posY-1 });

    float labelX = startX + infoIcon->getScaledContentSize().width + 4.0f;
    versionLabel->setPosition({ labelX, posY });

    auto containerNode = CCNode::create();
    containerNode->addChild(infoIcon);
    containerNode->addChild(versionLabel);
    return containerNode;
}


CCNode* TexturePackCell::createDownLoadsCounter(int downloads) {
    const CCSize container = getContentSize();

    auto formatWithCommas = [](int value) -> std::string {
        std::string numStr = std::to_string(value);
        int insertPosition = numStr.length() - 3;
        while (insertPosition > 0) {
            numStr.insert(insertPosition, ",");
            insertPosition -= 3;
        }
        return numStr;
        };

    std::string formattedDownloads = formatWithCommas(downloads);

    auto downloadsLabel = CCLabelBMFont::create(formattedDownloads.c_str(), "bigFont.fnt");
    downloadsLabel->setAnchorPoint({ 0.0f, 0.5f });
    downloadsLabel->setScale(0.4f);
    downloadsLabel->setColor({ 0, 255, 0 });

    auto downloadIcon = CCSprite::createWithSpriteFrameName("GJ_downloadsIcon_001.png");
    downloadIcon->setScale(0.45f);
    downloadIcon->setAnchorPoint({ 0.0f, 0.5f });

    auto containerNode = CCNode::create();

    float startX = (container.height / 2.0f) + 95.0f;
    float posY = (container.height / 2.0f) - 5.0f;
  
    downloadIcon->setPosition({ startX, posY-1 });

   
    float labelX = startX + downloadIcon->getScaledContentSize().width + 2.0f;
    downloadsLabel->setPosition({ labelX, posY });

	if (downloads > 0) {
        containerNode->addChild(downloadIcon);
        containerNode->addChild(downloadsLabel);
	}

    return containerNode;
}


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
    auto viewButtonSpr = ButtonSprite::create("Get","DMFont.fnt"_spr,"DM_button_01.png"_spr, 2);
    viewButtonSpr->setScale(1.7);
   
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
