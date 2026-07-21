#pragma once

#include <memory>
#include <utility>

#include "core/view_internal.hpp"

namespace DeclarativeUI {

template <typename Collection, typename Identifier, typename Builder>
View ForEach(
    const Collection& collection,
    Identifier identifier,
    Builder builder
)
{
    auto node = std::make_shared<ViewNode>();
    node->type = ViewType::ForEach;
    node->style.size.fillWidth = true;
    node->spacing = 12;

    node->dynamicChildren = [collection, identifier, builder] {
        std::vector<View> children;
        const auto values = collection.values();
        for (const auto& value : values) {
            [[maybe_unused]] const auto id = identifier(value);
            children.push_back(builder(value));
        }
        return children;
    };

    node->dynamicSubscription = [collection](std::function<void()> update) {
        return collection.subscribe([update](const auto&) { update(); });
    };

    return View(node);
}

} // namespace DeclarativeUI
