#include <dataclasses/Student.hpp>

#include <utility>
#include <IO/binary_io.h>

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

void Student::save(std::ofstream& out, float points) const {
    write_string(out, m_lastname);
    write_string(out, m_firstname);
    
    // Pour les variables simples (float, int), on écrit directement les octets
    out.write(reinterpret_cast<const char*>(&points), sizeof(points));
}

float Student::load(std::ifstream& in) {
    m_lastname = read_string(in);
    m_firstname = read_string(in);
    float points = 0.0f;
    in.read(reinterpret_cast<char*>(&points), sizeof(points));
    return points;
}