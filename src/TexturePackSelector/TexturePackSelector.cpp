#include <Geode/ui/Notification.hpp>
#include "TexturePackSelector.hpp"
#include "TexturePackCell.hpp"
#include "TexturePackCell.cpp"

bool TexturePackSelector::init() {
    if (!Popup::init(440.f, 290.f, "GJ_square01.png"))
        return false;

    m_bgSprite->setVisible(false);

    setID("TexturePackSelector");
    setKeypadEnabled(true);

    setupUI();
    showLoadingCircle(true);

    loadCachedData();
    createList();
    reloadData();

    return true;
}

std::string TexturePackSelector::getColoredText(const std::string& mode) {
    static const std::unordered_map<std::string, std::string> colorMap = {
        {"BLOOD","<cr>BLOOD</c>"},
        {"EARTH","<cc>EARTH</c>"},
        {"FIRE","<cr>FIRE</c>"},
        {"ICE","<cf>ICE</c>"},
        {"LAVA","<co>LAVA</c>"},
        {"LIGHT","<cc>LIGHT</c>"},
        {"MOON","<cb>MOON</c>"},
        {"POISON","<cg>POISON</c>"},
        {"SHADOW","<ca>SHADOW</c>"},
        {"SOUL","<cp>SOUL</c>"},
        {"STAR","<cy>STAR</c>"},
        {"BONUS","<cy>B</c><co>O</c><cr>N</c><cp>U</c><cb>S</c>"}
    };

    if (auto it = colorMap.find(mode); it != colorMap.end())
        return it->second;

    return mode;
}

void TexturePackSelector::loadCachedData() {
    auto seen = Mod::get()->getSavedValue<std::vector<std::string>>("seen-packs");
    std::unordered_set<std::string> seenSet(seen.begin(), seen.end());

    for (const auto& mode : modes) {
        auto& info = m_packInfo[mode];

        info.version = Mod::get()->getSavedValue<std::string>(mode + "_version");
        info.downloadLink = Mod::get()->getSavedValue<std::string>(mode + "_link");
        info.downloads = Mod::get()->getSavedValue<int>(mode + "_downloads");
        info.isAvailable = !info.version.empty() && info.version != "empty";

        if (!info.isAvailable) continue;

        info.isNew = !seenSet.contains(mode);

        auto installed = Mod::get()->getSavedValue<std::string>(mode + "_installed_version");
        info.hasUpdate = !installed.empty() && installed != info.version;
    }
}

void TexturePackSelector::savePackData() {
    std::vector<std::string> available;

    for (auto& [mode, info] : m_packInfo) {
        if (!info.isAvailable) continue;

        available.push_back(mode);

        Mod::get()->setSavedValue(mode + "_version", info.version);
        Mod::get()->setSavedValue(mode + "_link", info.downloadLink);
        Mod::get()->setSavedValue(mode + "_downloads", info.downloads);
    }

    Mod::get()->setSavedValue("available-packs", available);
}

void TexturePackSelector::showLoadingCircle(bool show) {
    if (show) {
        auto loadingCircle = LoadingCircle::create();
        loadingCircle->setParentLayer(this);
        loadingCircle->show();
        loadingCircle->setID("selector-loading-circle");
    }
    else {
        removeChildByID("selector-loading-circle");
    }
}

void TexturePackSelector::setupUI() {

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();
    auto size = m_mainLayer->getContentSize();

    auto titleSprite = CCSprite::createWithSpriteFrameName("DMv4_label_selector.png"_spr);
    titleSprite->setPosition({ size.width / 2, size.height - TITLE_Y_OFFSET });
    titleSprite->setID("title-sprite-text");
    titleSprite->setScale(1.5f);
    m_mainLayer->addChild(titleSprite);

    m_infoLabel = CCLabelBMFont::create("", "chatFont.fnt");
    m_infoLabel->setPosition({ INFO_X, INFO_Y });
    m_infoLabel->setID("info-text");
    m_infoLabel->setScale(.575f);
    m_infoLabel->setOpacity(200);
    m_mainLayer->addChild(m_infoLabel);

    m_buttonMenu->setLayout(SimpleColumnLayout::create()
        ->setMainAxisDirection(AxisDirection::TopToBottom)
        ->setMainAxisAlignment(MainAxisAlignment::Start)
        ->setMainAxisScaling(AxisScaling::Grow));
    m_buttonMenu->setContentSize(winSize);
    m_buttonMenu->removeAllChildren();

    auto backSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"_spr);
    auto backBtn = CCMenuItemSpriteExtra::create(backSpr, this, menu_selector(TexturePackSelector::onBack));
    backBtn->setPosition({ director->getScreenLeft() + 25.f, director->getScreenTop() - 22.f });
    m_buttonMenu->addChild(backBtn);

    auto reloadSpr = CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png"_spr);
    reloadSpr->setScale(0.825f);
    m_reloadBtn = CCMenuItemSpriteExtra::create(reloadSpr, this, menu_selector(TexturePackSelector::reloadDataCallBack));
    m_reloadBtn->setID("reload-button");
    m_reloadBtn->setPosition({ director->getScreenLeft() + 25.f, director->getScreenBottom() + 24.f });
    m_reloadBtn->setVisible(false);
    m_buttonMenu->addChild(m_reloadBtn);

    auto background = CCLayerColor::create({ 168, 85, 44 });
    background->setLayout(SimpleColumnLayout::create()
        ->setMainAxisDirection(AxisDirection::TopToBottom)
        ->setMainAxisAlignment(MainAxisAlignment::Start)
        ->setMainAxisScaling(AxisScaling::Grow));
    background->setScale(SCALE);
    background->setOpacity(255);
    background->setContentSize(LAYER_SIZE);
    background->setPosition(size / 2 + CCPoint{ 0, -5.f });
    background->setID("list-background");
    m_mainLayer->addChild(background, -3);

    auto top = CCSprite::createWithSpriteFrameName("DM_top-list.png"_spr);
    top->setPosition({ titleSprite->getPositionX(), titleSprite->getPositionY() + 7 });
    top->setScaleX(1.012f);
    m_mainLayer->addChild(top, -1);

    auto bottom = CCSprite::createWithSpriteFrameName("DM_bottom-list.png"_spr);
    bottom->setPosition({ size.width / 2, 25 });
    bottom->setScaleX(1.011f);
    m_mainLayer->addChild(bottom, -1);

    auto left = CCSprite::createWithSpriteFrameName("DM-side-list.png"_spr);
    left->setPosition({ size.width / 2 - LAYER_SIZE.width * SCALE / 2 - 5, size.height / 2 });
    left->setScaleY(3.35f);
    m_mainLayer->addChild(left, -2);

    auto right = CCSprite::createWithSpriteFrameName("DM-side-list.png"_spr);
    right->setPosition({ size.width / 2 + LAYER_SIZE.width * SCALE / 2 + 5, size.height / 2 });
    right->setFlipX(true);
    right->setScaleY(3.35f);
    m_mainLayer->addChild(right, -2);

    m_statusBG = CCScale9Sprite::create("square02b_small.png");
    m_statusBG->setColor({ 0, 0, 0 });
    m_statusBG->setOpacity(150);
    m_statusBG->setContentSize({ 240, 20 });
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
}

void TexturePackSelector::createList() {

    if (m_mainList)
        m_mainList->removeFromParent();

    auto size = m_mainLayer->getContentSize();

    m_mainList = ScrollLayer::create(LAYER_SIZE * SCALE);
    m_mainList->setPosition(size / 2 - (LAYER_SIZE * SCALE) / 2 + CCPoint{ 0, -5.f });

    m_mainList->m_contentLayer->setLayout(
        SimpleColumnLayout::create()
        ->setMainAxisDirection(AxisDirection::TopToBottom)
        ->setMainAxisAlignment(MainAxisAlignment::Start)
        ->setMainAxisScaling(AxisScaling::Grow)
    );

    int availableCount = 0;

    for (size_t i = 0; i < modes.size(); ++i) {
        const auto& mode = modes[i];
        const auto& info = m_packInfo[mode];

        std::string versionText = "not available";

        if (info.isAvailable) {
            availableCount++;

            if (info.isNew)
                versionText = "New!";
            else if (info.hasUpdate)
                versionText = "Update!";
            else
                versionText = "v" + info.version;
        }

        auto cell = TexturePackCell::create({
            CELL_SIZE,
            imageNames[i].c_str(),
            mode.c_str(),
            versionText.c_str(),
            info.downloads,
            this,
            menu_selector(TexturePackSelector::onSelectTP)
            });

        cell->setScale(SCALE);
        cell->button->setID(mode);
        m_mainList->m_contentLayer->addChild(cell);
    }

    if (m_infoLabel)
        m_infoLabel->setString(fmt::format("Available: {}", availableCount).c_str());

    m_mainList->m_contentLayer->updateLayout();
    m_mainList->moveToTop();
    m_mainLayer->addChild(m_mainList, -2);
}

void TexturePackSelector::reloadData() {
    auto req = web::WebRequest();
    req.timeout(std::chrono::seconds(10));

    m_listHolder.spawn(
        req.get(INDEX_URL),
        [this](web::WebResponse res) {

            if (!res.ok()) {
                showLoadingCircle(false);
                showNotification(getHttpError(res.code()), "GJ_deleteIcon_001.png");
                m_reloadBtn->setVisible(true);
                return reloadDownloadCounts();
            }

            processJsonData(res.json().unwrapOrDefault());
            reloadDownloadCounts();
        }
    );
}

void TexturePackSelector::getPendingUpdates(std::function<void(int)> callback) {
    const std::string dataURL = "https://raw.githubusercontent.com/iArtie/DarkModeV4-Index/refs/heads/main/texturepacks.json";

    auto req = web::WebRequest();
    req.timeout(std::chrono::seconds(10));

    auto holder = new async::TaskHolder<web::WebResponse>();

    holder->spawn(
        req.get(dataURL),
        [callback, holder](web::WebResponse res) {
            int updatesFound = 0;

            if (res.ok()) {
                auto jsonData = res.json().unwrapOrDefault();

                for (const auto& mode : modes) {
                    std::string installedVersion = Mod::get()->getSavedValue<std::string>(mode + "_installed_version");

                    if (installedVersion.empty()) continue;

                    if (jsonData.contains(mode) && jsonData[mode].contains("Lastest")) {
                        auto latestData = jsonData[mode]["Lastest"];

                        if (latestData.contains("version")) {
                            std::string latestVersion = latestData["version"].as<std::string>().unwrapOrDefault();

                            if (!latestVersion.empty() && latestVersion != "empty" && latestVersion != installedVersion) {
                                updatesFound++;
                            }
                        }
                    }
                }
            }

            if (callback) {
                callback(updatesFound);
            }

            delete holder;
        }
    );
}

void TexturePackSelector::processJsonData(const matjson::Value& jsonData) {
    auto seenPacks = Mod::get()->getSavedValue<std::vector<std::string>>("seen-packs");
    std::unordered_set<std::string> seenSet(seenPacks.begin(), seenPacks.end());
    std::vector<std::string> newSeenPacks = seenPacks;

    for (auto& [mode, info] : m_packInfo) {
        if (jsonData.contains(mode) && jsonData[mode].contains("Lastest")) {
            auto latestData = jsonData[mode]["Lastest"];

            if (latestData.contains("version")) {
                std::string version = latestData["version"].as<std::string>().unwrapOrDefault();
                std::string downloadLink = latestData["download"].as<std::string>().unwrapOrDefault();

                if (!version.empty() && version != "empty") {
                    bool wasAvailable = info.isAvailable;

                    info.isAvailable = true;
                    info.version = version;
                    info.downloadLink = downloadLink;

                    if (!wasAvailable && !seenSet.count(mode)) {
                        info.isNew = true;
                        newSeenPacks.push_back(mode);
                    }

                    std::string installedVersion = Mod::get()->getSavedValue<std::string>(mode + "_installed_version");
                    info.hasUpdate = !installedVersion.empty() && installedVersion != version;

                    continue;
                }
            }
        }

        info.isAvailable = false;
        info.version = "";
        info.downloadLink = "";
        info.isNew = false;
        info.hasUpdate = false;
    }

    Mod::get()->setSavedValue("seen-packs", newSeenPacks);
    savePackData();
    createList();
    m_reloadBtn->setVisible(true);
    showLoadingCircle(false);
    if (this->isVisible()) {
        showNotification("Index Updated!", "GJ_completesIcon_001.png");
    }
}

void TexturePackSelector::reloadDownloadCounts() {
    const std::string countURL = "https://raw.githubusercontent.com/iArtie/DarkModeV4-Index/refs/heads/main/downloadscount.json";

    auto req = web::WebRequest();
    req.timeout(std::chrono::seconds(10));

    m_downloadHolder.spawn(
        req.get(countURL),
        [this](web::WebResponse res) {
            if (!res.ok()) return;

            auto countData = res.json().unwrapOrDefault();
            processCountsData(countData);
        }
    );
}

void TexturePackSelector::processCountsData(const matjson::Value& countData) {
    bool updated = false;

    for (auto& [mode, info] : m_packInfo) {
        if (info.isAvailable && countData.contains(mode) && countData[mode].contains(info.version)) {
            int downloads = countData[mode][info.version]["download count"].as<int>().unwrapOrDefault();
            if (info.downloads != downloads) {
                info.downloads = downloads;
                updated = true;
            }
        }
    }

    if (updated) {
        savePackData();
        createList();
    }
}

void TexturePackSelector::reloadDataCallBack(CCObject*) {
    showLoadingCircle(true);
    reloadData();
}

void TexturePackSelector::onSelectTP(CCObject* sender) {
    auto button = typeinfo_cast<CCMenuItemSpriteExtra*>(sender);
    const auto& info = m_packInfo[button->getID()];

    if (!info.isAvailable) {
        FLAlertLayer::create("Oh no",
            "This texture pack is not available! Select another texture pack or wait for it to become available",
            "OK")->show();
        return;
    }

    std::string mode = button->getID();
    std::string coloredMode = getColoredText(mode);
    std::string message = "Are you sure you want to download the " + coloredMode + " version?";

    geode::createQuickPopup(
        "Download Texture Pack",
        message.c_str(),
        "No", "Yes",
        [this, button](auto, bool btn2) {
            if (btn2) onDownloadTP(button->getID());
        }
    );
}


void TexturePackSelector::onDownloadTP(std::string id) {
    const auto& info = m_packInfo[id];

    if (!info.isAvailable || info.downloadLink.empty()) {
        FLAlertLayer::create("Oh no",
            "This texture pack is not available! Select another texture pack or wait for it to become available",
            "OK")->show();
        return;
    }

    Notification::create("Downloading...",
        CCSprite::createWithSpriteFrameName("GJ_timeIcon_001.png"))->show();

    std::filesystem::remove_all(fmt::format("{}/packs/{}",
        Loader::get()->getInstalledMod("geode.texture-loader")->getConfigDir(), id));

    showProgressBar(true);

    auto req = web::WebRequest();
    req.timeout(std::chrono::seconds(30));

    req.onProgress([this](web::WebProgress const& progress) {
        if (float percent = progress.downloadProgress().value_or(0.f)) {
            if (m_progressBar) m_progressBar->setValue(percent / 100.f);
        }
        });

    m_downloadTP.spawn(
        req.get(info.downloadLink),
        [this, id](web::WebResponse res) {
            handleDownloadResponse(res, id);
        }
    );
}

std::string TexturePackSelector::getHttpError(int code) {
    if (code <= 0) return "Connection error - No internet";
    if (code == 408 || code == 504) return "Download timeout - Try again";
    if (code >= 400 && code < 500) return "Download failed - File not found";
    if (code >= 500) return "Server error - Try again later";
    return "Download failed - Unknown error";

}

bool TexturePackSelector::isValidZip(const std::string& path) {
    if (!std::filesystem::exists(path) || std::filesystem::file_size(path) < 4)
        return false;

    std::ifstream file(path, std::ios::binary);
    char sig[2];
    file.read(sig, 2);
    return sig[0] == 'P' && sig[1] == 'K';
}

void TexturePackSelector::handleDownloadResponse(web::WebResponse& res, const std::string& id) {
    showProgressBar(false);

    if (!res.ok()) {
        log::error("Download failed: {} - Code: {}",
            res.string().unwrapOr("Unknown error"),
            res.code());

        showNotification(getHttpError(res.code()), "GJ_deleteIcon_001.png");
        return;
    }

    auto basePath = Loader::get()->getInstalledMod("geode.texture-loader")->getConfigDir();

    auto path = fmt::format("{}/packs/{}.zip", basePath, id);
    auto result = res.into(path);

    if (!result.isOk() || !isValidZip(path)) {
        std::filesystem::remove(path);
        showNotification("Invalid ZIP file", "GJ_deleteIcon_001.png");
        return;
    }

    Mod::get()->setSavedValue(id + "_installed_version", m_packInfo[id].version);
    m_packInfo[id].hasUpdate = false;

    savePackData();
    createList();

    showNotification("Download Successful", "GJ_completesIcon_001.png");

    if (m_updateCallback) {
        int remaining = std::count_if(
            m_packInfo.begin(),
            m_packInfo.end(),
            [](auto& p) { return p.second.hasUpdate; }
        );
        m_updateCallback(remaining);
    }
}

void TexturePackSelector::showProgressBar(bool show) {
    if (m_progressBar) {
        m_progressBar->setVisible(show);
        m_progressBar->setValue(0.f);
        m_progressBar->stopAllActions();
    }
    if (m_statusBG) {
        m_statusBG->setVisible(show);
    }
}

void TexturePackSelector::showNotification(const std::string& text, const std::string& spriteFrame) {
    Notification::create(text, CCSprite::createWithSpriteFrameName(spriteFrame.c_str()))->show();
}

void TexturePackSelector::onBack(CCObject*) {
    keyBackClicked();
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