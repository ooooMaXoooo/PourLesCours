#pragma once

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <vector>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h> // Toujours après windows.h

class App {
public :
    App();
    ~App();

    void launch();

    void shutdown();

private:
    GLFWwindow* m_window;
    ImGuiIO* m_io;

    bool m_show_sidebar = true;
    int m_current_feature = 0; // l'indice de la feature active
    std::vector<const char*> m_features_names;

private:    
    bool glfw_init();
    void imgui_init();
    void imgui_settings();

    void update();
};