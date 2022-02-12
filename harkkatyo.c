//Tekijä Iida Vaaja
//Opiskelijanro 0606562
//Pohjana käytetty viikon 7 tyopajaa


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct puusolmu_t {
  int luku;
  short tila; /* tasapainoilmaisin */
  struct puusolmu_t *vasen, *oikea;
} puusolmu, *puuosoitin;

void lisaa_solmu(puuosoitin *, int, int *);
void oikea_kierto(puuosoitin *, int *);
void vasen_kierto(puuosoitin *, int *);
void printTreeSide(puuosoitin, int);

void readFile(puuosoitin *emo, int *etp) {
    FILE *reading; 
    char name[20];
    int depth = 0, count = 0;
    int luku;
    //clock_t start_t, end_t;

    printf("Anna tiedoston nimi: ");
    scanf("%s", &name);

    if ((reading = fopen(name, "r")) == NULL) {
        printf("Tiedoston lukeminen epäonnistui\n");
        exit(1);
    }
    //printf("start\n");
    //start_t = clock();
    while (fscanf(reading, "%d", &luku) != EOF) {
        lisaa_solmu(emo, luku, etp);
        printTreeSide(*emo, depth);
        printf("\n---------------------------------\n\n");
    }

    //end_t = clock();
    fclose(reading);
    //printf("close\n");
    
    //printf("%f", ((double)(end_t-start_t)/CLOCKS_PER_SEC));
}

void printTreeSide(puuosoitin solmu, int depth) { 
    //apuja katsottu
    //https://stackoverflow.com/questions/3899636/how-can-i-print-a-binary-tree-search-class-vertically
    if(solmu == NULL) return;
    
    printTreeSide(solmu->oikea, depth+1);
    
    for (int i = 0; i < depth; i++) {
        printf("\t");
    }
    printf("%d, (%d)\n", solmu->luku, solmu->tila);

    printTreeSide(solmu->vasen, depth+1);
}

void findKey(puuosoitin solmu, int key) {
    if (solmu == NULL) {
        printf("Avain ei ole puussa\n\n");
    }
    
    else if(solmu->luku == key) {
        printf("Avain löytyi puusta\n\n");
        return;
    }

    else if (solmu->luku < key) {
        findKey(solmu->oikea, key);
    }

    else if (solmu->luku > key) {
        findKey(solmu->vasen, key);
    }

}

int main() {

    int etp = 0, i, ch = 1, depth = 0, avain;
    puuosoitin puu = NULL;

    while(ch != 0) {

        printf("Mitä haluat tehdä?\n");
        printf("1) Lue tiedosto ja tulosta puu\n2) Hae avainta puusta\n3) Lisää yksittäinen avain puuhun\n0) Lopeta\n");
        printf("Anna luku: ");
        scanf("%d", &ch);

        if(ch == 0) {}
    
        else if(ch == 1) {
            readFile(&puu, &etp);
            printTreeSide(puu, depth);
            printf("\n");
        }

        else if(ch == 2) {
            printf("Anna avain: ");
            scanf("%d", &avain);
            findKey(puu, avain);
        }  

        else if(ch == 3) {
            printf("Avain, joka lisätään puuhun: ");
            scanf("%d", &avain);
            lisaa_solmu(&puu, avain, &etp);
            printTreeSide(puu, depth);
            printf("\n");
        }
        
        else {
            printf("Tuntematon valinta\n\n");
        }

        
    }

    return 0;
}

void lisaa_solmu(puuosoitin *emo, int luku, int *etp)
{
    if(!(*emo))
    {
        *etp = 1;
        if(!(*emo = (puuosoitin)malloc(sizeof(puusolmu))))
        {
            perror("malloc");
            exit(1);
        }
        (*emo)->vasen = (*emo)->oikea = NULL;
        (*emo)->tila = 0;
        (*emo)->luku = luku;
    } else if(luku < (*emo)->luku)
    {
        lisaa_solmu(&(*emo)->vasen, luku, etp);
        if(*etp)
            {
                switch((*emo)->tila) {
                case -1:
                    (*emo)->tila = 0;
                    *etp = 0;
                    break;
                case 0:
                    (*emo)->tila = 1;
                    break;
                case 1:
                    vasen_kierto(emo, etp);
            }
        }
    } else if(luku > (*emo)->luku)
    {
        lisaa_solmu(&(*emo)->oikea, luku, etp);
        if(*etp)
        {   
            switch((*emo)->tila) {
            case 1:
                (*emo)->tila = 0;
                *etp = 0;
                break;
            case 0:
                (*emo)->tila = -1;
                break;
            case -1:
                oikea_kierto(emo, etp);
            }
        }
    } else
    {
        *etp = 0;
        printf("Luku %d on jo puussa\n", luku);
    }
}

void vasen_kierto(puuosoitin *emo, int *etp)
{
    puuosoitin lapsi, lapsenlapsi;

    lapsi = (*emo)->vasen;
    if(lapsi->tila == 1) /* LL-kierto */
    {
        printf("tehdään LL kierto kohtaan %d\n\n", lapsi->luku);
        (*emo)->vasen = lapsi->oikea;
        lapsi->oikea = *emo;
        (*emo)->tila = 0;
        (*emo) = lapsi;
    } 
    else /* LR-kierto */
    {
        printf("tehdään LR kierto kohtaan %d\n\n", lapsi->luku);
        lapsenlapsi = lapsi->oikea;
        lapsi->oikea = lapsenlapsi->vasen;
        lapsenlapsi->vasen = lapsi;
        (*emo)->vasen = lapsenlapsi->oikea;
        lapsenlapsi->oikea = *emo;
        switch(lapsenlapsi->tila)
        {
            case 1:
                (*emo)->tila = -1;
                lapsi->tila = 0;
                break;
            case 0:
                (*emo)->tila = lapsi->tila = 0;
                break;
            case -1:
                (*emo)->tila = 0;
                lapsi->tila = 1;
                break;
        }
        *emo = lapsenlapsi;
    }
    (*emo)->tila = 0;
    *etp = 0;
}

void oikea_kierto(puuosoitin *emo, int *etp)
{
    puuosoitin lapsi, lapsenlapsi;

    lapsi = (*emo)->oikea;
    if(lapsi->tila == -1) /* RR-kierto */
    {
        printf("tehdään RR kierto kohtaan %d\n\n", lapsi->luku);
        (*emo)->oikea = lapsi->vasen;
        lapsi->vasen = *emo;
        (*emo)->tila = 0;
        (*emo) = lapsi;
    } 
    else /* RL-kierto */
    {
        lapsenlapsi = lapsi->vasen;
        lapsi->vasen = lapsenlapsi->oikea;
        lapsenlapsi->oikea = lapsi;
        (*emo)->oikea = lapsenlapsi->vasen;
        lapsenlapsi->vasen = *emo;
        switch(lapsenlapsi->tila)
        {
            case -1:
                (*emo)->tila = 1;
                lapsi->tila = 0;
                break;
            case 0:
                (*emo)->tila = lapsi->tila = 0;
                break;
            case 1:
                (*emo)->tila = 0;
                lapsi->tila = -1;
        }
        *emo = lapsenlapsi;
    }
    (*emo)->tila = 0;
    *etp = 0;
}