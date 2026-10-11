#pragma once

#include <vector>
#include <string>
#include <dataclasses/Student.hpp>

class Class {
public :
    Class(std::string name, float bonus_value, float high_malus_value, float low_malus_value, float seuil, size_t size);

    size_t size() const;
    std::string get_name() const;
    const char* get_name_c_str() const;
    void clear();

    void add_student(Student s);
    void remove_student(size_t index);

    void give_points_to_student(float pts, size_t student_index);
    void remove_points_to_student(float pts, size_t student_index);
    void give_bonus_to_student(size_t student_index);
    void give_high_malus_to_student(size_t student_index);
    void give_low_malus_to_student(size_t student_index);

    const Student get_student(size_t index) const;
    const float get_student_point(size_t index) const;

    float* get_student_point_increment(size_t index);

    void set_name(std::string name) { m_name = std::move(name); }
    void set_bonus_value(float bonus_value) { m_bonus_value = bonus_value; }
    void set_high_malus_value(float malus_value) { m_high_malus_value = malus_value; }
    void set_low_malus_value(float malus_value) { m_low_malus_value = malus_value; }
    void set_seuil(float seuil) { m_seuil = seuil; }

    void set_student_firstname(std::string name, size_t index) {m_students[index].set_firstname(std::move(name));}
    void set_student_lastname(std::string name, size_t index) {m_students[index].set_lastname(std::move(name));}


    float get_bonus_value() const { return m_bonus_value; }
    float get_high_malus_value() const { return m_high_malus_value; }
    float get_low_malus_value() const { return m_low_malus_value; }
    float get_seuil() const { return m_seuil; }

    std::string get_student_firstname(size_t index) const {return m_students[index].get_fist_name();}
    std::string get_student_lastname(size_t index) const {return m_students[index].get_last_name();}

private:
    std::vector<Student> m_students;
    std::vector<float> m_points;
    std::vector<float> m_point_increments;
    
    std::string m_name;

    float m_bonus_value;
    float m_high_malus_value;
    float m_low_malus_value;

    float m_seuil;
};