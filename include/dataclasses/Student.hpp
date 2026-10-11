#pragma once

#include <string>

class Student {
public:
    Student(std::string lastname, std::string firstname);

    std::string get_full_name() const;
    std::string get_fist_name() const;
    std::string get_last_name() const;

    void set_firstname(std::string name);
    void set_lastname(std::string name);

private:
    std::string m_lastname;
    std::string m_firstname;
};