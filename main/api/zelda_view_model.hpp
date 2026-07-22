#pragma once

#include "declarative_ui.hpp"

namespace App::Zelda {

struct CompendiumEntry {
    int id = 0;
    DeclarativeUI::String name;
    DeclarativeUI::String category;
    DeclarativeUI::String description;
    DeclarativeUI::String imageUrl;
    DeclarativeUI::String locations;
    DeclarativeUI::String drops;
};

class CompendiumViewModel final : public DeclarativeUI::ObservableObject {
public:
    DeclarativeUI::PublishedList<CompendiumEntry> entries = publishedList(DeclarativeUI::Array<CompendiumEntry>());
    DeclarativeUI::Published<DeclarativeUI::String> status = published(DeclarativeUI::String("Connecting to Hyrule..."));
    DeclarativeUI::Published<bool> loading = published(true);

    void load(const char* ssid, const char* password) const;
};

} // namespace App::Zelda
