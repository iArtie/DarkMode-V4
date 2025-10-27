#pragma once
#include <Geode/Geode.hpp>
#include <map>

using namespace geode::prelude;

struct ColorComparator {
    bool operator()(const ccColor3B& a, const ccColor3B& b) const {
        if (a.r != b.r) return a.r < b.r;
        if (a.g != b.g) return a.g < b.g;
        return a.b < b.b;
    }
};

namespace DMUtils {
    void NodePrinter(CCNode* node);
}
