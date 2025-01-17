# SupermarketSimulation

Il progetto **SupermarketSimulation** è una simulazione di un supermercato progettata per mettere in pratica concetti di gestione delle risorse, concorrenza e sincronizzazione.
Il progetto include una struttura basata su **clienti**, **cassieri** e **code** che simula il flusso di acquisti, gestione delle code alle casse, e il rispetto di un numero massimo di clienti ammessi all'interno del supermercato in ogni momento.

## Struttura del Progetto

Il progetto è organizzato nella seguente struttura di directory e file:

```plaintext
.
├── LICENSE
├── README.md
├── client
│   ├── client.c
│   ├── client_include
│   │   ├── client_models.h
│   │   └── gui.h
│   ├── client_models_impl
│   │   ├── connection.c
│   │   ├── gui.c
│   │   └── receive.c
│   ├── dockerfile
│   └── makefile
├── doc
│   ├── documentazione.pdf
│   ├── istruzioni_docker_compose.md
│   └── tracce_progetti.pdf
├── docker-compose.yml
└── server
    ├── dockerfile
    ├── makefile
    ├── server.c
    ├── server_include
    │   ├── colors.h
    │   └── server_models.h
    └── server_models_impl
        ├── cassa.c
        ├── cliente.c
        ├── connection.c
        └── supermercato.c
```

### Descrizione della Struttura

- **client/**: Contiene il codice sorgente e gli header per la simulazione dei clienti.
  - `client.c`: File principale che gestisce il ciclo di vita dei clienti.
  - `dockerfile`: File per creare l'immagine Docker del modulo client.
  - `client_include/`: Contiene gli header per le funzionalità di gestione dei clienti.
    - `client_models`: Definizioni principali del cliente.
    - `gui.h`: Funzionalità per l'interfaccia grafica.
    - `receive.h`: Funzione per la gestione della comunicazione client-server.
  - `client_models_impl/`: Implementazioni delle funzioni definite negli header.
    - `connection.c`, `gui.c`, `receive.c`: Funzionalità modulari del client.
  - `makefile`: File di build per compilare il modulo client.

- **doc/**: Contiene documentazione aggiuntiva.
  - [Istruzioni docker compose](https://github.com/lorenzotecchia/SupermarketSimulation/blob/right/doc/istruzioni_docker_compose.md): Istruzioni per configurare e utilizzare Docker Compose per lanciare il progetto.
  - [Traccia](https://github.com/lorenzotecchia/SupermarketSimulation/blob/right/doc/tracce_progetti.pdf): Documento descrittivo con le specifiche dei progetti.
  - [Documentazione completa](https://github.com/lorenzotecchia/SupermarketSimulation/blob/right/doc/documentazione.pdf): Documentazione completa del progetto

- **docker-compose.yml**: Configurazione per Docker Compose per orchestrare l'esecuzione di client e server.

- **server/**: Contiene il codice sorgente e gli header per il lato server del supermercato.
  - `server.c`: File principale che gestisce le operazioni del supermercato.
  - `dockerfile`: File per creare l'immagine Docker del modulo server.
  - `server_include/`: Contiene gli header relativi alle strutture dati e al funzionamento del server.
    - `colors.h`: Header per la gestione dei colori nella console.
    - `server_models.h`: Definizioni delle strutture dati per i clienti, casse e supermercato.
  - `server_models_impl/`: Implementazioni delle funzionalità relative ai modelli.
    - `cassa.c`, `cliente.c`, `supermercato.c`: Moduli che gestiscono le singole entità della simulazione.
  - `makefile`: File di build per compilare il modulo server.

## Descrizione delle Componenti della Simulazione

### 1. Simulazione dei Clienti
   - I clienti vengono creati e ammessi al supermercato fino a un massimo stabilito, dopo di che ulteriori clienti vengono messi in una lista d'attesa esterna.
   - Ogni cliente segue un ciclo di comportamento:
     - **Scelta degli acquisti**: I clienti passano un periodo di tempo casuale a scegliere gli articoli, con un numero di articoli scelti anch'esso variabile.
     - **Assegnazione alla cassa**: Una volta terminata la scelta, il cliente si mette in coda alla cassa disponibile con meno clienti in attesa.
     - **Pagamento**: Dopo l'attesa in coda, il cliente viene servito da un cassiere per un periodo proporzionale al numero di articoli acquistati.
   - Al termine dell'operazione di pagamento, il cliente lascia il supermercato.

### 2. Gestione dei Cassieri
   - Il numero di casse attive è definito all'inizio della simulazione, e ogni cassiere opera indipendentemente.
   - Ogni cassiere ha una coda propria, e serve i clienti in ordine di arrivo.
   - Durante l'operazione di servizio, il tempo di pagamento è calcolato sulla base del numero di articoli selezionati dal cliente.
   - Una volta servito un cliente, il cassiere gestisce il passaggio al cliente successivo o rimane inattivo se non ci sono clienti in attesa.

### 3. Gestione delle Code e Sincronizzazione
   - Le code dei clienti sono implementate come strutture separate per ogni cassa. La gestione è tale che i clienti si distribuiscono in base alla disponibilità delle casse, ottimizzando i tempi di attesa.
   - Viene implementata una logica di sincronizzazione per garantire che le risorse (casse e code) siano gestite correttamente in un ambiente concorrente.
   - Se il numero di clienti raggiunge il limite massimo all’interno del supermercato, nuovi clienti vengono messi in una lista d’attesa esterna finché non si liberano posti.
   - La logica di sincronizzazione si occupa di monitorare le risorse e gestire il flusso di clienti tra lista d'attesa esterna, code di acquisti e casse.

### 4. Logiche di Simulazione Avanzate
   - **Limitazione dinamica dei clienti**: Un limite massimo di clienti all'interno del supermercato è imposto per evitare sovraffollamento. Il sistema ammette nuovi clienti solo quando altri lasciano il supermercato, mantenendo una capienza controllata.
   - **Assegnazione dinamica delle casse**: I clienti vengono assegnati alla cassa con meno clienti in attesa, ottimizzando il flusso e minimizzando i tempi di attesa.

### 5. Output della Simulazione
   - La simulazione genera output testuali per tracciare il flusso di ogni cliente, compresi:
     - L'ingresso nel supermercato.
     - La fase di scelta degli acquisti.
     - L'assegnazione e l'attesa in coda presso una cassa specifica.
     - Il pagamento e l'uscita dal supermercato.
   - Ogni operazione (come la scelta degli acquisti e il pagamento) è accompagnata da un'indicazione del tempo trascorso e del numero di articoli acquistati, simulando una dinamica realistica di acquisti in un supermercato.

