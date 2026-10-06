#include "App.hpp"

#include <cstdlib>

App::App() : m_io(nullptr) {
    if(!glfw_init()) std::exit(1);
    imgui_init();
    imgui_settings();

    m_features_names.reserve(3);
    m_features_names.emplace_back("Convertisseur");
    m_features_names.emplace_back("Calculatrice");
    m_features_names.emplace_back("Statistiques");
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

bool App::glfw_init() {
    if (!glfwInit())
        return 1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow *window = glfwCreateWindow(450, 320, "Convertisseur de Notes", nullptr, nullptr);
    if (!window)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // VSync actif
}

void App::imgui_init() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_io = &ImGui::GetIO();
    (void)(*m_io); // à quoi ça sert ?
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

    ImGui::Begin("Main", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);

    // 1. Barre d'en-tête avec le bouton Menu
    if (ImGui::Button("☰"))
    {
        m_show_sidebar = !m_show_sidebar; // Alterne l'affichage du menu
    }
    ImGui::SameLine();
    ImGui::TextDisabled(" / %s", m_features_names[m_current_feature]); // Fil d'ariane visuel
    ImGui::Separator();

    // 2. Menu latéral (Sidebar)
    if (m_show_sidebar)
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

        // Crucial : indique que la prochaine zone doit être dessinée à droite du menu
        ImGui::SameLine();
    }

    // 3. Zone de contenu principal (qui prend tout l'espace restant)
    // ImVec2(0, 0) indique à BeginChild de remplir l'espace disponible
    ImGui::BeginChild("MainContent", ImVec2(0, 0), false);

    // Routage vers la bonne classe selon le choix de l'utilisateur
    switch (m_current_feature)
    {
    case 0:
        // Ex: m_gradeConverter.Render();
        ImGui::Text("Interface du Convertisseur de notes");
        break;
    case 1:
        // Ex: m_calculator.Render();
        ImGui::Text("Interface de la feature 2");
        break;
    case 2:
        // Ex: m_stats.Render();
        ImGui::Text("Interface de la feature 3");
        break;
    }

    ImGui::EndChild(); // Fin de la zone principale
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
