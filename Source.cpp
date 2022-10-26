#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iomanip>

using namespace std;

struct Grad {
	int x, y;
	Grad* next;
};

//funkcija koja ispisuje trenutno stanje
//ulancane liste gradova.
void ispis(Grad* lista) {
	Grad* glava = lista;
	int i = 0;
	cout << " ::";
	while (glava) {
		if (i == 10) {
			cout << endl;
			i = 0;
		}
		cout << "(" << glava->x << ", " << glava->y << ")";
		if (!(glava->next == nullptr))
			cout << " -> ";
		glava = glava->next;
		i++;
	}
	cout << "::";
	cout << endl;
}

//funkcija koja brise listu iz memorije
//da bi se izbegla situacija curenja memorije
//pri kraju rada programa.
void dealokacija(Grad*& lista) {
	Grad* tek;
	while (lista) {
		tek = lista;
		lista = lista->next;
		delete tek;
	}
}

//funkcija koja omogucuje korisniku da pojedinacno
//unese koordinate svh gradova ili da dozvoli
//programu da izgenerise nasumicne koordinate
//u opsegu od 0 do 100.
Grad* unosgradova(int n, const int high) {
	int s, i;
	Grad* novi;
	Grad* glava = nullptr, * tek = nullptr;

	cout << "\t" << "Kako zelite da unesete koordinate gradova?" << endl <<
		"\t" << "Ako zelite sami uneti koordinate unesite broj ""1""," << endl <<
		"\t" << "a ako zelite nasumicne koordinate pritisnite bilo sta: ";
	cin >> s;
	cout << endl;
	switch (s) {
	case 1: {
		for (i = 0; i < n; i++) {
			novi = new Grad;
			int x, y;
			cout << "\t" << "Grad " << i + 1 << ": X koordinata: ";
			cin >> x;
			cout << "\t" << "Grad " << i + 1 << ": Y koordinata: ";
			cin >> y;
			novi->x = x;
			novi->y = y;
			novi->next = nullptr;
			if (!glava)
				glava = novi;
			else if (tek)
				tek->next = novi;
			tek = novi;
		}
		cout << endl;
		tek = glava;
		for (i = 0; i < n; i++) {
			cout << "\t" << "Grad " << i + 1 << ": (" << 
				tek->x << ", " << tek->y << ")   ";
			if ((tek->x / 10) == 0) cout << " ";
			if ((tek->y / 10) == 0) cout << " ";
			if ((i + 1) % 5 == 0) cout << endl;
			tek = tek->next;
		}
		cout << endl;
		break;
	}
	default: {
		for (i = 0; i < n; i++) {
			novi = new Grad;
			novi->x = rand() % high;
			novi->y = rand() % high;
			novi->next = nullptr;
			if (!glava)
				glava = novi;
			else if (tek)
				tek->next = novi;
			tek = novi;
			if (i % 5 == 0) cout << "\t";
			cout << "Grad " << i + 1 << ": (" << novi->x << 
				", " << novi->y << ")   ";
			if ((novi->x / 10) == 0) cout << " ";
			if ((novi->y / 10) == 0) cout << " ";
			if ((i + 1) % 5 == 0) cout << endl;
		}
		cout << endl;
	}
	}
	return glava;
}

//funkcija koja vrsi kopiju originalne
//liste gradova i koja se koristi u obradi
//heuristike dok se originalna lista koristi
//za odredjivanje pocetnog grada za obradu.
Grad* kopija(Grad* lista, int n) {
	int i;
	Grad* novi, * p = lista;
	Grad* glava = nullptr, * tek = nullptr;

	for (i = 0; i < n; i++) {
		novi = new Grad;
		novi->x = p->x;
		novi->y = p->y;
		novi->next = nullptr;
		if (!glava)
			glava = novi;
		else if (tek)
			tek->next = novi;
		tek = novi;
		p = p->next;
	}
	return glava;
}

//funkcija koja vraca vrednosti razdaljine
//izmedju dva grada.
double distanca(int x1, int x2, int y1, int y2) {
	int c1 = x1 - x2;
	int c2 = y1 - y2;
	return sqrt(pow(c1, 2) + pow(c2, 2));
}

//funkcija koja zamenjuje koordinate dva grada.
//njen smisao je prespajanje gradova ako je
//pronadjen grad koji je blizi tekucem gradu
//od trenutnog suseda.
void swap(Grad* stari, Grad* novi) {
	int tek;

	tek = stari->x;
	stari->x = novi->x;
	novi->x = tek;

	tek = stari->y;
	stari->y = novi->y;
	novi->y = tek;
}

//glavni algoritam "Heuristike najblizeg grada"
//koji nalazi najblizeg suseda tekucem gradu.
//funkcija uzima listu gradova i polazi od prvog
//trazeci njegovog najblizeg suseda, zatim
//postavlja njegovog najblizeg suseda kao tekuceg
//i trazi njemu najblizeg suseda... 
//algoritam se ponavlja dok se ne naidje
//na poslednji grad.
//u ovom programu se poslednji grad NE SPAJA sa 
//pocetnim.
void obrada(Grad* lista, int n, double* duz) {
	int i, j;
	double d, duzina = 0, dmin, niz[100] = {};
	Grad* p = lista;
	Grad* q = nullptr, * r = nullptr;

	ispis(lista);
	for (i = 0; i < n - 1; i++) {
		dmin = -1;
		q = p->next;

		for (j = i + 1; j < n; j++) {
			d = distanca(p->x, q->x, p->y, q->y);
			if (d == 0) {
				q = q->next;
				continue;
			}
			if (d < dmin || dmin == -1) {
				dmin = d;
				r = q;
			}
			q = q->next;
		}
		niz[i] = dmin;
		duzina += dmin;
		if (!(p->next == r)) {
			swap(p->next, r);
			ispis(lista);
		}
		p = p->next;
		if (!p)
			break;
	}
	cout << endl << "\t" << "Zbir puteva:" << endl << "  ";
	for (i = 0; i < n - 1; i++) {
		if (i % 10 == 0 && i != 0) cout << endl << "  ";
		cout << setprecision(2) << fixed << niz[i];
		if (i < n - 2) cout << " + ";
	}
	cout << endl << "   => Duzina predjenog puta je: " << 
		setprecision(2) << fixed << duzina << endl << endl;
	*duz = duzina;
}

//telo programa u kome korisnik unosi proizvoljan
//broj gradova ne veci od 100 i nakon unosa koordinata
//bira da li zeli da izvrsi obradu algoritma za odredjen grad
//ili da dozvoli programu izvrsavanje algoritma nad svim gradovima
//pri cemu ce program rezultate svih obrada uporediti i
//ispisati najoptimalniji put i grad od koga taj put pocinje.
//na kraju se korisniku daje opcija da nastavi sa ponovnim
//koriscenjem programa ili da iz njega izadje.
int main() {
	int n, i, j, s;
	const int high = 100;
	double duz, niz[100];
	Grad* lista, * lista1;

	srand(time(NULL));
	cout << endl;
	cout << "\t" << "      *~~~~~~~~~~~~~~~~~~~~~~~~~~*" << endl;
	cout << "\t" << "       HEURISTIKA NAJBLIZEG GRADA" << endl;
	cout << "\t" << "      *~~~~~~~~~~~~~~~~~~~~~~~~~~*" << endl;
	while (1) {
		cout << "\t" << "Koliko gradova zelite da trgovac obidje? ";
		cin >> n;
		cout << endl;

		while (n <= 1 || n>100) {
			cout << "\t" << "Neispravan unos, pokusajte ponovo!" << endl;
			cout << "\t" << "Koliko gradova zelite da trgovac obidje? ";
			cin >> n;
			cout << endl;
		}

		lista = unosgradova(n, high);
		cout << endl;

		cout << "\t" << "Program ce vrsiti obradu heuristike za svaki grad.";
		cout << endl << "\t" << "Ako zelite da vi izaberete od kog grada " << 
			"trgovac polazi unesite broj ""1"": ";
		cin >> s;
		cout << endl;

		switch (s) {
		case 1: {
			int grad;
			lista1 = kopija(lista, n);
			Grad* p = lista1;

			cout << "\t" << "Unesite broj grada: ";
			cin >> grad;

			while (grad < 1 || grad > n) {
				cout << "\t" << "Neispravan unos, pokisajte ponovo!";
				cout << "\t" << "Unesite broj grada: ";
				cin >> grad;
			}

			for (j = 0; j < grad - 1; j++)
				p = p->next;

			if (!(lista1 == p)) swap(lista1, p);

			cout << endl;
			cout << "\t" << "       Polazeci od "<< grad <<". grada:" << endl;
			obrada(lista1, n, &duz);
			niz[0] = duz;
			dealokacija(lista1);
			cout << endl;
			break;
		}
		default: {
			for (i = 0; i < n; i++) {
				lista1 = kopija(lista, n);
				Grad* p = lista1;
				for (j = 0; j < i; j++)
					p = p->next;
				if (!(lista1 == p)) swap(lista1, p);
				cout << "\t" << "       Polazeci od " << j + 1 << 
					". grada:" << endl;
				obrada(lista1, n, &duz);
				niz[i] = duz;
				dealokacija(lista1);
				cout << endl;
			}

			double dmin = niz[0];
			int grad = 0;
			for (i = 0; i < n; i++) {
				if (niz[i] < dmin) {
					dmin = niz[i];
					grad = i;
				}
			}
			cout << "\t" << "---------------------------------"<<
				"--------------------------------------" << endl;
			cout << "\t" << "|  Najkraci put izmedju gradova je " << 
				setprecision(2) << fixed << dmin << ", ako se krene od " << 
				grad + 1 << ". grada.  |" << endl;
			cout << "\t" <<"----------------------------------"<<
				"-------------------------------------" << endl << endl;
		}
		}

		cout << "\t" << "Ukoliko zelite da nastavite sa programom,"<<
			" unesite broj ""1""," << endl << "\t" << 
			"a ukoliko zelite da izadjete iz programa, unesite bilo sta:  ";
		int k;
		cin >> k;
		if (k != 1) break;
		cout << endl;
	}
	system("cls");
	cout << endl << "Kraj rada." << endl << endl;
	system("pause");
	return 0;
}