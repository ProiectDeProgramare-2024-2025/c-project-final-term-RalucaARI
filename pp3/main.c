#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#ifdef _WIN32
    #define CLEAR_SCREEN "cls"
#else
    #define CLEAR_SCREEN "clear"
#endif

typedef struct {
    int id;
    int capacitate;
    char facilitati[256];
    float pret;
} sala;

typedef struct {
    int id_sala;
    char nume_client[50];
    char perioada[20];
} rezervare;


sala *sali=NULL;
rezervare *rezervari=NULL;
int nr_sali=0,nr_rezervari=0;


void verificaFisier(const char *numeFisier)
{
    FILE *file=fopen(numeFisier,"r");
    if (!file)
    {
        file=fopen(numeFisier, "w");
        if (file)
            fclose(file);
    } else
        fclose(file);
}


void incarcaSali()
{
    verificaFisier("sali.txt");
    FILE *file=fopen("sali.txt", "r");
    if (!file)
        return;
    sala temp;
    while(fscanf(file,"%d %d \"%255[^\"]\" %f",&temp.id,&temp.capacitate,temp.facilitati,&temp.pret)==4)
    {
        sali=realloc(sali,(nr_sali+1)*sizeof(sala));
        sali[nr_sali++]=temp;
    }
    fclose(file);
}

void incarcaRezervari()
{
    verificaFisier("rezervari.txt");
    FILE *file=fopen("rezervari.txt", "r");
    if (!file)
        return;
    rezervare temp;
    while(fscanf(file,"%d%s%s",&temp.id_sala,temp.nume_client,temp.perioada)==3)
    {
        rezervari=realloc(rezervari,(nr_rezervari+1)*sizeof(rezervare));
        rezervari[nr_rezervari++]=temp;
    }
    fclose(file);
}


void salveazaRezervari()
{
    FILE *file=fopen("rezervari.txt","w");
    if (!file)
        return;
    int i;
    for(i=0;i<nr_rezervari;i++)
        fprintf(file,"%d %s %s\n",rezervari[i].id_sala,rezervari[i].nume_client,rezervari[i].perioada);
    fclose(file);
}

void afiseazaSali()
{
    printf("\033[36mSali disponibile:\033[0m\n");
    int i;
    for(i=0;i<nr_sali;i++)
        printf("\033[31mID:\033[0m %d, \033[31mCapacitate:\033[0m %d, \033[31mFacilitati:\033[0m %s, \033[31mPret:\033[0m %.2f\n",sali[i].id,sali[i].capacitate,sali[i].facilitati,sali[i].pret);
}

void cautaSala(int capacitate, const char *facilitati)
{
    printf("Sali care corespund criteriilor:\n");
    int gasit=0;
    int i;
    for(i=0;i<nr_sali;i++)
        if (sali[i].capacitate>=capacitate && strstr(sali[i].facilitati,facilitati))
        {
            printf("\033[31mID:\033[0m %d, \033[31mCapacitate:\033[0m %d, \033[31mFacilitati:\033[0m %s, \033[31mPret:\033[0m %.2f\n",sali[i].id,sali[i].capacitate,sali[i].facilitati,sali[i].pret);
            gasit = 1;
        }
    if(!gasit)
        printf("\033[32mNu s-au gasit sali potrivite.\033[0m\n");
}

void rezervaSala(int id_sala,const char *nume_client,const char *perioada)
{
    rezervari=realloc(rezervari,(nr_rezervari+1)*sizeof(rezervare));
    rezervari[nr_rezervari].id_sala=id_sala;
    strcpy(rezervari[nr_rezervari].nume_client,nume_client);
    strcpy(rezervari[nr_rezervari].perioada,perioada);
    nr_rezervari++;
    printf("\033[32mRezervare efectuata cu succes!\n\033[0m");
}


void afiseazaRezervari()
{
    printf("\033[36mRezervari existente:\033[0m\n");
    int i;
    for(i=0;i<nr_rezervari;i++)
        printf("\033[31mID Sala:\033[0m %d, \033[31mClient:\033[0m %s, \033[31mPerioada:\033[0m %s\n",rezervari[i].id_sala,rezervari[i].nume_client,rezervari[i].perioada);
}

void anuleazaRezervare(const char *nume_client,const char *perioada)
{
    int i,gasit=0,j;
    for(i=0;i<nr_rezervari;i++)
    {
        if(strcmp(rezervari[i].nume_client,nume_client)==0 && strcmp(rezervari[i].perioada,perioada)==0)
        {
            gasit=1;
            for(j=i;j<nr_rezervari-1;j++)
                rezervari[j]=rezervari[j+1];

            nr_rezervari--;
            rezervari=realloc(rezervari,nr_rezervari*sizeof(rezervare));
            printf("\033[32mRezervare anulata cu succes!\033[0m\n");
            break;
        }
    }
    if (!gasit)
        printf("\033[32mRezervarea nu a fost gasita.\033[0m\n");
}

int main()
{
    int optiune;
    incarcaSali();
    incarcaRezervari();
    do
    {
        system(CLEAR_SCREEN);
        printf("\033[32m----Rezervare sala evenimente----\033[0m\n");
        printf("\033[36m\nMeniu:\033[0m\n");
        printf("1. \033[33mAfiseaza sali\033[0m\n");
        printf("2. \033[33mCauta sala\033[0m\n");
        printf("3. \033[33mRezerva sala\033[0m\n");
        printf("4. \033[33mAfiseaza rezervari\033[0m\n");
        printf("5. \033[33mAnuleaza rezervare\033[0m\n");
        printf("6. \033[33mIesire\033[0m\n");
        printf("\033[36mAlege o optiune: \033[0m\n");
        scanf("%d", &optiune);
        while(getchar()!='\n');
        switch (optiune)
        {
            case 1:
                afiseazaSali();
                break;
            case 2:
            {
                int capacitate;
                char facilitati[256];
                printf("\033[36mIntroduceti capacitatea minima: \033[0m");
                scanf("%d",&capacitate);
                while (getchar()!='\n');
                printf("\033[36mIntroduceti facilitati \033[0m(ex: WiFi,proiector,parcare,Scena,lumini,aer conditionat,sunet profesional): ");
                fgets(facilitati,256,stdin);
                facilitati[strcspn(facilitati,"\n")]=0;
                cautaSala(capacitate,facilitati);
                break;
            }
            case 3:
            {
                int id_sala;
                char nume[50],perioada[20];
                printf("\033[36mIntroduceti ID-ul salii: \033[0m");
                scanf("%d",&id_sala);
                while(getchar()!='\n');
                printf("\033[36mIntroduceti numele clientului: \033[0m");
                fgets(nume,50,stdin);
                nume[strcspn(nume, "\n")]=0;
                printf("\033[36mIntroduceti perioada \033[0m(ex:06.07.2025):");
                fgets(perioada,20,stdin);
                perioada[strcspn(perioada,"\n")]=0;
                rezervaSala(id_sala,nume,perioada);
                salveazaRezervari();
                break;
            }
            case 4:
                afiseazaRezervari();
                break;
            case 5:
            {
                char nume[50],perioada[20];
                printf("\033[36mIntroduceti numele clientului pentru anulare: \033[0m");
                fgets(nume,50,stdin);
                nume[strcspn(nume,"\n")]=0;
                printf("\033[36mIntroduceti perioada rezervarii \033[0m(ex:06.07.2025):");
                fgets(perioada,20,stdin);
                perioada[strcspn(perioada,"\n")]=0;
                anuleazaRezervare(nume,perioada);
                salveazaRezervari();
                break;
            }
            case 6:
                salveazaRezervari();
                break;
        }
        if(optiune>6){
            printf("\033[31mOptiune invalida.\033[0m Apasa Enter pentru a continua...");
            while(getchar()!='\n');
        }
        else
        if(optiune!=6){
            printf("\033[31m\nApasa Enter pentru a continua...\033[0m");
            while(getchar()!='\n');}
    } while(optiune!=6);
    free(sali);
    free(rezervari);
    return 0;
}
