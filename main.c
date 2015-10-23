#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <string.h>

struct registro {
    int id;
    char descripcion[30];
};

void mostrarRegistro(char nombreArch[]);
void printMenu();
int funcTrans(int matr);
void agrReg(char nombArch[]);
int irPos(char nomArch[], int matr);
void modReg(char nomArch[]);
void delReg(char nomArch[]);

int main(){
    FILE *f;
    int i,opc,matr;
    char nomArch[30];
    char input[50];
    int mFlag = 0;

    printf("Cual es el nombre del archivo?: ");
    scanf("%s",nomArch);

    if( access( nomArch, F_OK ) != -1 ) {
        // file exists
        if((f=fopen(nomArch,"rb"))==NULL){
            printf("Error al abrir el archivo!\n");
            exit(0);
        }
        printf("\nEl archivo existe!\n\n");
        printf("Ingresa una letra y luego presiona enter...\n");
        scanf("%s", input);
        fclose(f);
    } else {
        // file doesn't exist
        struct registro est;
        printf("\n\nArchivo no existe! Se procedera a crear el esqueleto del archivo!\n\n");
        if((f=fopen(nomArch,"wb"))==NULL){
            printf("Error al abrir el archivo!\n");
            exit(0);
        }
        printf("Ingresa una letra y luego presiona enter...\n");
        scanf("%s",input);
        est.id =0;
        strcpy(est.descripcion,"");
        for(i=0;i<100;i++){
            fwrite(&est,sizeof(est),1,f);
        }
        fclose(f);
    }

    while(mFlag!=1){
        printMenu();
        scanf("%d",&opc);

        switch(opc)
        {
            case 1:
                system("clear");
                agrReg(nomArch);
                break;

            case 2:
                system("clear");
                printf("Ingresar el id del registro: ");
                scanf("%d",&matr);
                irPos(nomArch,matr);
                break;

            case 3:
                system("clear");
                modReg(nomArch);
                break;
            case 4:
                system("clear");
                delReg(nomArch);
                break;

            case 5:
                system("clear");
                mostrarRegistro(nomArch);
                printf("Ingresa una letra y luego presiona enter...\n");
                scanf("%s",input);
                break;
            case 6:
                system("clear");
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

void mostrarRegistro(char nombreArch[]){
    FILE *f1;
    struct registro curReg;
    int pos = 0;

    if((f1=fopen(nombreArch,"rb"))==NULL){
        printf("Error al abrir el archivo!\n");
        exit(0);
    }

    while(!feof(f1)){
        fread(&curReg,sizeof(curReg),1,f1);
        if(feof(f1))
            break;
        printf("id: %d ",curReg.id);
        printf("Descripcion: %s ",curReg.descripcion);
        printf("Posicion en memoria: %d",pos);
        printf("\n");
        pos++;
    }
    fclose(f1);
}

void agrReg(char nombArch[]){
    int n,i,pos;
    int cont =1;
    char input[50];
    FILE *f1;
    struct registro est1,est2;

    printf("Cuantos registros quieres ingresar?\n");
    scanf("%d",&n);

    if((f1=fopen(nombArch,"rb+"))==NULL){
        printf("Error al abrir el archivo!\n");
        exit(0);
    }

    for(i=1;i<=n;i++){
        printf("Escribe el nombre del registro %d: ",i);
        scanf("%s",est1.descripcion);
        printf("\n");
        printf("Escribe el id: ");
        scanf("%d",&est1.id);
        printf("\n");
        //Aplicar func de transformacion
        pos = funcTrans(est1.id);
        //Ir a la posicion
        fseek(f1,(sizeof(est1)*(pos-1)),SEEK_SET);
        //Leer para saber si esta ocupado el archivo
        fread(&est2,sizeof(est2),1,f1);
        //Preguntar si esta ocupado el lugar
        if(est2.id >0){
            printf("Lugar ocupado!\n\n");
            while(!feof(f1)){
                //Avanzar el apuntador y leer
                fread(&est2,sizeof(est2),1,f1);
                cont+=1;
                //empezar a leer 1 por uno
                if(feof(f1)){
                    rewind(f1);
                    fread(&est2,sizeof(est2),1,f1);
                }
                if(est2.id ==0){
                    fseek(f1,(sizeof(est1)*(-1)),SEEK_CUR);
                    fwrite(&est1,sizeof(est1),1,f1);
                    printf("Archivos ingresados correctamente!\n\n");
                    break;
                }
                if(cont==100){
                    printf("No hay mas espacio para agregar el registro!");
                    break;
                }
            }
        }

        else{
            fseek(f1,(sizeof(est1)*(pos-1)),SEEK_SET);
            fwrite(&est1,sizeof(est1),1,f1);
            printf("Archivos ingresados correctamente!\n\n");
        }

    }
    fclose(f1);
    printf("Ingresa una letra y luego presiona enter...\n");
    scanf("%s",input);
}

int irPos(char nomArch[], int matr){

    FILE *f2;
    int cont = 0;
    struct registro registro;
    char input[50];

    if((f2=fopen(nomArch,"rb"))==NULL){
        printf("Error al abrir el archivo!\n");
        exit(0);
    }

    while(!feof(f2)){
        fread(&registro,sizeof(registro),1,f2);
        cont+=1;
        if(feof(f2)){
            printf("registro no encontrado!\n");
            cont = 0;
            break;
        }
        if(registro.id == matr){
            printf("\nregistro encontrado!: \n\n");
            printf("Descripcion: %s \n",registro.descripcion);
            printf("id: %d \n",registro.id);
            break;
        }
    }

    fclose(f2);
    printf("Ingresa una letra y luego presiona enter...\n");
    scanf("%s",input);
    return cont;
}

void modReg(char nomArch[]){
    FILE *f3;
    int matr1;
    struct registro estu;
    char input[50];

    printf("Ingresar el id del registro que desea modificar: ");
    scanf("%d",&matr1);
    int pos1 = irPos(nomArch,matr1);

    if(pos1 != 0){

        if((f3=fopen(nomArch,"rb+"))==NULL){
            printf("Error al abrir el archivo!\n");
            exit(0);
        }

        printf("Escribe el id: ");
        scanf("%d",&estu.id);
        printf("\n");
        printf("Escribe la descripcion del registro: ");
        scanf("%s",estu.descripcion);
        printf("\n");

        fseek(f3,(sizeof(estu)*(pos1-1)),SEEK_SET);
        fwrite(&estu,sizeof(estu),1,f3);
        printf("Registro modificado correctamente!\n\n");
        fclose(f3);

    }
    printf("Ingresa una letra y luego presiona enter...\n");
    scanf("%s",input);
}

void delReg(char nomArch[]){
    FILE *f4;
    int matr2;
    struct registro registroi;
    char input[50];

    printf("Ingresar el id del registro que desea eliminar: ");
    scanf("%d",&matr2);
    int pos1 = irPos(nomArch,matr2);

    if(pos1 != 0){
        if((f4=fopen(nomArch,"rb+"))==NULL){
            printf("Error al abrir el archivo!\n");
            exit(0);
        }
        registroi.id =0;
        strcpy(registroi.descripcion,"");

        fseek(f4,(sizeof(registroi)*(pos1-1)),SEEK_SET);
        fwrite(&registroi,sizeof(registroi),1,f4);
        printf("Registro eliminado correctamente!\n\n");
        fclose(f4);
    }

    printf("Ingresa una letra y luego presiona enter...\n");
    scanf("%s",input);
}

int funcTrans(int matr){
    int pos;
    pos = matr%100;
    return pos;
}

void printMenu(){
    system("clear");
    printf("Favor de elegir una opcion: \n\n");
    printf("1. Cargar registros a memoria\n");
    printf("2. Consultar un registro\n");
    printf("3. Modificar un registro\n");
    printf("4. Eliminar un registro de la memoria\n");
    printf("5. Mostrar todos los registros de la memoria\n");
    printf("6. Salir del programa\n\n");
}
