#pragma once

#include <features/Feature.hpp>

#include <vector>
#include <dataclasses/Class.hpp>

class StudentRanking : public Feature {
public:
    StudentRanking();

    void draw_IMGui() override;

private:
    std::vector<Class> m_classes;

    int m_current_class = 0;

private :
    void draw_classes_header();
    void draw_students_info(int class_idx);
    std::vector<const char*> get_classes_names() const;
};