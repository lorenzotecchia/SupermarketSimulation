#include "../include_client/gui.h"

void print_welcome_message() {
  printf("\033[1;36m");  // Colore ciano
  printf("+-------------------------------------------------------+\n");
  printf("|                                                       |\n");
  printf("|          🌟 Benvenuto al Simulatore Supermercato 🌟   |\n");
  printf("|                                                       |\n");
  printf("| 🛒 Dove la programmazione incontra lo shopping 🛒     |\n");
  printf("|                                                       |\n");
  printf("|    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~    |\n");
  printf("|       🥦 🍞 🍎 🧀 🍪 🥛   I reparti sono aperti!      |\n");
  printf("|    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~    |\n");
  printf("|                                                       |\n");
  printf("|        Premi un tasto per iniziare la tua avventura!  |\n");
  printf("|                                                       |\n");
  printf("+-------------------------------------------------------+\n");
  printf("\033[0m");  // Reset colore
}


// Funzione per visualizzare la griglia con il supermercato
void display_grid(int position[][2], int num_omini) {
    system("clear");

    // Stampa la scritta "Super Market Simulator" sopra il supermercato
    printf(BLUE "                SUPER MARKET SIMULATOR                \n" RESET);
    printf(BLUE "======================================================\n" RESET);

    // Stampa la griglia con gli omini e il supermercato
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int printed = 0;

            // Stampa gli omini
            for (int i = 0; i < num_omini; i++) {
                if (position[i][0] == y && position[i][1] == x) {
                    printf(BLUE "%s" RESET, OMINO);
                    printed = 1;
                    break;
                }
            }

            // Costruzione del supermercato con mura e una porta centrale
            if (y >= HEIGHT - 8) {
                if (y == HEIGHT - 8 || y == HEIGHT - 1) { // Muro superiore e inferiore
                    if (x == WIDTH / 2) { // Porta centrale
                        printf("%s", DOOR);
                    } else {
                        printf(BLUE "%s" RESET, WALL);
                    }
                    printed = 1;
                } else if (x == 0 || x == WIDTH - 1) { // Muri laterali
                    printf(BLUE "%s" RESET, WALL);
                    printed = 1;
                }
            }

            // Se nessun elemento è stato stampato, lascia lo spazio vuoto
            if (!printed) {
                printf(" ");
            }
        }
        printf("\n");
    }
}

// Funzione per muovere gli omini verso il supermercato e poi farli uscire
void move_omini(int position[][2], int num_omini, int *time_in_supermarket) {
    for (int i = 0; i < num_omini; i++) {
        // Se l'omino è sopra la porta, entra e trova una posizione casuale all'interno del supermercato
        if (position[i][0] == HEIGHT - 9 && position[i][1] == WIDTH / 2 && time_in_supermarket[i] == 0) {
            // Posizione casuale all'interno del supermercato
            position[i][0] = HEIGHT - 7 - (rand() % (HEIGHT - 10)); // Riga casuale all'interno delle mura
            position[i][1] = 1 + rand() % (WIDTH - 2);              // Colonna casuale tra 1 e WIDTH - 2
            time_in_supermarket[i] = rand() % 10 + 5;               // Tempo casuale che l'omino trascorre nel supermercato
        }
        // Dopo che l'omino ha passato il tempo assegnato, esce
        else if (time_in_supermarket[i] > 0) {
            time_in_supermarket[i]--;
            if (time_in_supermarket[i] == 0) {
                // L'omino torna alla porta
                position[i][0] = HEIGHT - 9;
                position[i][1] = WIDTH / 2;
            }
        }
        // L'omino esce una volta raggiunta la porta
        else if (position[i][0] > 0) {
            position[i][0]--;
        }
    }
}

// Funzione principale per simulare il movimento degli omini
void simulate_galaga(int num_omini) {
    if (num_omini < 1 || num_omini > 100) {
        printf("Numero non valido. Usa un numero tra 1 e 100.\n");
        return;
    }

    // Inizializza il generatore di numeri casuali
    srand(time(NULL));

    // Array per mantenere le posizioni degli omini (riga, colonna) e il tempo che trascorrono nel supermercato
    int position[num_omini][2];
    int time_in_supermarket[num_omini];
    for (int i = 0; i < num_omini; i++) {
        position[i][0] = i * 2; // Posizione verticale iniziale
        position[i][1] = WIDTH / 2; // Allineato con la porta
        time_in_supermarket[i] = 0; // Inizialmente gli omini non sono nel supermercato
    }

    // Ciclo di animazione
    while (1) {
        display_grid(position, num_omini);
        move_omini(position, num_omini, time_in_supermarket);
        usleep(200000); // pausa di 200ms
    }
}

void print_shopping_message(int client_id) {
  printf("\033[1;33m");  // Colore giallo
  printf("+-------------------------------------------------------+\n");
  printf("|                                                       |\n");
  printf("|   🛒  Il cliente %d sta facendo acquisti...           |\n", client_id);
  printf("|                                                       |\n");
  printf("|       Si prega di attendere mentre esplora i reparti! |\n");
  printf("|                                                       |\n");
  printf("+-------------------------------------------------------+\n");
  printf("\033[0m");  // Reset colore
}

void print_waiting_in_queue_message(int client_id) {
  printf("\033[1;35m");  // Colore magenta
  printf("+-------------------------------------------------------+\n");
  printf("|                                                       |\n");
  printf("|   ⏳  Il cliente %d è in fila alla cassa...           |\n", client_id);
  printf("|                                                       |\n");
  printf("|       Sarà servito a breve!                           |\n");
  printf("|                                                       |\n");
  printf("+-------------------------------------------------------+\n");
  printf("\033[0m");  // Reset colore
}

void print_closing_message()   {
  printf("\033[1;31m");  // Colore rosso
  printf("+-------------------------------------------------------+\n");
  printf("|                                                       |\n");
  printf("|          🛒  Il supermercato è ora chiuso!            |\n");
  printf("|                                                       |\n");
  printf("|       Tutti i clienti hanno completato i loro acquisti.|\n");
  printf("|                                                       |\n");
  printf("|        Grazie per la visita! Buona giornata!          |\n");
  printf("|                                                       |\n");
  printf("+-------------------------------------------------------+\n");
  printf("\033[0m");  // Reset colore
}

