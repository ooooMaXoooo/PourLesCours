#include <dataclasses/Class.hpp>

#include <stdexcept>

Class::Class(const char* name, size_t size) : m_name(name) {
    m_students.reserve(size);
    m_points.reserve(size);
    m_bonuses.reserve(size);
    m_maluses.reserve(size);
}

size_t Class::size() const {
    return m_students.size();
}

const char *Class::get_name() const {
    return m_name;
}

void Class::add_student(Student s) {
    m_students.push_back(std::move(s));
    m_points.push_back(0);
    m_bonuses.push_back(false);
    m_maluses.push_back(false);
}

void Class::give_points_to_student(float pts, size_t student_index) {
    if (student_index >= size())
        throw std::invalid_argument("Impossible d'ajouter des points à un étudiant n'existant pas ! (student_index trop grand)");
    
    m_points[student_index] += pts;
}

void Class::give_bonus_to_student(size_t student_index){
    if (student_index >= size())
        throw std::invalid_argument("Impossible d'ajouter un bonus à un étudiant n'existant pas ! (student_index trop grand)");

    m_bonuses[student_index] = true;
}

void Class::give_malus_to_student(size_t student_index) {
    if (student_index >= size())
        throw std::invalid_argument("Impossible d'ajouter un malus à un étudiant n'existant pas ! (student_index trop grand)");

    m_maluses[student_index] = true;
}




void Class::remove_points_to_student(float pts, size_t student_index) {
    if (student_index >= size())
        throw std::invalid_argument("Impossible d'enlever des points à un étudiant n'existant pas ! (student_index trop grand)");

    m_points[student_index] -= pts;
}

void Class::remove_bonus_to_student(size_t student_index) {
    if (student_index >= size())
        throw std::invalid_argument("Impossible d'enlever un bonus à un étudiant n'existant pas ! (student_index trop grand)");

    m_bonuses[student_index] = false;
}

void Class::remove_malus_to_student(size_t student_index) {
    if (student_index >= size())
        throw std::invalid_argument("Impossible d'enlever un malus à un étudiant n'existant pas ! (student_index trop grand)");

    m_maluses[student_index] = false;
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

const bool Class::get_student_bonus(size_t index) const {
    if (index >= size())
        throw std::invalid_argument("l'étudiant demandé n'existe pas ! (student_index trop grand)");

    return m_bonuses[index];
}

const bool Class::get_student_malus(size_t index) const {
    if (index >= size())
        throw std::invalid_argument("l'étudiant demandé n'existe pas ! (student_index trop grand)");

    return m_maluses[index];
}
