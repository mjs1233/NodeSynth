#include "EditorScene.hpp"
#include "Delay.hpp"
#include "SinusoidalOscillator.hpp"

EditorScene::EditorScene() : state(editor_state::idle) {//audio_manager(AudioManager(2,48000)) {

    audio_manager.init(2,48000);
}

void EditorScene::update(SceneHandler* handler) {


    update_topbar();
    audio_manager.ui_update();
}

EditorScene::~EditorScene() {

}

void EditorScene::update_topbar() {

    if (ImGui::BeginMainMenuBar()) {

        if (ImGui::BeginMenu("File")) {

            if (ImGui::MenuItem("New")) {

            }
            if (ImGui::MenuItem("Open")) {

            }
            if (ImGui::MenuItem("Save")) {

            }

            ImGui::Separator();

            if (ImGui::MenuItem("Exit")) {

            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Add")) {

            if (ImGui::MenuItem("Delay")) {

                //add Delay
                audio_manager.add<AudioProcessor::Delay>();
            }
            if (ImGui::MenuItem("OSC - SIN 440hz")) {

                //add sin osc
                audio_manager.add<AudioProcessor::SinusoidalOscillator>();
            }

            ImGui::EndMenu();
        }


        if (ImGui::Button("play")) {

            audio_manager.play();
        }
        else if (ImGui::Button("stop")) {

            audio_manager.stop();
        }

    }
    ImGui::EndMainMenuBar();

}

void EditorScene::update_node() {

}
    
