#include <stdio.h>

/* Para declarar en el main hace asi: int mat[m][n]; (m y n previamente definidos), para llamar a la funcion 
   crearMAT(&mat[0][0],m,n);
   .
   CADA VEZ QUE LLAMES A LA FUNCION PONES EL NOMBRE Y DENTRO DE LOS PARENTESIS ASI:
   (&mat[0][0],m,n) M -> CANT FILAS N -> CANT DE COLUMNAS
*/
int *crearMAT(int *mat,int m ,int n){
    int i, j;
    for(i = 0; i < m;i++){
        for(j = 0; j < n; j++){
            *(mat+i*n+j) = 0;
        }
    }    

    return mat;
}

// Lo mismo de arriba
void cargarMAT(int *mat,int m ,int n){
    int i, j;
    for(i = 0; i < m;i++){
        for(j = 0; j < n; j++){
        int temp;
        printf("Ingrese el elemento [%d][%d] = ",i,j);
        scanf("%d",&temp);
        *(mat+i*n+j) = temp; 

        }
    }    
}
// Lo mismo de arriba
void imprimirmatrizv1(int *mat,int m, int n){
    for (int i = 0; i<m ; i++)
    {

        for (int j = 0; j<n; j++)
        {
                printf("[%d]\t", *(mat+i*n+j));
        }
    printf("\n");
    }

}

// Declarar en el main un puntero igual que el primero
void imprimirmatrizv2(int mat[],int m, int n){
    for (int i = 0; i<m ; i++)
    {

        for (int j = 0; j<n; j++)
        {
            printf("%d,", mat[i*n+j]);
        }
    printf("\n");
    }

}
char *matrizCadena(int mat[],int m, int n){
    static char final[100]="[";
    for (int i = 0; i<m ; i++)
    {
        for (int j = 0; j<n; j++)
        {
            char temp[10];
           // printf("%d,", mat[i*n+j]);
            sprintf(temp,"%d",mat[i*n+j]);
            strcat(final,temp);
            if (i <= 8){
                strcat(final,",");
            }else if (i == 9){
                if (j<n-1){
                    strcat(final,",");
                }
            }
        }
    }
    strcat(final,"]");
    return final;

}



