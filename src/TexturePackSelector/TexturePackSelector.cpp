#include <Geode/ui/Notification.hpp>
#include "TexturePackSelector.hpp"
#include "TexturePackCell.hpp"
#include "TexturePackCell.cpp"


static CCSize LAYER_SIZE = { 410.f, 235.f };

bool TexturePackSelector::init()
{
    if (!Popup<>::initAnchored(440.f, 290.f, "GJ_square01.png")) return false;

    GameManager::get()->fadeInMenuMusic();

    this->m_bgSprite->setVisible(false);
  
    this->setID("TexturePackSelector");

    this->setKeypadEnabled(true);


    reloadData();
    
    auto director = CCDirector::sharedDirector();
    auto size = m_mainLayer->getContentSize();

	auto titleSprite = CCSprite::createWithSpriteFrameName("DMv4_label_selector.png"_spr);

  
    titleSprite->setPosition({ size.width / 2, size.height - 27.f });
    titleSprite->setID("title-sprite-text");
    titleSprite->setScale(1.5f);
    m_mainLayer->addChild(titleSprite);

    m_infoLabel = CCLabelBMFont::create("", "chatFont.fnt");
    m_infoLabel->setPosition({ 85, 28 });
    m_infoLabel->setID("info-text");
    m_infoLabel->setScale(.575f);
    m_infoLabel->setOpacity(200.f);
    m_mainLayer->addChild(m_infoLabel);


    m_buttonMenu->setLayout(
        SimpleColumnLayout::create()
        ->setMainAxisDirection(AxisDirection::TopToBottom)
        ->setMainAxisAlignment(MainAxisAlignment::Start)
        ->setMainAxisScaling(AxisScaling::Grow)
    );
    m_buttonMenu->setContentSize(director->getWinSize());

    auto reloadSpr = CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png"_spr);
    reloadSpr->setScale(0.825);

    reloadBtn = CCMenuItemSpriteExtra::create(reloadSpr, this, menu_selector(TexturePackSelector::reloadDataCallBack));
    reloadBtn->setID("reload-button");
    reloadBtn->setPosition(ccp(director->getScreenLeft() + 25.f, director->getScreenBottom() + 24.f));

    reloadBtn->setVisible(false);

	m_buttonMenu->removeAllChildren();

    CCSprite* backSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"_spr);
    CCMenuItemSpriteExtra* backBtn = CCMenuItemSpriteExtra::create(backSpr, this, menu_selector(TexturePackSelector::onBack));

    backBtn->setPosition(ccp(director->getScreenLeft() + 25.f, director->getScreenTop() - 22.f));

    m_buttonMenu->addChild(reloadBtn);
	m_buttonMenu->addChild(backBtn);
   
    float distanceFromCenter = 0.f;
    float heightOffset = -5.f;
    float scale = .88f;

    auto m_background = CCLayerColor::create({ 168,85,44 });
    m_background->setLayout(
        SimpleColumnLayout::create()
        ->setMainAxisDirection(AxisDirection::TopToBottom)
        ->setMainAxisAlignment(MainAxisAlignment::Start)
        ->setMainAxisScaling(AxisScaling::Grow)
    );
  
    m_background->setScale(scale);
    m_background->setOpacity(255);
    m_background->setContentSize(LAYER_SIZE);
    m_background->setPosition(size / 2 + CCPoint{ -distanceFromCenter, heightOffset });
    m_background->setID("list-background");
    m_mainLayer->addChild(m_background,-3);

 
    auto top = CCSprite::createWithSpriteFrameName("DM_top-list.png"_spr);

    top->setPosition({ titleSprite->getPositionX(),titleSprite->getPositionY()+7 });

    top->setScaleX(1.012);
    auto bottom = CCSprite::createWithSpriteFrameName("DM_bottom-list.png"_spr);

    bottom->setPosition({ size.width / 2, 25 });
	bottom->setScaleX(1.011);

	m_mainLayer->addChild(top,-1);
	m_mainLayer->addChild(bottom, -1);

  
    m_mainList = ScrollLayer::create(LAYER_SIZE * scale);
    m_mainList->m_contentLayer->setLayout(
        SimpleColumnLayout::create()
        ->setMainAxisDirection(AxisDirection::TopToBottom)
        ->setMainAxisAlignment(MainAxisAlignment::Start)
        ->setMainAxisScaling(AxisScaling::Grow)
    );
    m_mainList->setPosition(
        size / 2 + CCPoint{ -distanceFromCenter, heightOffset } - LAYER_SIZE * scale / 2
    );
 
	auto left = CCSprite::createWithSpriteFrameName("DM-side-list.png"_spr);
	left->setPosition({ size.width / 2 - LAYER_SIZE.width * scale / 2 - 5, size.height / 2 });

	auto right = CCSprite::createWithSpriteFrameName("DM-side-list.png"_spr);
	right->setPosition({ size.width / 2 + LAYER_SIZE.width * scale / 2 + 5, size.height / 2 });
	right->setFlipX(true);

    left->setScaleY(3.350);
	right->setScaleY(3.350);

    m_mainLayer->addChild(left, -2);
    m_mainLayer->addChild(right, -2);


    const CCSize cellSize = { 410.0f, 80.0f };


    auto loadingCircle = LoadingCircle::create();
		
	loadingCircle->setParentLayer(this);
	loadingCircle->show();

	loadingCircle->setID("selector-loading-circle");

    this->runAction(CCSequence::create(
         	CCDelayTime::create(1.0f), 
			CCCallFunc::create(this, callfunc_selector(TexturePackSelector::loadEnter)),
            CCCallFunc::create(this, callfunc_selector(TexturePackSelector::showSuccessNotification)),
			nullptr
    ));

    m_mainList->m_contentLayer->setContentHeight(m_mainList->getContentHeight());
    m_mainList->m_contentLayer->updateLayout();
	m_mainList->moveToTop();
  
    return true;
}

void TexturePackSelector::onBack(CCObject* sender) {
	keyBackClicked();
}

void TexturePackSelector::onDownloadTP(std::string id) {
    std::string fileName = fmt::format("{}/packs/{}.zip", Loader::get()->getLoadedMod("geode.texture-loader")->getConfigDir(), id);
    std::string downloadLink = Mod::get()->getSavedValue<std::string>(id + "_link");

    if (downloadLink == "empty") {
        FLAlertLayer::create(
            "Oh no",
            "This texture pack is not available! Select another texture pack or wait for it to become available",
            "OK"
        )->show();
        return;
    }

    Notification::create("Downloading...", CCSprite::createWithSpriteFrameName("GJ_timeIcon_001.png"))->show();

    std::filesystem::remove_all(fmt::format("{}/packs/{}", Loader::get()->getInstalledMod("geode.texture-loader")->getConfigDir(), id));

    if (m_progressBar && m_statusBG) {
        m_progressBar->stopAllActions();
        m_progressBar->setValue(0.f);
        m_progressBar->setVisible(true);

        m_statusBG->setContentSize({220,25});
        m_statusBG->setVisible(true);
    }

    auto req = web::WebRequest();
    

    m_downloadTP.bind([this, id](web::WebTask::Event* e) {
        if (auto progress = e->getProgress()) {
            float percent = progress->downloadProgress().value_or(0.f);
            if (m_progressBar) m_progressBar->setValue(percent / 100.f);
            return; 
        }

      
        if (m_progressBar) {
            m_progressBar->setVisible(false);
            m_progressBar->setValue(0.f);
        }
        if (m_statusBG) {
            m_statusBG->setVisible(false);
        }

        if (web::WebResponse* res = e->getValue()) {
            bool success;

            if (res->into(fmt::format("{}/packs/{}.zip", Loader::get()->getInstalledMod("geode.texture-loader")->getConfigDir(), id))) {
                success = true;
            }
            else {
                success = false;
            }

            if (success) {
                Mod::get()->setSavedValue(id + "_installed_version", Mod::get()->getSavedValue<std::string>(id + "_version"));
                TexturePackSelector::loadEnter();
                Notification::create("Download Successful", CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png"))->show();
            }
            else {
                Notification::create("Download Failed", CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png"))->show();
                std::filesystem::remove(fmt::format("{}/packs/{}.zip", Loader::get()->getInstalledMod("geode.texture-loader")->getConfigDir(), id));
            }
        }
        else if (e->isCancelled()) {
            log::info("Download was cancelled.");
        }
        });

    m_downloadTP.setFilter(req.get(downloadLink));
}

void TexturePackSelector::onSelectTP(CCObject* sender)
{
auto button = typeinfo_cast<CCMenuItemSpriteExtra*>(sender);

geode::createQuickPopup(
	"Download Texture Pack",
	"Are you sure you want to download the " + button->getID() + " version? ",
	"No", "Yes",      // buttons
	[this, button](auto, bool btn2) {
		if (btn2) {
			onDownloadTP(button->getID());
		}
	}

);


}

void TexturePackSelector::showSuccessNotification() {
    Notification::create("Index Updated!", CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png"))->show();
}

void TexturePackSelector::loadEnter()
{
		TexturePackSelector::reloadData();

		auto director = CCDirector::sharedDirector();
		auto winSize = director->getWinSize();
		CCArray* texturepacks = CCArray::create();
        const CCSize cellSize = { 410.0f, 80.0f };

       

        auto size = m_mainLayer->getContentSize();
        float distanceFromCenter = 0.f;
        float heightOffset = -5.f;
        float scale = .88f;

        
    
        m_statusBG = CCScale9Sprite::create("square02b_small.png");
		m_statusBG->setColor({ 0, 0, 0 });
		m_statusBG->setOpacity(150);
        m_statusBG->setContentSize({ 200, 20 });
        m_statusBG->setVisible(false);
        m_statusBG->setPosition({ size.width / 2, 25 });
        m_mainLayer->addChild(m_statusBG);


        m_progressBar = Slider::create(nullptr, nullptr);
        m_progressBar->setID("progress-bar");
        m_progressBar->m_touchLogic->m_thumb->setVisible(false);
        m_progressBar->setScale(1.f);
        m_progressBar->setContentSize({ 200, 20 }); 
        m_progressBar->setAnchorPoint({ 0.5f, 0.5f });
        m_progressBar->setPosition(m_statusBG->getPosition());
        m_progressBar->setVisible(false);
        m_mainLayer->addChild(m_progressBar);


        m_mainList->m_contentLayer->removeAllChildrenWithCleanup(true);

        m_mainList = ScrollLayer::create(LAYER_SIZE * scale);


        size_t i = 0;
        for (const auto& name : imageNames) {

            std::string downloadsChar = modes[i] + "_downloads";

            auto node = TexturePackCell::create({
                cellSize,
                name.c_str(),
                modes[i].c_str(),
                versions[i].c_str(),
                Mod::get()->getSavedValue<int>(downloadsChar),
                this,
                menu_selector(TexturePackSelector::onSelectTP)
                });
            node->setScale(.88f);
            node->button->setID(modes[i].c_str());
            m_mainList->m_contentLayer->addChild(node);


            ++i;
        }

        m_mainList->m_contentLayer->setLayout(
            SimpleColumnLayout::create()
            ->setMainAxisDirection(AxisDirection::TopToBottom)
            ->setMainAxisAlignment(MainAxisAlignment::Start)
            ->setMainAxisScaling(AxisScaling::Grow)
        );
        m_mainList->setPosition(
            size / 2 + CCPoint{ -distanceFromCenter, heightOffset } - LAYER_SIZE * scale / 2
        );
        m_mainList->setID("tp-lists");
        m_mainLayer->addChild(m_mainList, -2);

        m_mainList->m_contentLayer->setContentHeight(m_mainList->getContentHeight());
        m_mainList->m_contentLayer->updateLayout();
        m_mainList->moveToTop();

		this->removeChildByID("selector-loading-circle");

        reloadBtn->setVisible(true);
}

void TexturePackSelector::reloadDataCallBack(CCObject* sender)
{

    TexturePackSelector::reloadData();

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();
    CCArray* texturepacks = CCArray::create();
    const CCSize cellSize = { 410.0f, 80.0f };



    auto size = m_mainLayer->getContentSize();
    float distanceFromCenter = 0.f;
    float heightOffset = -5.f;
    float scale = .88f;

	m_mainLayer->removeChild(m_mainList);

    auto loadingCircle = LoadingCircle::create();

    loadingCircle->setParentLayer(this);
    loadingCircle->show();

    loadingCircle->setID("selector-loading-circle");

    m_mainList->m_contentLayer->removeAllChildrenWithCleanup(true);

    m_mainList = ScrollLayer::create(LAYER_SIZE * scale);


    size_t i = 0;
    for (const auto& name : imageNames) {

        std::string downloadsChar = modes[i] + "_downloads";

        auto node = TexturePackCell::create({
            cellSize,
            name.c_str(),
            modes[i].c_str(),
            versions[i].c_str(),
            Mod::get()->getSavedValue<int>(downloadsChar),
            this,
            menu_selector(TexturePackSelector::onSelectTP)
            });
        node->setScale(.88f);
        node->button->setID(modes[i].c_str());
        m_mainList->m_contentLayer->addChild(node);


        ++i;
    }

    m_mainList->m_contentLayer->setLayout(
        SimpleColumnLayout::create()
        ->setMainAxisDirection(AxisDirection::TopToBottom)
        ->setMainAxisAlignment(MainAxisAlignment::Start)
        ->setMainAxisScaling(AxisScaling::Grow)
    );
    m_mainList->setPosition(
        size / 2 + CCPoint{ -distanceFromCenter, heightOffset } - LAYER_SIZE * scale / 2
    );
    m_mainList->setID("tp-lists");
    m_mainLayer->addChild(m_mainList, -2);

    m_mainList->m_contentLayer->setContentHeight(m_mainList->getContentHeight());
    m_mainList->m_contentLayer->updateLayout();
    m_mainList->moveToTop(); 

    this->removeChildByID("selector-loading-circle");


    Notification::create("Index Updated!", CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png"))->show();

}
void TexturePackSelector::reloadData()
{
    std::string dataURL;

    dataURL = "https://raw.githubusercontent.com/iArtie/DarkModeV4-Index/refs/heads/main/texturepacks.json";


    int availableCount = 0;

    // List listener (Thanks minemaker0430)
    m_listListener.bind([&](web::WebTask::Event* e) {
        if (auto res = e->getValue()) {
            if (res->ok()) {

  
                matjson::Value jsonData = res->json().unwrapOrDefault();
                Mod::get()->setSavedValue("cached-json", jsonData);

            }
            else if (e->isCancelled()) {
                log::info("Cancelled request.");
            }
        }
        });

 
    auto listReq = web::WebRequest();
    m_listListener.setFilter(listReq.get(dataURL));

    versions.clear();
    auto jsonData = Mod::get()->getSavedValue<matjson::Value>("cached-json");

    auto knownVariants = Mod::get()->getSavedValue<std::vector<std::string>>("known-variants");
    std::unordered_set<std::string> knownSet(knownVariants.begin(), knownVariants.end());
    std::vector<std::string> newKnownVariants = knownVariants;

   /* std::string currentPack = Mod::get()->getSavedValue<std::string>("current-pack");*/

    for (const auto& mode : modes) {
        if (jsonData.contains(mode)) {
            auto modeData = jsonData[mode];

            if (modeData["Lastest"].contains("version")) {
                auto latestData = modeData["Lastest"];
                std::string version = latestData["version"].as<std::string>().unwrapOrDefault();
                std::string downloadLink = latestData["download"].as<std::string>().unwrapOrDefault();

                Mod::get()->setSavedValue(mode + "_version", version);
                Mod::get()->setSavedValue(mode + "_link", downloadLink);

                if (version == "empty" || version.empty()) {
                    versions.push_back("not available");
                }
                else {
                    if (!knownSet.count(mode)) {
                        versions.push_back("New!");
                        newKnownVariants.push_back(mode);
                    }
                    else {
                        std::string installedVersion = Mod::get()->getSavedValue<std::string>(mode + "_installed_version");

						std::cout << "Installed version for " << mode << ": " << installedVersion << std::endl;

						std::cout << "Current version for " << mode << ": " << version << std::endl;
                        if (installedVersion.empty())
                        {
                            versions.push_back("v" + version);
                        }
                        else
                        {
                            if (installedVersion != version) {
                                versions.push_back("Update!");
                            }
                            else
                            {
                                versions.push_back("v" + version);
                            }
                        }
                       
                    }
      
                    availableCount++;
                }
            }
            else {
                Mod::get()->setSavedValue(mode + "_version", empty);
                Mod::get()->setSavedValue(mode + "_link", empty);
                versions.push_back("not available");
             
            }
        }
        else {
            Mod::get()->setSavedValue(mode + "_version", empty);
            Mod::get()->setSavedValue(mode + "_link", empty);
            versions.push_back("not available");
        }
    }

   
    Mod::get()->setSavedValue("known-variants", newKnownVariants);


    if (m_infoLabel) {
        m_infoLabel->setString(fmt::format("Available: {}", availableCount).c_str());
    }

    reloadDownloadCounts();

}


void TexturePackSelector::reloadDownloadCounts() {
    std::string countURL = "https://raw.githubusercontent.com/iArtie/DarkModeV4-Index/refs/heads/main/downloadscount.json";

    m_downloadListener.bind([&](web::WebTask::Event* e) {
        if (auto res = e->getValue()) {
            if (res->ok()) {
                matjson::Value countData = res->json().unwrapOrDefault();
                Mod::get()->setSavedValue("cached-counts", countData);

                for (const auto& mode : modes) {
                    if (countData.contains(mode)) {
                        auto modeData = countData[mode];

                      
                        std::string savedVersion = Mod::get()->getSavedValue<std::string>(mode + "_version");
                        if (modeData.contains(savedVersion)) {
                            auto versionInfo = modeData[savedVersion];
                            int downloadCount = versionInfo["download count"].as<int>().unwrapOrDefault();

                            // Add version to the versions list
                            if (downloadCount == 0) {
                                downloadsCounter.push_back("no downloads");
                            }
                            else {
                                downloadsCounter.push_back(std::to_string(downloadCount));
                            }

                            Mod::get()->setSavedValue(mode + "_downloads", downloadCount);
                        }
                        else {
                            Mod::get()->setSavedValue(mode + "_downloads", 0);
                        }
                    }
                    else {
                        Mod::get()->setSavedValue(mode + "_downloads", 0);
                    }
                }
            }
        }
        });

    auto downloadReq = web::WebRequest();
    m_downloadListener.setFilter(downloadReq.get(countURL));
}



TexturePackSelector* TexturePackSelector::create() {
    auto ret = new TexturePackSelector();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}