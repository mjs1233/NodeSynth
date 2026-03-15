#pragma once
#include <imgui.h>
#include <print>
#include "Scene.hpp"

#include "AudioManager.hpp"

class EditorScene : public SceneBase {

private:
	AudioManager audio_manager;


public:

	EditorScene()  {//audio_manager(AudioManager(2,48000)) {


	}

	virtual void update(SceneHandler* handler) override {
        
        
        update_topbar();

	}

    ~EditorScene() {
    
    }

private:
	void update_topbar() {

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
        }
        ImGui::EndMainMenuBar();

	}

    void update_node() {

        audio_manager.ui_update();
    }

};