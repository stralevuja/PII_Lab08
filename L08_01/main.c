#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
    #include <windows.h>
    #define sleep(s) Sleep((s) * 1000)
#else
    #include <unistd.h>
#endif

#define MAX_NAZIV 32

// Cvor reda (ulancana reprezentacija)
typedef struct process {
    char naziv[MAX_NAZIV]; // naziv procesa
    int vrijeme;           // vrijeme izvrsavanja u sekundama
    struct process *next;  // pokazivac na sljedeci element reda
} Process;

// Red se drzi kao pokazivaci na glavu (odakle vadimo) i rep (gdje dodajemo)
typedef struct {
    Process *head;
    Process *tail;
    int size;
} Queue;

// Inicijalizacija praznog reda
void queue_init(Queue *q) {
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
}

// Dodaje novi proces na kraj reda (enqueue)
void enqueue(Queue *q, const char *naziv, int vrijeme) {
    Process *novi = malloc(sizeof(Process));
    if (!novi) {
        printf("Greska pri alokaciji memorije.\n");
        return;
    }
    strncpy(novi->naziv, naziv, MAX_NAZIV - 1);
    novi->naziv[MAX_NAZIV - 1] = '\0';
    novi->vrijeme = vrijeme;
    novi->next = NULL;

    if (q->tail == NULL) {
        // Red je bio prazan
        q->head = novi;
        q->tail = novi;
    } else {
        // Dodajemo na kraj reda
        q->tail->next = novi;
        q->tail = novi;
    }
    q->size++;
}

// Uklanja i vraca proces sa pocetka reda (dequeue)
// Vraca NULL ako je red prazan
Process *dequeue(Queue *q) {
    if (q->head == NULL) return NULL;

    Process *stari = q->head;
    q->head = q->head->next;

    // Ako smo ispraznili red, rep takodje treba biti NULL
    if (q->head == NULL) q->tail = NULL;

    q->size--;
    return stari;
}

int main(void) {
    srand(time(NULL)); // inicijalizacija generatora slucajnih brojeva

    Queue q;
    queue_init(&q);

    // Korisnik unosi broj procesa
    int n;
    printf("Unesite broj procesa: ");
    scanf("%d", &n);

    // Kreiranje i dodavanje procesa u red
    printf("\nKreirani procesi:\n");
    for (int i = 1; i <= n; i++) {
        char naziv[MAX_NAZIV];
        snprintf(naziv, sizeof(naziv), "proces%d", i);

        // Nasumicno vrijeme izmedju 1 i 5 sekundi
        int vrijeme = (rand() % 5) + 1;

        enqueue(&q, naziv, vrijeme);
        printf("naziv_procesa: %s; vrijeme_izvrsavanja: %ds\n", naziv, vrijeme);
    }

    // Cekamo da korisnik unese "START"
    char unos[16];
    printf("\nUnesite START za pokretanje izvrsavanja: ");
    do {
        scanf("%15s", unos);
    } while (strcmp(unos, "START") != 0);

    // Izvrsavamo procese redom (FIFO)
    printf("\nIzvrsavanje procesa:\n");
    Process *p;
    while ((p = dequeue(&q)) != NULL) {
        printf("Izvrsavam: %s (%ds)...\n", p->naziv, p->vrijeme);
        sleep(p->vrijeme); // simulacija izvrsavanja
        printf("%s zavrsen.\n\n", p->naziv);
        free(p); // oslobadjamo memoriju zavrsenog procesa
    }

    printf("Svi procesi su izvrseni.\n");
    return 0;
}