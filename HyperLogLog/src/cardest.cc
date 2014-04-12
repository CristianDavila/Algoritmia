#include <iostream>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
using namespace std;

// Durante todo el algoritmo trabajaremos con numeros naturales
typedef unsigned char u_char;								
typedef unsigned int u_int;

// Constante que se aplica varias veces a la hora de realizar la estimacion
static const double pow_2_32 = 4294967296.0;				

// Numero primo mas grande de 32 bits
u_int p = 4294967291;			
// Tabla para guardar el numero de "leading zeros"
vector<u_char> M;		
// Vector para guardar los numeros aleatorios generados para la funcion de hash
vector<u_int> a;											

/*
 * La funcion initialization se encarga de inicializar las variables 
 * necesarias para el algoritmo. Concretamente, realiza cuatro 
 * inicializaciones diferentes:
 * 		1. Inicializa la variable alpha segun el valor de m
 *		2. Inicializa las dos tablas con las posiciones correspondientes.
 *		3. Inicializa la variable b con el numero de bits necesario para 
 * 		   codificar el total de posiciones de la tabla M.
 *		4. Genera 30 numeros aleatorios que son guardados en el vector 
 * 		   a para su utilizacion en la funcion de hash. 
 */
void initialization(int m, double &alpha, int &b) {
    // Inicializacion de alpha
	switch(m) {												
		case 16:
			alpha = 0.673;
			break;
		case 32:
			alpha = 0.697;
			break;
		case 64:
			alpha = 0.709;
			break;
		default:
			alpha = 0.7213/(1.0 + 1.079/m);
    }
    
    // Inicializacion de M
    M = vector<u_char> (m, 0);								
	
	// Inicializacion de a
	a = vector<u_int> (30);									
	
	// Inicializacion de b
	b = 0;													
	while(m > 1) {
		b++;
		m = m >> 1;
	}
	
	// La semilla de la funcion random se obtiene a partir del tiempo del procesador, 
	// asi obtenemos numeros aleatorios diferentes por cada ejecucion
	srand(time(NULL));										
	
	// Generacion de los 30 numeros aleatorios (cualquier string de la entrada 
	// tiene un tamaño como maximo de 30 caracteres)
	for(int i = 0; i < 30; i++) {							
		a[i] = rand();
	}
}

/*
 * La funcion universal_hash recibe como parametro un string y devuelve un hashvalue 
 * de 32 bits. Para que la funcion de hash sea universal se utiliza un vector que 
 * contiene 30 numeros generados aleatoriamente, uno por cada caracter posible que puede
 * tener el string recibido por parametro. Ademas, por este motivo, tambien se utiliza 
 * un numero primo a la hora de realizar el modulo sobre el hashvalue. De esta manera, 
 * se consigue una probabilidad de colision de 1/m, que es considerablemente pequeña.
 */
u_int universal_hash(string key) {
    u_int hash_value = 0;
    
	// Se calcula el hashvalue a partir de la suma de las multiplicaciones entre 
	// las mismas posiciones de los vectores a y key
    for(int i = 0; i < key.size(); i++)					
        hash_value += ((u_int) key[i])*a[i];
	
	// Se le aplica al hashvalue el modulo del numero primo mas grande de 32 bits
	hash_value = hash_value % p;							
    
    return hash_value;
}

/*
 * La funcion leading_zeros recibe como parametro un valor de 32 bits y la posicion 
 * inicial desde donde hacer la comprobacion. Por lo tanto, a partir de estos elementos,
 * la funcion devuelve la posicion del primer bit a 1 cogiendo como posicion inicial 
 * la recibida por parametro.
 */
u_char leading_zeros(u_int w, int b) {
	// La variable zeros se inicializa con 1 porque hay que devolver 
	// la posicion del primer bit a 1.
	u_char zeros = 1;										
	int i = 31-b;
	
	bool finish = false;
	while(i >= 0 && !finish) {	
		if((w >> i) & 1)	
			// Si el bit a comprobar esta a 1, se finalizara el bucle
			finish = true;
		else
			// En caso contrario se ha hayado un cero mas, por lo tanto, se suma uno a la variable
			zeros++;										
		
		i--;
	}
	
	return zeros;
}

/*
 * La funcion aggregation se encarga de la insercion de los elementos. Por cada elemento, 
 * se efectuan las siguientes acciones:
 *		1. Se le aplica la funcion de hash al elemento para obtener el hashvalue.
 *		2. A partir del hashvalue, se obtiene la posicion del elemento en la tabla 
 * 		   utilizando los primeros b bits de este.
 *		3. Seguidamente, con el resto de bits se le aplica la funcion leading_zeros 
 * 		   para obtener la posicion del primer bit a 1.
 *		4. A partir del resultado anterior, si este es mayor que el que esta guardado 
 * 		   en su posicion de la tabla, se sobrescribe dicha posicion.
 *		5. Se suma 1 al contador que cuenta el total de palabras del conjunto.	
 */
u_int aggregation(int b) {
	// Inicializacion de las variables
	u_int x, idx, w, count; 									
	string word;
	
	count = 0;
	
	// Por cada linea de la entrada se obtiene un nuevo elemento
    while(getline(cin, word)) {			
		// Aplicacion de la funcion de hash al elemento
        x = universal_hash(word);		
		// Obtencion de la posicion en la tabla M
		idx = x >> (32 - b);									
		
		// Aplicacion de la funcion leading_zeros
		u_char lz = leading_zeros(x, b);						
						
		if(M[idx] < lz) {	
			// Si el numero de leading_zeros es mayor que el valor guardado, se sobrescribe
			M[idx] = lz;
		}
		
		// Se cuenta una palabra mas al total de palabras
		count++;												
    }
    
    return count;
}

/*
 * La funcion linear_counting 
 */
double linear_counting(int m, int v) {
	return (m * log((double) m/(double) v));
}

/*
 * La funcion computation se encarga de realizar la estimacion de la 
 * cardinalidad a partir de los valores guardados en la tabla M. Ademas,
 * una vez calculada la estimacion, si esta se encuentra en un rango 
 * muy pequeño o, al contrario, se encuentra en un rango muy grande,se efectua
 * una correccion de esta estimacion.
 */
double computation(int m, double alpha) {
	double E;
	
	// Calculo de un componente de la estimacion
	double sum = 0.0;
	for(int i = 0; i < m; i++) {							
		sum += 1.0/pow(2.0, M[i]);
	}
	
	// Calculo de la estimacion
	E = (alpha*m*m)/sum;									
			
	if(E <= (5*m)/2) {					// Small range correction
		// Se cuentan el numero de registros de la tabla M que sean iguales a 0,
		// es decir, que no han sido modificados
		int v = 0;
		for(int i = 0; i < m; i++) {
			if(M[i] == 0)
				v++;										
		}
				
		if(v > 0) {						
			// Si existe algun registro a 0 en la tabla M, 
			// se utiliza la estimacion de la funcion linear_counting
			return linear_counting(m, v);					
		} else {											
			// En caso contrario, se utiliza la estimacion inicial
			return E;
		}
	} else if(E <= (pow_2_32/30.0)) {	// Intermediate range (no correction)
		return E;
	} else {							// Large range correction
		return ((-pow_2_32) * log(1 - E/pow_2_32));
	}
}

int main(int argc, char * argv[]) {
	// Inicializacion de m, con un valor por defecto de 256
    int m = 256;											
	int b;
	double alpha;
    
	// Comprobacion del flag -M
    if(argc == 3) {											
        if(strcmp(argv[1], "-M") == 0) {
			// Si se ha utilizado el flag, la variable m obtiene el valor 
			// que ha sido pasado por parametro
            m = atoi(argv[2]);								
        }
    }
    
    initialization(m, alpha, b);
    
	u_int N = aggregation(b);
	
	u_int E = computation(m, alpha);
	
	cout << E << " " << N << endl;
}