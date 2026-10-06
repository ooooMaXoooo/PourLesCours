#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Feature {
public :
    Feature() {}

    virtual void draw_IMGui() = 0;
};