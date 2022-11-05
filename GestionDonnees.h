#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <array>

std::vector<size_t> DecodageEntete(std::ifstream& image);
std::vector<std::string> ChoixPalette(std::string Choix);

std::vector<std::vector<int>> CreationImage(std::ifstream& FichierImage, std::vector<size_t> TailleImage, std::array<size_t, 2> FacteursReduction);

void Parametrage(std::string parametre, bool& ArgHelp, bool& ArgInput, bool& Argoutput, bool& ArgPalette, bool& ArgWidth, bool& ArgHeight);