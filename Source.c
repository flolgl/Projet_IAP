#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { FAUX = 0, VRAI = 1 } Booleen;
Booleen EchoActif = FAUX;

// Messages emis par les instructions -----------------------------------------
#define MSG_DEVELOPPE "## nouvelle specialite \"%s\" ; cout horaire \"%d\"\n" 
#define MSG_EMBAUCHE "## nouveau travailleur \"%s\" competent pour la specialite \"%s\"\n"
#define MSG_INTERRUPTION "## fin de programme\n" 
#define MSG_CHECK_TRAVAILLEURS "la specialite %s peut etre prise en charge par : "
#define MSG_NEW_CLIENT "## nouveau client \"%s\"\n"
#define MSG_CHECK_COMMANDE_CLIENT "le client %s a commande : "
#define MSG_CHECK_COMMANDE_ALL_CLIENT "## consultation des commandes effectuees par chaque client\n"
#define MSG_NEW_COMMANDE "## nouvelle commande \"%s\", par client \"%s\"\n"
#define MSG_CHECK_SUPERVISION "etat des taches pour %s : "
#define MSG_NEW_TACHE "## la commande \"%s\" requiere la specialite \"%s\" (nombre d’heures \"%d\")\n"
#define MSG_CHECK_CHARGE "## consultation de la charge de travail de \"%s\"\n"
#define MSG_AVANCEE_PROGRESSION "## pour la commande \"%s\", pour la specialite \"%s\" : \"%d\" heures de plus ont ete realisees\n"
#define MSG_REALLOCARTION_NEEDED "## une reallocation est requise\n"
#define MSG_CHECK_ALL_SPECIALITES "## consultation des specialites\n"
#define MSG_REALLOCATION_NEEDED "## une reallocation est requise\n"

// Lexemes -------------------------------------------------------------------- 
#define LGMOT 35
#define NBCHIFFREMAX 5 
#define MAX_STRING_LENGHT 50
typedef char Mot[LGMOT + 1];
//	Donnees −−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
//	specialites −−−−−−−−−−−−−−−−−−−−−
#define MAX_SPECIALITES 10

typedef  struct {
	Mot nom;
	int  cout_horaire;
} Specialite;

typedef  struct {
	Specialite tab_specialites[MAX_SPECIALITES];
	unsigned int nb_specialites;

} Specialites;

// travailleurs --------------------------
#define MAX_TRAVAILLEURS 50
typedef struct {
	Mot nom;
	Booleen tags_competences[MAX_SPECIALITES];
}Travailleur;

typedef struct {
	Travailleur tab_travailleurs[MAX_TRAVAILLEURS];
	unsigned int nb_travailleurs;
}Travailleurs;

// client ----------------------------

#define MAX_CLIENTS 10

typedef struct {
	Mot table_clients[MAX_CLIENTS];
	unsigned int nb_clients;
}Clients;


// commandes−−−−−−−−−−−−−−−−−−−−−−−
#define MAX_COMMANDES 500

typedef  struct {
	unsigned  int  nb_heures_requises;
	unsigned  int  nb_heures_effectuees;
} Tache;

typedef  struct {
	Mot nom;
	unsigned  int  idx_client;
	Tache  taches_par_specialite[MAX_SPECIALITES];  //  nb_heures_requises==0 <=> pas  de  tache  pour  cette  specialite
} Commande;

typedef  struct {
	Commande tab_commandes[MAX_COMMANDES];
	unsigned  int  nb_commandes;
} Commandes;


// ----------------------------- UTILITAIRE --------------------------------------

Booleen check_table(char* tableau_a_check[], Mot string_a_check) {
	unsigned int i;
	int table_size = sizeof(tableau_a_check);
	Booleen response = VRAI;
	for (i = 0; i > table_size; ++i) {
		if (strcmp(tableau_a_check[i], string_a_check) == 0) {
			response = FAUX;
			break;
		}
	}
	return response;
}

void get_id(Mot id) {
	scanf("%s", id);
	if (EchoActif) {
		printf(">>echo %s\n", id);
	}
}


int get_int() {
	char buffer[NBCHIFFREMAX + 1];
	scanf("%s", buffer);
	if (EchoActif) {
		printf(">>echo %s\n", buffer);
	}
	return atoi(buffer);
}

Booleen lire(char* chaine, int longueur)
{
	char* positionEntree = NULL;

	// On lit le texte saisi au clavier
	if (fgets(chaine, longueur, stdin) != NULL)  
	{
		positionEntree = strchr(chaine, '\n'); 
		if (positionEntree != NULL) 
		{
			*positionEntree = '\0'; 
		}
		return VRAI;
	}
	else
	{
		return FAUX;
	}
}


// Instructions --------------------------------------------------------------- 
// developpe --------------------------- 
void traite_developpe(Specialites* all_specialites) {
	Mot nom_specialite;
	get_id(nom_specialite);
	int cout_horaire = get_int();

	Specialite specialite_to_register;
	strcpy(specialite_to_register.nom, nom_specialite);
	specialite_to_register.cout_horaire = cout_horaire;

	if (all_specialites->nb_specialites > MAX_SPECIALITES) {
		//printf("Impossible, +%d specialites\n", MAX_SPECIALITES);
		return;
	}

	all_specialites->tab_specialites[all_specialites->nb_specialites] = specialite_to_register;
	all_specialites->nb_specialites += 1;
}


// embauche ---------------------------
void traite_embauche(const Specialites* all_specialites, Travailleurs* all_travailleurs) {
	Mot nom_employe, embauche_nom_specialite;
	unsigned int i;
	get_id(nom_employe);
	get_id(embauche_nom_specialite);
	Travailleur travailleur_to_register;


	if (all_travailleurs->nb_travailleurs > MAX_TRAVAILLEURS) {
		//printf("Impossible, +%d specialites\n", MAX_TRAVAILLEURS);
		return;
	}

	strcpy(travailleur_to_register.nom, nom_employe);
	for (i = 0; i < all_specialites->nb_specialites; i++) {
		if (strcmp(embauche_nom_specialite, all_specialites->tab_specialites[i].nom) == 0) {
			travailleur_to_register.tags_competences[i] = VRAI;
			break;
		}
	}


	all_travailleurs->tab_travailleurs[all_travailleurs->nb_travailleurs] = travailleur_to_register;
	all_travailleurs->nb_travailleurs += 1;
}

// demarche ---------------------------
void traite_new_client(Clients* all_clients) {
	Mot nom_client;
	get_id(nom_client);

	if (all_clients->nb_clients > MAX_CLIENTS) {
		//printf("Impossible, +%d clients\n", MAX_CLIENTS);
		return;
	}

	strcpy(all_clients->table_clients[all_clients->nb_clients], nom_client);
	all_clients->nb_clients += 1;
}

// commande ---------------------------
void traite_new_commande(Commandes* all_commandes, const Clients* all_clients, const Specialites* all_specialites) {
	Mot nom_commande, commande_nom_client;
	Commande commande_to_register;
	unsigned int i, j;
	get_id(nom_commande);
	get_id(commande_nom_client);

	if (all_commandes->nb_commandes > MAX_COMMANDES) {
		return;
	}

	strcpy(commande_to_register.nom, nom_commande);
	for (i = 0; i < all_clients->nb_clients; i++) {
		if (strcmp(commande_nom_client, all_clients->table_clients[i]) == 0) {
			commande_to_register.idx_client = i;
			break;
		}
	}

	for (j = 0; j < all_specialites->nb_specialites; j++) {
		commande_to_register.taches_par_specialite[j].nb_heures_requises = 0;
	}

	all_commandes->tab_commandes[all_commandes->nb_commandes] = commande_to_register;
	all_commandes->nb_commandes += 1;
}

// tache ---------------------------
void traitement_tache(Commandes* all_commandes, const Specialites* all_specialites) {
	Mot tache_nom_commande, tache_nom_specialite;
	Tache tache_to_register;
	unsigned int i, j;
	get_id(tache_nom_commande);
	get_id(tache_nom_specialite);
	tache_to_register.nb_heures_requises = get_int();
	tache_to_register.nb_heures_effectuees = 0;

	for (i = 0; i < all_commandes->nb_commandes; i++) { 
		if (strcmp(all_commandes->tab_commandes[i].nom, tache_nom_commande) == 0) { // Si parmi toutes les commandes tache_nom_commande == une commande alors
			for (j = 0; j < all_specialites->nb_specialites; j++) {
				if (strcmp(all_specialites->tab_specialites[j].nom, tache_nom_specialite) == 0) { // Si parmi toutes les specialites tache_nom_specialite == une specialite alors
					all_commandes->tab_commandes[i].taches_par_specialite[j] = tache_to_register; // On met tache_to_register à l'id de la specialite car c'est trié par specialite
				}
			}
		}
	}
}

// progression ---------------------------
void traitement_avancee_progression(Commandes* all_commandes, const Specialites* all_specialites) {
	Mot progression_nom_commande, progression_nom_specialite, progression_passe_necessaire;
	int progression_avancement_temps;
	unsigned int i = 0;
	unsigned int j;
	Booleen isPassePresent = FAUX;
	char progression_string[MAX_STRING_LENGHT];

	// Etant donné que "passe" est un argument facultatif. On ne peut pas utiliser scanf donc utilisation de fgets pour récupérer toute la ligne -> lire = fgets mais sans le \n
	lire(progression_string, MAX_STRING_LENGHT);
	// Couper la string en plusieurs petites strings
	char* progression_words = strtok(progression_string, " ");

	while (progression_words != NULL) {
		switch (i){
			case 0: 
				strcpy(progression_nom_commande, progression_words);
				break;
			case 1: 
				strcpy(progression_nom_specialite, progression_words);
				break;
			case 2: 
				progression_avancement_temps = atoi(progression_words);
				break;
			case 3: 
				strcpy(progression_passe_necessaire, progression_words);
				isPassePresent = VRAI;
				break;
			default:
				break;
		}
		progression_words = strtok(NULL, " ");
		++i;
	}

	for (i = 0; i < all_commandes->nb_commandes; ++i){
		if (strcmp(all_commandes->tab_commandes[i].nom, progression_nom_commande) == 0) {
			for (j = 0; j < all_specialites->nb_specialites; ++j) {
				if (strcmp(all_specialites->tab_specialites[j].nom, progression_nom_specialite) == 0) {
					all_commandes->tab_commandes[i].taches_par_specialite[j].nb_heures_effectuees += progression_avancement_temps;
				}

			}
			break;
		}
	}

	//printf(MSG_AVANCEE_PROGRESSION, progression_nom_commande, progression_nom_specialite, progression_avancement_temps);

	//if (isPassePresent == VRAI) {
	//	printf(MSG_REALLOCATION_NEEDED);
	//}
}



// ------------------------- AFFICHAGES -------------------------------------------

//specialites ---------------------------
// Affichage de toutes les spécialités 
void affichage_all_specialites(Specialites* all_specialites) {

	unsigned int i;
	printf("specialites traitees : ");
	for (i = 0; i < all_specialites->nb_specialites; i++) {
		printf("%s/%d", all_specialites->tab_specialites[i].nom, all_specialites->tab_specialites[i].cout_horaire);
		if (i != all_specialites->nb_specialites - 1) {
			printf(", ");
		}
	}
	printf("\n");
}


// travailleurs ---------------------------
void affichage_travailleurs(const Travailleurs* all_travailleurs, const Specialites* all_specialites) {
	Mot affichage_nom_specialite;
	char a[1000] = { '\0' };
	char b[1000] = { '\0' };
	get_id(affichage_nom_specialite);
	unsigned int i, j;
	if (strcmp(affichage_nom_specialite, "tous") == 0) {
		for (i = 0; i < all_specialites->nb_specialites; i++) {
			printf(MSG_CHECK_TRAVAILLEURS, all_specialites->tab_specialites[i].nom);
			for (j = 0; j < all_travailleurs->nb_travailleurs; j++) {
				if (all_travailleurs->tab_travailleurs[j].tags_competences[i] == VRAI) {
					if (a[0] == '\0') {
						strcpy(a, all_travailleurs->tab_travailleurs[j].nom);
						strcpy(b, all_travailleurs->tab_travailleurs[j].nom);
					}
					else{
						strcpy(b, all_travailleurs->tab_travailleurs[j].nom);
						strcat(b, ", ");
						strcat(b, a);
					}
				}
				if (j == all_travailleurs->nb_travailleurs - 1) {
					printf("%s", b);
					printf("\n");
					strcpy(a, "");
					strcpy(b, "");
				}
			}
		}
	}
	else {
		for (i = 0; i < all_specialites->nb_specialites; i++) {
			if (strcmp(all_specialites->tab_specialites[i].nom, affichage_nom_specialite) == 0) {
				printf(MSG_CHECK_TRAVAILLEURS, affichage_nom_specialite);
				for (j = 0; j < all_travailleurs->nb_travailleurs; j++) {
					if (all_travailleurs->tab_travailleurs[j].tags_competences[i] == VRAI) {
						if (a[0] == '\0') {
							strcpy(a, all_travailleurs->tab_travailleurs[j].nom);
							strcpy(b, all_travailleurs->tab_travailleurs[j].nom);
						}
						else {
							strcpy(b, all_travailleurs->tab_travailleurs[j].nom);
							strcat(b, ", ");
							strcat(b, a);
						}
					}
					if (j == all_travailleurs->nb_travailleurs - 1) {
						printf("%s", b);
						printf("\n");
						strcpy(a, "");
						strcpy(b, "");
					}
				}
			}
		}
	}
}

// client ---------------------------
void affichage_clients(const Clients* all_clients, const Commandes* all_commandes) {
	Mot affichage_nom_client;
	get_id(affichage_nom_client);
	unsigned int i, j, compteur_commandes;

	for (i = 0; i < all_clients->nb_clients; i++) {
		compteur_commandes = 0;
		if (strcmp(affichage_nom_client, "tous") == 0) {
			printf(MSG_CHECK_COMMANDE_CLIENT, all_clients->table_clients[i]);
			for (j = 0; j < all_commandes->nb_commandes; j++) {
				if (all_commandes->tab_commandes[j].idx_client == i) {
					if (compteur_commandes != 0) {
						printf(", ");
					}
					printf("%s", all_commandes->tab_commandes[j].nom);
					++compteur_commandes;

				}
			}
			printf("\n");
		}
		else {
			if (strcmp(affichage_nom_client, all_clients->table_clients[i]) == 0) {
				printf(MSG_CHECK_COMMANDE_CLIENT, all_clients->table_clients[i]);
				for (j = 0; j < all_commandes->nb_commandes; j++) {
					if (all_commandes->tab_commandes[j].idx_client == i) {
						if (compteur_commandes != 0) {
							printf(", ");
						}
						printf("%s", all_commandes->tab_commandes[j].nom);
						++compteur_commandes;
					}
				}
				printf("\n");
				break;
			}
		}
	}
}

// supervision ---------------------------
void affichage_supervision(const Commandes* all_commandes, const Specialites* all_specialites) {
	unsigned int i, j, compteur_commandes;
	for (i = 0; i < all_commandes->nb_commandes; i++) {
		compteur_commandes = 0;
		printf(MSG_CHECK_SUPERVISION, all_commandes->tab_commandes[i].nom);
		for (j = 0; j < all_specialites->nb_specialites; j++) {
			//printf("%d", all_commandes->tab_commandes[i].taches_par_specialite[j].nb_heures_requises);
			if (all_commandes->tab_commandes[i].taches_par_specialite[j].nb_heures_requises != 0) {
				if (compteur_commandes != 0) {
					printf(", ");
				}
				printf("%s:%d/%d", all_specialites->tab_specialites[j].nom, all_commandes->tab_commandes[i].taches_par_specialite[j].nb_heures_effectuees, all_commandes->tab_commandes[i].taches_par_specialite[j].nb_heures_requises);
				++compteur_commandes;
			}
		}
		printf("\n");
	}
	//printf(MSG_CHECK_SUPERVISION);
}

// charge ---------------------------
void affichage_charge() {
	Mot charge_nom_travailleur;
	get_id(charge_nom_travailleur);
	printf(MSG_CHECK_CHARGE, charge_nom_travailleur);
}


// ------------------------------------ END -----------------------------------------
// interruption ------------------------ 
void traite_interruption() {
	printf(MSG_INTERRUPTION);
}

// ------------------------------------ MAIN/Boucle principale -----------------------------------------
int main(int argc, char* argv[]) {
	Specialites toutes_les_specialites = { "", 0 }; // 0 -> nb Specialites/Travailleurs/Clients... au départ
	Travailleurs tous_les_travailleurs = { "", 0 };
	Clients tous_les_clients = { "", 0 };
	Commandes toutes_les_commandes = { "", 0 };
	if (argc >= 2 && strcmp("echo", argv[1]) == 0) {
		EchoActif = VRAI;
	}
	Mot buffer;
	while (VRAI) {
		get_id(buffer);


		if (strcmp(buffer, "developpe") == 0) {
			traite_developpe(&toutes_les_specialites);
			continue;
		}
		if (strcmp(buffer, "embauche") == 0) {
			traite_embauche(&toutes_les_specialites, &tous_les_travailleurs);
			continue;
		}			
		if (strcmp(buffer, "demarche") == 0) {
			traite_new_client(&tous_les_clients);
			continue;
		}			
		if (strcmp(buffer, "commande") == 0) {
			traite_new_commande(&toutes_les_commandes, &tous_les_clients, &toutes_les_specialites);
			continue;
		}			
		if (strcmp(buffer, "tache") == 0) {
			traitement_tache(&toutes_les_commandes, &toutes_les_specialites);
			continue;
		}			
		if (strcmp(buffer, "progression") == 0) {
			traitement_avancee_progression(&toutes_les_commandes, &toutes_les_specialites);
			continue;
		}	





		if (strcmp(buffer, "charge") == 0) {
			affichage_charge();
			continue;
		}	
		if (strcmp(buffer, "client") == 0) {
			affichage_clients(&tous_les_clients, &toutes_les_commandes);
			continue;
		}			
		if (strcmp(buffer, "travailleurs") == 0) {
			affichage_travailleurs(&tous_les_travailleurs, &toutes_les_specialites);
			continue;
		}		
		if (strcmp(buffer, "specialites") == 0) {
			affichage_all_specialites(&toutes_les_specialites);
			continue;
		}		
		if (strcmp(buffer, "supervision") == 0) {
			affichage_supervision(&toutes_les_commandes, &toutes_les_specialites);
			continue;
		}


		if (strcmp(buffer, "interruption") == 0) {
			traite_interruption();
			break;
		}


		printf("!!! instruction inconnue >%s< !!!\n", buffer);
	}
	return 0;
}
