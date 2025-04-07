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
    printf("Sali disponibile:\n");
    int i;
    for(i=0;i<nr_sali;i++)
        printf("ID: %d, Capacitate: %d, Facilitati: %s, Pret: %.2f\n",sali[i].id,sali[i].capacitate,sali[i].facilitati,sali[i].pret);
}

void cautaSala(int capacitate, const char *facilitati)
{
    printf("Sali care corespund criteriilor:\n");
    int gasit=0;
    int i;
    for(i=0;i<nr_sali;i++)
        if (sali[i].capacitate>=capacitate && strstr(sali[i].facilitati,facilitati))
        {
            printf("ID: %d, Capacitate: %d, Facilitati: %s, Pret: %.2f\n",sali[i].id,sali[i].capacitate,sali[i].facilitati,sali[i].pret);
            gasit = 1;
        }
    if(!gasit)
        printf("Nu s-au gasit sali potrivite.\n");
}

void rezervaSala(int id_sala,const char *nume_client,const char *perioada)
{
    rezervari=realloc(rezervari,(nr_rezervari+1)*sizeof(rezervare));
    rezervari[nr_rezervari].id_sala=id_sala;
    strcpy(rezervari[nr_rezervari].nume_client,nume_client);
    strcpy(rezervari[nr_rezervari].perioada,perioada);
    nr_rezervari++;
    printf("Rezervare efectuata cu succes!\n");
}


void afiseazaRezervari()
{
    printf("Rezervari existente:\n");
    int i;
    for(i=0;i<nr_rezervari;i++)
        printf("ID Sala: %d, Client: %s, Perioada: %s\n",rezervari[i].id_sala,rezervari[i].nume_client,rezervari[i].perioada);
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
            printf("Rezervare anulata cu succes!\n");
            break;
        }
    }
    if (!gasit)
        printf("Rezervarea nu a fost gasita.\n");
}

int main()
{
    int optiune;
    incarcaSali();
    incarcaRezervari();
    do
    {
        system(CLEAR_SCREEN);
        printf("\nMeniu:\n");
        printf("1. Afiseaza sali\n");
        printf("2. Cauta sala\n");
        printf("3. Rezerva sala\n");
        printf("4. Afiseaza rezervari\n");
        printf("5. Anuleaza rezervare\n");
        printf("6. Iesire\n");
        printf("Alege o optiune: ");
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
                printf("Introduceti capacitatea minima: ");
                scanf("%d",&capacitate);
                while (getchar()!='\n');
                printf("Introduceti facilitati: ");
                fgets(facilitati,256,stdin);
                facilitati[strcspn(facilitati,"\n")]=0;
                cautaSala(capacitate,facilitati);
                break;
            }
            case 3:
            {
                int id_sala;
                char nume[50],perioada[20];
                printf("Introduceti ID-ul salii: ");
                scanf("%d",&id_sala);
                while(getchar()!='\n');
                printf("Introduceti numele clientului: ");
                fgets(nume,50,stdin);
                nume[strcspn(nume, "\n")]=0;
                printf("Introduceti perioada: ");
                fgets(perioada,20,stdin);
                perioada[strcspn(perioada,"\n")]=0;
                rezervaSala(id_sala,nume,perioada);
                break;
            }
            case 4:
                afiseazaRezervari();
                break;
            case 5:
            {
                char nume[50],perioada[20];
                printf("Introduceti numele clientului pentru anulare: ");
                fgets(nume,50,stdin);
                nume[strcspn(nume,"\n")]=0;
                printf("Introduceti perioada rezervarii: ");
                fgets(perioada,20,stdin);
                perioada[strcspn(perioada,"\n")]=0;
                anuleazaRezervare(nume,perioada);
                break;
            }
            case 6:
                salveazaRezervari();
                break;
        }
        if(optiune>6){
            printf("\nOptiune invalida. Apasa Enter pentru a continua...");
            while(getchar()!='\n');
        }
        else
        if(optiune!=6){
            printf("\nApasa Enter pentru a continua...");
            while(getchar()!='\n');}
    } while(optiune!=6);
    free(sali);
    free(rezervari);
    return 0;
}
