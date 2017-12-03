/*	
	Sistemas Concurrentes
	Alumno: Jesus R. Gonzalez L. 
	Cedula:23.543.415 
	Busqueda de un numero aleatorio en un bloque de 1.000.000 de numeros desordenados y ninguno se repite
	Aplicando OpenMPI Scatter
	Determinar Tiempo: Busqueda Secuencial y Con OpenMPI

	Para Compilar:jesus@laptop:~/Escritorio$ mpic++ busqueda.cpp -o busqueda.out
	Para Ejecutar:jesus@laptop:~/Escritorio$ mpirun -np 2 busqueda.out
	
*/






//Includes
#include <stdio.h>
#include "mpi.h" //OpenMPI
#include <iostream>     
#include <algorithm>    
#include <vector>       
#include <ctime>        
#include <cstdlib>      
#include <fstream>	
#include <string>
#include <time.h>

//Declaracion de variables de Tamaño y de envio por proceso
#define send 1000000 //Bloque root
#define recv 500000  //Matriz Local


using namespace std;

int main( int argc, char **argv )
{
    //Variables isend y irecv, root y local
    int isend[send]; int irecv[recv];int opc=0;
    int rank, size, i,leer_archivo,num_ramdon=0;
    vector<int> myvector2;vector<int> myvector;
	
	
	//Generar ramdon
    srand(time(NULL));
    num_ramdon=rand()%1000000;
	
	//Iniciar OpenMPI
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );

	//HILO MAESTRO
    if(rank == 0)
    {
	
	/*
	//Crear el Archivo con 1 millon de numeros, si tengo el archivo comentar
	ofstream file("entrada.txt");
	if(!file)
	{
		cout << "Error no se puede abrir el archivo: " <<endl;
                return 1;
	}
		//LLenado de Numeros al vector
	 for (int i=0; i<1000000; ++i) {myvector.push_back(i);}
	 	//Desordenar el Vector 
  	 random_shuffle ( myvector.begin(), myvector.end() );
 	
		//LLenar Archivo de Numeros con el vector
	for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
	{
  

   	  file<<*it<<'\n';
    
        
  	}
  
       file.close();
	*/
     

      //LECTURA DEL ARCHIVO DE NUMEROS
      ifstream file1("entrada.txt");
        if(!file1)
        {
           cout << "Error no se puede abrir el archivo: " << file1;
           return 1;
        }
  	//Lectura de archivo y guardado en un vector
       while (file1 >> leer_archivo)
       {
    
         myvector2.push_back(leer_archivo); 

       }

      file1.close();

	//Busqueda Secuencial y Calcular Tiempo
	  clock_t begin1 = clock();
    for(vector<int>::iterator it=myvector2.begin(); it!=myvector2.end(); ++it)
    {		
       if(num_ramdon==myvector2[*it])
         {
            cout<<"el numero a Buscar "<<num_ramdon<<endl;
	    cout<<" se encuentra en la posicion: "<<" ["<<*it<<"] "<<endl; 
            break;                   
          }
  
     }
       clock_t end1 = clock();
       double time_spent1 = (double)(end1 - begin1) / CLOCKS_PER_SEC;
       cout<<"TIEMPO: Segundos Secuencial "<<time_spent1<<endl;


        for(int j=0;j<send;j++) {isend[j] = myvector2[j];}
	//Descripcion de Scatter
	cout<<"Procesos: "<<size<<endl;
	cout<<"distribuidos entre "<<send/size<<endl;



    //Cierre del hilo Maestro


    }
	 
      
	
     //Scatter 
     //&isend Dirección inicial del buffer de salida
     //Send/size divide la carga entre procesos y significa el Número de elementos que se envía a cada proceso
     //Tipo de dato que se va a enviar MPI_INT
     //&irecv Direción del buffer de recepción 
     //Send/size Número de elementos que espera recibir cada proceso
     //MPI_INT Tipo de datos de los elementos a recibir
     //rank del proceso raíz el que realizará el envío
     //Comunicador por el que realizar la comunicación.
	
     MPI_Scatter(&isend, send/size, MPI_INT, &irecv, send/size, MPI_INT, 0, MPI_COMM_WORLD);

 	 /* Busqueda de un numero aleatorio En distintos procesos al mismo tiempo*/

      clock_t begin = clock();
        for (i = 0; i < (send / size); i++) 
         {
    		if(irecv[i]==num_ramdon)
		{

			
			cout<<"Proceso que lo encontro "<<rank<<endl;
			
			
			clock_t end = clock();
       			double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
			
      		        cout<<"TIEMPO: Concurrentes "<<time_spent<<" Segundos "<<endl;
			
			break;
			
		}
         }


	
	myvector.clear();
	myvector2.clear();
    MPI_Finalize();
    return 0;
}
