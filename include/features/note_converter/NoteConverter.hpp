#pragma once

#include <features/Feature.hpp>
#include <array>

/// @brief Renvoi une valeur floattante rendant compte de la note donnée parmi un ensemble fini de notes ordonnées.
/// @param gradeIdx L'indice de la note.
/// @param valMin La valeur minimale atteinte par la valeur de retour.
/// @param valMax La valeur maximale atteinte par la valeur de retour.
/// @param nbVals Le nombre de valeurs prises par les notes. (`0 <= gradeIdx < nbVals`)
float valueOfGrade(int gradeIdx, int valMin, int valMax, int nbVals);

class NoteConverter : public Feature {
public:    
    NoteConverter();

    void draw_IMGui() override;

private:
    const std::array<const char *, 13> m_grades = {"E", "D-", "D", "D+", "C-", "C", "C+", "B-", "B", "B+", "A-", "A", "A+"};
    std::array<float, 13> m_values = {0};

    int m_current_item = 0;
};