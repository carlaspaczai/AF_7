/*
* Student: Spaczai Carla Noemi
* Grupa: 30229
* Am implementat operatiile de management ale unui arbore de statistica de ordine.
* 
* BUILD_TREE:
* In cadrul acestei functii, alegem radacina arborelui ca fiind elementul de la mijlocul vectorului. In mod recursiv, vom alege mereu copiii din stanga ca fiind
* elementul de la mijlocul 'sirului' aflat in stanga radacinii si copiii din dreapta ca fiind elementul de la mijlocul 'sirului' aflat in dreapta radacinii.
* Repetam acest proces pana introducem toate elementele din vector in arbore.
* Complexitate: O(n). 
* 
* OS_SELECT:
* In cadrul acestei functii, comparam indicele primit ca parametru cu dimensiunea subarborelui stang a radacinii + 1. Daca cele doua sunt egale, nodul cautat e
* radacina actuala. Daca indicele primit ca parametru este mai mare decat dimensiunea subarborelui stang a radacinii + 1, noua radacina va fi copilul din dreapta
* a radacinii actuale. Daca indicele primit ca parametru este mai mic decat dimensiunea subarborelui stang a radacinii + 1, noua radacina va fi copilul din
* stanga a radacinii actuale. Repetam acest proces in mod recursiv pana cand nodul cautat a fost gasit sau radacina actuala este NULL.
* Complexitate: O(log(n)).
* 
* OS_DELETE:
* In cadrul acestei functii, apelam OS_SELECT pentru a cauta nodul pe care vrem sa il stergem. Odata gasit, verificam daca acesta nu are deloc copii, are un
* copil stang, are un copil drept sau are 2 copii.
* Pentru primele trei cazuri, verificam daca nodul are parinte si in functie de asta refacem legaturile.
* Pentru ultimul caz, nodul pe care vrem sa il stergem va prelua cheia predecesorului sau si stergem acest predecesor. Verificam daca predecesorul nu are copii
* sau are un copil stang. In functie de asta, refacem legaturile.
* Odata cu fiecare stergere, modificam dimensiunea tuturor stramosilor.
* Complexitate: O(log(n)).
*/

#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

Profiler P("OS");

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5

typedef struct NodeBST 
{
    struct NodeBST* Parent;
    int Data;
    struct NodeBST* Left;
    struct NodeBST* Right;
    int Size;
} NodeBST;

NodeBST* BUILD_TREE(int v[], int p, int r)
{
	if (r < p)
	{
		return NULL;
	}
	int MID = (r + p) / 2;
	NodeBST* ROOT = (NodeBST*)malloc(sizeof(NodeBST));
	ROOT->Data = v[MID];
	ROOT->Size = r - p + 1;
	ROOT->Left = BUILD_TREE(v, p, MID - 1);
	ROOT->Right = BUILD_TREE(v, MID + 1, r);
	ROOT->Parent = NULL;
	if (ROOT->Left != NULL)
	{
		ROOT->Left->Parent = ROOT;
	}
	if (ROOT->Right != NULL)
	{
		ROOT->Right->Parent = ROOT;
	}
	return ROOT;
}

void prettyPrint(NodeBST* ROOT, int numarSpatii)
{
	if (ROOT != NULL)
	{
		for (int j = 0; j < numarSpatii; j++)
		{
			printf("	");
		}
		numarSpatii++;
		/*if (ROOT->Parent == NULL)
		{
			printf("%d (ROOT)\n", ROOT->Data);
		}
		else if (ROOT->Parent->Right == ROOT)
		{
			printf("%d (RIGHT)\n", ROOT->Data);
		}
		else if (ROOT->Parent->Left == ROOT)
		{
			printf("%d (LEFT)\n", ROOT->Data);
		}*/
		printf("%d\n", ROOT->Data);
		prettyPrint(ROOT->Right, numarSpatii);
		prettyPrint(ROOT->Left, numarSpatii);
	}
}

NodeBST* OS_SELECT(NodeBST* ROOT, int i, int n)
{
	Operation OP = P.createOperation("OS_SELECT", n);

	if (ROOT == NULL)
	{
		return NULL;
	}
	int r;
	OP.count();
	if (ROOT->Left == NULL)
	{
		r = 1;
	}
	else
	{
		OP.count();
		r = ROOT->Left->Size + 1;
	}	
	if (i == r)
	{
		return ROOT;
	}
	else if (i < r)
	{
		return OS_SELECT(ROOT->Left, i, n);
	}
	else
	{
		return OS_SELECT(ROOT->Right, i - r, n);
	}
}

NodeBST* OS_SELECT_D(NodeBST* ROOT, int i, int n)
{
	Operation OP = P.createOperation("OS_DELETE", n);

	if (ROOT == NULL)
	{
		return NULL;
	}
	int r;
	OP.count();
	if (ROOT->Left == NULL)
	{
		r = 1;
	}
	else
	{
		OP.count();
		r = ROOT->Left->Size + 1;
	}
	if (i == r)
	{
		return ROOT;
	}
	else if (i < r)
	{
		return OS_SELECT_D(ROOT->Left, i, n);
	}
	else
	{
		return OS_SELECT_D(ROOT->Right, i - r, n);
	}
}

NodeBST* Predecessor(NodeBST* ROOT, int n)
{
	Operation OP = P.createOperation("OS_DELETE", n);

	OP.count();
	if (ROOT == NULL || ROOT->Left == NULL)
	{
		return NULL;
	}
	OP.count();
	ROOT = ROOT->Left;
	while (ROOT->Right != NULL)
	{
		OP.count(2);
		ROOT = ROOT->Right;
	}
	return ROOT;
}

NodeBST* OS_DELETE(NodeBST* ROOT, int i, int n)
{
	Operation OP = P.createOperation("OS_DELETE", n);

	OP.count();
	NodeBST* ROOOOT = OS_SELECT_D(ROOT, i, n);
	if (ROOOOT == NULL)
	{
		return NULL;
	}
	if (ROOOOT->Left == NULL && ROOOOT->Right == NULL)
	{
		OP.count(2);
		OP.count();
		if (ROOOOT->Parent != NULL)
		{
			OP.count();
			NodeBST* S1 = ROOOOT->Parent;
			OP.count();
			if (ROOOOT->Parent->Right == ROOOOT)
			{
				OP.count(2);
				ROOOOT->Parent->Right = NULL;
				ROOOOT->Parent = NULL;
				free(ROOOOT);
			}
			else
			{
				OP.count(2);
				ROOOOT->Parent->Left = NULL;
				ROOOOT->Parent = NULL;
				free(ROOOOT);
			}
			while (S1 != NULL)
			{
				S1->Size = S1->Size - 1;
				OP.count();
				S1 = S1->Parent;
			}
		}
		else
		{
			free(ROOOOT);
		}
	}
	else if (ROOOOT->Left == NULL)
	{
		OP.count(3);
		OP.count();
		if (ROOOOT->Parent != NULL)
		{
			OP.count();
			NodeBST* S2 = ROOOOT->Parent;
			OP.count();
			if (ROOOOT->Parent->Right == ROOOOT)
			{
				OP.count(2);
				ROOOOT->Parent->Right = ROOOOT->Right;
				ROOOOT->Right->Parent = ROOOOT->Parent;
				free(ROOOOT);
			}
			else
			{
				OP.count(2);
				ROOOOT->Parent->Left = ROOOOT->Right;
				ROOOOT->Right->Parent = ROOOOT->Parent;
				free(ROOOOT);
			}
			while (S2 != NULL)
			{
				S2->Size = S2->Size - 1;
				OP.count();
				S2 = S2->Parent;
			}
		}
		/*else
		{
			OP.count(2);
			ROOOOT->Right->Parent = NULL;
			ROOOOT->Right = NULL;
			free(ROOOOT);
		}*/
	}
	else if (ROOOOT->Right == NULL)
	{
		OP.count(4);
		OP.count();
		if (ROOOOT->Parent != NULL)
		{
			OP.count();
			NodeBST* S3 = ROOOOT->Parent;
			OP.count();
			if (ROOOOT->Parent->Right == ROOOOT)
			{
				OP.count(2);
				ROOOOT->Parent->Right = ROOOOT->Left;
				ROOOOT->Left->Parent = ROOOOT->Parent;
				free(ROOOOT);
			}
			else
			{
				OP.count(2);
				ROOOOT->Parent->Left = ROOOOT->Left;
				ROOOOT->Left->Parent = ROOOOT->Parent;
				free(ROOOOT);
			}
			while (S3 != NULL)
			{
				S3->Size = S3->Size - 1;
				OP.count();
				S3 = S3->Parent;
			}
		}
		/*else
		{
			OP.count(2);
			ROOOOT->Left->Parent = NULL;
			ROOOOT->Left = NULL;
			free(ROOOOT);
		}*/
	}
	else
	{
		OP.count(4);
		OP.count();
		NodeBST* PS = Predecessor(ROOOOT, n);
		OP.count();
		ROOOOT->Data = PS->Data;
		OP.count();
		NodeBST* S4 = PS->Parent;
		OP.count();
		if (PS->Left != NULL)
		{
			OP.count();
			if (PS->Parent->Right == PS)
			{
				OP.count(2);
				PS->Parent->Right = PS->Left;
				PS->Left->Parent = PS->Parent;
				free(PS);
			}
			else
			{
				OP.count(2);
				PS->Parent->Left = PS->Left;
				PS->Left->Parent = PS->Parent;
				free(PS);
			}
		}
		else
		{
			OP.count();
			if (PS->Parent->Right == PS)
			{
				OP.count(2);
				PS->Parent->Right = NULL;
				PS->Parent = NULL;
				free(PS);
			}
			else
			{
				OP.count(2);
				PS->Parent->Left = NULL;
				PS->Parent = NULL;
				free(PS);
			}
		}
		while (S4 != NULL)
		{
			S4->Size = S4->Size - 1;
			OP.count();
			S4 = S4->Parent;
		}
	}
	return ROOT;
}

void demo()
{
	int v[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
	int n = sizeof(v) / sizeof(v[0]);

	NodeBST* ROOT = BUILD_TREE(v, 0, n - 1);
	printf("Arborele initial este:\n");
	prettyPrint(ROOT, 0);
	printf("\n");

	int i;
	printf("'i' este: ");
	scanf_s("%d", &i);
	NodeBST* ROOOT1 = OS_SELECT(ROOT, i, n);
	//printf("%d\n", ROOOT1->Size);
	if (ROOOT1 == NULL)
	{
		printf("E NULL!\n");
	}
	else
	{
		printf("Cheia cautata este: %d\n", ROOOT1->Data);
		printf("\n");
		printf("Arborele dupa stergerea lui %d este:\n", i);
	}
	NodeBST* ROOOT2 = OS_DELETE(ROOT, i, n);
	prettyPrint(ROOOT2, 0);

	printf("\n");
	printf("'i' este: ");
	scanf_s("%d", &i);
	if (ROOOT2 != NULL)
	{
		ROOOT1 = OS_SELECT(ROOOT2, i, n);
	}
	else
	{
		ROOOT1 = OS_SELECT(ROOT, i, n);
	}
	//printf("%d\n", ROOOOT1->Size);
	if (ROOOT1 == NULL)
	{
		printf("E NULL!\n");
	}
	else
	{
		printf("Cheia cautata este: %d\n", ROOOT1->Data);
		printf("\n");
		printf("Arborele dupa stergerea lui %d este:\n", i);
	}
	if (ROOOT2 != NULL)
	{
		ROOOT2 = OS_DELETE(ROOOT2, i, n);
	}
	else
	{
		ROOOT2 = OS_DELETE(ROOT, i, n);
	}
	prettyPrint(ROOOT2, 0);

	printf("\n");
	printf("'i' este: ");
	scanf_s("%d", &i);
	if (ROOOT2 != NULL)
	{
		ROOOT1 = OS_SELECT(ROOOT2, i, n);
	}
	else
	{
		ROOOT1 = OS_SELECT(ROOT, i, n);
	}
	//printf("%d\n", ROOOOOT1->Size);
	if (ROOOT1 == NULL)
	{
		printf("E NULL!\n");
	}
	else
	{
		printf("Cheia cautata este: %d\n", ROOOT1->Data);
		printf("\n");
		printf("Arborele dupa stergerea lui %d este:\n", i);
	}
	if (ROOOT2 != NULL)
	{
		ROOOT2 = OS_DELETE(ROOOT2, i, n);
	}
	else
	{
		ROOOT2 = OS_DELETE(ROOT, i, n);
	}
	prettyPrint(ROOOT2, 0);
}

void perf()
{
	for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
	{
		for (int test = 0; test < NR_TESTS; test++)
		{
			int* v = (int*)calloc(n, sizeof(int));
			for (int i = 1; i <= n; i++)
			{
				v[i - 1] = i;
			}
			NodeBST* ROOT = BUILD_TREE(v, 0, n - 1);
			int m = n;
			while (m)
			{
				int I = rand() % m;
				I++;
				NodeBST* RT1 = OS_SELECT(ROOT, I, n);
				ROOT = OS_DELETE(ROOT, I, n);
				m--;
			}
		}
	}
	P.divideValues("OS_SELECT", NR_TESTS);
	P.divideValues("OS_DELETE", NR_TESTS);
	P.createGroup("Comparare", "OS_SELECT", "OS_DELETE");
	P.showReport();
}

int main()
{
	demo();
	//perf();
	return 0;
}