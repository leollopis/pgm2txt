#include "AffichageConsole.h"

std::string DemandeNomFichier() {
	std::cout << "Inserez le nom d'un fichier (pgm)  : ";//On lui demande un nom de fichier qu'on retourne au programme principal
	std::string Nom;
	std::cin >> Nom;
	return Nom;
}

std::string DemandeNomTxtAModifier() {
	std::cout << "Inserez le nom d'un fichier : ";//On lui demande un nom de fichier qu'on retourne au programme principal
	std::string NomSortie;
	std::cin >> NomSortie;
	return NomSortie;
}

std::array<size_t, 2> FacteurVoulu(std::vector<size_t> TailleImageOrigine) {
	std::cout << "La taille du fichier est : " << TailleImageOrigine[0] << " " << TailleImageOrigine[1] << '\n';
	std::cout << "Quelle réduction souhaitez vous ? (Reduction hauteur, reduction largueur)\n";
	size_t FacteurHauteur, FacteurLargueur;//L'utilisateur renseigner ici les facteurs de réduction qu'il souhaite appliquer à son image
	std::cin >> FacteurHauteur >> FacteurLargueur;
	return { FacteurHauteur, FacteurLargueur };
}

void AffichageHelp() {
	//On affiche ce qu'est censé voir l'utilisateur lorsqu'il rentre l'argument "--help"
	std::cout << "Option :\n"
		<< "--input fichier  \t Spécifie le fichier image à convertir\n"
		<< "\t\t\t Si ce paramètre n'est pas spécifié, le fichier est demandé via la\n"
		<< "\t\t\t console.\n\n"
		<< "--output fichier \t Spécifie le nom du fichier texte qui contiendra l'Ascii Art.\n"
		<< "\t\t\t Si ce paramètre n'est pas spécifié, l'Ascii Art est sortie dans la\n"
		<< "\t\t\t console.\n\n"
		<< "--palette fichier \t Spécifie un fichier texte contenant la palette de couleur Ascii.\n"
		<< "\t\t\t Chaque ligne du fichier contient un charactère en UTF-8, du plus\n"
		<< "\t\t\t sombre au plus clair.\n"
		<< "\t\t\t Si ce paramètre n'est pas spécifié, la palette par défaut est\n"
		<< "\t\t\t \"W\", \"w\", \"l\", \"i\", \":\", \", \", \".\", \" \" \n\n"
		<< "--width nombre  \t Spécifie la largeur max de l'Ascii Art.\n"
		<< "\t\t\t Si ce paramètre n'est pas spécifié, aucune limite n'est fixée.\n"
		<< "\t\t\t Voir Remarques.\n\n"
		<< "--height nombre \t Spécifie la hauteur max de l'Ascii Art.\n"
		<< "\t\t\t Si ce paramètre n'est pas spécifié, aucune limite n'est fixée.\n"
		<< "\t\t\t Voir Remarques.\n\n"
		<< "--help     \t\t Affiche cette aide.";

	return;
}

void AffichageImage(std::vector<std::vector<int>> Image, std::string NomFichierSortie, std::vector<std::string> Palette) {

	//On vérifie qu'on est bien un nom de fichier à ouvrir
	bool NomFichierEcrireRenseigner = false;
	std::ofstream out;
	if (NomFichierSortie != "") {
		out.open(NomFichierSortie);
		NomFichierEcrireRenseigner = true;
	}

	for (std::vector<int> Lignes : Image) {//Pour chaque pixels 
		for (int Pixel : Lignes) {
			if (Pixel < 0)
				Pixel += 255;
			std::string AsciiColor = Palette[0];
			double Ecart = 255 / Palette.size();//Créer une plage selon la quantité de caractères qu'on a dans une palette
			for (size_t i = 0; i < Palette.size(); ++i) {//on vas vérifier sa valeur et on vas lui assigner le caractère correspondant dans la palette
				if (Pixel < Ecart * i && Pixel <= Ecart * (i+1)) {
					AsciiColor = Palette[Palette.size() - i];
					break;
				}
			}
			if (NomFichierEcrireRenseigner)//Si fichier texte renseigné on écrit dedans sinon on écrit dans le cmd
				out << AsciiColor;
			else
				std::cout << AsciiColor;
		}
		if (NomFichierEcrireRenseigner)//Si fichier texte renseigné on écrit dedans sinon on écrit dans le cmd
			out << "\n";
		else
			std::cout << "\n";
	}
}