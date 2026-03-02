#include "TexturePackCell.hpp"


using namespace geode::prelude;

TexturePackCell* TexturePackCell::create(const CreateArg& arg) {
    auto ret = new TexturePackCell();
    if (ret && ret->init(arg)) {
        ret->autorelease();
    }
    else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool TexturePackCell::init(const CreateArg& arg) {
    if (!CCNode::init()) return false;

    setContentSize(arg.size);
    setupBackground(std::string(arg.version));

    
    addChild(createIcon(arg.iconFrame));
    addChild(createTitle(arg.titleFrame));
    addChild(createVersion(arg.version));

    if (!(arg.version == "Update!" || arg.version == "New!"))
    {
        addChild(createDownloadsCounter(arg.downloadsCounter));
    }
   
    addChild(createViewButton(arg.target, arg.callback));

    return true;
}

void TexturePackCell::setupBackground(const std::string& version) {
    m_cellBG = CCScale9Sprite::create("square02b_001.png", { 0, 0, 80, 80 });
    m_cellBG->setOpacity(90);
    m_cellBG->setContentSize({ CELL_WIDTH, CELL_HEIGHT });
    m_cellBG->setPosition(getContentSize() / 2);
    m_cellBG->setColor(ccBLACK);

 
    static const std::unordered_map<std::string, ccColor3B> colorMap = {
        { "Update!", { 0, 255, 0 } },    // green
		{ "New!",     { 0, 224, 255 } },  // light blue
    };

    auto it = colorMap.find(version);
    if (it != colorMap.end()) {
        m_cellBG->setColor(it->second);
    }

    addChild(m_cellBG, -1);
}

CCNode* TexturePackCell::createIcon(std::string_view frame) {
    auto sprite = CCSprite::createWithSpriteFrameName(frame.data());
    sprite->setScale(ICON_SCALE);
    sprite->setPosition({ getContentSize().height / 2, getContentSize().height / 2 });
    return sprite;
}

CCNode* TexturePackCell::createTitle(std::string_view frame) {
    auto label = CCLabelBMFont::create(frame.data(), "bigFont.fnt");
    label->setAnchorPoint({ 0.0f, 0.5f });
    label->setPosition(getContentSize().height / 2 + 35.0f, getContentSize().height / 2 + 20.0f);
    label->setScale(0.6f);
    return label;
}

CCNode* TexturePackCell::createVersion(std::string_view frame) {
    auto versionLabel = CCLabelBMFont::create(frame.data(), "DMFont.fnt"_spr);
    versionLabel->setAnchorPoint({ 0.0f, 0.5f });
    versionLabel->setScale(0.5f);

    auto infoIcon = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    infoIcon->setAnchorPoint({ 0.0f, 0.5f });
    infoIcon->setScale(0.45f);

    float startX = getContentSize().height / 2 + 35.0f;
    float posY = getContentSize().height / 2 - 6.0f;

    infoIcon->setPosition({ startX, posY });
    versionLabel->setPosition(startX + infoIcon->getScaledContentSize().width + 4.0f, posY);

    auto container = CCNode::create();
    container->addChild(infoIcon);
    container->addChild(versionLabel);
    return container;
}

CCNode* TexturePackCell::createDownloadsCounter(int downloads) {
    if (downloads <= 0) return CCNode::create();

    
    std::string numStr = std::to_string(downloads);
    for (int i = numStr.length() - 3; i > 0; i -= 3) {
        numStr.insert(i, ",");
    }

    auto downloadsLabel = CCLabelBMFont::create(numStr.c_str(), "bigFont.fnt");
    downloadsLabel->setAnchorPoint({ 0.0f, 0.5f });
    downloadsLabel->setScale(0.4f);
    downloadsLabel->setColor({ 0, 255, 0 });

    auto downloadIcon = CCSprite::createWithSpriteFrameName("GJ_downloadsIcon_001.png");
    downloadIcon->setScale(0.45f);
    downloadIcon->setAnchorPoint({ 0.0f, 0.5f });

    float startX = getContentSize().height / 2 + 95.0f;
    float posY = getContentSize().height / 2 - 6.0f;

    downloadIcon->setPosition({ startX, posY });
    downloadsLabel->setPosition(startX + downloadIcon->getScaledContentSize().width + 2.0f, posY);

    auto container = CCNode::create();
    container->addChild(downloadIcon);
    container->addChild(downloadsLabel);
    return container;
}

CCNode* TexturePackCell::createViewButton(CCObject* target, SEL_MenuHandler callback) {
    auto menu = CCMenu::create();
    menu->setAnchorPoint({ 0.0f, 0.0f });
    menu->setPosition(getContentSize().width - getContentSize().height / 2 - 5.0f, getContentSize().height / 2);
    menu->setScale(BUTTON_SCALE);

    auto viewButtonSpr = ButtonSprite::create("Get", "DMFont.fnt"_spr, "DM_button_01.png"_spr, 2);
    viewButtonSpr->setScale(1.7f);

    button = CCMenuItemSpriteExtra::create(viewButtonSpr, nullptr, target, callback);
    button->setPosition({ -35.0f, 0.0f });
    menu->addChild(button);

    return menu;
}