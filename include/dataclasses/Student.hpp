#pragma once

class Student {
public:
    Student(const char* lastname, const char* firstname);

    const char *get_full_name() const;
    const char *get_fist_name() const;
    const char *get_last_name() const;

private:
    const char *m_lastname;
    const char *m_firstname;
};