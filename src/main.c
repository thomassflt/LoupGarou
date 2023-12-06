#include <SDL.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "fonction.h"

joueur* playerList = NULL;

const int role_count = voleur - villageois;


void print_player(joueur player){
    printf("%d \n",player.role);
    printf("%d \n",player.pouv);
    printf("%d\n",player.status);
}

int randomize(int randRange){
    return (int)rand()%randRange;
}

int *distribution_role(joueur* pList, int pNum, int maxLoup){
    //SI IL Y'A MOINS DE 12 JOUEURS, AJOUTER 2 LOUP GAROUS SINON 3 LOUP GAROUS

    int deck[] = {villageois,villageois,villageois,villageois,villageois,villageois,villageois,villageois,villageois,villageois,villageois,villageois,villageois
    ,cupidon, sorciere,chasseur,fille,voleur};
    int checkTab[] = {0,0,0,0,0,0,0,0};
    static int carteVoleur[2];

    srand(time(NULL));

    do{
        int joueurAlea = randomize(pNum);
        if(pList[joueurAlea].role != loup && pList[joueurAlea].role != voyante){
            pList[joueurAlea].role = loup;
            checkTab[1] = checkTab[1]+1;
        }
        /*On va générer aléatoirement un nombre qui correspondera a l'index d'un joueur de la liste
        Et on attribue le role VOYANTE uniquement si ce joueur n'est pas loup  et si il n'est pas voyante(par mesure de securite)*/
    }while(checkTab[1] < maxLoup);
    
    do{
        int joueurAlea = randomize(pNum);
        if(pList[joueurAlea].role != loup && pList[joueurAlea].role != voyante){
            pList[joueurAlea].role = voyante;
            checkTab[2] = checkTab[2]+1;
        }
        /*On va générer aléatoirement un nombre qui correspondera a  l'index d'un joueur de la liste
        Et on attribue le role VOYANTE uniquement si ce joueur n'est pas loup  et si il n'est pas voyante(par mesure de securite)*/
    }while(checkTab[2] < 1);

    
    int k = 0;

    if(checkTab[2]>=1 && checkTab[1]>= maxLoup){
        for(int i=0; i<pNum +2;i++){
            if(pList[i].role !=loup && pList[i].role != voyante && i < pNum){
                int roleAlea = randomize(17-k);
                pList[i].role = deck[roleAlea];
                deck[roleAlea] = deck[17-k];
                k++;
            }
            else if (i >= pNum)
            {
                
                int roleAlea = randomize(17-k);
                carteVoleur[i] = deck[roleAlea];
                deck[roleAlea] = deck[17-k];
                k++;
            }
        }
    }


    return carteVoleur;//Retourne un tableau contenant les cartes pour le voleur
////////////AJOUT DE 2 CARTE ALEA QUE LE VOLEUR POURRA VOLER
}  

int main(int argc, char** argv)
{
    int nombreJoueur=0, maxLoup = 0, premierTour = 0, tour1 = 1, resultat = 0;
    int *carteVoleur;
    //initialise l'audio et la vidéo
   SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
   //initialise SDL mixer
   Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    //charge un fichier audio
    Mix_Music *background = Mix_LoadMUS("audio/Ambiance Loup Garou (320 kbps).mp3");
    Mix_VolumeMusic(50);
    Mix_Chunk *startgame = Mix_LoadWAV("audio/startgame.wav");
    Mix_Chunk *debut = Mix_LoadWAV("audio/debut.wav");
    Mix_Chunk *suivant = Mix_LoadWAV("audio/Joueursuivant.wav");
    Mix_Chunk *dortCupidon = Mix_LoadWAV("audio/Cupidondort.wav");
    Mix_Chunk *dortLoup = Mix_LoadWAV("audio/dortLoup.wav");
    Mix_Chunk *dortSorciere = Mix_LoadWAV("audio/dortSorciere.wav");
    Mix_Chunk *dortVillage = Mix_LoadWAV("audio/dortVillage.wav");
    Mix_Chunk *mortJoueur = Mix_LoadWAV("audio/mortJoueur.wav");
    Mix_Chunk *reveilCupidon = Mix_LoadWAV("audio/reveilCupidon.wav");
    Mix_Chunk *reveilLoup = Mix_LoadWAV("audio/reveilLoup.wav");
    Mix_Chunk *reveilSorciere = Mix_LoadWAV("audio/reveilSorciere.wav");
    Mix_Chunk *reveilVillage = Mix_LoadWAV("audio/reveilVillage.wav");
    Mix_Chunk *reveilVoleur = Mix_LoadWAV("audio/reveilvoleur.wav");
    Mix_Chunk *reveilVoyante = Mix_LoadWAV("audio/reveilVoyante.wav");
    Mix_Chunk *dortVoleur = Mix_LoadWAV("audio/voleurdort.wav");
    Mix_Chunk *voteLoup = Mix_LoadWAV("audio/voteLoup.wav");
    Mix_Chunk *dortVoyante = Mix_LoadWAV("audio/Voyantedort.wav");

   //Creer une fenetre
    SDL_Window *window = SDL_CreateWindow("Loup-Garou", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);

   
    //Creer un rendu
 //   SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
   


 
    
    Mix_PlayMusic(background, -1);
    

    
    bool running = true;
    SDL_Event event;


    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if(event.key.keysym.sym == SDLK_ESCAPE)
            {
                running = false;
                break;
            }

            do{
        printf("Veuillez entrer le nombre de joueurs : ");
        scanf("%d", &nombreJoueur);
    }while(nombreJoueur< 8 || nombreJoueur > 18);

    //CHAQUE JOUEUR ENTRE SON NOM
    system("cls");


    //Le nombre max de loup par partie
    
    if(nombreJoueur < 12){
        maxLoup = 2; 
    }else{
        maxLoup = 3; 
    }

    playerList = malloc(nombreJoueur * sizeof(joueur));    
    
    for(int i=0; i<nombreJoueur;i++){
        playerList[i].status = alive;
    }

    carteVoleur = malloc(2 * sizeof(int));    
    
    carteVoleur = distribution_role(playerList, nombreJoueur, maxLoup);

    Mix_PlayChannel(-1,debut,0);

    sleep(7);

    for(int i=0; i<nombreJoueur;i++){
        
        printf("Joueur %d, voici votre role : ", i);
        role_printf(playerList, i);
        printf("Rentrez votre pseudo : ");
        scanf("%s", playerList[i].pseudo);
        system("cls");
        if (i != nombreJoueur-1){
        Mix_PlayChannel(-1,suivant,0);
        sleep(2);
        }
    }

/*    printf("Carte voleur : \n");

    for(int i=0;i<2;i++){
        printf("%d \n", carteVoleur[i]);
    }

    printf("Joueurs :\n");

    for(int i=0; i<nombreJoueur;i++){
        print_player(playerList[i]);
        printf("Suivant :\n");
    }*/
    initialisation(playerList, nombreJoueur);

  //  Mix_PauseMusic();
    Mix_PlayChannel(-1,startgame,0);
    printf("LA PARTIE PEUT ALORS COMMENCER ! LE VILLAGE S'ENDORT.....\n");
    sleep(4);
 //   Mix_PlayMusic(background,-1);
    system("cls");



/////////////////////////////PARTIE//////////////////////////////////
    if(premierTour == 0){
        
        for (int i = 0; i < nombreJoueur; ++i)
        {
            if (playerList[i].role == voleur)
            {
                Mix_PlayChannel(-1,reveilVoleur,0);
                printf("Le voleur se reveille.....\n");
                Voleur(carteVoleur, playerList, i);
                Mix_PlayChannel(-1,dortVoleur,0);
                printf("Le voleur peut se rendormir.....\n");
                sleep(3);
                system("cls");
            }
        }

        for (int i = 0; i < nombreJoueur; ++i)
        {
            if (playerList[i].role == cupidon)
            {
                Mix_PlayChannel(-1,reveilCupidon,0);
                printf("Cupidon se reveille.....\n");
                Cupidon(playerList, nombreJoueur);
                //AFFICHE LES AMOUREUX
                Mix_PlayChannel(-1,dortCupidon,0);
                sleep(3);
                system("cls");
            }
        }
        premierTour = 1;
    }

    do{

        for (int i = 0; i < nombreJoueur; ++i)
        {
            if (playerList[i].role == voyante && playerList[i].status == 0) 
            {
                Mix_PlayChannel(-1,reveilVoyante,0);
                printf("La voyante se reveille et souhaite prendre connaissance du role d'un joueur.....\n");
                Voyant(playerList, nombreJoueur);
                Mix_PlayChannel(-1,dortVoyante,0);
                printf("La voyante peut se rendormir.....\n");
                sleep(3);
                system("cls");
            }
        }

        for (int i = 0; i < nombreJoueur; ++i)
        {
            if (playerList[i].role == loup && playerList[i].status == 0) 
            {
                Mix_PlayChannel(-1,reveilLoup,0);
                printf("Les loups garous se reveillent. Rassemblez vous et tuez quelqu'un\n");
                Loupgarou(playerList, nombreJoueur, 0);
                Mix_PlayChannel(-1,dortLoup,0);
                printf("Les loups garous peuvent se rendormir.....\n");
                sleep(3);
                system("cls");
                break;
            }
        }

        for (int i = 0; i < nombreJoueur; ++i)
        {
            if (playerList[i].role == sorciere && playerList[i].status == 0) 
            {
                Mix_PlayChannel(-1,reveilSorciere,0);
                printf("La sorciere se reveille.\n");
                Sorciere(playerList, nombreJoueur, tour1);
                Mix_PlayChannel(-1,dortSorciere,0);
                sleep(3);
                system("cls");
            }
        }
        Mix_PlayChannel(-1,reveilVillage,0);
        printf("Le village se reveille.\n");
        sleep(2);
        Tour(playerList, nombreJoueur, tour1);
        resultat = fin(playerList, nombreJoueur);
        if (resultat != 0)
        {
            break;
        }
        if (tour1 == 1)
        {
            Capitaine(playerList, nombreJoueur, 0);
        }
        Mix_PlayChannel(-1,voteLoup,0);
        printf("Nous allons proceder aux votes afin d'eliminer un potentiel loup-garou; \n");
        Vote(playerList, nombreJoueur, 0);
        for (int i = 0; i < nombreJoueur ; i++){
            if (playerList[i].status == 1 && playerList[i].tour == tour1 && playerList[i].role == 5)
//cherche si un joueur mort la nuit derniere est le chasseur, si oui on lance la fonction chasseur qui permet au chasseur de tuer une derniere personne de son choix.
		{
            Mix_PlayChannel(-1,mortJoueur,0);
			printf("%s est mort cette nuit. Son role etait : \n Chasseur \n" ,playerList[i].pseudo);
			Chasseur(playerList, nombreJoueur);
		}
        }
        resultat = fin(playerList, nombreJoueur);
        if (resultat != 0)
        {
            break;
        }
        //VERIFIER SI IL Y A UNE VICTOIRE (LOUP OU VILLAGEOIS), SI C'EST LE CAS , QUITTER LA BOUCLE
        Mix_PlayChannel(-1,dortVillage,0);
        printf("Le village peut se rendormir.....\n");
        sleep(3);
        system("cls");
        tour1++;

    }while(resultat == 0);//AUCUN CAMP N'A GAGNE   

    switch(resultat){
        case 1 :
            printf("Les loup-garous remportent la partie !!\n");
            break;
        default :
            printf("Felicitation, les villageois remportent la partie !!\n");
            break;
    }
    


    free(playerList);
    running = false;
    break;

        }
        
    }
    

   SDL_DestroyWindow(window);
    Mix_FreeMusic(background);
    Mix_FreeChunk(startgame);
    Mix_FreeChunk(debut);
    Mix_FreeChunk(suivant);
    Mix_FreeChunk(dortCupidon);
    Mix_FreeChunk(dortLoup);
    Mix_FreeChunk(dortSorciere);
    Mix_FreeChunk(dortVillage);
    Mix_FreeChunk(mortJoueur);
    Mix_FreeChunk(reveilCupidon);
    Mix_FreeChunk(reveilLoup);
    Mix_FreeChunk(reveilSorciere);
    Mix_FreeChunk(reveilVillage);
    Mix_FreeChunk(reveilVoleur);
    Mix_FreeChunk(reveilVoyante);
    Mix_FreeChunk(dortVoleur);
    Mix_FreeChunk(voteLoup);
    Mix_FreeChunk(dortVoyante);
 //   Mix_FreeChunk(verre);
   Mix_CloseAudio();
   SDL_Quit();

    return 0;


}
