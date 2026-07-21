#pragma once

#include "compose_view_model.hpp"

namespace DeclarativeUI::Examples::Compose::Components {

View hero(const StateObject<ComposeViewModel>& model);
View progress(const StateObject<ComposeViewModel>& model);
View taskRow(const ComposeTask& task);

} // namespace DeclarativeUI::Examples::Compose::Components
