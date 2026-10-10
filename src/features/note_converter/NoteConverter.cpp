#include <features/note_converter/NoteConverter.hpp>

float valueOfGrade(int gradeIdx, int valMin, int valMax, int nbVals) {
    return valMin + gradeIdx * (valMax - valMin) / (nbVals - 1);
}

NoteConverter::NoteConverter() {
    for (int i = 0; i < m_grades.size(); i++)
    {
        m_values[i] = valueOfGrade(i, 2, 20, m_grades.size());
    }
}

void NoteConverter::draw_IMGui() {
    ImGui::TextDisabled("Convertisseur de notes");
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text("Sélectionnez une note :");
    ImGui::SetNextItemWidth(150.0f);
    ImGui::Combo("##Note", &m_current_item, m_grades.data(), m_grades.size());

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    float note_20 = m_values[m_current_item];
    float note_10 = note_20 / 2.0f;

    ImGui::Text("Équivalence pour : %s", m_grades[m_current_item]);
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
}
