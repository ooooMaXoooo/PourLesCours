#include <dataclasses/Student.hpp>

#include <string>

Student::Student(const char *lastname, const char *firstname) : m_lastname(lastname), m_firstname(firstname) {

}

const char *Student::get_full_name() const {
    return (std::string(get_last_name()) + std::string(" ") + std::string(get_fist_name())).c_str();
}

const char *Student::get_fist_name() const {
    return m_firstname;
}

const char *Student::get_last_name() const
{
    return m_lastname;
}
