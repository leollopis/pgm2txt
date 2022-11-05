#include "GestionDonnees.h"

void Parametrage(std::string parametre, bool& ArgHelp, bool& ArgInput, bool& Argoutput, bool& ArgPalette, bool& ArgWidth, bool& ArgHeight)
{
	//Detection des arguments
	std::string help = "--help", input = "--input", output = "--output", palette = "--palette", width = "--width", height = "--height";
	if (parametre == help)
	{
		ArgHelp = true;
	}
	else if (parametre == input)
	{
		ArgInput = true;
	}
	else if (parametre == output)
	{
		Argoutput = true;
	}
	else if (parametre == palette)
	{
		ArgPalette = true;
	}
	else if (parametre == width) {
		ArgWidth = true;
	}
	else if (parametre == height) {
		ArgHeight = true;
	}

	return;
}


std::vector<size_t> DecodageEntete(std::ifstream& FichierPGM) {
	std::string LigneLu;
	std::vector<size_t> DonneesEntete;//On stockera les données utile dans ce vecteur

	//Lecture de la première ligne du fichier
	std::getline(FichierPGM, LigneLu);
	if (LigneLu != "P5") { //Vérification que sur la première ligne on retrouve bien les characters "P5"
		std::cerr << "Le fichier semble ne pas être bien formulé...\n";
		return { 0,0 };
	}

	//Lecture de la deuxième Ligne
	std::getline(FichierPGM, LigneLu);
	if (LigneLu[0] != '#') { //Vérification que sur la deuxième ligne, le premier characters est bien "#"
		std::cerr << "Le fichier semble ne pas être bien formulé...\n";
		return { 0,0 };
	}

	//Lecture de la troisième Ligne (Ici la ligne comporte des données que l'on veut récuperer)
	std::getline(FichierPGM, LigneLu);
	std::string PartieDeLigne;
	std::stringstream ss(LigneLu);
	for (size_t i = 0; i < 2; ++i)
	{
		std::getline(ss, PartieDeLigne, ' ');//On sait que c'est données sont séparer par un espace, on peut dont utiliser un stringstream
		size_t Data;

		//Ici c'est deux lignes nous permette de convertir des string en size_t
		std::stringstream Convertissseur(PartieDeLigne);
		Convertissseur >> Data;

		DonneesEntete.push_back(Data);
	}

	//Lecture de la quatrième Ligne
	std::getline(FichierPGM, LigneLu);
	if (LigneLu != "255") {//Vérification que sur la quatrième ligne on retrouve bien "255"
		std::cerr << "Le fichier semble ne pas être bien formulé...\n";
		return { 0,0 };
	}

	return DonneesEntete;//Si tout c'est bien passé on retourne les données de l'entête utile
}

std::vector<std::string> ChoixPalette(std::string Choix) {

	std::vector<std::string> PaletteChoisi;
	std::string NomPalette = Choix;

	std::getline(std::cin >> std::ws, NomPalette);//On récupère le nom de la palette qu'a entré l'utilisateur
	std::ifstream ContenuePalette(NomPalette + ".txt");

	//On vas maintenant vérifier si l'utilisateur à renseigner l'extension du fichier
	std::string Extension;
	
	NomPalette += ".txt";//Sinon on l'ajoute au nom

	if (ContenuePalette.is_open()) {//On vérifie qu'on arrive à ouvrir le bon fichier texte
		std::string Ligne;
		while (!ContenuePalette.eof())//On parcoure le fichier texte pour récuperer toute la palette
		{
			std::getline(ContenuePalette, Ligne);
			PaletteChoisi.push_back(Ligne);
		}
	}
	else {//Si l'utilisateur a renseigner un nom de fichier érroné on utilisera la palette de base :
		return { " ",".",",",":","i","l","w","W" };
	}

	std::vector<std::string> PaletteRenverser;//Ici on inverse les palettes car c'est plus facile pour nous à utilisé
	for (int i = int(PaletteChoisi.size()) - 1; i >= 0; i--) {
		PaletteRenverser.push_back(PaletteChoisi[i]);
	}

	return PaletteRenverser;//On retourne la palette
}

std::vector<std::vector<int>> CreationImage(std::ifstream& FichierImage, std::vector<size_t> TailleImage, std::array<size_t, 2> FacteursReduction) {
	//On associe la hauteur/largueur et leur facteur de réduction à des variables
	size_t Largueur = TailleImage[0], Hauteur = TailleImage[1], FacteurReductionLargueur = FacteursReduction[0], FacteurReductionHauteur = FacteursReduction[1];


	//Ici on récupère tout les données de l'image
	std::vector<char>TableauDonnees(Largueur * Hauteur);
	FichierImage.read(TableauDonnees.data(), Largueur * Hauteur);

	//Pour mieux exploite l'image, on vas crée un vecteur à double colonnes pour avoir les données de toute la ligne pour chaque ligne au lieu d'avoir juste un bloc de donnée entier
	std::vector<std::vector<int>> ImageComplete;
	std::vector<int> LigneDonnees;

	for (char ch : TableauDonnees) {
		//On récupère chaque caractère du bloc de donnée
		LigneDonnees.push_back(int(ch));//On envoie chaque caracterss dans un vecteur qui renseignera toute la ligne de données
		//On connais la largueur de l'image donc on peut connaître le nombre de caractères qu'il y aura pour chaque ligne
		if (!Largueur) {//Quand on sais qu'on est arriver à la fin d'une ligne, il nous faut passer à la suivante
			Largueur = TailleImage[0];//On recommence une ligne donc on 'remet les compteurs à zéro'
			ImageComplete.push_back(LigneDonnees);//On envoie la ligne qu'on viens de récupèrer dans notre vecteur à double colonnes
			LigneDonnees.clear();//On nettoie le vecteur où on stocker la ligne pour en stocker une nouvelle
		}
		Largueur--;//Cette décrémentation nous sert à connaitre quand on arrive au bout de notre ligne de caracterss
	}

	//Les largueurs et hauteurs ayant étant utilise dans les boucles précedente, on a maintenant besoin de leur redonner leurs valeurs de départ
	Largueur = TailleImage[0];
	Hauteur = TailleImage[1];

	//Pour pouvoir réduire les images il vas nous falloir faire des moyennes entre plusieurs valeur
	//On vas créer un nouveau vecteur à double colonne qui comportera notre nouvelle image

	std::vector<std::vector<int>> NouvelleImage = ImageComplete;//Au cas ou l'image ne subis pas de changement du aux facteurs, on l'initialise comme étant l'image de base

	//Pour faire la moyenne des valeurs on a choisi de prendre un 'pixel' de notre image d'origine et de l'adition avec les pixel 'autour' de lui avant de faire une moyenne
	//Afin de trouver la couleur 'moyenne' de ce pixel
	if (FacteurReductionHauteur != 1 && FacteurReductionLargueur != 1) {//On execute cette partie que si il y a un facteur de réduction
		for (size_t i = 0; i < ImageComplete.size(); i += FacteurReductionHauteur) {
			for (size_t j = 0; j < ImageComplete[i].size(); j += FacteurReductionLargueur) {//On incrémente nos index des facteur de réduciton afin de ne pas faire la moyenne sur des pixel qui on déjà était 'visé'
				int ValeurPixel = 0;//On initialise la valeur de notre pixel
				for (size_t k = 0; k < FacteurReductionHauteur; ++k) {
					for (size_t l = 0; l < FacteurReductionLargueur; ++l) {
						int ValeurTemporaire = int(ImageComplete[i][j]); //Il nous faut une valeur par défaut donc on prend le pixel 'd'origine'
						if ((i + k < ImageComplete.size() - 1) && (j + l) < ImageComplete[i + k].size() && (i + k) < ImageComplete.size()) {//Vérification qui nous evite d'essayer d'utiliser des valeurs se trouvant hors du tableau
							ValeurTemporaire = int(ImageComplete[i + k][j + l]);
						}
						//Les caracterss ayant parfois des valeurs négatifs
						//On s'est rendu compte que la valeur 0 corresponder à du noir, que la valeur 127 à du gris
						//Mais que la valeur -1 étant du gris (le gris juste après celui qu'on trouve en 127) et que -128 était du blanc
						if (ValeurTemporaire < 0)//Donc si l'on detecte une valeur négatif
							ValeurTemporaire += 255;//On lui ajoute 255 afin de la faire passer négatif et de la 'mettre' à la suite du gris à 127
						ValeurPixel += ValeurTemporaire;//On ajoute notre valeur temporaire à notre pixel
					}
				}
				//Maintenant que notre nouveau pixel contient les valeurs de tout les pixels autours de lui, on lui fais maintenant ça moyenne
				if (FacteurReductionHauteur != 1 && FacteurReductionLargueur == 1)
					ValeurPixel /= (FacteurReductionHauteur);
				else if (FacteurReductionHauteur == 1 && FacteurReductionLargueur != 1)
					ValeurPixel /= (FacteurReductionLargueur);
				else
					ValeurPixel /= (FacteurReductionLargueur + FacteurReductionHauteur);
				LigneDonnees.push_back(ValeurPixel);//On envoie maintenant ce pixel dans notre novuelle ligne
			}
			NouvelleImage.push_back(LigneDonnees);//On envoie la nouvelle ligne dans notre vecteur contenant notre nouvelle image
			LigneDonnees.clear();//On nettoie notre ligne
		}
	}

	return NouvelleImage;//On renvoie notre image final
}
