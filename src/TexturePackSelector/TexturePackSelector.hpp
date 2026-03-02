#pragma once
#include <Geode/DefaultInclude.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/ScrollLayer.hpp>
#include <filesystem>

using namespace geode::prelude;

class TexturePackSelector : public Popup {
protected:

    ScrollLayer* m_mainList = nullptr;
    CCLabelBMFont* m_infoLabel = nullptr;
    CCScale9Sprite* m_statusBG = nullptr;
    Slider* m_progressBar = nullptr;
    CCMenuItemSpriteExtra* m_reloadBtn = nullptr;

    async::TaskHolder<web::WebResponse> m_downloadTP;
    async::TaskHolder<web::WebResponse> m_listHolder;
    async::TaskHolder<web::WebResponse> m_downloadHolder;

    inline static const char* INDEX_URL =
        "https://raw.githubusercontent.com/iArtie/DarkModeV4-Index/refs/heads/main/texturepacks.json";

    inline static const char* COUNT_URL =
        "https://raw.githubusercontent.com/iArtie/DarkModeV4-Index/refs/heads/main/downloadscount.json";

    inline static const std::vector<std::string> modes = {
        "METAL","BLOOD","EARTH","FIRE","ICE",
        "LAVA","LIGHT","MOON","POISON","SHADOW",
        "SOUL","STAR","BONUS"
    };

    inline static const std::vector<std::string> imageNames = {
        "DMv4_Metal_Btn.png"_spr,"DMv4_Blood_Btn.png"_spr,
        "DMv4_Earth_Btn.png"_spr,"DMv4_Fire_Btn.png"_spr,
        "DMv4_Ice_Btn.png"_spr,"DMv4_Lava_Btn.png"_spr,
        "DMv4_Light_Btn.png"_spr,"DMv4_Moon_Btn.png"_spr,
        "DMv4_Poison_Btn.png"_spr,"DMv4_Shadow_Btn.png"_spr,
        "DMv4_Soul_Btn.png"_spr,"DMv4_Star_Btn.png"_spr,
        "DMv4_Bonus_Btn.png"_spr
    };

    struct PackInfo {
        std::string version;
        std::string downloadLink;
        int downloads = 0;
        bool isNew = false;
        bool hasUpdate = false;
        bool isAvailable = false;
    };

    float SCALE = .88f;
    CCSize LAYER_SIZE = { 410.f, 235.f };
    float TITLE_Y_OFFSET = 27.f;
    float INFO_X = 85.f;
    float INFO_Y = 28.f;
    const CCSize CELL_SIZE = { 410.0f, 80.0f };
    std::unordered_map<std::string, PackInfo> m_packInfo;
    std::function<void(int)> m_updateCallback = nullptr;

    bool init() override;
    void setupUI();
    void createList();
    void loadCachedData();
    void savePackData();

    void reloadData();
    void reloadDownloadCounts();
    void processJsonData(const matjson::Value& jsonData);
    void processCountsData(const matjson::Value& countData);

    void handleDownloadResponse(web::WebResponse& res, const std::string& id);
    bool isValidZip(const std::string& path);
    std::string getHttpError(int code);
    void migrateAndCleanSaves();
    void showLoadingCircle(bool show);
    void showProgressBar(bool show);
    void showNotification(const std::string& text, const std::string& sprite);

    static std::string getColoredText(const std::string& mode);

public:
    static TexturePackSelector* create();

    void reloadDataCallBack(CCObject*);
    void onSelectTP(CCObject*);
    void onDownloadTP(std::string id);
    void onBack(CCObject*);
    static void getPendingUpdates(std::function<void(int)> callback);

    void setUpdateCallback(std::function<void(int)> cb) { m_updateCallback = cb; }
};