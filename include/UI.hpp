#pragma once
#include "UIComponent.hpp"
#include "UIHandler.hpp"

#include "Button.hpp"
#include "NodeUI.hpp"

#include "Rect.hpp"


using UIHandler = UIHandlerBase<Button<Rect>,NodeUI>;