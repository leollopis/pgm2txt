#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <array>

std::string DemandeNomFichier();
std::string DemandeNomTxtAModifier();
std::array<size_t, 2> FacteurVoulu(std::vector<size_t> TailleImageOrigine);

void AffichageHelp();
void AffichageImage(std::vector<std::vector<int>> Image, std::string NomFichierSortie, std::vector<std::string> Palette);