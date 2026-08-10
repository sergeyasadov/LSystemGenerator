#include "Pipeline.hpp"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <string>

bool UI::Editor::Pipeline::draw() {
    ImGui::InputText("Axiom", &this->axiom);
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
    else {
        ImGui::Text("");
    }

    return ImGui::Button("Apply");
}

std::expected<LSystem::Interpreter, std::string>
UI::Editor::Pipeline::build() {
    if(this->axiom.empty()) {
        this->error = "Axiom cannot be empty";
        return std::unexpected(this->error);
    }

    if(this->iters < 1) {
        this->error = "Iterations must be greater than zero";
        return std::unexpected(this->error);
    }


    LSystem::Interpreter interpreter;

    interpreter.setAxiom(std::move(this->axiom));
    interpreter.setIterations(std::move(this->iters));

    for(const auto& rule : this->rules) {
        if(rule.symbol.size() > 1) {
            this->error = "Rule symbol must be a single character";
            return std::unexpected(this->error);
        }
        interpreter.pushRule({
            .var = {
                .value = rule.symbol[0],
                .command = interpreter.makeCommand(rule.command)
            },
            .replacement = rule.replacement
        });
    }

    for(const auto& constant : this->constants) {
        if(constant.name.size() > 1) {
            this->error = "Constant name must be a single character";
            return std::unexpected(this->error);
        }
        interpreter.pushConstant({
            .value = constant.name[0],
            .command = interpreter.makeCommand(constant.command)
        });
    }

    this->error.clear();
    return interpreter;
}


void UI::Editor::Pipeline::drawRules() {
    if(ImGui::Button("Add Rule")) {
        this->rules.push_back({
            .id = this->nextId++
        });
    }

    static constexpr const char* commands[] = {"FORWARD", "ROTATE", "SAVE", "RESTORE"};

    if(ImGui::BeginTable("Rules", 4, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders)) {
        ImGui::TableSetupColumn("Symbol");
        ImGui::TableSetupColumn("Replacement");
        ImGui::TableSetupColumn("Command");//, ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Delete");
        ImGui::TableHeadersRow();

        for(auto& rule : this->rules) {
            ImGui::PushID(static_cast<int>(rule.id));
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::SetNextItemWidth(50.f);
            ImGui::InputText("##Symbol", &rule.symbol);

            ImGui::TableSetColumnIndex(1);
            ImGui::InputText("##Replacement", &rule.replacement);

            ImGui::TableSetColumnIndex(2);
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

    static constexpr const char* commands[] = {"FORWARD", "ROTATE", "SAVE", "RESTORE"};

    if(ImGui::BeginTable("Constants", 4, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders)) {
        ImGui::TableSetupColumn("Symbol");
        ImGui::TableSetupColumn("Argument");
        ImGui::TableSetupColumn("Command");
        ImGui::TableSetupColumn("Delete");
        ImGui::TableHeadersRow();

        for(auto& constant : this->constants) {
            ImGui::PushID(static_cast<int>(constant.id));
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::SetNextItemWidth(50.f);
            ImGui::InputText("##Symbol", &constant.name);

            ImGui::TableSetColumnIndex(1);
            ImGui::SetNextItemWidth(50.f);
            ImGui::InputFloat("##Argument", &constant.command.argument);

            ImGui::TableSetColumnIndex(2);
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
