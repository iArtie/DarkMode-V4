#include "utils.hpp"

std::map<ccColor3B, ccColor3B, ColorComparator> layerColorMap = {
    {{194, 114, 62}, {30, 30, 30}},
    {{191, 114, 62}, {30, 30, 30}},
    {{51, 68, 153}, {30, 30, 30}},
    {{114, 63, 31}, {30, 30, 30}},
    {{63, 63, 127}, {30, 30, 30}},
    {{144, 79, 39}, {30, 30, 30}},
    {{0, 13, 98}, {30, 30, 30}},
    {{161, 88, 44}, {48, 48, 48}},
    {{40, 53, 119}, {48, 48, 48}},
    {{168, 85, 44}, {48, 48, 48}},
    {{50, 50, 100}, {48, 48, 48}},
    {{156, 85, 42}, {48, 48, 48}},
    {{0, 0, 83}, {48, 48, 48}}
};

std::map<ccColor3B, ccColor3B, ColorComparator> scale9SpriteMap = {
    {{0, 56, 141}, {20, 20, 20}},
    {{130, 64, 33}, {20, 20, 20}},
    {{0, 46, 117}, {20, 20, 20}},
    {{0, 36, 91}, {20, 20, 20}},
    {{0, 31, 79}, {20, 20, 20}},
    {{123, 60, 31}, {15, 15, 15}},
    {{108, 60, 36}, {15, 15, 15}},
    {{0, 39, 98}, {10, 10, 10}},
    {{76, 42, 25}, {25, 25, 25}},
    {{130, 64, 32}, {20, 20, 20}},
    {{32, 49, 130}, {20, 20, 20}},
    {{191, 114, 62}, {20, 20, 20}},
    {{176, 92, 52}, {20, 20, 20}}
};

void DMUtils::NodePrinter(CCNode* node) {
    if (!node) return;

    // CCLayerColor
    if (auto layer = typeinfo_cast<CCLayerColor*>(node)) {
        auto it = layerColorMap.find(layer->getColor());
        auto parent = node->getParent();

        if (it != layerColorMap.end())
            layer->setColor(it->second);

        if (Mod::get()->getSettingValue<bool>("transparent-lists") && layer->getOpacity() != 10)
        {
            bool isValidCell =
                typeinfo_cast<LevelCell*>(parent) ||
                typeinfo_cast<GJScoreCell*>(parent) ||
                typeinfo_cast<LevelListCell*>(parent) ||
                typeinfo_cast<MapPackCell*>(parent) ||
                dynamic_cast<geode::GenericListCell*>(parent) != nullptr;

            bool isListLayer = typeinfo_cast<GJListLayer*>(node);
            bool hasListView = node->getChildByID("list-view") != nullptr;

            auto grandparent = parent ? parent->getParent() : nullptr;
            auto loadingCircle = parent ? parent->getChildByType<LoadingCircle*>(0) : nullptr;
            bool hasVisibleLoadingCircle = loadingCircle && loadingCircle->isVisible();
            bool isGeodeListView = dynamic_cast<geode::ListView*>(node) != nullptr;
            bool isBackgroundLayer = isListLayer && node->getID() == "background";

            if (!isBackgroundLayer && (isValidCell || (isListLayer && (hasListView || hasVisibleLoadingCircle)) || isGeodeListView)) {
                if (typeinfo_cast<DailyLevelNode*>(grandparent)) {
                    auto size = node->getContentSize();
                    node->setVisible(size.height == 90 && size.width == 160);
                }
                layer->setOpacity(50);
            }
        }
       
    }

    // CCScale9Sprite
    if (auto sprite = typeinfo_cast<CCScale9Sprite*>(node)) {
        auto it = scale9SpriteMap.find(sprite->getColor());
        if (it != scale9SpriteMap.end())
            sprite->setColor(it->second);
    }

    for (auto child : node->getChildrenExt<CCNode>())
        NodePrinter(child);
}
