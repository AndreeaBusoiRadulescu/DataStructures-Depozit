#include "pch.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

// D1(head) <-> D2 <-> D3 <-> D4

typedef struct Depozit {
	int id;
	char* denumire;
	char* localitate;
	float suprafata;
	float capacitate_stocare;
	int nr_angajati;
} Depozit;

typedef struct NodLista {
	Depozit* depozit;
	NodLista* next; //vecin dreapta
	NodLista* prev; //vecin stanga
} NodLista;

typedef struct NodArbore {
	Depozit* depozit;
	NodArbore* stanga; //vecin stanga
	NodArbore* dreapta; //vecin dreapta
} NodArbore;

void atribuire(Depozit *a, Depozit *b) {
	//a primeste detaliile lui b
	a->id = b->id;
	a->denumire = (char*)malloc(sizeof(char*)*strlen(b->denumire));
	strcpy(a->denumire, b->denumire);
	a->localitate = (char*)malloc(sizeof(char*)*strlen(b->localitate));
	strcpy(a->localitate, b->localitate);
	a->suprafata = b->suprafata;
	a->capacitate_stocare = b->capacitate_stocare;
	a->nr_angajati = b->nr_angajati;
}

void inserareArbore(NodArbore** head, Depozit de_inserat) {
	if ((*head) == NULL) {
		//Alocare head
		(*head) = (NodArbore*)malloc(sizeof(NodArbore));
		(*head)->stanga = NULL;
		(*head)->dreapta = NULL;
		(*head)->depozit = (Depozit*)malloc(sizeof(Depozit));
		//Primeste valorile obiectului de_inserat
		atribuire((*head)->depozit, &de_inserat);
	}
	else
	{
		NodArbore* nod_curent;
		nod_curent = (*head); //Iteram incepand de la head
		//Alocare nod nou
		NodArbore *nod_nou = (NodArbore*)malloc(sizeof(NodArbore));
		nod_nou->depozit = (Depozit*)malloc(sizeof(Depozit));
		nod_nou->dreapta = NULL;
		nod_nou->stanga = NULL;

		NodArbore* ultimul_parinte = NULL;
		while (nod_curent != NULL)
		{
			ultimul_parinte = nod_curent;
			if (de_inserat.id > nod_curent->depozit->id) // Mergem pe partea dreapta
			{
				nod_curent = nod_curent->dreapta;
			}
			else if (de_inserat.id < nod_curent->depozit->id) // Mergem pe partea stanga
			{
				nod_curent = nod_curent->stanga;
			}
		}
		//Ultimul_parinte tine minte nodul de care vom atasa noua valoare
		if (de_inserat.id > ultimul_parinte->depozit->id) // Atasam pe partea dreapta
		{
			ultimul_parinte->dreapta = nod_nou;
		}
		else if (de_inserat.id < ultimul_parinte->depozit->id) // Atasam pe partea stanga
		{
			ultimul_parinte->stanga = nod_nou;
		}
		//Atribuire valori
		atribuire(nod_nou->depozit, &de_inserat);
	}
}

void inserareLista(NodLista** head, Depozit de_inserat) {

	//Avem capul null
	if ((*head) == NULL) {
		//Alocare head
		(*head) = (NodLista*)malloc(sizeof(NodLista));
		(*head)->prev = NULL;
		(*head)->next = NULL;
		(*head)->depozit = (Depozit*)malloc(sizeof(Depozit));
		//Primeste valorile obiectului de_inserat
		atribuire((*head)->depozit, &de_inserat);
	}
	else
	{
		//Nu avem capul null => Exista o lista
		//Parcurgem lista pana gasim un loc ok pentru inserat noul nod.
		//Un loc ok == ID-urile sunt sortate crescator
		//Ex: 4 <-> 6 <-> 10 <-> 13. Avem de inserat id 8 -> vecin dreapta a lui 6 si vecin stanga a lui 10
		NodLista* nod_curent;
		nod_curent = (*head); //Iteram incepand de la head
		bool inserat = false;
		//Alocare nod nou
		NodLista *nod_nou = (NodLista*)malloc(sizeof(NodLista));
		nod_nou->depozit = (Depozit*)malloc(sizeof(Depozit));
		while (nod_curent->next != NULL && inserat == false) {

			if (nod_curent->depozit->id <= de_inserat.id && nod_curent->next->depozit->id > de_inserat.id)
			{
				//Nextul devine de_inserat
				nod_nou->next = nod_curent->next; // 8->
				nod_nou->prev = nod_curent; // <-8

				nod_curent->next = nod_nou; //6->
				nod_curent->next->prev = nod_nou; // <-10

				inserat = true; //Se va iesi din while
			}
			else
			{
				nod_curent = nod_curent->next;
			}
		}
		if (inserat == false) //Nu a gasit loc in lista pentru inserat. Inseram la finalul listei
		{
			//nod_curent este pe ultima pozitie
			nod_curent->next = nod_nou; //13->nod_nou
			nod_nou->prev = nod_curent; //13<-nod_nou
			nod_nou->next = NULL;
		}
		//Atribuire valori
		atribuire(nod_nou->depozit, &de_inserat);
	}
}

Depozit citesteDepozit(char* linieFisier) {
	const char delimitator[] = ",";
	char *token;

	Depozit depozit;
	//id
	token = strtok(linieFisier, delimitator);
	depozit.id = atoi(token);
	//denumire
	token = strtok(NULL, delimitator);
	depozit.denumire = (char*)malloc(sizeof(char*)*strlen(token));
	strcpy(depozit.denumire, token);
	//localitate
	token = strtok(NULL, delimitator);
	depozit.localitate = (char*)malloc(sizeof(char*)*strlen(token));
	strcpy(depozit.localitate, token);
	//suprafata
	token = strtok(NULL, delimitator);
	depozit.suprafata = atof(token);
	//capacitate_stocare
	token = strtok(NULL, delimitator);
	depozit.capacitate_stocare = atof(token);
	//nr_angajati
	token = strtok(NULL, delimitator);
	depozit.nr_angajati = atoi(token);

	return depozit;
}

void afisareDepozit(Depozit depozit) {
	printf("%d %s %s %f %f %d\n", depozit.id, depozit.denumire,
		depozit.localitate, depozit.suprafata, depozit.capacitate_stocare, depozit.nr_angajati);
}

void afisareLista(NodLista* head) {
	NodLista* nod_curent = head;
	while (nod_curent != NULL) {
		Depozit* depozit = nod_curent->depozit;
		afisareDepozit(*depozit);
		nod_curent = nod_curent->next;
	}
}

void afisareArboreRSD(NodArbore* head) {
	if (head == NULL) return;
	//radacina
	afisareDepozit(*(head->depozit));
	//stanga
	afisareArboreRSD(head->stanga);
	//dreapta
	afisareArboreRSD(head->dreapta);
}

void modificaDenumireLista(NodLista* head, int id, const char denumire[]) {
	NodLista* nod_curent = head;
	while (nod_curent != NULL) {
		Depozit* depozit = nod_curent->depozit;
		if (depozit->id == id) {
			free(depozit->denumire);
			depozit->denumire = (char*)malloc(sizeof(char)*strlen(denumire));
			strcpy(depozit->denumire, denumire);
			break; //oprim cautarea
		}
		nod_curent = nod_curent->next;
	}
}

void modificaDenumireArbore(NodArbore* head, int id, const char denumire[]) {
	NodArbore* nod_curent = head;
	while (nod_curent != NULL) {
		Depozit* depozit = nod_curent->depozit;
		if (depozit->id == id) //Nodul cautat
		{
			free(depozit->denumire);
			depozit->denumire = (char*)malloc(sizeof(char)*strlen(denumire));
			strcpy(depozit->denumire, denumire);
			break;
		}
		else if (id > depozit->id)
		{
			nod_curent = nod_curent->dreapta;
		}
		else if (id < depozit->id)
		{
			nod_curent = nod_curent->stanga;
		}
	}
}

float nrMediuAngajatiLista(NodLista* head, const char localitate[])
{
	NodLista* nod_curent = head;
	int contor = 0;
	float suma = 0;
	while (nod_curent != NULL) {
		Depozit* depozit = nod_curent->depozit;
		if (strcmp(depozit->localitate, localitate) == 0) {
			contor++;
			suma += depozit->nr_angajati;
		}
		nod_curent = nod_curent->next;
	}
	if (contor != 0) return suma / contor;
	return 0;
}

void conversieListaLaArbore(NodLista* headLista, NodArbore** headArbore) {

	//Parcurgere lista
	*headArbore = NULL;
	while (headLista != NULL) {
		inserareArbore(headArbore, *(headLista->depozit));
		headLista = headLista->next;
	}
}

void dezalocareLista(NodLista* headLista) {
	NodLista* nod_curent = headLista;
	while (nod_curent != NULL) {
		NodLista* next = nod_curent->next;
		Depozit* depozit = nod_curent->depozit;
		//dezalocare depozit
		free(depozit->denumire);
		free(depozit->localitate);
		free(depozit);
		//dezalocare nod_curent
		free(nod_curent);
		nod_curent = next;
	}
}

void dezalocareArbore(NodArbore* headArbore) {
	if (headArbore == NULL) return;
	dezalocareArbore(headArbore->stanga);
	dezalocareArbore(headArbore->dreapta);
	//dezalocare depozit
	Depozit* depozit = headArbore->depozit;
	free(depozit->denumire);
	free(depozit->localitate);
	free(depozit);
	//dezalocare nod_curent
	free(headArbore);
}

void citireLista(NodLista** headLista, const char fisier[])
{
	(*headLista) = NULL;
	Depozit de_inserat;
	FILE *fp;

	fp = fopen(fisier, "r");
	char buffer[500];
	while (fgets(buffer, sizeof(buffer), fp)) {
		de_inserat = citesteDepozit(buffer);
		inserareLista(headLista, de_inserat);
		free(de_inserat.denumire);
		free(de_inserat.localitate);
	}
	fclose(fp);
}

int main()
{
	NodLista* headLista = NULL; //capul de inceput al listei
	citireLista(&headLista, "depozite.txt");

	//Lista
	printf("Lista originala: \n");
	afisareLista(headLista);
	printf("\nLista dupa modificarea localitatiei: \n");
	modificaDenumireLista(headLista, 13, "NumeNouLista");
	afisareLista(headLista);
	printf("\nNumar mediu de angajati pentru depozitul din Pitesti: %f\n\n", nrMediuAngajatiLista(headLista, "Pitesti"));


	//Conversie lista->arbore
	//Va rezulta un arbore cu toate elemente puse doar pe partea dreapta!
	NodArbore* headArbore = NULL; //capul de inceput al arborelui
	conversieListaLaArbore(headLista, &headArbore);
	printf("Arborele original: \n");
	afisareArboreRSD(headArbore);
	printf("\nArborele dupa modificarea localitatiei: \n");
	modificaDenumireArbore(headArbore, 21, "NumeNouArbore");
	afisareArboreRSD(headArbore);

	////Testare dezalocare lista
	//for (;;)
	//{
	//  NodLista* headLista = NULL; //capul de inceput al listei
	//  citireLista(&headLista, file);
	//  dezalocareLista(headLista);
	//}

	////Testare dezalocare arbore
	//for (;;)
	//{
	//  NodArbore* headArbore = NULL; //capul de inceput al arborelui
	//  conversieListaLaArbore(headLista, &headArbore);
	//  dezalocareArbore(headArbore);
	//}
}