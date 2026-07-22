#include "api/zelda_view.hpp"

#include "api/zelda_view_model.hpp"

using namespace DeclarativeUI;

namespace App::Zelda {
namespace {

View pill(const String& label, Color color)
{
    return Text(label).foregroundColor(color).padding(8, 4)
        .background(Color::hex(0x162A3E)).cornerRadius(8);
}

View details(const CompendiumEntry& entry)
{
    return VStack({
        ToolBar({ NavigationBackLink(), Title(entry.name), Spacer() }),
        ScrollView({
            VStack({
                Image(entry.imageUrl.c_str(), ImageSource::URL).imageStorageSize(320, 320).fillWidth().height(210)
                    .background(Color::hex(0x10283A)).cornerRadius(18),
                VStack({
                    HStack({ pill(entry.category, Color::Cyan()), Spacer(), Text(String::format("#%d", entry.id)).foregroundColor(Color::LightGray()) }),
                    Text(entry.description).foregroundColor(Color::White()),
                    Title("Common locations"),
                    Text(entry.locations).foregroundColor(Color::hex(0xB7D8E8)),
                    Title("Recoverable materials"),
                    Text(entry.drops).foregroundColor(Color::hex(0xF5CF8E))
                }).fillWidth().padding(18).spacing(12).background(Color::hex(0x173147)).cornerRadius(18)
            }).fillWidth().spacing(16)
        }).grow().fillWidth().paddingRight(6)
    }).fill().padding(18).spacing(14).background(Color::hex(0x071723));
}

View row(const CompendiumEntry& entry)
{
    View label = HStack({
        Image(entry.imageUrl.c_str(), ImageSource::URL).imageStorageSize(96, 96).frame(74, 74)
            .background(Color::hex(0x0A2131)).cornerRadius(12),
        VStack({
            Text(entry.name).foregroundColor(Color::White()),
            Text(entry.description).foregroundColor(Color::hex(0x9FC4D5)).height(38),
            pill(entry.category, Color::Cyan())
        }).grow().spacing(5),
        Text(">").foregroundColor(Color::Cyan())
    }).fill().spacing(12).alignment(Alignment::Center);

    return NavigationLink(label, details(entry))
        .fillWidth().height(104).background(Color::hex(0x143047)).cornerRadius(16)
        .border(Color::hex(0x28516B), 1).padding(12);
}

} // namespace

View compendium(const char* ssid, const char* password)
{
    static StateObject<CompendiumViewModel> model;
    model->load(ssid, password);

    View entries = ForEach(
        model->entries,
        [](const CompendiumEntry& entry) { return entry.id; },
        [](const CompendiumEntry& entry) { return row(entry); }
    ).spacing(12);

    View home = VStack({
        ToolBar({ Title("Hyrule Compendium"), Spacer(), pill("BOTW", Color::Yellow()) }),
        Text("Monsters of Breath of the Wild").foregroundColor(Color::hex(0x93BED1)),
        Text(Bind(model->status)).foregroundColor(Color::Cyan()),
        Loading().hidden(Bind(model->loading, [](bool value) { return !value; })),
        ScrollView({entries}).grow().fillWidth().paddingRight(6)
    }).fill().padding(18).spacing(13).background(Color::hex(0x071723));

    return NavigationStack({home});
}

} // namespace App::Zelda
