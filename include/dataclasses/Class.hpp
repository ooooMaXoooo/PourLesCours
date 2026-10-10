#pragma once

#include <vector>

#include <dataclasses/Student.hpp>

class Class {
public :
    Class(const char* name, size_t size);

    size_t size() const;
    const char* get_name() const;

    void add_student(Student s);

    void give_points_to_student(float pts, size_t student_index);
    void give_bonus_to_student(size_t student_index);
    void give_malus_to_student(size_t student_index);

    void remove_points_to_student(float pts, size_t student_index);
    void remove_bonus_to_student(size_t student_index);
    void remove_malus_to_student(size_t student_index);

    const Student get_student(size_t index) const;
    const float get_student_point(size_t index) const;
    const bool get_student_bonus(size_t index) const;
    const bool get_student_malus(size_t index) const;

private:
    std::vector<Student> m_students;
    std::vector<float> m_points;
    std::vector<bool> m_bonuses;
    std::vector<bool> m_maluses;

    const char* m_name;
};