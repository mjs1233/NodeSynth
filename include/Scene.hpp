#pragma once
#include <concepts>
#include <memory>

class SceneHandler;

class SceneBase {

public:
	virtual void update(SceneHandler* sceneHandler) = 0;
};

template<typename T>
concept SceneTrait =
std::is_base_of<SceneBase, T>::value
&& std::is_constructible<T>::value
&& std::is_destructible<T>::value;