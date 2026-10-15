#include "App.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <cstdlib>
#include <iostream>
#include <features/note_converter/NoteConverter.hpp>

App::App(int width, int height) : m_io(nullptr) {
    if(!glfw_init(width, height)) std::exit(1);
    imgui_init();
    imgui_settings();
}

App::~App() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void App::launch() {
    while (!glfwWindowShouldClose(m_window)) {
        update();
    }
}

void App::shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void App::add_feature(const char *feature_name, std::unique_ptr<Feature> feature) {
    m_features_names.emplace_back(feature_name);
    m_features.push_back(std::forward<std::unique_ptr<Feature>>(feature)); // qu'est ce qui ce passe pour la ressource ?
}

void App::reserve_features(size_t size) {
    m_features_names.reserve(size);
    m_features.reserve(size);
}

bool App::glfw_init(int width, int height) {
    if (!glfwInit())
        return 1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    #ifndef APP_NAME
    #define APP_NAME "Mon Application" // Sécurité si la compilation CMake échoue
    #endif

    m_window = glfwCreateWindow(width, height, APP_NAME, nullptr, nullptr);
    if (!m_window)
        return 1;
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1); // VSync actif

    // icone
    const char* icon_path = "assets/logo.png"; 

    GLFWimage images[1];
    // Le "4" force le chargement en format RGBA (Red Green Blue Alpha)
    images[0].pixels = stbi_load(icon_path, &images[0].width, &images[0].height, 0, 4);

    if (images[0].pixels) {
        // Si l'image est bien chargée, on l'applique à la fenêtre
        glfwSetWindowIcon(m_window, 1, images);

        // On libère la mémoire immédiatement, GLFW en a fait une copie interne
        stbi_image_free(images[0].pixels);
    } else {
        std::cout << "Attention: Impossible de charger l'icone GLFW\n";
    }

    return true;
}

void App::imgui_init() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_io = &ImGui::GetIO();
}

void App::imgui_settings() {
    // Thème sombre moderne avec angles arrondis
    ImGui::StyleColorsDark();
    ImGuiStyle &style = ImGui::GetStyle();
    style.WindowRounding = 8.0f;
    style.FrameRounding = 6.0f;
    style.ChildRounding = 6.0f;
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.18f, 0.18f, 0.21f, 1.00f);

    change_font_size();

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

void App::update() {
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Fenêtre ImGui qui couvre toute la fenêtre du système d'exploitation
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(m_io->DisplaySize);
    change_font_size();

    ImGui::Begin("Main", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);

    // 1. Barre d'en-tête avec le bouton Menu
    draw_header();

    // 2. Menu latéral (Sidebar)
    if (m_show_sidebar)
    {
        draw_sidebar();

        // Indique que la prochaine zone doit être dessinée à droite du menu
        ImGui::SameLine();
    }

    // 3. Zone de contenu principal (qui prend tout l'espace restant)
    draw_main_content();

    ImGui::End();      // Fin de la fenêtre globale

    // Rendu final OpenGL
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(m_window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(m_window);
}

void App::change_font_size() {
    int width=0, height=0;
    glfwGetWindowSize(m_window, &width, &height);
    
    float scale_factor = min(width / s_BASE_WINDOW_WIDTH, height / s_BASE_WINDOW_HEIGHT);
    float base_font_size = 17.0f; // La police par défaut d'ImGui fait initialement 13 pixels

    ImFontConfig config;
    config.SizePixels = base_font_size * scale_factor;

    // Demande à ImGui de charger sa police interne avec cette configuration
    m_io->Fonts->AddFontDefault(&config);
}

void App::draw_header() {
    if (ImGui::Button(m_show_sidebar ? "<<<" : ">>>"))
    {
        m_show_sidebar = !m_show_sidebar; // Alterne l'affichage du menu
    }
    ImGui::SameLine();
    ImGui::TextDisabled(" / %s", m_features_names[m_current_feature]); // Fil d'ariane visuel
    ImGui::Separator();
}

void App::draw_sidebar()
{
    // Crée une sous-fenêtre avec une largeur fixe de 180 pixels
    ImGui::BeginChild("Sidebar", ImVec2(180, 0), true);

    // Génère les boutons du menu
    for (int i = 0; i < m_features_names.size(); i++)
    {
        // ImGui::Selectable donne un effet "bouton plat" qui reste en surbrillance si sélectionné
        if (ImGui::Selectable(m_features_names[i], m_current_feature == i))
        {
            m_current_feature = i;
        }
    }
    ImGui::EndChild();
}

void App::draw_main_content() {
    // ImVec2(0, 0) indique à BeginChild de remplir l'espace disponible
    ImGui::BeginChild("MainContent", ImVec2(0, 0), false);

    m_features[m_current_feature]->draw_IMGui();

    ImGui::EndChild(); // Fin de la zone principale
}
