#pragma once

#include <features/Feature.hpp>

#include <vector>
#include <array>
#include <functional>
#include <dataclasses/Class.hpp>
#include <dataclasses/Student.hpp>

class StudentRanking : public Feature {
public:
    StudentRanking();
    virtual ~StudentRanking();

    void draw_IMGui() override;

private:
    std::vector<Class> m_classes;
    int m_current_class = 0;

    bool m_is_create_class_open = false;
    bool m_is_class_editor_open = false;

    /* Variables relatives à la construction d'une classe */
    static constexpr size_t MAX_CLASSNAME_SIZE = 128;
    char m_build_class__name[MAX_CLASSNAME_SIZE];

    float m_build_class__bonus_value=0.;
    float m_build_class__high_malus_value=0.;
    float m_build_class__low_malus_value=0.;
    float m_build_class__seuil=0.;

    static constexpr size_t MAX_NAME_LENGTH = 128;
    std::vector<std::array<char, MAX_NAME_LENGTH>> m_build_class__students_lastname;
    std::vector<std::array<char, MAX_NAME_LENGTH>> m_build_class__students_firstname;

    Class m_editor__anterior_class;
    int m_editor__current_class = 0;

    bool m_highlight_students = false;

    enum SortMode {
        Sort_None,
        Sort_Lastname,
        Sort_Firstname,
        Sort_Points
    };

    SortMode m_sort_mode = Sort_None;
    bool m_sort_asc = true; // true = croissant, false = décroissant

private :
    void draw_class_selector();
    void draw_classes_header();
    void draw_students_info(int class_idx);

    void draw_create_class_window();
    void draw_class_editor_window();

    void draw_class_students_editor(std::function<void(size_t)> suppression_callback = {}, std::function<void()> addition_callback = {});
    void draw_class_settings_editor();

    std::vector<const char*> get_classes_names() const;

    void reset_build_class_attributes();
    void sync_class_attributes();
};