#pragma once
#include <imgui.h>
#include <print>
#include "Scene.hpp"

#include "AudioManager.hpp"

class EditorScene : public SceneBase {

private:
	AudioManager audio_manager;

    enum class editor_state {
        idle,
        connect,
        probe
    };

    editor_state state;

    struct connect_state_data {
        uint32_t start_point_id;
    };



public:

    EditorScene();

    virtual void update(SceneHandler* handler) override;

    ~EditorScene();

private:
    void update_topbar();
    void update_node();

};