#pragma once
#include "Scene.hpp"
#include <tuple>
#include <functional>
#include <memory>
#include "EditorScene.hpp"

class SceneHandler {

private:

	std::shared_ptr<SceneBase> scene;
public:

	SceneHandler() {
		change<EditorScene>();
	}

	void update() {
		scene->update(this);
	}
	
	template <SceneTrait T, typename ...Args>
	requires std::is_constructible<T, Args...>::value
	void change(Args&& ...args) {

		scene = std::make_shared<T>(std::forward<Args>(args)...);
	}
};