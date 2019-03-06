#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <math.h>

using namespace std;
ofstream pomiar("cykle.txt");


// wyswietlanie macierzy

void drukuj(int n, int **NM) {

	for (int x = 0; x<n; x++) {
		for (int y = 0; y<n; y++) printf("%c ", NM[x][y] ? '#' : '.');
		cout << endl;
	}
}

// kopiowanie macierzy

void kopiuj(int n, int **NM, int **NM_copy) {

	for (int x = 0; x<n; x++)
		for (int y = 0; y<n; y++)
			NM_copy[x][y] = NM[x][y];

}

// przechodzenie grafu w głab

void dfs(int n, int v, bool* visited, int **NM) { //analizuj v

	visited[v] = 1;
	//	cout << v << "-> ";
	for (int x = 0; x<n; x++) 		// dla wszystkich wierzchołkow x
		if (NM[v][x] && !visited[x]) 	// jesli istnieje krawedz v-x i x nie byl odwiedzony
			dfs(n, x, visited, NM); // analizuj x

}

// generowanie grafu eulerowskiego

void generuj(int n, int m, int **NM) {

	// tworzenie losowego grafu nieskierowanego o m krawędziach [macierz sąsiedztwa]

	int i = m, x, y;
	while (i) {
		y = rand() % n; // losowy wierzchołek (od 0 do n-1)
		x = rand() % n;
		if (NM[x][y] != 1 && x != y) { // bez petli własnych
			NM[x][y] = NM[y][x] = 1;
			i--;
		}
	}

	// nadawanie parzystości wierzchołkom

	int deg;
	for (x = 0; x<n - 1; x++) { 	// dla n-1 wierzchołków (graf spójny zawiera parzysta liczbe wierzchołków o nieparzystych stopniach)
		deg = 0;
		for (y = 0; y<n; y++) if (NM[x][y]) deg++;			// obliczamy deg(x)

		if (deg % 2) {						// jeśli stopień wierzchołka jest nieparzysty
			y = rand() % (n - x - 1) + x + 1; 			// od x+1 do n-1 (dowony większy od x)
			if (NM[x][y]) {				// i wylosowana krawędź istnieje, to usuwamy ją
				NM[x][y] = 0;
				NM[y][x] = 0;
				i--;
			}
			else {						// else: dodajemy w wylosowanym miejscu krawędź
				NM[x][y] = 1;
				NM[y][x] = 1;
				i++;
			}
		}

	}
	printf("m = %d, ", m + i); // m sie zmienia :/

							   // sprawdzanie spojnosci grafu

	bool *visited = new bool[n];
	bool all_visited = 1;
	dfs(n, 0, visited, NM);

	for (x = 0; x<n - 1; x++) if (!visited[x]) { all_visited = 0; break; }

	if (all_visited) printf("graf spojny\n");
	else {
		printf("graf niespojny\nponowne generowanie: ");
		generuj(n, m, NM);
	}

}

// szukanie cyklu Eulera w grafie

void Euler(int n, int** NM, int v) {

	for (int x = 0; x<n; x++) { 		// dla wszystkich wierzchołkow x
		if (NM[v][x]) {  		// jesli istnieje krawędz v-x
			NM[v][x] = NM[x][v] = 0; // usun te krawędz (odpowiednik zaznaczenia jako odwiedzona)
			Euler(n, NM, x);  // analizuj x
		}
	}
	//	printf("%d, ", v); 			// wypisz w kolejnosci zdejmowania ze stosu

}

// szukanie cyklu Hamiltona w grafie

int nr = 0, CH = 0, found = 0;	// dlugosc aktualnej sciezki, liczba cykli w grafie, flaga znalezienia cyklu

void Hamilton(int n, int v, int* path, bool* visited, int** NM, double start2) {
	//	if(found) return 5;
	path[nr++] = v;					// dopisujemy wierzcholek do sciezki

	if (nr != n) {					// jesli sciezka jest jeszcze niepelna
		visited[v] = 1;
		for (int x = 0; x<n; x++) 		// dla wszystkich wierzchołkow x
			if (NM[v][x] && !visited[x]) 	// jesli istnieje krawedz v-x i x nie byl odwiedzony
				Hamilton(n, x, path, visited, NM, start2); // przejdz do x
		visited[v] = 0;
	}


	else if (NM[v][0]) {  				// jesli scieżka zawiera juz wszystkie wierzchołki i jest cyklem
										//			printf("CH: ");
										//			for (int x=0; x<n; x++);  printf("%d, ", path[x]); // wypisz ja
										//			printf("0\n");
		if (!found) {
			double_t koniec2 = 1.0*clock() / CLOCKS_PER_SEC;
			cout << "H1 = " << koniec2 - start2 << endl;
			found = 1;
		} 	// po znalezieniu pierwszego cyklu wypisz czas
		CH++;				// liczba cykli++
	}
	nr--;//usuwamy wierzchołek ze scieżki
}

// MAIN

int main() {

	double d = 0.6;
	int n = 15; 		// liczba wierzchołków
	int m = n*(n - 1) / 2 * d;		// liczba krawędzi
	srand(time(NULL));

	int **NM = new int*[n];
	for (int x = 0; x<n; x++) NM[x] = new int[n];	// tablica jest inicjalizowana zerami

	printf("OK, n = %d, m = %d \n", n, m);
	generuj(n, m, NM);
	//	drukuj(n, NM);

	// cykl Eulera

	int **NM_copy = new int*[n];
	for (int x = 0; x<n; x++) NM_copy[x] = new int[n];
	kopiuj(n, NM, NM_copy);

	//	printf("CE: ");
	double_t start1 = 1.0*clock() / CLOCKS_PER_SEC;
	Euler(n, NM_copy, 0);
	double_t koniec1 = 1.0*clock() / CLOCKS_PER_SEC;
	cout << "E = " << koniec1 - start1 << endl;

	// cykl Hamiltona
	bool *visited = new bool[n];
	int *path = new int[n];
    double_t start2 = 1.0*clock() / CLOCKS_PER_SEC;
    double_t start3 = 1.0*clock() / CLOCKS_PER_SEC;
	Hamilton(n, 0, path, visited, NM, start2);
	double_t koniec3 = 1.0*clock() / CLOCKS_PER_SEC;
	cout << "HA = " << koniec3 - start3 << endl;//
	cout << "CH = " << CH / 2 << endl;


	return 0;

}

