#include "../../include/gui.h"

void print_welcome_message() {
  printf("\033[1;36m");  // Colore ciano
  printf("+-------------------------------------------------------+\n");
  printf("|                                                       |\n");
  printf("|          🌟 Benvenuto al Simulatore Supermercato 🌟   |\n");
  printf("|                                                       |\n");
  printf("|   🛒   Dove la programmazione incontra lo shopping   🛒   |\n");
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

void print_closing_message() {
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

