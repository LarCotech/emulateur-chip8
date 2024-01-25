#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "proc.h"

/*
int automate (int index, char* ch);

int main () {
    automate(0, "alEzz3_eei");
    return 1;
}

int automate (int index, char* ch) {
    int etat = 0;
    while (etat != 3 && ch[index] == '\0') {
        printf("%c", ch[index]);
        if (etat == 1) {
            if (!(ch[index] >= 'a' && ch[index] <= 'z')
            || !(ch[index] >= 'A' && ch[index] <= 'Z')
            || ch[index] != '_') {
                etat = 3;
            }
        }
        else if (etat == 2){
            if (ch[index] >= '0' && ch[index] <= '9') etat = 2;
            else if ((ch[index] >= 'a' && ch[index] <= 'z')
            || (ch[index] >= 'A' && ch[index] <= 'Z')
            || ch[index] == '_') {
                etat = 1;
            }
            else printf("Erreur");
        } else {
            if (ch[index] == 0) {
                printf("Erreur 0 au début de la chaine\n");
            }
            else if (ch[index]) etat = 2;
            else if (ch[index]) etat = 1;
            else printf("Erreur");
        }
        index++;
    }
    if (etat == 3) {
        printf("Chaine reconnu !");
        return 0;
    }
    else {
        printf("Chaine non reconnu");
        return 1;
    }
}
 */

/*- Structure Émulateur -*/
typedef struct Machine {
    cpu* cpu;
    // Libprovided
    // Display & Sprite
    struct Display* display;
    struct Sprite* sprite;
    // Keyboard
    struct Keyboard* keyboard;
    // Speaker
    struct Speaker* speaker;
} Machine;

int Machine_init(Machine* machine);

int main (int argc, char** argv) {

    // Nom Rom passé en paramètre argv
    if (argc != 2) {
        fprintf(stderr, "Unexpected number of arguments.\n");
        fprintf(stderr, "Usage: emulator <ROM.ch8>\n");
        return 1;
    }

    // Initialiser l'émulateur
    Machine machine;
    Machine_init(&machine);

    // Ouvrir le fichier ROM
    FILE* romFile = fopen("", "rb");
    if (romFile == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier ROM.\n");
        return 1;
    }

    // Obtenir la taille du fichier
    fseek(romFile, 0, SEEK_END);
    long romSize = ftell(romFile);
    rewind(romFile);

    size_t bytesRead = fread(machine.cpu->RAM, 1, romSize, romFile);
    if (bytesRead != romSize) {
        fprintf(stderr, "Erreur de lecture du fichier ROM.\n");
        // Ferme ROM
        fclose(romFile);
        // Libérer la macihine
        return 1;
    }

    // Fermer ROM
    fclose(romFile);
    // Libérer la machine

    return 0;
}


int Machine_init(Machine* machine) {
    machine->cpu = initrCpu();
    // Init SLD elt
    // Display & Sprite
    struct Display* display;
    struct Sprite* sprite;
    // Keyboard
    struct Keyboard* keyboard;
    // Speaker
    struct Speaker* speaker;
    return 0;
}

/*
   if (argc != 2) {
       fprintf(stderr, "Unexpected number of arguments.\n");
       fprintf(stderr, "Usage: emulator <ROM.ch8>\n");
       return 1;
   }
   if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_AUDIO)) {
       errcode = SDL;
   }
   else {
       printf("The SDL library has been initialized successfully.\n");
       struct Machine machine;
       if (!Machine_init(&machine)) {
           printf("The CHIP 8 emulator is ready.\n");
           if (!Machine_load(&machine,argv[1])) {
               printf("The ROM <%s> has been loaded.\n",argv[1]);
               printf("Run...\n");
               while (
                       !Machine_loop(&machine)
                       );
           }
           Machine_destroy(&machine);
       }
       SDL_Quit();
   }
   if (errcode != QUIT) {
       fprintf(stderr,"The program has terminated abnormally (errcode=%s)\n",errorstr());
       return 1;
   }*/