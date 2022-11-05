#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <array>

#include "GestionDonnees.h"
#include "AffichageConsole.h"

#ifdef _WIN32
#include <Windows.h>
#endif

int main(int NbArguments, char* ListeArguments[])
{
#ifdef _WIN32
	SetConsoleOutputCP(65001);
	SetConsoleCP(65001);
#endif 
	std::string NomFichier, NomFichierSortie;
	bool ArgHelp = false, ArgInput = false, ArgOutput = false, ArgPaletteCharacters = false, ArgWidth = false, ArgHeight = false;
	std::vector<std::string> PaletteCharacters;

	for (size_t i = 1; i < NbArguments; i += 2)
	{
		std::string Parametre = ListeArguments[i];
		Parametrage(Parametre, ArgHelp, ArgInput, ArgOutput, ArgPaletteCharacters, ArgWidth, ArgHeight);//On utilisera des booléan pour vérifier si tel ou tel argument a était entré (on utilise des références pour changer les variables à traver la fonction)
		if (ArgHelp) {
			AffichageHelp();//On affiche les aides pour l'utilisateur
			return 0;//On termine le programme comme demandé (on nous demande de terminé le programme si l'argument entré est "--help")
		}

		if (ArgInput) {
			NomFichier = ListeArguments[i + 1];//Si l'utilisateur a renseigner un nom de fichier à lire après avoir entré l'argument "--input"
			/*if (NomFichier == "")//Si il a renseigner l'argument "--input" mais n'as pas renseigner de nom, on lui demande
				NomFichier = DemandeNomFichier();*/
		}
		else {
			NomFichier = DemandeNomFichier();//On n'as pas de ficheir à lire par défauts donc même si l'utilisateur n'a pas renseigner "--input" on lui demande
		}

		if (ArgOutput) {
			NomFichierSortie = ListeArguments[i + 1];//Si l'utilisateur souhaite faire ecrire son image ASCII sur un fichier texte, on récupère le nom qu'il a renseigner après l'argument "--output"
			/*if (NomFichierSortie == "")
				NomFichierSortie = DemandeNomTxtAModifier();//Pareil que pour input, si l'utilisateur a renseigner "--output" mais n'a pas mis de nom de fichier derrière, on lui demande*/
		}//Ici, si aucun nom de fichier à écrire n'est renseigner on ne lui en écrir pas et on n'affiche l'image ASCII seulement dans la console

		if (ArgPaletteCharacters) {
			std::cout << "Quelle palette voulez vous utiliser ?\n";
			PaletteCharacters = ChoixPalette(ListeArguments[i + 1]);//Si l'utilisateur souhaite choisir sa palette, on lui demande son choix
		}
		else
			PaletteCharacters = { " ",".",",",":","i","l","w","W" };//On initialise une palette par défauts si l'utilisateur n'en a pas choisi

	}

	//On vas maintenant vérifier si l'utilisateur à renseigner l'extension du fichier
	std::string Extension;

	NomFichier = "Google.pgm";//Sinon on l'ajoute au nom

	std::ifstream FichierLu(NomFichier, std::ios_base::binary);//On ouvre le fichier demander
	if (!FichierLu.is_open()) {//On vérifie sont ouverture
		std::cerr << "Problème d'ouverture du fichier \"" << NomFichier << "...\n";
		return -1;//Si l'ouverture c'est mal passé, on affiche un message d'erreur et on retourn -1
	}

	std::vector<size_t> DonneesDeEntete = DecodageEntete(FichierLu);
	if (DonneesDeEntete[0] == 0 && DonneesDeEntete[1] == 0)//Notre fonction nous retourner que l'image dans le fichier avait une taille de 0 0 si il découvrait une erreur dans la formulation du fichier
		return -1;//En retournant -1 on termine le fichier en précisant qu'il y a eu une erreur

	std::array<size_t, 2> FacteurReduction = FacteurVoulu(DonneesDeEntete);

	std::vector<std::vector<int>> NouvelleImage = CreationImage(FichierLu, DonneesDeEntete, FacteurReduction);
	AffichageImage(NouvelleImage, NomFichierSortie, PaletteCharacters);

	return 0;
}