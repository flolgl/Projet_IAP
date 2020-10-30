#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { FAUX = 0, VRAI = 1 } Booleen;
Booleen EchoActif = FAUX;
Booleen StopProgramme = FAUX;

// Messages emis par les instructions -----------------------------------------
#define MSG_FACTURATION_INTERRUPTION "facturations : " 
#define MSG_CHECK_TRAVAILLEURS "la specialite %s peut etre prise en charge par : "
#define MSG_CHECK_COMMANDE_CLIENT "le client %s a commande : "
#define MSG_CHECK_SUPERVISION "etat des taches pour %s : "
#define MSG_CHECK_CHARGE "charge de travail pour %s : "
#define MSG_FACTURATION "facturation %s : "
#define MSG_INTERRUPTION "## fin de programme"

// Lexemes -------------------------------------------------------------------- 
#define LGMOT 35
#define NBCHIFFREMAX 5 
#define MAX_STRING_LENGHT 118
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
	unsigned int nb_heures_a_effectuer;
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
	unsigned int idx_travailleur;
} Tache;

typedef  struct {
	Mot nom;
	unsigned  int  idx_client;
	Tache  taches_par_specialite[MAX_SPECIALITES];
	Booleen commande_finie;
} Commande;

typedef  struct {
	Commande tab_commandes[MAX_COMMANDES];
	unsigned  int  nb_commandes;
} Commandes;

// ----------------------------- Prototypes --------------------------------------
/**
	*La fonction "affichage facturation"
	*Affiche une facturation par spécialité utilisée lorsqu'une commande est terminée.

	@param l'entier i, Commandes, Specialites et Clients
*/
void affichage_facturation(unsigned int i, const Commandes* all_commandes, const Specialites* all_specialites, const Clients* all_clients);

/**
	*La fonction "affichage facturation totale"
	*Affiche une facturation globale lorsque toutes les commandes sont terminées.

	@param Clients, Commandes et Specialites 
*/
void facturation_totale(const Clients* all_clients, const Commandes* all_commandes, const Specialites* all_specialites);
/**
	*La fonction "check si facturation totale" de type booleen
	*Vérifie et Retourne Vrai si toutes les commandes sont terminées et Faux si elles ne sont pas terminées

	@param Commandes
	@return "Vrai" ou "Faux"
*/
Booleen check_si_facturation_totale(const Commandes* all_commandes);

void get_id(Mot id);
int get_int();
Booleen lire(char* chaine, int longueur);

int find_commande(Commandes* all_commandes, Mot nom_commande);
int find_specialite(Specialites* all_specialites, Mot nom_specialite);
int find_travailleur(Travailleurs* all_travailleurs, Mot nom_travailleur);
int find_client(Clients* all_clients, Mot nom_client);

/**
	*La Fonction "developpe"
	*Déclare une nouvelle spécialité de tâches. Le Mot nom_specialite correspond au nom de la spécialité ; l’entier cout_horaire indique le prix d’une heure de travail pour cette spécialité.
	Le système traite au maximum 10 spécialités.
	@param Specialites

*/
void traite_developpe(Specialites* all_specialites);

/**
	*La Fonction "embauche"
	*Déclare un nouveau travailleur. Mot nom_employe correspond à son nom ; Mot embauche_nom_specialite à sa spécialité.
	  Peut être utilisée pour un travailleur déjà déclaré. Dans ce cas, une nouvelle spécialité est ajoutée pour ce travailleur.
	Le système traite au maximum 50 travailleurs.

	@warning La spécialité doit avoir été précédement déclarée et Chaque travailleur doit avoir un nom distinct.
	@param Specialites et Travailleurs
*/
void traite_embauche(const Specialites* all_specialites, Travailleurs* all_travailleurs);

/**
	*La fonction "demarche"
	*Déclare un client. Mot nom_client correspond a son nom.
	Le système traite au maximum 100 clients.

	@warning Chaque client doit avoir un nom distinct.
	@param Clients

*/
void traite_new_client(Clients* all_clients);

/**
	*La fonction "commande"
	*Déclare une nouvelle commande. Mot nom_commande définit la commande. Mot commande_nom_client correspond au nom du client effectuant la commande.
	Le système traite au maximum 500 commandes.

	@warning  Chaque commande doit avoir un nom distinct
	@param Commandes, Clients et Specialites
*/
void traite_new_commande(Commandes* all_commandes, const Clients* all_clients, const Specialites* all_specialites);

/**
	*La fonction "tache"
	*Déclare une tâche nécessaire à la réalisation d’une commande. Mot tache_nom_commande désigne la commande ; Mot tache_nom_specialite désigne la spécialité concernée.
	L’entier nb_heure indique le nombre d’heures nécessaires pour réaliser la tâche.
	Chaque commande peut avoir au maximum une tâche par spécialité

	@param Commandes, Specialites et Travailleurs
*/
void traitement_tache(Commandes* all_commandes, const Specialites* all_specialites, Travailleurs* all_travailleurs);
void assign_tache_to_travailleur(Travailleurs* all_travailleurs, const Specialites* all_specialites, Commandes* all_commandes, Mot tache_nom_specialite, unsigned int index_commande, unsigned int index_specialite, unsigned int nb_heures_requises);

/**
	*La fonction "progression"
	*Déclare l’avancement d’une tâche. Mot progression_nom_commande désigne la commande ; Mot progression_nom_specialite la spécialité ; l’entier progression_avancement_temps indique le nombre d’heures à ajouter à son avancement
	Lorsque le Mot progression_passe_necessaire est ajouté, il indique qu’il faut, en plus du comportement ci-dessus, réaffecter la tâche.
*/
void traitement_avancee_progression(Commandes* all_commandes, const Specialites* all_specialites, Travailleurs* all_travailleurs, Clients* all_clients);

/**
	*La fonction "check si commande est finie"
	*Vérifie et Retourne Vrai si toutes les commandes sont terminées ou Faux si elles ne sont pas terminées
*/
Booleen check_if_commande_est_finie(unsigned int i, const Commandes* all_commandes, const Specialites* all_specialites);


void traite_reallocation(Commandes* all_commandes, const Specialites* all_specialites, Travailleurs* all_travailleurs, Mot nom_specialite, Mot nom_commande, unsigned int old_idx_travailleur);
void progression_traitement_ligne(char* progression_string, Mot* progression_nom_commande, Mot* progression_nom_specialite, int* progression_avancement_temps, Mot* progression_passe_necessaire, Booleen* isPassePresent);

/**
	*La fonction "affichage specialités"
	*Affiche la liste des spécialités, dans l’ordre de leur déclaration, ainsi que le coût associé.
	@param Specialites (vérifie la ligne 165 et 670 il n'y'a pas une erreur de Parametre ?)
*/
void affichage_all_specialites(Specialites* all_specialites);

/**
	*La fonction "affichage travailleurs"
	*Execute et affiche les fonctions "affichage_travailleurs_tous" et "affichage_travailleur_unique" en fonction de la présence du mot clé "tous" ou non.

	@param Specialites
*/
void affichage_travailleurs(const Travailleurs* all_travailleurs, const Specialites* all_specialites);

/**
	*La fonction "affichage travailleurs tous"
	*Affiche la liste des travailleurs pour toutes les spécialités. Les spécialités sont affichées dans l’ordre de leur déclaration ; pour une même
	spécialité, les travailleurs sont affichés par ordre de déclaration.

	@param Travailleurs et Specialites
*/
void affichage_travailleurs_tous(const Travailleurs* all_travailleurs, const Specialites* all_specialites);

/**
	*La fonction "affichage travailleurs unique"
	*Affiche la liste des travailleurs compétents pour la spécialité choisie, par ordre de déclaration des travailleurs.

	@param Travailleurs, Specialites et Mot
*/
void affichage_travailleur_unique(const Travailleurs* all_travailleurs, const Specialites* all_specialites, Mot nom_specialite);

/**
	*La fonction "affichage Clients"
	*Execute et affiche les fonctions "affichage_clients_tous" et "affichage_client_unique" en fonction de la présence du mot clé "tous" ou non.

	@param Clients et Commande
*/
void affichage_clients(const Clients* all_clients, const Commandes* all_commandes);

/**
	*La fonction "affichage client tous"
	*Affiche la liste de toutes les commandes, par ordre de déclaration de client, et de commande.

	@param Clients et Commandes
*/
void affichage_clients_tous(const Clients* all_clients, const Commandes* all_commandes);

/**
	*La fonction "affichage client unique"
	*Affiche la liste des commandes effectuées par le client choisi, selon leur ordre de déclaration

	@param Clients, Commande et Mot
*/
void affichage_client_unique(const Clients* all_clients, const Commandes* all_commandes, Mot affichage_nom_client);

/**
	*La fonction "affichage supervision"
	*Affiche la description de l’état d’avancement des commandes et des tâches associées, par ordre de commandes, et de spécialité.

	@param Commandes et Specialites
*/
void affichage_supervision(const Commandes* all_commandes, const Specialites* all_specialites);

/**
	*La fonction "affichage charge"
	*Pour le travailleur choisi, affiche le nombre d’heures restantes pour l’accomplissement de chacune de ses tâches . 
	Les tâches sont affichées par ordre de commande, et de spécialité.

	@param Commandes, Specialites et Travailleurs
*/
void affichage_charge(Commandes* all_commandes, const Specialites* all_specialites, Travailleurs* all_travailleurs);

/**
	*La fonction "interruption"
	*Quitte le programme
*/
void traite_interruption();

// ------------------------------------ MAIN/Boucle principale -----------------------------------------
int main(int argc, char* argv[]) {
	Specialites toutes_les_specialites;
	Travailleurs tous_les_travailleurs;
	Clients tous_les_clients;
	Commandes toutes_les_commandes;
	toutes_les_specialites.nb_specialites = 0;
	tous_les_travailleurs.nb_travailleurs = 0;
	tous_les_clients.nb_clients = 0;
	toutes_les_commandes.nb_commandes = 0;
	if (argc >= 2 && strcmp("echo", argv[1]) == 0) {
		EchoActif = VRAI;
	}
	Mot buffer;
	while (VRAI) {
		get_id(buffer);
		if (StopProgramme == VRAI) {
			break;
		}
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
			traitement_tache(&toutes_les_commandes, &toutes_les_specialites, &tous_les_travailleurs);
			continue;
		}
		if (strcmp(buffer, "progression") == 0) {
			traitement_avancee_progression(&toutes_les_commandes, &toutes_les_specialites, &tous_les_travailleurs, &tous_les_clients);
			continue;
		}

		if (strcmp(buffer, "charge") == 0) {
			affichage_charge(&toutes_les_commandes, &toutes_les_specialites, &tous_les_travailleurs);
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

// ----------------------------- UTILITAIRE --------------------------------------

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

Booleen lire(char* chaine, int longueur){ // Crédits au site du zero
	char* positionEntree = NULL;

	// On lit le texte saisi au clavier
	if (fgets(chaine, longueur, stdin) != NULL){
		positionEntree = strchr(chaine, '\n'); 
		if (positionEntree != NULL) 
		{
			*positionEntree = '\0'; 
		}
		return VRAI;
	}
	else {
		return FAUX;
	}
}

int find_commande(Commandes* all_commandes, Mot nom_commande) {
	int i;
	for (i = 0; i < all_commandes->nb_commandes; i++) {
		if (strcmp(all_commandes->tab_commandes[i].nom, nom_commande) == 0) {
			return i;
		}
	}
	return -1;
}

int find_specialite(Specialites* all_specialites, Mot nom_specialite) {
	int i;
	for (i = 0; i < all_specialites->nb_specialites; i++) {
		if (strcmp(all_specialites->tab_specialites[i].nom, nom_specialite) == 0) {
			return i;
		}
	}
	return -1;
}

int find_travailleur(Travailleurs* all_travailleurs, Mot nom_travailleur) {
	int i;
	for (i = 0; i < all_travailleurs->nb_travailleurs; i++) {
		if (strcmp(all_travailleurs->tab_travailleurs[i].nom, nom_travailleur) == 0) {
			return i;
		}
	}
	return -1;
}

int find_client(Clients* all_clients, Mot nom_client) {
	int i;
	for (i = 0; i < all_clients->nb_clients; i++) {
		if (strcmp(all_clients->table_clients[i], nom_client) == 0) {
			return i;
		}
	}
	return -1;
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

	if (all_specialites->nb_specialites > MAX_SPECIALITES) { printf("err01"); return;}

	all_specialites->tab_specialites[all_specialites->nb_specialites] = specialite_to_register;
	all_specialites->nb_specialites += 1;
}


// embauche ---------------------------
void traite_embauche(const Specialites* all_specialites, Travailleurs* all_travailleurs) {
	Mot nom_employe, embauche_nom_specialite;
	int i,j;
	get_id(nom_employe);
	get_id(embauche_nom_specialite);
	Travailleur travailleur_to_register;

	j = find_travailleur(all_travailleurs, nom_employe);
	i = find_specialite(all_specialites, embauche_nom_specialite);

	if (all_travailleurs->nb_travailleurs > MAX_TRAVAILLEURS) {printf("err02.1"); return;}
	if (i == -1) {printf("err02.2"); return;}

	if (j == -1) {
		strcpy(travailleur_to_register.nom, nom_employe);
		travailleur_to_register.tags_competences[i] = VRAI;

		travailleur_to_register.nb_heures_a_effectuer = 0;
		all_travailleurs->tab_travailleurs[all_travailleurs->nb_travailleurs] = travailleur_to_register;
		all_travailleurs->nb_travailleurs += 1;
	}
	else {
		all_travailleurs->tab_travailleurs[j].tags_competences[i] = VRAI;
	}
}

// demarche ---------------------------
void traite_new_client(Clients* all_clients) {
	Mot nom_client;
	get_id(nom_client);

	if (all_clients->nb_clients > MAX_CLIENTS) {printf("err03.1"); return;}
	if (find_client(all_clients, nom_client) != -1) {printf("err03.2"); return;}

	strcpy(all_clients->table_clients[all_clients->nb_clients], nom_client);
	all_clients->nb_clients += 1;
}


// commande ---------------------------
void traite_new_commande(Commandes* all_commandes, const Clients* all_clients, const Specialites* all_specialites) {
	Mot nom_commande, commande_nom_client;
	Commande commande_to_register;
	unsigned int j;
	int i;
	get_id(nom_commande);
	get_id(commande_nom_client);

	i = find_client(all_clients, commande_nom_client);

	if (all_commandes->nb_commandes > MAX_COMMANDES) {printf("err04.1"); return;}
	if (i == -1) {printf("err04.2"); return;}

	commande_to_register.idx_client = i;
	strcpy(commande_to_register.nom, nom_commande);
	commande_to_register.commande_finie = FAUX;


	for (j = 0; j < all_specialites->nb_specialites; j++) {
		commande_to_register.taches_par_specialite[j].nb_heures_requises = 0;
		commande_to_register.taches_par_specialite[j].nb_heures_effectuees = 0;
	}

	all_commandes->tab_commandes[all_commandes->nb_commandes] = commande_to_register;
	all_commandes->nb_commandes += 1;
}


// tache ---------------------------

void traitement_tache(Commandes* all_commandes, const Specialites* all_specialites, Travailleurs* all_travailleurs) {
	Mot tache_nom_commande, tache_nom_specialite;
	Tache tache_to_register;
	Booleen peutOnContinuer = FAUX;
	int i, j;
	get_id(tache_nom_commande);
	get_id(tache_nom_specialite);
	tache_to_register.nb_heures_requises = get_int();
	tache_to_register.nb_heures_effectuees = 0;

	i = find_commande(all_commandes, tache_nom_commande);
	j = find_specialite(all_specialites, tache_nom_specialite);
	if (j == -1 || i == -1) {printf("err05, i=%d j=%d",i,j); return;}

	all_commandes->tab_commandes[i].taches_par_specialite[j] = tache_to_register;
	peutOnContinuer = VRAI;

		

	if (peutOnContinuer == VRAI) {
		assign_tache_to_travailleur(all_travailleurs, all_specialites, all_commandes, tache_nom_specialite, i, j, tache_to_register.nb_heures_requises);
	}
}

void assign_tache_to_travailleur(Travailleurs* all_travailleurs, const Specialites* all_specialites, Commandes* all_commandes, Mot tache_nom_specialite, unsigned int index_commande, unsigned int index_specialite, unsigned int nb_heures_requises){
	unsigned int i, id_travailleur; 
	int nb_heures_min, j;

	nb_heures_min = -1;
	j = find_specialite(all_specialites, tache_nom_specialite);

	if (j == -1) {printf("err06.1"); return;}

	for (i = 0; i < all_travailleurs->nb_travailleurs; i++) {
		if (all_travailleurs->tab_travailleurs[i].tags_competences[j] == VRAI) { 
			if (nb_heures_min == -1) {
				nb_heures_min = all_travailleurs->tab_travailleurs[i].nb_heures_a_effectuer;
				id_travailleur = i;
			}
			else {
				if (nb_heures_min > all_travailleurs->tab_travailleurs[i].nb_heures_a_effectuer) {
					nb_heures_min = all_travailleurs->tab_travailleurs[i].nb_heures_a_effectuer;
					id_travailleur = i;
				}
			}
		}
	}

	if (nb_heures_min == -1) {printf("err06.2"); return;}

	all_commandes->tab_commandes[index_commande].taches_par_specialite[index_specialite].idx_travailleur = id_travailleur;
	all_travailleurs->tab_travailleurs[id_travailleur].nb_heures_a_effectuer += nb_heures_requises; 

}

// progression ---------------------------

void traitement_avancee_progression(Commandes* all_commandes, const Specialites* all_specialites, Travailleurs* all_travailleurs, Clients* all_clients) {
	Mot progression_nom_commande, progression_nom_specialite, progression_passe_necessaire;
	int progression_avancement_temps, i, j;
	unsigned int idx_travailleur; 
	Booleen isPassePresent = FAUX;
	char progression_string[MAX_STRING_LENGHT];

	lire(progression_string, MAX_STRING_LENGHT);

	progression_traitement_ligne(&progression_string, &progression_nom_commande, &progression_nom_specialite, &progression_avancement_temps, progression_passe_necessaire, &isPassePresent);

	i = find_commande(all_commandes, progression_nom_commande);
	j = find_specialite(all_specialites, progression_nom_specialite);
	if (i == -1 || j == -1) {printf("err07"); return;}

	all_commandes->tab_commandes[i].taches_par_specialite[j].nb_heures_effectuees += progression_avancement_temps;
	idx_travailleur = all_commandes->tab_commandes[i].taches_par_specialite[j].idx_travailleur;
	all_travailleurs->tab_travailleurs[idx_travailleur].nb_heures_a_effectuer -= progression_avancement_temps;
					
	if (all_commandes->tab_commandes[i].taches_par_specialite[j].nb_heures_requises <= all_commandes->tab_commandes[i].taches_par_specialite[j].nb_heures_effectuees) {
		if (check_if_commande_est_finie(i, all_commandes, all_specialites, progression_nom_specialite) == VRAI) {
			affichage_facturation(i, all_commandes, all_specialites, all_clients);
		}
	}

	if (isPassePresent == VRAI) {
		traite_reallocation(all_commandes, all_specialites, all_travailleurs, progression_nom_specialite, progression_nom_commande, idx_travailleur);
		
	}
}

void progression_traitement_ligne(char* progression_string, Mot* progression_nom_commande, Mot* progression_nom_specialite, int* progression_avancement_temps, Mot* progression_passe_necessaire, Booleen* isPassePresent) {
	char* progression_words;
	unsigned int i = 0;

	progression_words = strtok(progression_string, " ");
	while (progression_words != NULL) {
		switch (i) {
		case 0:
			strcpy(*progression_nom_commande, progression_words);
			break;
		case 1:
			strcpy(*progression_nom_specialite, progression_words);
			break;
		case 2:
			*progression_avancement_temps = atoi(progression_words);
			break;
		case 3:
			strcpy(*progression_passe_necessaire, progression_words);
			if (strcmp(progression_passe_necessaire, "passe") == 0) {
				*isPassePresent = VRAI;
			}
			break;
		default:
			break;
		}
		progression_words = strtok(NULL, " ");
		++i;
	}
}

Booleen check_if_commande_est_finie(unsigned int i, Commandes* all_commandes, const Specialites* all_specialites) {

	unsigned int j;
	for (j = 0; j < all_specialites->nb_specialites; j++) {
		if (all_commandes->tab_commandes[i].taches_par_specialite[j].nb_heures_effectuees < all_commandes->tab_commandes[i].taches_par_specialite[j].nb_heures_requises) {
			return FAUX;
		}
	}
	all_commandes->tab_commandes[i].commande_finie = VRAI;
	return VRAI;
}

void traite_reallocation(Commandes* all_commandes, const Specialites* all_specialites, Travailleurs* all_travailleurs, Mot nom_specialite, Mot nom_commande, unsigned int old_idx_travailleur) {
	unsigned int i, new_idx_travailleur, nb_heures_restantes;
	int nb_heures_min, j,k;
	
	nb_heures_min = -1;
	j = find_specialite(all_specialites, nom_specialite); 
	k = find_commande(all_commandes, nom_commande);

	if (k == -1 || j == -1) {printf("err08.1"); return;}

	all_commandes->tab_commandes[k].taches_par_specialite[j].idx_travailleur = 51; // Enlever la tache du travailleur pour permettre une comparaison
	nb_heures_restantes = all_commandes->tab_commandes[k].taches_par_specialite[j].nb_heures_requises - all_commandes->tab_commandes[k].taches_par_specialite[j].nb_heures_effectuees;
	all_travailleurs->tab_travailleurs[old_idx_travailleur].nb_heures_a_effectuer -= nb_heures_restantes;

	for (i = 0; i < all_travailleurs->nb_travailleurs; i++) {
		if (all_travailleurs->tab_travailleurs[i].tags_competences[j] == VRAI) { 
			if (nb_heures_min == -1) {
				nb_heures_min = all_travailleurs->tab_travailleurs[i].nb_heures_a_effectuer; 
				new_idx_travailleur = i;
			}
			else {
				if (nb_heures_min > all_travailleurs->tab_travailleurs[i].nb_heures_a_effectuer) {
					nb_heures_min = all_travailleurs->tab_travailleurs[i].nb_heures_a_effectuer;
					new_idx_travailleur = i;
				}
			}
		}

	}
	if (nb_heures_min == -1) {printf("err08.2"); return;}
	all_commandes->tab_commandes[k].taches_par_specialite[j].idx_travailleur = new_idx_travailleur;
	all_travailleurs->tab_travailleurs[new_idx_travailleur].nb_heures_a_effectuer += nb_heures_restantes;
}


// ------------------------- AFFICHAGES -------------------------------------------

// facturation ---------------------------
void affichage_facturation(unsigned int i, const Commandes* all_commandes, const Specialites* all_specialites, const Clients* all_clients) {
	unsigned int j, compteur_specialites;
	compteur_specialites = 0;
	unsigned long prix_total_par_specialite;
	printf(MSG_FACTURATION, all_commandes->tab_commandes[i].nom);
	for (j = 0; j < all_specialites->nb_specialites; j++) {
		if (all_commandes->tab_commandes[i].taches_par_specialite[j].nb_heures_requises != 0) {
			prix_total_par_specialite = all_commandes->tab_commandes[i].taches_par_specialite[j].nb_heures_requises * all_specialites->tab_specialites[j].cout_horaire;
			if (compteur_specialites != 0) {
				printf(", ");
			}
			printf("%s:%lu", all_specialites->tab_specialites[j].nom, prix_total_par_specialite);
			++compteur_specialites;
		}
	}
	printf("\n");
	if (check_si_facturation_totale(all_commandes) == VRAI) {
		facturation_totale(all_clients, all_commandes, all_specialites); 
	}
}


Booleen check_si_facturation_totale(const Commandes* all_commandes) {
	unsigned int i;
	for (i = 0; i < all_commandes->nb_commandes; i++) {
		if (all_commandes->tab_commandes[i].commande_finie == FAUX) {
			return FAUX;
		}
	}
	return VRAI;
}

void facturation_totale(const Clients* all_clients, const Commandes* all_commandes, const Specialites* all_specialites) {
	unsigned int i, j, k;
	unsigned long total_par_client;
	printf(MSG_FACTURATION_INTERRUPTION);
	for (i = 0; i < all_clients->nb_clients; i++) {
		total_par_client = 0;
		for (j = 0; j < all_commandes->nb_commandes; j++) {
			if (all_commandes->tab_commandes[j].idx_client == i) {
				for (k = 0; k < all_specialites->nb_specialites; k++) {
					total_par_client = total_par_client + (all_commandes->tab_commandes[j].taches_par_specialite[k].nb_heures_requises * all_specialites->tab_specialites[k].cout_horaire);
				}
			}
		}
		if (i != 0) {
			printf(", ");
		}
		printf("%s:%lu", all_clients->table_clients[i], total_par_client);
	}
	printf("\n");
	StopProgramme = VRAI;
}

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
	get_id(affichage_nom_specialite);

	if (strcmp(affichage_nom_specialite, "tous") == 0) {
		affichage_travailleurs_tous(all_travailleurs, all_specialites);
	}
	else {
		affichage_travailleur_unique(all_travailleurs, all_specialites, affichage_nom_specialite);
	}
}

void affichage_travailleurs_tous(const Travailleurs* all_travailleurs, const Specialites* all_specialites) {
	unsigned int i, j, compteur_travailleurs;
	for (i = 0; i < all_specialites->nb_specialites; i++) {
		compteur_travailleurs = 0;
		printf(MSG_CHECK_TRAVAILLEURS, all_specialites->tab_specialites[i].nom);
		for (j = 0; j < all_travailleurs->nb_travailleurs; j++) {
			if (all_travailleurs->tab_travailleurs[j].tags_competences[i] == VRAI) {

				if (compteur_travailleurs != 0) {
					printf(", ");
				}
				printf("%s", all_travailleurs->tab_travailleurs[j].nom);
				++compteur_travailleurs;
			}
			if (j == all_travailleurs->nb_travailleurs - 1) {
				printf("\n");
			}
		}
	}
}

void affichage_travailleur_unique(const Travailleurs* all_travailleurs, const Specialites* all_specialites, Mot nom_specialite) {
	unsigned int j, compteur_travailleurs;
	int i;

	compteur_travailleurs = 0;
	i = find_specialite(all_specialites, nom_specialite);

	if (i == -1) {printf("err09"); return;}

	printf(MSG_CHECK_TRAVAILLEURS, nom_specialite);
	for (j = 0; j < all_travailleurs->nb_travailleurs; j++) {
		if (all_travailleurs->tab_travailleurs[j].tags_competences[i] == VRAI) {

			if (compteur_travailleurs != 0) {
				printf(", ");
			}
			printf("%s", all_travailleurs->tab_travailleurs[j].nom);
			++compteur_travailleurs;
		}
		if (j == all_travailleurs->nb_travailleurs - 1) {
			printf("\n");
		}
	}
}

// client ---------------------------
void affichage_clients(const Clients* all_clients, const Commandes* all_commandes) {
	Mot affichage_nom_client;
	get_id(affichage_nom_client);

	if (strcmp(affichage_nom_client, "tous") == 0) {
		affichage_clients_tous(all_clients, all_commandes);
	}
	else {
		affichage_client_unique(all_clients, all_commandes, affichage_nom_client);
	}
}

void affichage_clients_tous(const Clients* all_clients, const Commandes* all_commandes) {
	unsigned int i, j, compteur_commandes;

	for (i = 0; i < all_clients->nb_clients; i++) {
		compteur_commandes = 0;
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
}

void affichage_client_unique(const Clients* all_clients, const Commandes* all_commandes, Mot affichage_nom_client) {
	int i;
	unsigned int j, compteur_commandes;

	compteur_commandes = 0;
	i = find_client(all_clients, affichage_nom_client);
	if (i == -1) {printf("err10"); return;}

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

// supervision ---------------------------
void affichage_supervision(const Commandes* all_commandes, const Specialites* all_specialites) {
	unsigned int i, j, compteur_commandes;
	for (i = 0; i < all_commandes->nb_commandes; i++) {
		compteur_commandes = 0;
		printf(MSG_CHECK_SUPERVISION, all_commandes->tab_commandes[i].nom);
		for (j = 0; j < all_specialites->nb_specialites; j++) {
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
}

// charge --------------------------- sprint 4
void affichage_charge(Commandes* all_commandes, const Specialites* all_specialites, Travailleurs* all_travailleurs) {
	Mot charge_nom_travailleur;
	get_id(charge_nom_travailleur);
	unsigned int j, k, compteur_travailleur, nb_heures_restantes;
	int i;

	compteur_travailleur = 0;
	i = find_travailleur(all_travailleurs, charge_nom_travailleur);
	
	printf(MSG_CHECK_CHARGE, charge_nom_travailleur);
	for (j = 0; j < all_commandes->nb_commandes; j++) {
		for (k = 0; k < all_specialites->nb_specialites; k++) {
			if (all_commandes->tab_commandes[j].taches_par_specialite[k].idx_travailleur == i) {
				nb_heures_restantes = all_commandes->tab_commandes[j].taches_par_specialite[k].nb_heures_requises - all_commandes->tab_commandes[j].taches_par_specialite[k].nb_heures_effectuees;
				if (nb_heures_restantes>0){

					if (compteur_travailleur != 0){
						printf(", ");
					}
					printf("%s/%s/%dheure(s)", all_commandes->tab_commandes[j].nom, all_specialites->tab_specialites[k].nom, nb_heures_restantes);
					++compteur_travailleur;
				}
			}
		}
	}
	printf("\n");
}


// ------------------------------------ END -----------------------------------------
// interruption ------------------------ 
void traite_interruption() {
	printf(MSG_INTERRUPTION);
}