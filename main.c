#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <string.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

struct programa {
    int id;
    char contenido[50];
};

void mostrarPrograma(char nombreProg[]);
void printMenu();
void agrProg(char nombProg[]);
int irPos(char nomProg[], int id);
void delProg(char nomProg[]);
void cargarSistema(char nomProg[]);

int main(){
    FILE *f;
    int i,opc,id;
    char nomProg[30];
    char input[50];
    int mFlag = 0;

    system("clear");
    printf("Cual es el nombre del programa de memoria? : ");
    scanf("%s",nomProg);

    if( access( nomProg, F_OK ) != -1 ) {
        // file exists
        if((f=fopen(nomProg,"rb"))==NULL){
            printf("Error al abrir el programa!\n");
            exit(0);
        }
        printf("\nPrograma encontrado, prosiguiendo a cargar el sistema operativo!\n\n");
        printf("Ingresa una letra y luego presiona enter...\n");
        scanf("%s", input);
        fclose(f);
        cargarSistema(nomProg);
    } else {
        // file doesn't exist
        struct programa prog;
        printf("\n\nLa memoria no existe! Se procedera a crear el esqueleto del programa!\n\n");
        if((f=fopen(nomProg,"wb"))==NULL){
            printf("Error al abrir el programa!\n");
            exit(0);
        }
        printf("Ingresa una letra y luego presiona enter...\n");
        scanf("%s",input);
        prog.id =0;
        strcpy(prog.contenido,"");
        for(i=0;i<5;i++){
            fwrite(&prog,sizeof(prog),1,f);
        }
        fclose(f);
        cargarSistema(nomProg);
    }

    while(mFlag!=1){
        printMenu();
        scanf("%d",&opc);

        switch(opc)
        {
            case 1:
                system("clear");
                agrProg(nomProg);
                break;

            case 2:
                system("clear");
                printf("Ingresar el id del programa: ");
                scanf("%d",&id);
                irPos(nomProg,id);
                break;
                
            case 3:
                system("clear");
                delProg(nomProg);
                break;

            case 4:
                system("clear");
                mostrarPrograma(nomProg);
                printf("Ingresa una letra y luego presiona enter...\n");
                scanf("%s",input);
                break;
            case 5:
                system("clear");
                printf(KNRM"");
                mFlag = 1;
                break;
            default:
                system("clear");
                printf("Error: opcion no valida! \n\n");
                printf("Ingresa una letra y luego presiona enter...\n");
                scanf("%s",input);
                break;
        }
    }
    printf("Ingresa una letra y luego presiona enter...\n");
    scanf("%s",input);
    return 0;
}

void mostrarPrograma(char nombreProg[]){
    FILE *f1;
    struct programa curProg;
    int pos = 0;

    if((f1=fopen(nombreProg,"rb"))==NULL){
        printf("Error al abrir el programa!\n");
        exit(0);
    }

    printf(KGRN"----------------------------------------------------------------------------------------------------\n");
    printf("|IdPrograma|                                                        Contenido | Posicion en Memoria|\n");
    printf("----------------------------------------------------------------------------------------------------\n");
    while(!feof(f1)){
        fread(&curProg,sizeof(curProg),1,f1);
        if(feof(f1))
            break;
        printf("|");
        printf(KBLU"%10d",curProg.id);
        printf(KGRN"|");
        printf(KCYN"%66s",curProg.contenido);
        printf(KGRN"|");
        printf(KRED"%20d",pos);
        printf(KGRN"|");
        printf("\n");
        pos++;
    }
    printf(KGRN"----------------------------------------------------------------------------------------------------\n");
    printf(KNRM"");
    fclose(f1);
}

void agrProg(char nombProg[]) {
    
    int pos = 1;
    int cont = 1;
    char input[50];
    char linea[50];
    char nomProg[30];
    FILE *f1, *f2;
    struct programa prog1, prog2;

    if ((f1 = fopen(nombProg, "rb+")) == NULL) {
        printf("Error al abrir el programa!\n");
        exit(0);
    }

    //Pproguntar nombre del programa y cargarlo
    printf("\n");
    printf("Escribe el nombre del programa: ");
    scanf("%s", nomProg);
    printf("\n");
    if (access(nomProg, F_OK) != -1) {
        // file exists
        if ((f2 = fopen(nomProg, "rt")) == NULL) {
            printf("Error al abrir el programa!\n");
            exit(0);
        }
        printf("\nPrograma encontrado!\n\n");

        //Cargar a memoria el contenido
        while(fgets(linea,50,f2)) {
            printf("Contenido del programa: %s", linea);
            printf("\n");
            printf("\n");
        }
        strcpy(prog1.contenido,linea);

        //Ir a la posicion
        fseek(f1, (sizeof(prog1) * (pos - 1)), SEEK_SET);

        //Leer para saber si esta ocupado el programa
        fread(&prog2, sizeof(prog2), 1, f1);

        //Pproguntar si proga ocupado el lugar
        if (prog2.id > 0) {
            printf("Lugar ocupado!\n\n");
            while (!feof(f1)) {
                //Avanzar el apuntador y leer
                fread(&prog2, sizeof(prog2), 1, f1);
                cont += 1;
                //empezar a leer 1 por uno
                if (feof(f1)) {
                    rewind(f1);
                    fread(&prog2, sizeof(prog2), 1, f1);
                }
                if (prog2.id == 0) {
                    fseek(f1, (sizeof(prog1) * (-1)), SEEK_CUR);
                    prog1.id = cont;
                    fwrite(&prog1, sizeof(prog1), 1, f1);
                    printf("Programass ingresados correctamente!\n\n");
                    break;
                }
                if (cont == 5) {
                    printf("No hay mas espacio para agprogar el programa!");
                    break;
                }
            }
        }
        else{
            fseek(f1,(sizeof(prog1)*(pos-1)),SEEK_SET);
            prog1.id = cont;
            fwrite(&prog1,sizeof(prog1),1,f1);
            printf("Programas ingresados correctamente!\n\n");
            fclose(f2);
        }

    }
        
    fclose(f1);
    printf("Ingresa una letra y luego presiona enter...\n");
    scanf("%s",input);
}

int irPos(char nomProg[], int id){

    FILE *f2;
    int cont = 0;
    struct programa programa;
    char input[50];

    if((f2=fopen(nomProg,"rb"))==NULL){
        printf("Error al abrir el programa!\n");
        exit(0);
    }

    while(!feof(f2)){
        fread(&programa,sizeof(programa),1,f2);
        cont+=1;
        if(feof(f2)){
            printf("programa no encontrado!\n");
            cont = 0;
            break;
        }
        if(programa.id == id){
            printf("\nprograma encontrado!: \n\n");
            printf("Descripcion: %s \n",programa.contenido);
            printf("id: %d \n",programa.id);
            break;
        }
    }

    fclose(f2);
    printf("Ingresa una letra y luego presiona enter...\n");
    scanf("%s",input);
    return cont;
}


void delProg(char nomProg[]){
    FILE *f4;
    int id2;
    struct programa programa1;
    char input[50];

    printf("Ingresar el id del programa que desea eliminar: ");
    scanf("%d",&id2);
    int pos1 = irPos(nomProg,id2);

    if(pos1 != 0){
        if((f4=fopen(nomProg,"rb+"))==NULL){
            printf("Error al abrir el programa!\n");
            exit(0);
        }
        programa1.id =0;
        strcpy(programa1.contenido,"");

        fseek(f4,(sizeof(programa1)*(pos1-1)),SEEK_SET);
        fwrite(&programa1,sizeof(programa1),1,f4);
        printf("Programa eliminado correctamente!\n\n");
        fclose(f4);
    }

    printf("Ingresa una letra y luego presiona enter...\n");
    scanf("%s",input);
}

void cargarSistema(char nomProg[]){

    system("clear");

    FILE *f,*f2;
    char input[50],linea[50];
    struct programa prog1;

    if((f=fopen(nomProg,"rb+"))==NULL){
        printf("Error al abrir el programa!\n");
        exit(1);
    }

    if (access("sistema_operativo.txt", F_OK) != -1) {
        // file exists
        if ((f2 = fopen("sistema_operativo.txt", "rt")) == NULL) {
            printf("Error al abrir el programa!\n");
            exit(1);
        }
        printf("\nSistema operativo encontrado!\n\n");

        while (fgets(linea, 50, f2)) {
            printf("Nombre del Sistema Operativo: %s", linea);
            printf("\n");
            printf("\n");
        }

        strcpy(prog1.contenido, linea);
        fseek(f,(sizeof(prog1)*(0)),SEEK_SET);
        prog1.id = 1;
        fwrite(&prog1,sizeof(prog1),1,f);
        printf("Sistema operativo cargado exitosamente!\n\n");
        fclose(f2);
        fclose(f);
        printf("Ingresa una letra y luego presiona enter...\n");
        scanf("%s",input);

    } else{
        printf("Error al cargar el sistema opertaivo!");
        exit(1);
    }
}

void printMenu(){
    system("clear");
    printf("Favor de elegir una opcion: \n\n");
    printf("1. Cargar programas a memoria\n");
    printf("2. Consultar un programa\n");
    printf("3. Eliminar un programa de la memoria\n");
    printf("4. Mostrar todos los programas de la memoria\n");
    printf("5. Salir del programa\n\n");
}
