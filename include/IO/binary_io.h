#pragma once

#include <fstream>
#include <string>
#include <vector>

// Écrit la taille du texte, puis le texte
inline void write_string(std::ofstream& out, const std::string& str) {
    size_t size = str.size();
    out.write(reinterpret_cast<const char*>(&size), sizeof(size));
    if (size > 0) out.write(str.data(), size);
}

// Lit la taille, prépare la place, puis lit le texte
inline std::string read_string(std::ifstream& in) {
    size_t size = 0;
    in.read(reinterpret_cast<char*>(&size), sizeof(size));
    std::string str(size, '\0'); // Prépare un buffer de la bonne taille
    if (size > 0) in.read(str.data(), size);
    return str;
}