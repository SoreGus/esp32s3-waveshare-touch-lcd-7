#include "examples.hpp"

#include "compose_components.hpp"
#include "compose_view_model.hpp"
#include "examples/shared/example_layout.hpp"

namespace DeclarativeUI::Examples::Compose {

View dashboard()
{
    DeclarativeUI::StateObject<ComposeViewModel> model;
    DeclarativeUI::State<bool> showExplanation(true);

    View explanation = Shared::row(
        Text("This screen combines StateObject, Published, State, Bind, ListState, ForEach and ZStack.")
            .foregroundColor(Color::LightGray()),
        Color::Indigo()
    );
    explanation.hidden(Bind(showExplanation, [](bool visible) { return !visible; }));

    View content = VStack({
        Components::hero(model),
        Components::progress(model),
        explanation,
        ::DeclarativeUI::ForEach(model->tasks,
            [](const ComposeTask& task) { return task.id; },
            [](const ComposeTask& task) { return Components::taskRow(task); })
            .spacing(12)
    });
    content.fillWidth().spacing(16);

    View focusButton = Shared::bottomActionButton("Focus", [model] {
        model->toggleFocusMode();
    });
    focusButton.background(Bind(model->focusMode, [](bool enabled) {
        return enabled ? Color::Indigo() : Color::Blue();
    }));

    View actions = HStack({
        Shared::bottomActionButton("Complete", [model] {
            model->completeNextTask();
        }),
        Shared::bottomActionButton("Add task", [model] {
            model->addTask();
        }),
        focusButton,
        Shared::bottomActionButton("About", [showExplanation] {
            showExplanation.update([](bool value) { return !value; });
        })
    });
    actions.fillWidth().spacing(10);

    return Shared::collectionScreen(Shared::card(content), actions);
}

} // namespace DeclarativeUI::Examples::Compose
