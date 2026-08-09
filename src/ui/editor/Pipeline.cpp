#include "Pipeline.hpp"

#include <imgui.h>

bool UI::Editor::Pipeline::draw() {
    ImGui::InputText("Axiom", this->axiom.data(), 32);
    ImGui::InputInt("Iterations", &this->iters);

    ImGui::SeparatorText("Rules");
    this->drawRules();

    ImGui::SeparatorText("Constants");
    this->drawConstants();

    if(!this->error.empty()) {
        ImGui::TextColored(
            ImVec4{1.0f, 0.0f, 0.0f, 1.0f},
            "%s",
            this->error.c_str()
        );
    }

    return ImGui::Button("Apply");
}

std::expected<LSystem::Interpreter, std::string>
UI::Editor::Pipeline::build() const {
    if(this->axiom.empty()) {
        return std::unexpected("Axiom cannot be empty");
    }

    if(this->iters < 1) {
        return std::unexpected("Iterations must be greater than zero");
    }

    // if(this->rules.empty()) {
    //     return std::unexpected("Rules cannot be empty");
    // }

    // if(this->constants.empty()) {
    //     return std::unexpected("Constants cannot be empty");
    // }

    for(const auto& rule : this->rules) {
        if(rule.symbol.size() != 1) {
            return std::unexpected("Rule symbol must be a single character");
        }
    }
}


void UI::Editor::Pipeline::drawRules() {
    if(ImGui::Button("Add Rule")) {
        this->rules.push_back({
            .id = this->nextId++
        });
    }

    if(ImGui::BeginTable("Rules", 4, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders)) {
        ImGui::TableSetupColumn("ID");
        ImGui::TableSetupColumn("Symbol");
        ImGui::TableSetupColumn("Replacement");
        ImGui::TableSetupColumn("Command");//, ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableHeadersRow();

        for(auto& rule : this->rules) {
            ImGui::PushID(static_cast<int>(rule.id));
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::SetNextItemWidth(50.f);
            ImGui::InputText("##Symbol", rule.symbol.data(), 32);

            ImGui::TableSetColumnIndex(1);
            ImGui::InputText("##Replacement", rule.replacement.data(), 32);

            ImGui::TableSetColumnIndex(2);
            static constexpr const char* commands[] = {"FORWARD", "BACKWARD", "LEFT", "RIGHT"};
            int command = static_cast<int>(rule.command.type);

            if(ImGui::Combo("##Command", &command, commands, 4)) {
                rule.command.type = static_cast<LSystem::CommandType>(command);
            }

            ImGui::TableSetColumnIndex(3);
            if(ImGui::Button("X")) {
                this->removeRule(rule.id);
            }

            ImGui::PopID();
        }
    }
    ImGui::EndTable();
}

void UI::Editor::Pipeline::drawConstants() {
    if(ImGui::Button("Add Constant")) {
        this->constants.push_back({
            .id = this->nextId++
        });
    }


    if(ImGui::BeginTable("Constants", 3, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders)) {
        for(auto& constant : this->constants) {
            ImGui::PushID(static_cast<int>(constant.id));
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::SetNextItemWidth(50.f);
            ImGui::InputText("##Name", constant.name.data(), 32);

            ImGui::TableSetColumnIndex(1);
            ImGui::SetNextItemWidth(50.f);
            // ImGui::InputText("##Symbol", constant.command.argument, 32);
            ImGui::InputFloat("##Argument", &constant.command.argument);

            ImGui::TableSetColumnIndex(2);
            static constexpr const char* commands[] = {"FORWARD", "BACKWARD", "LEFT", "RIGHT"};
            int command = static_cast<int>(constant.command.type);

            if(ImGui::Combo("##Command", &command, commands, 4)) {
                constant.command.type = static_cast<LSystem::CommandType>(command);
            }

            ImGui::TableSetColumnIndex(3);
            if(ImGui::Button("X")) {
                this->removeConstant(constant.id);
            }

            ImGui::PopID();
        }
    }
    ImGui::EndTable();
}

void UI::Editor::Pipeline::removeRule(std::uint64_t id) {
    std::erase_if(this->rules, [id](const auto& r) {
        return r.id == id;
    });
}

void UI::Editor::Pipeline::removeConstant(std::uint64_t id) {
    std::erase_if(this->constants, [id](const auto& c) {
        return c.id == id;
    });
}
