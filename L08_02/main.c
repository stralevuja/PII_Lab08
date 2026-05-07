#include <stdio.h>
#include <stdlib.h>

#define MAX 5 // maksimalni broj elemenata u baferu

/* ---- Kružni bafer (sekvencijalna reprezentacija, bez prepisivanja) ---- */

typedef struct {
    int podaci[MAX]; // niz u kojem se cuvaju elementi
    int head;        // indeks odakle vadimo (dequeue)
    int tail;        // indeks gdje dodajemo (enqueue)
    int count;       // trenutni broj elemenata
} CircularBuffer;

// Inicijalizacija praznog bafera
void bafer_init(CircularBuffer *b) {
    b->head = 0;
    b->tail = 0;
    b->count = 0;
}

// Provjera da li je bafer pun
int bafer_pun(CircularBuffer *b) {
    return b->count == MAX;
}

// Provjera da li je bafer prazan
int bafer_prazan(CircularBuffer *b) {
    return b->count == 0;
}

// Dodaje element u bafer (vraca 1 ako uspjesno, 0 ako je bafer pun)
int bafer_dodaj(CircularBuffer *b, int vrijednost) {
    if (bafer_pun(b)) {
        printf("Upozorenje: bafer je pun, nije moguce dodati element.\n");
        return 0;
    }
    b->podaci[b->tail] = vrijednost;
    b->tail = (b->tail + 1) % MAX; // kružno pomjeramo rep
    b->count++;
    return 1;
}

// Uklanja element iz bafera (vraca uklonjenu vrijednost kroz pokazivac)
// Vraca 1 ako uspjesno, 0 ako je bafer prazan
int bafer_ukloni(CircularBuffer *b, int *vrijednost) {
    if (bafer_prazan(b)) {
        printf("Upozorenje: bafer je prazan, nema elemenata za uklanjanje.\n");
        return 0;
    }
    *vrijednost = b->podaci[b->head];
    b->head = (b->head + 1) % MAX; // kružno pomjeramo glavu
    b->count--;
    return 1;
}

// Ispisuje sve elemente bafera (od glave prema repu)
void bafer_ispis(CircularBuffer *b) {
    if (bafer_prazan(b)) {
        printf("Bafer je prazan.\n");
        return;
    }
    printf("Elementi bafera: ");
    for (int i = 0; i < b->count; i++) {
        printf("%d ", b->podaci[(b->head + i) % MAX]);
    }
    printf("\n");
}

/* ---- Stek (ulancana reprezentacija, LIFO) za cuvanje historije unosa ---- */

typedef struct stack_node {
    int vrijednost;
    struct stack_node *next;
} StackNode;

// Guranje elementa na vrh steka
void push(StackNode **top, int vrijednost) {
    StackNode *novi = malloc(sizeof(StackNode));
    if (!novi) return;
    novi->vrijednost = vrijednost;
    novi->next = *top;
    *top = novi;
}

// Ispisuje sve elemente steka od vrha prema dnu (obrnuti redoslijed unosa)
void stack_ispis(StackNode *top) {
    if (top == NULL) {
        printf("Nisu uneseni nikakvi elementi.\n");
        return;
    }
    printf("Svi uneseni elementi u obrnutom redoslijedu:\n");
    StackNode *current = top;
    while (current != NULL) {
        printf("%d\n", current->vrijednost);
        current = current->next;
    }
}

// Oslobadja memoriju steka
void stack_free(StackNode **top) {
    while (*top != NULL) {
        StackNode *temp = *top;
        *top = (*top)->next;
        free(temp);
    }
}

/* ---- Glavni program ---- */

int main(void) {
    CircularBuffer bafer;
    bafer_init(&bafer);

    StackNode *historija = NULL; // stek sa svim unesenim elementima

    int izbor;
    do {
        // Meni
        printf("\n--- MENI ---\n");
        printf("1. Dodaj element u bafer\n");
        printf("2. Ukloni element iz bafera\n");
        printf("3. Ispis elemenata bafera\n");
        printf("0. Izlaz\n");
        printf("Izbor: ");
        scanf("%d", &izbor);

        switch (izbor) {
            case 1: {
                int val;
                printf("Unesite prirodan broj: ");
                scanf("%d", &val);
                if (val <= 0) {
                    printf("Unesite prirodan broj (veci od 0).\n");
                    break;
                }
                // Pokusaj dodavanja u bafer
                if (bafer_dodaj(&bafer, val)) {
                    push(&historija, val); // bilježimo u historiju samo ako je uspjesno dodano
                    printf("Element %d dodan u bafer.\n", val);
                }
                break;
            }
            case 2: {
                int uklonjeni;
                if (bafer_ukloni(&bafer, &uklonjeni)) {
                    printf("Uklonjen element: %d\n", uklonjeni);
                }
                break;
            }
            case 3:
                bafer_ispis(&bafer);
                break;
            case 0:
                break;
            default:
                printf("Nepoznat izbor.\n");
        }
    } while (izbor != 0);

    // Ispis svih unesenih elemenata u obrnutom redoslijedu (LIFO - stek)
    printf("\n");
    stack_ispis(historija);

    stack_free(&historija);
    return 0;
}