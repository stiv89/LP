#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funciones.h"

//Encabezado
int menuprincipal();
int menuparametros();
int submenuparametros();
int empezarpartida();
void menuautores();
void menuayuda();


int cambio = 1;//Modo de juego, cambio = 1 -> Somos servidor, 2-> Somos cliente
//Teniendo en cuenta la variable cambio estas dos van cambiando

int modojuego = 2; // 1 Local , 0 Visitante, 2 sin configurar
int jugador   = 2; // 2 No configurado, 3 -> 1(Jugador 1), 4-> 2(Jugador 2)

int empezarpartida(){

    if (cambio == 1){ //Somos servidor
        modojuego=1;//Local
        jugador=3;// Jugador 1
    }else if(cambio == 2){//Somos cliente
        modojuego=0;// Visitante
        jugador=4;//Jugador 2
    }
    
    proceso(cambio); // LLamamos a la funcion donde se procedera a conectar o crear el socket dependiendo de la varuable cambio para saber si somos clientes o servidores
}

void menuayuda(){
    printf("Pasos para el uso del programa: ");
    printf("1- bla bla bla");
}
void menuautores(){
    printf("\n\tEsteban Manuel Jara Noceda\t\n\n\n");
    printf("\n\tAriel\t\n\n\n"); // Completa tu nombre aca dp
}
int menuprincipal(){
    int opcion = 0; // Opcion elegida
    do{
        printf("\n \t MENU DEL PROGRAMA\n");
        printf("Opciones de navegacion:\n");
        printf("1. Empezar partida\n");
        printf("2. Configurar parametros\n");
        printf("3. Autores\n");
        printf("4. Ayuda\n");
        printf("5. Salir\n\n");
        printf("Elegir una opcion (1-5) >>:");
        scanf("%d",&opcion);
        if (opcion < 1 || opcion > 5)
        {
            printf("Opcion invalida, intentelo de nuevo!\n\n\n");
        }
    }
    while ((opcion < 1) || (opcion > 5)); 
    
    return opcion;
}
int submenuparametros(){
    int opcion = 0; // Opcion elegida
    do{
        printf("\n \t Configurar parametros\n");
        printf("Opciones de navegacion:\n");
        printf("1. Modo local\n");
        printf("2. Modo visita\n");
        printf("3. Directorio de archivos\n");
        printf("4. Ir atras\n\n");
        printf("Elegir una opcion (1-5) >>:");
        scanf("%d",&opcion);
        if (opcion < 1 || opcion > 5)
        {
            printf("Opcion invalida, intentelo de nuevo!\n\n\n");
        }
    }while(opcion < 1 || opcion > 4);

    return opcion;

}
int menuparametros(){
    int opcion = 0;
    do{
        opcion = submenuparametros();
        switch (opcion)
        {
            case 1: // Si elegimos primero empezar partida al incio entonces somos servidor
            {
                cambio = 1;
                modojuego = 1;
                jugador = 3;
            }
            break;
            case 2:
            {
                cambio = 2;
                modojuego = 0;
                jugador = 4;
            }
            break;
            case 3:
            {
                char direccion[50];
                opcion = 3;
                printf("Cual es el directorio del archivos de salida?>: ");
                scanf("%s",direccion);
            }
            break;
            case 4:
            {
                opcion = 4;
                printf("Adios\n");
                
            }
            break;


        }
    }while (opcion != 4);

    return opcion;
    
}
int main(){
    int opcion = 0;
    char modo[][50]={"Cliente","Servidor","No configurado","1","2"}; //Lista de switches para el Menu principal donde vamos a ir alternando de acuerdo a los parametros elegidos

    while(opcion != 5)
    {  
        printf("ID GRUPO:2\t\t\tJugador: %s\nModo activo: %s\n",modo[jugador],modo[modojuego]);
        opcion = menuprincipal();
        switch (opcion)
        {//Opciones
        case 1:
            empezarpartida();
            break;
        case 2:
            menuparametros();            
            break;
        case 3:
            menuautores();
            opcion=3;
            break;
        case 4:
            opcion=4;
            break;
        case 5:
            opcion=5;
            break;
        default:
            break;
        }
    }
    return 0;
}