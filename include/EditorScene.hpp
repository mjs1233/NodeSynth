#pragma once
#include <imgui.h>
#include <print>
#include "Scene.hpp"

#include "AudioSystem.hpp"

class EditorScene : public SceneBase {

private:
	AudioManager audio_manager;


public:

	EditorScene() : audio_manager(AudioManager(2,48000)) {


	}

	virtual void update(SceneHandler* handler) override {
        
        
        update_topbar();


	}

    ~EditorScene() {
    
    }

private:
	void update_topbar() {

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New", "Ctrl+N"))
                {
                    // New 처리
                }
                if (ImGui::MenuItem("Open", "Ctrl+O"))
                {
                    // Open 처리
                }
                if (ImGui::MenuItem("Save", "Ctrl+S"))
                {
                    // Save 처리
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Exit"))
                {
                    // 종료 처리
                }
                ImGui::EndMenu();
            }
        }
        ImGui::EndMainMenuBar();

	}

};