#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct CursaAeriana
{
	unsigned short int id;
	char* destinatie;
	char* companieAeriana;
	unsigned short int distanta;//%hu
	unsigned short int nrPasageri;//%hu
}CursaAeriana;

typedef struct Nod
{
	CursaAeriana* info;
	struct Nod* next;
}Nod;

typedef struct HashT
{
	Nod** vector;
	int dim;
}HashT;

CursaAeriana* initializareCursa(unsigned short int id, const char* destinatie,const char* companieAeriana, unsigned short int distanta, unsigned short int nrPasageri)
{
	CursaAeriana* cursaAeriana = (CursaAeriana*)malloc(sizeof(CursaAeriana));
	cursaAeriana->id = id;
	cursaAeriana->destinatie = (char*)malloc(strlen(destinatie) + 1);
	strcpy(cursaAeriana->destinatie, destinatie);
	cursaAeriana->companieAeriana = (char*)malloc(strlen(companieAeriana) + 1);
	strcpy(cursaAeriana->companieAeriana, companieAeriana);
	cursaAeriana->distanta = distanta;
	cursaAeriana->nrPasageri = nrPasageri;
	return cursaAeriana;
}
Nod* initializareNod(CursaAeriana* cursaAeriana)
{
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = cursaAeriana;
	nodNou->next = NULL;
	return nodNou;
}
HashT* initializareHashT()
{
	HashT* hashT = (HashT*)malloc(sizeof(HashT));
	hashT->dim = 20;
	hashT->vector = (Nod**)malloc(sizeof(Nod*) * hashT->dim);
	for (int i = 0; i < hashT->dim; i++)
	{
		hashT->vector[i] = NULL;
	}
	return hashT;
}
int functieHashT(HashT* hashT, const char* destinatie)
{
	return (destinatie[0] - 'A') % hashT->dim;
}
HashT* inserareHashT(HashT* hashT, CursaAeriana* cursaAeriana)
{
	if (hashT->vector != NULL)
	{
		int poz = functieHashT(hashT, cursaAeriana->destinatie);
		Nod* nodNou = initializareNod(cursaAeriana);
		if (hashT->vector[poz] == NULL)
		{
			hashT->vector[poz] = nodNou;
		}
		else
		{
			Nod* temp = hashT->vector[poz];
			while (temp->next != NULL)
			{
				temp = temp->next;
			}
			temp->next = nodNou;
		}
	}
	return hashT;
}
void afisareHashT(HashT* hashT)
{
	if (hashT->vector != NULL)
	{
		for (int i = 0; i < hashT->dim; i++)
		{
			if (hashT->vector[i] != NULL)
			{
				Nod* temp = hashT->vector[i];
				while (temp != NULL)
				{
					printf("Pozitie: %d\n", i);
					printf("%d %s %s %d %d\n", temp->info->id, temp->info->destinatie,temp->info->companieAeriana,
						temp->info->distanta, temp->info->nrPasageri);
					temp = temp->next;
				}
			}
		}
	}
}
void dezalocareHashT(HashT* hashT)
{
	if (hashT->vector != NULL)
	{
		for (int i = 0; i < hashT->dim; i++)
		{
			if (hashT->vector[i] != NULL)
			{
				Nod* temp = hashT->vector[i];
				while (temp != NULL)
				{
					Nod* nodDeSters = temp;
					temp = temp->next;
					free(nodDeSters->info->destinatie);
					free(nodDeSters->info->companieAeriana);
					free(nodDeSters->info);
					free(nodDeSters);
					nodDeSters = temp;
				}
			}
		}
		free(hashT->vector);
		free(hashT);
	}
}
//functie care det nr de curse pt o destinatie data ca param
int nrCurse(HashT* hashT, const char* destinatie)
{
	if (hashT->vector != NULL)
	{
		int poz = functieHashT(hashT, destinatie);
		int nrCurse = 0;

		Nod* temp = hashT->vector[poz];
		while (temp != NULL)
		{
			if (strcmp(temp->info->destinatie, destinatie) == 0)
			{
				nrCurse++;
			}
			temp = temp->next;
		}
		return nrCurse;
	}
}
//functia care modifica den unei companii in tabela, noua denumire este data in param
void modificare(HashT* hashT, unsigned short int id, const char* nouaDenumire)
{
	for (int i = 0; i < hashT->dim; i++)
	{
		Nod* temp = hashT->vector[i];
		while (temp != NULL)
		{
			if (temp->info->id == id)
			{
				free(temp->info->companieAeriana);
				temp->info->companieAeriana = (char*)malloc(strlen(nouaDenumire) + 1);
				strcpy(temp->info->companieAeriana, nouaDenumire);
			}
			temp = temp->next;
		}
	}
}

//functia care sterge cursele cu destinatia data ca param
void stergereCursa(HashT* hashT, const char* destinatie)
{
	int poz = functieHashT(hashT, destinatie);
	Nod* temp = hashT->vector[poz];
	Nod* prev = NULL;
	while (temp != NULL)
	{
		if (strcmp(temp->info->destinatie, destinatie) == 0)
		{
			if (prev == NULL)
			{
				hashT->vector[poz] = temp->next;
			}
			else
			{
				prev->next = temp->next;
			}
			Nod* nodDeSters = temp;
			temp = temp->next;
			free(nodDeSters->info->destinatie);
			free(nodDeSters->info->companieAeriana);
			free(nodDeSters->info);
			free(nodDeSters);
		}
		else
		{
			prev = temp;
			temp = temp->next;
		}
	}
}
//functie afisare cursa dupa destinatie
void afisare(HashT* hashT, char* destinatie)
{
	int nrAparitii = 0;
	if (hashT->vector != NULL)
	{
		int poz = functieHashT(hashT, destinatie);
		Nod* temp = hashT->vector[poz];
		while (temp != NULL)
		{
			if (strcmp(temp->info->destinatie, destinatie) == 0)
			{
				printf("%s", temp->info->destinatie);
				nrAparitii++;
				
			}
			temp = temp->next;
		}
		if (nrAparitii == 0)
		{
			printf("Destinatia nu exista\n");
		}
	}
}

void main()
{
	FILE* f = fopen("CursaAeriana.txt", "r");
	if (f != NULL)
	{
		char bufferLine[300], sep_list[] = ",\n";
		HashT* hashT = initializareHashT();

		while (fgets(bufferLine, sizeof(bufferLine), f))
		{
			char* token = strtok(bufferLine, sep_list);
			unsigned short int id = (unsigned short int)atoi(token);

			token = strtok(NULL, sep_list);
			char* destinatie = (char*)malloc(strlen(token) + 1);
			strcpy(destinatie, token);

			token = strtok(NULL, sep_list);
			char* companieAeriana = (char*)malloc(strlen(token) + 1);
			strcpy(companieAeriana, token);

			token = strtok(NULL, sep_list);
			unsigned short int distanta = (unsigned short int)atoi(token);

			token = strtok(NULL, sep_list);
			unsigned short int nrPasageri = (unsigned short int)atoi(token);



			CursaAeriana* cursaAeriana = initializareCursa(id, destinatie,companieAeriana, distanta, nrPasageri);
			hashT = inserareHashT(hashT, cursaAeriana);


		}
		fclose(f);
		afisareHashT(hashT);

		printf("Cerinta 2:\n");
		int nr = 0;
		nr = nrCurse(hashT, "Paris");
		printf("%d\n", nr);


		printf("Cerinta 3:\n");
		modificare(hashT, 3, "AirLineQatar");
		afisareHashT(hashT);

		printf("Cerinta 4:\n");
		stergereCursa(hashT,"Paris");
		//afisareHashT(hashT);
		afisare(hashT, "Paris");
		afisare(hashT, "Roma");



		dezalocareHashT(hashT);
	}
}
