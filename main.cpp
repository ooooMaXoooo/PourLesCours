#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h> // Toujours après windows.h
#include <array>
#include <string_view>





float valueOfGrade(int gradeIdx, int valMin, int valMax, int nbVals) {
    return valMin + gradeIdx * (valMax - valMin) / (nbVals-1);
}



#ifdef _WIN32
int APIENTRY WinMain(HINSTANCE, HINSTANCE, PSTR, int)
#else
int main()
#endif
{
    if (!glfwInit()) return 1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(450, 320, "Convertisseur de Notes", nullptr, nullptr);
    if (!window) return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // VSync actif

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    
    // Thème sombre moderne avec angles arrondis
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 8.0f;
    style.FrameRounding = 6.0f;
    style.ChildRounding = 6.0f;
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.18f, 0.18f, 0.21f, 1.00f);

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Données des notes
    const std::array<const char*, 13> grades = { "E", "D-", "D", "D+", "C-", "C", "C+", "B-", "B", "B+", "A-", "A", "A+" };
    std::array<float, 13> values = {0};

    for (int i=0; i < grades.size(); i++) {
        values[i] = valueOfGrade(i, 2, 20, grades.size());
    }


    static int current_item = 0;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Fenêtre ImGui qui couvre toute la fenêtre du système d'exploitation
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);
        
        ImGui::Begin("Main", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);

        ImGui::TextDisabled("🎓 Convertisseur de notes");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Sélectionnez une note :");
        ImGui::SetNextItemWidth(150.0f);
        ImGui::Combo("##Note", &current_item, grades.data(), grades.size());

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        float note_20 = values[current_item];
        float note_10 = note_20 / 2.0f;

        ImGui::Text("Équivalence pour : %s", grades[current_item]);
        ImGui::Spacing();

        // Encart de résultats stylisé
        ImGui::BeginChild("Resultats", ImVec2(0, 100), true);
        ImGui::Columns(2, nullptr, false);
        
        ImGui::TextColored(ImVec4(0.5f, 0.8f, 1.0f, 1.0f), "Note sur 10");
        ImGui::SetWindowFontScale(1.5f);
        ImGui::Text("%.1f / 10", note_10);
        ImGui::SetWindowFontScale(1.0f);
        
        ImGui::NextColumn();
        ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Note sur 20");
        ImGui::SetWindowFontScale(1.5f);
        ImGui::Text("%.1f / 20", note_20);
        ImGui::SetWindowFontScale(1.0f);
        
        ImGui::EndChild();
        ImGui::End();

        // Rendu final OpenGL
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}