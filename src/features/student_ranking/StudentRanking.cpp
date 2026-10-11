#include <features/student_ranking/StudentRanking.hpp>
#include <dataclasses/Student.hpp>

StudentRanking::StudentRanking()
    : m_editor__anterior_class("", 0.0f, 0.0f, 0.0f, 0.0f, 0)
{
    m_build_class__name[0] = '\0';
}

void StudentRanking::draw_IMGui() {
    // Petit header
    ImGui::TextDisabled("Participation des élèves");

    ImGui::Separator();
    ImGui::Spacing();

    // sélection de la classe
    draw_class_selector();

    ImVec2 size = ImGui::GetContentRegionAvail();
    // Encart de résultats stylisé
    ImGui::BeginChild("Resultats", ImVec2(0, 0.95 * size.y), true);


    constexpr int N_Cols = 2;
    ImGui::BeginTable("str_id", N_Cols);
    /* 2 colonnes :
    • Nom élève
    • Points
    */

    draw_classes_header();
    ImGui::TableNextRow();

    // affichage des élèves
    draw_students_info(m_current_class);

    ImGui::EndTable();
    ImGui::EndChild();
}

void StudentRanking::draw_class_selector() {
    ImGui::Text("Sélectionnez une classe :");
    ImGui::SetNextItemWidth(150.0f);
    std::vector<const char*> names = get_classes_names();
    ImGui::Combo("##Classe", &m_current_class, names.data(), names.size());

    if (m_is_class_editor_open) {
        m_current_class = m_editor__current_class;
    }

    if(m_classes.size() != 0) {
        ImGui::SameLine();
        if (ImGui::Button("edit") && !m_is_create_class_open && !m_is_class_editor_open) {
            m_is_class_editor_open = true;

            m_editor__anterior_class = m_classes[m_current_class];
            m_editor__current_class = m_current_class;
        }
        
        ImGui::SameLine();
        if (ImGui::Button("-") && !m_is_class_editor_open && !m_is_create_class_open) {
            //? supprime la classe actuellement sélectionnée
            m_classes.erase(m_classes.begin() + m_current_class);
            m_current_class = m_current_class > 0 ? m_current_class-1 : 0;
        }
    }
    // bouton add class
    ImGui::SameLine();
    if(ImGui::Button("+") && !m_is_class_editor_open) {
        //? ouvre la fenêtre d'ajout de classe
        m_is_create_class_open = true;
    }
    if (m_is_create_class_open && !m_is_class_editor_open) { // on empêche les 2 fenêtres de s'ouvrir en même temps pour éviter d'utiliser les mêmes variables dans 2 fenêtres.
        draw_create_class_window();
    }
    if (m_is_class_editor_open && !m_is_create_class_open) { // on empêche les 2 fenêtres de s'ouvrir en même temps pour éviter d'utiliser les mêmes variables dans 2 fenêtres.
        sync_class_attributes();
        draw_class_editor_window();
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // aspect visuel de confirmation UI (bon pour l'UX ?)
    ImGui::Text("Classe %s", m_classes.size() == 0 ? "None" : names[m_current_class]);
    ImGui::Spacing();
}

void StudentRanking::draw_classes_header()
{
    /* 2 colonnes :
    • Nom élève
    • Points
    */

    ImGui::TableNextColumn();
    if (m_classes.size() == 0) ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Elève - 0");
    else ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Elève %d", m_classes[m_current_class].size());

    ImGui::TableNextColumn();
    ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Points");
}

void StudentRanking::draw_students_info(int class_idx) {
    if (m_classes.size() == 0) return;

    Class& c = m_classes[class_idx];

    for(size_t i=0; i < c.size(); i++) {

        {
            ImGui::PushID(static_cast<int>(i));

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(c.get_student(i).get_full_name().c_str());

            ImGui::TableNextColumn();
            ImGui::Text("%.1f", c.get_student_point(i));

            ImGui::SameLine();
            ImGui::Spacing();
            ImGui::SameLine();
            
            float* point_increment = c.get_student_point_increment(i);
            ImGui::SetNextItemWidth(30);
            ImGui::InputFloat("", point_increment, 0.0f, 0.0f, "%.1f");
            ImGui::SameLine();
            if(ImGui::Button("+")) {
                c.give_points_to_student(*point_increment, i);
            }
            ImGui::SameLine();
            if(ImGui::Button("-")) {
                c.remove_points_to_student(*point_increment, i);
            }

            ImGui::SameLine();
            if (ImGui::ColorButton("##Bonus", ImVec4(50./255, 168./255, 82./255, 1.0f), ImGuiColorEditFlags_NoTooltip)) {
                c.give_bonus_to_student(i);
            }

            ImGui::SameLine();
            if (ImGui::ColorButton("##Carton Jaune", ImVec4(181./255, 153./255, 14./255, 1.0f), ImGuiColorEditFlags_NoTooltip)) {
                c.give_low_malus_to_student(i);
            }

            ImGui::SameLine();
            if (ImGui::ColorButton("##Carton Rouge", ImVec4(212./255, 13./255, 13./255, 1.0f), ImGuiColorEditFlags_NoTooltip)) {
                c.give_high_malus_to_student(i);
            }

            ImGui::PopID();
        }
    }
}

void StudentRanking::draw_create_class_window() {
    ImGui::Begin("Menu de création de classe");
    
    draw_class_settings_editor();
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Ajout des élèves
    draw_class_students_editor();

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::Button("Créer")) {
        const size_t size = m_build_class__students_lastname.size();
        m_classes.emplace_back(m_build_class__name, m_build_class__bonus_value, m_build_class__high_malus_value, m_build_class__low_malus_value, m_build_class__seuil, size);

        for(size_t i=0; i<size; i++) {
            m_classes.back().add_student(Student(m_build_class__students_lastname[i].data(), m_build_class__students_firstname[i].data()));
        }
        
        reset_build_class_attributes();
        m_is_create_class_open = false;
    }

    ImGui::SameLine();
    if (ImGui::Button("Annuler")) {
        reset_build_class_attributes();
        m_is_create_class_open = false;
    }
    

    ImGui::End();
}

void StudentRanking::draw_class_editor_window() {
    ImGui::Begin("Menu d'édition de classe");
    
    draw_class_settings_editor();

    m_classes[m_current_class].set_name(m_build_class__name);
    m_classes[m_current_class].set_bonus_value(m_build_class__bonus_value);
    m_classes[m_current_class].set_seuil(m_build_class__seuil);
    m_classes[m_current_class].set_high_malus_value(m_build_class__high_malus_value);
    m_classes[m_current_class].set_low_malus_value(m_build_class__low_malus_value);
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // création des callbacks :
    std::function<void(size_t)> suppression_callback = [this](size_t i) -> void { this->m_classes[m_current_class].remove_student(i);};
    std::function<void()> addition_callback = [this]() -> void {this->m_classes[m_current_class].add_student(Student{"", ""});};

    // Ajout des élèves
    draw_class_students_editor(suppression_callback, addition_callback);

    // on met les noms à jour :
    for (size_t i=0; i < m_classes[m_current_class].size(); i++) {
        m_classes[m_current_class].set_student_firstname(m_build_class__students_firstname[i].data(), i);
        m_classes[m_current_class].set_student_lastname(m_build_class__students_lastname[i].data(), i);
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::Button("Sauvegarder")) {

        m_classes[m_current_class].set_name(m_build_class__name);
        m_classes[m_current_class].set_bonus_value(m_build_class__bonus_value);
        m_classes[m_current_class].set_seuil(m_build_class__seuil);
        m_classes[m_current_class].set_high_malus_value(m_build_class__high_malus_value);
        m_classes[m_current_class].set_low_malus_value(m_build_class__low_malus_value);
        
        reset_build_class_attributes();
        m_is_class_editor_open = false;
    }

    ImGui::SameLine();
    if (ImGui::Button("Annuler")) {
        m_classes[m_current_class] = m_editor__anterior_class;

        reset_build_class_attributes();
        m_is_class_editor_open = false;
    }
    

    ImGui::End();
}

void StudentRanking::draw_class_students_editor(std::function<void(size_t)> suppression_callback, std::function<void()> addition_callback) {
    if (ImGui::BeginTable("student table", 1)) {
        const float width = ImGui::GetWindowWidth(); 
        
        // 1. Déclarer la première ligne et la première colonne pour le titre
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Elèves %d", m_build_class__students_firstname.size());

        // 2. Boucle des élèves
        for(size_t i = 0; i < m_build_class__students_lastname.size(); i++) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();

            // PushID crée un contexte unique basé sur l'index (i) pour éviter les collisions
            ImGui::PushID(static_cast<int>(i)); 
            
            ImGui::SetNextItemWidth(width / 4);
            ImGui::InputTextWithHint("##Nom", "Nom", m_build_class__students_lastname[i].data(), MAX_NAME_LENGTH);
            
            ImGui::SameLine();
            
            // Contraint la largeur du second champ à 120 pixels
            ImGui::SetNextItemWidth(width / 4); 
            ImGui::InputTextWithHint("##Prenom", "Prénom", m_build_class__students_firstname[i].data(), MAX_NAME_LENGTH);
            
            // Un bouton pour supprimer cet élève précis de la liste
            ImGui::SameLine();
            if (ImGui::Button("-")) {
                m_build_class__students_lastname.erase(m_build_class__students_lastname.begin() + i);
                m_build_class__students_firstname.erase(m_build_class__students_firstname.begin() + i);
                if (suppression_callback) suppression_callback(i);
            }

            ImGui::PopID(); // Fin du contexte unique
        }

        // 3. Dernière ligne pour le bouton +
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        if (ImGui::Button("+")) {
            m_build_class__students_lastname.emplace_back(std::array<char, MAX_NAME_LENGTH>());
            m_build_class__students_firstname.emplace_back(std::array<char, MAX_NAME_LENGTH>());
            if (addition_callback) addition_callback();
        }

        ImGui::EndTable();
    }
}

void StudentRanking::draw_class_settings_editor() {
    const float width = ImGui::GetWindowWidth();

    ImGui::SetNextItemWidth(width / 4);
    ImGui::InputTextWithHint("##", "nom de la classe", m_build_class__name, MAX_CLASSNAME_SIZE);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    
    ImGui::SetNextItemWidth(width / 4);
    ImGui::InputFloat("Valeur de seuil", &m_build_class__seuil, 0.0f, 0.0f, "%.1f");
    ImGui::SetNextItemWidth(width / 4);
    ImGui::InputFloat("Valeur du bonus (positif)", &m_build_class__bonus_value, 0.0f, 0.0f, "%.1f");
    ImGui::SetNextItemWidth(width / 4);
    ImGui::InputFloat("Valeur du carton jaune (positif)", &m_build_class__low_malus_value, 0.0f, 0.0f, "%.1f");
    ImGui::SetNextItemWidth(width / 4);
    ImGui::InputFloat("Valeur du carton rouge (positif)", &m_build_class__high_malus_value, 0.0f, 0.0f, "%.1f");
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
        res.push_back(m_classes[i].get_name_c_str());
    }

    return res;
}

void StudentRanking::reset_build_class_attributes() {
    m_build_class__name[0] = '\0';
    m_build_class__bonus_value = 0;
    m_build_class__high_malus_value = 0;
    m_build_class__low_malus_value = 0;
    m_build_class__seuil = 0;
    m_build_class__students_lastname.clear();
    m_build_class__students_firstname.clear();
}

void StudentRanking::sync_class_attributes() {
    m_build_class__bonus_value = m_classes[m_current_class].get_bonus_value();
    m_build_class__high_malus_value = m_classes[m_current_class].get_high_malus_value();
    m_build_class__low_malus_value = m_classes[m_current_class].get_low_malus_value();

    std::string name = m_classes[m_current_class].get_name();
    for(size_t i=0; i<name.size(); i++) m_build_class__name[i] = name[i];
    name[name.size()] = '\0';
    
    m_build_class__seuil = m_classes[m_current_class].get_seuil();

    m_build_class__students_firstname.clear();
    m_build_class__students_lastname.clear();

    for(size_t i=0; i<m_classes[m_current_class].size(); i++) {
        std::string firstname = m_classes[m_current_class].get_student_firstname(i);
        std::string lastname = m_classes[m_current_class].get_student_lastname(i);

        m_build_class__students_firstname.push_back(std::array<char, MAX_NAME_LENGTH>());
        for(size_t j=0; j < firstname.size(); j++) {
            m_build_class__students_firstname.back()[j] = firstname[j];
        }
        m_build_class__students_firstname.back()[firstname.size()] = '\0';


        m_build_class__students_lastname.push_back(std::array<char, MAX_NAME_LENGTH>());
        for(size_t j=0; j < lastname.size(); j++) {
            m_build_class__students_lastname.back()[j] = lastname[j];
        }
        m_build_class__students_lastname.back()[lastname.size()] = '\0';
    }
}
