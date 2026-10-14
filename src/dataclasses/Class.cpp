#include <dataclasses/Class.hpp>

#include <utility>
#include <stdexcept>
#include <algorithm> // pour std::sort
#include <numeric> // pour std::iota
#include <IO/binary_io.h>

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

void Class::reserve(size_t capacity) {
    m_students.reserve(capacity);
    m_points.reserve(capacity);
    m_point_increments.reserve(capacity);
}

void Class::sort_by_lastname(bool ascendent) {
    if (ascendent) {
        apply_sort([this](size_t a, size_t b) {
            return m_students[a].get_last_name() < m_students[b].get_last_name();
        });
    } else {
        apply_sort([this](size_t a, size_t b) {
            return m_students[a].get_last_name() > m_students[b].get_last_name();
        });
    }
}

void Class::sort_by_firstname(bool ascendent) {
    if (ascendent) {
        apply_sort([this](size_t a, size_t b) {
            return m_students[a].get_first_name() < m_students[b].get_first_name();
        });
    } else {
        apply_sort([this](size_t a, size_t b) {
            return m_students[a].get_first_name() > m_students[b].get_first_name();
        });
    }
}

void Class::sort_by_points(bool ascendent) {
    if (ascendent) {
        [this](size_t a, size_t b) {
            return m_points[a] < m_points[b]; 
        };
    } else {
        [this](size_t a, size_t b) {
            return m_points[a] > m_points[b]; 
        };
    }
}

void Class::save(std::ofstream& out) const {
    write_string(out, m_name);
    out.write(reinterpret_cast<const char*>(&m_bonus_value), sizeof(m_bonus_value));
    out.write(reinterpret_cast<const char*>(&m_high_malus_value), sizeof(m_high_malus_value));
    out.write(reinterpret_cast<const char*>(&m_low_malus_value), sizeof(m_low_malus_value));
    out.write(reinterpret_cast<const char*>(&m_seuil), sizeof(m_seuil));

    // Sauvegarde du vecteur d'élèves avec leur points
    size_t nb_students = m_students.size();
    out.write(reinterpret_cast<const char*>(&nb_students), sizeof(nb_students));
    for(size_t i=0; i<m_students.size(); i++) {
        const Student& student = m_students[i];
        student.save(out, m_points[i]);
    }
}

void Class::load(std::ifstream& in) {
    m_name = read_string(in);
    in.read(reinterpret_cast<char*>(&m_bonus_value), sizeof(m_bonus_value));
    in.read(reinterpret_cast<char*>(&m_high_malus_value), sizeof(m_high_malus_value));
    in.read(reinterpret_cast<char*>(&m_low_malus_value), sizeof(m_low_malus_value));
    in.read(reinterpret_cast<char*>(&m_seuil), sizeof(m_seuil));

    // Chargement du vecteur d'élèves
    size_t nb_students = 0;
    in.read(reinterpret_cast<char*>(&nb_students), sizeof(nb_students));
    m_students.clear();
    this->reserve(nb_students);
    for (size_t i = 0; i < nb_students; ++i) {
        Student s("", ""); // Création d'un élève vide
        float points = s.load(in);        // Remplissage via le fichier
        m_students.push_back(std::move(s));
        m_points.push_back(points);
        m_point_increments.push_back(1.0f);
    }
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

void Class::apply_sort(const std::function<bool(size_t, size_t)>& less_than) {
    if (m_students.empty()) return;

    // 1. Créer un tableau contenant 0, 1, 2, ..., N-1
    std::vector<size_t> indices(m_students.size());
    std::iota(indices.begin(), indices.end(), 0);

    // 2. Trier ces indices en utilisant ta fonction de comparaison
    std::sort(indices.begin(), indices.end(), less_than);

    // 3. Créer de nouveaux tableaux pour recevoir les données triées
    std::vector<Student> sorted_students;
    std::vector<float> sorted_points;
    
    // Optimisation : réserver la mémoire à l'avance
    sorted_students.reserve(m_students.size());
    sorted_points.reserve(m_points.size());

    // 4. Appliquer le nouvel ordre
    for (size_t idx : indices) {
        // std::move évite de copier les std::string, c'est très performant
        sorted_students.push_back(std::move(m_students[idx]));
        sorted_points.push_back(std::move(m_points[idx]));
    }

    // 5. Remplacer les anciens tableaux par les nouveaux
    m_students = std::move(sorted_students);
    m_points = std::move(sorted_points);
}
