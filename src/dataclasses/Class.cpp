#include <dataclasses/Class.hpp>

#include <utility>
#include <stdexcept>

Class::Class(std::string name, float bonus_value, float high_malus_value, float low_malus_value, float seuil, size_t size) : 
    m_name(std::move(name)),
    m_bonus_value(bonus_value),
    m_high_malus_value(high_malus_value),
    m_low_malus_value(low_malus_value),
    m_seuil(seuil)
{
    m_students.reserve(size);
    m_points.reserve(size);
    m_point_increments.reserve(size);
}

size_t Class::size() const {
    return m_students.size();
}

std::string Class::get_name() const {
    return m_name;
}

const char *Class::get_name_c_str() const {
    return m_name.c_str();
}

void Class::clear() {
    m_students.clear();
    m_points.clear();
    m_point_increments.clear();
}

void Class::add_student(Student s) {
    m_students.push_back(std::move(s));
    m_points.push_back(0.);
    m_point_increments.push_back(1.);
}

void Class::remove_student(size_t index) {
    m_students.erase(m_students.begin() + index);
    m_points.erase(m_points.begin() + index);
    m_point_increments.erase(m_point_increments.begin() + index);
}

void Class::give_points_to_student(float pts, size_t student_index) {
    if (student_index >= size())
        throw std::invalid_argument("Impossible d'ajouter des points à un étudiant n'existant pas ! (student_index trop grand)");
    
    m_points[student_index] += pts;
}

void Class::remove_points_to_student(float pts, size_t student_index) {
    if (student_index >= size())
        throw std::invalid_argument("Impossible d'enlever des points à un étudiant n'existant pas ! (student_index trop grand)");

    m_points[student_index] -= pts;
}

void Class::give_bonus_to_student(size_t student_index){
    give_points_to_student(m_bonus_value, student_index);
}

void Class::give_high_malus_to_student(size_t student_index) {
    remove_points_to_student(m_high_malus_value, student_index);
}

void Class::give_low_malus_to_student(size_t student_index) {
    remove_points_to_student(m_low_malus_value, student_index);
}



const Student Class::get_student(size_t index) const {
    if (index >= size())
        throw std::invalid_argument("l'étudiant demandé n'existe pas ! (student_index trop grand)");

    return m_students[index];
}

const float Class::get_student_point(size_t index) const {
    if (index >= size())
        throw std::invalid_argument("l'étudiant demandé n'existe pas ! (student_index trop grand)");

    return m_points[index];
}

float *Class::get_student_point_increment(size_t index) {
    if (index >= size())
        throw std::invalid_argument("l'étudiant demandé n'existe pas ! (student_index trop grand)");

    return &(m_point_increments[index]);
}
