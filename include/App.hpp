#pragma once

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <vector>
#include <memory>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h> // Toujours après windows.h

#include <features/Feature.hpp>
#include <features/features.hpp>

class App {
public :
    App();
    ~App();

    /// @brief Lance l'application. Assurez-vous d'avoir ajouté au moins une foctionnalitée avant de la lancer . (à check)
    void launch();

    /// @brief fonction appellée pour la destruction de la fenêtre.
    void shutdown();

    /// @brief Ajoute une fonctionnalitée à l'app.
    /// @param feature_name le nom de la fonctionnalitée
    /// @param feature la fonctionnalitée
    void add_feature(const char* feature_name, std::unique_ptr<Feature> feature);

    /// @brief Réserve de la place pour ajouter des fonctionnalitées sans réallocation.
    /// @param size la nouvelle capacité
    void reserve_features(size_t size);

private:
    GLFWwindow* m_window;
    ImGuiIO* m_io;

    bool m_show_sidebar = true;
    int m_current_feature = 0; // l'indice de la feature active
    std::vector<const char*> m_features_names;
    std::vector<std::unique_ptr<Feature>> m_features;

private:    
    bool glfw_init();
    void imgui_init();
    void imgui_settings();

    void update();

    void draw_header();
    void draw_sidebar();
    void draw_main_content();
};