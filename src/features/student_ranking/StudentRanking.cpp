#include <features/student_ranking/StudentRanking.hpp>

StudentRanking::StudentRanking() {
}

void StudentRanking::draw_IMGui() {
    // Petit header
    ImGui::TextDisabled("Note des élèves");

    ImGui::Separator();
    ImGui::Spacing();

    // sélection de la classe
    ImGui::Text("Sélectionnez une classe :");
    ImGui::SetNextItemWidth(150.0f);
    std::vector<const char*> names = get_classes_names();
    ImGui::Combo("##Classe", &m_current_class, names.data(), names.size());

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // aspect visuel de confirmation UI (bon pour l'UX ?)
    ImGui::Text("Classe %s", m_classes[m_current_class]);
    ImGui::Spacing();

    // Encart de résultats stylisé
    ImGui::BeginChild("Resultats", ImVec2(0, 100), true);
    constexpr int N_Cols = 4;
    ImGui::BeginTable("str_id", N_Cols);
    /* 4 colonnes :
    • Nom élève
    • Points
    • Bonus
    • carton
    */

    draw_classes_header();
    ImGui::TableNextRow();

    // affichage des élèves
    draw_students_info(m_current_class);

    ImGui::EndTable();
    ImGui::EndChild();
}

void StudentRanking::draw_classes_header() {
    /* 4 colonnes :
    • Nom élève
    • Points
    • Bonus
    • carton
    */

    ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Elève");

    ImGui::TableNextColumn();
    ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Points");

    ImGui::TableNextColumn();
    ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Bonus");

    ImGui::TableNextColumn();
    ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "carton");
}

void StudentRanking::draw_students_info(int class_idx) {
    if (m_classes.size() == 0) return;

    Class& c = m_classes[class_idx];

    for(size_t i=0; i < c.size(); i++) {

        {
            ImGui::Text(c.get_student(i).get_full_name());
            ImGui::TableNextColumn();

            ImGui::Text("%.1f", c.get_student_point(i));
            ImGui::SameLine();
            float add = 0.0f;
            ImGui::InputFloat("Add", &add, 0.0f, 0.0f, "%.1f");
            ImGui::SameLine();
            if(ImGui::Button("+")) {
                c.give_points_to_student(add, i);
            }
            ImGui::TableNextColumn();

            if(ImGui::ColorButton("##Un carre de couleur", c.get_student_bonus(i) ? ImVec4(50/255, 168/255, 82/255, 1.0f) : ImVec4(99/255, 99/255, 99/255, 1.0f))) {
                c.give_bonus_to_student(i);
            }
            ImGui::TableNextColumn();

            if (ImGui::ColorButton("##Un carre de couleur", c.get_student_bonus(i) ? ImVec4(212 / 255, 13 / 255, 13 / 255, 1.0f) : ImVec4(99 / 255, 99 / 255, 99 / 255, 1.0f))) {
                c.give_bonus_to_student(i);
            }
        }

        ImGui::TableNextRow();
    }
}

std::vector<const char *> StudentRanking::get_classes_names() const {
    std::vector<const char*> res;

    if (m_classes.size() == 0) {
        res.reserve(1);
        res.push_back("None");
        return res;
    }

    res.reserve(m_classes.size());
    for (size_t i=0; i < m_classes.size(); i++) {
        res.push_back(m_classes[i].get_name());
    }

    return res;
}
