#include <dataclasses/Student.hpp>

#include <utility>

Student::Student(std::string lastname, std::string firstname) : m_lastname(std::move(lastname)), m_firstname(std::move(firstname)) {

}

std::string Student::get_full_name() const {
    return (m_lastname + std::string(" ") + m_firstname);
}

std::string Student::get_fist_name() const {
    return m_firstname;
}

std::string Student::get_last_name() const
{
    return m_lastname;
}

void Student::set_firstname(std::string name) {
    m_firstname = std::move(name);
}

void Student::set_lastname(std::string name) {
    m_lastname = std::move(name);
}