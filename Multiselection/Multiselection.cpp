#include <iostream>
#include <vector>

using namespace std;


template<typename T>
void lectura(vector<T> & v, int size){
    for(int i=0; i<size; ++i)
        cin >> v[i];
}

template<typename T>
void escritura(vector<int> & rangos, int p, vector<T> & input){
    for(int i=0; i<p; ++i){
        if(i != 0) cout << ' ';
        cout << input[rangos[i]-1];
    }
    cout << endl;
}


template<typename T>
void intercanvi(T & x, T & y){
    T aux = x;
    x = y;
    y = aux;
}


/*La funcion Partition se encarga de pseudo-ordenar el vector en torno a un pivote:
 *coloca todos los elementos que son menor o igual en la parte izquierda del vector,
 *y todos los mayores en la parte derecha.
*/
// Pre: 0 <= end
// Post: La función devuelve la posicion del k-ésimo elemento del vector.
template<typename T>
int Partition(vector<T> & array, int start, int end) {
    int left, right, pivot;

    pivot = array[start];
    left = start;
    right = end;

    // Mientras no se cruzen los índices
    while (left < right) {
        while (array[right] > pivot)    right--;

        while ((left < right) && (array[left] <= pivot))    left++;

        // Si todavía no se cruzan los indices seguimos intercambiando
        if (left < right)   intercanvi(array[left], array[right]);
    }

    // Los índices ya se han cruzado, ponemos el pivot en el lugar que le corresponde
    intercanvi(array[right], array[start]);

    return right;
}


/*La funcion cercaDicotomica hace una busqueda binaria en el vector de rangos. Si se encuentra un rango
 *tal que sea igual al elemento 'k', se devuelve, en caso contrario devuelve la posicion del rango
 *inmediato mas pequeño.
*/
int cercaDicotomica(int k, const vector<int> & ranks, int start, int end){

    if (start > end)
        return end; //Si no se encuentra en el vector, devolvemos la posicion del rango menor a k

    else{
        int middle = (end-start)/2 + start;
        if (k < ranks[middle]) return cercaDicotomica(k, ranks, start, middle-1);
        else if (k > ranks[middle]) return cercaDicotomica(k, ranks, middle+1, end);
        else return middle;
    }
}


/* Justificacion*/

/*La funcion multiselect, basada en Quicksort, divide el vector de rangos en dos mitades seleccionando
 *un pivote, a continuacion busca el pivote en el vector de rangos, y obtiene su posicion, o en caso de
 *no encontrarse en el vector de rangos, optiene la posicion del menor siguiente.
 *A continuacion, si el pivote 'k' se encontraba en el vector de rangos, hace dos llamadas a multiselect,
 *una con los rangos 0 a r-1 y los rangos r+1 a p - 1 (p = rangos.size()), pero si rangos[r] < k entonces
 *hace las llamadas recursivas con los rangos 0 a r y los rangos r+1 a rankEnd.
 *Si uno de los subvectores es vacio, el algoritmo no hace nada.
*/
// Pre: 1 <= ranks[0] < ranks[p-1] <= X.size()
// Post: Input contiene los elementos p-ésimos menores en sus p-esimas posiciones.
template<typename T>
void multiselect(vector<T> & input, vector<int> & ranks, int inputStart, int inputEnd, int rankStart, int rankEnd) {

    if (inputStart <= inputEnd and rankStart <= rankEnd){

        int k = Partition(input, inputStart, inputEnd);             //Obtenemos el pivote.
        if(ranks.size() == 1) return;                               //Hemos acabado si solo hay un rango.
        int r = cercaDicotomica(k, ranks, rankStart, rankEnd+1);    //Obtenemos la posicion del pivote en 'ranks'

        if(ranks[r] == k){
            multiselect(input, ranks, inputStart, k-1, rankStart, r-1);
            multiselect(input, ranks, k+1, inputEnd, r+1, rankEnd);
        }
        else if(ranks[r] < k){
            multiselect(input, ranks, inputStart, k, rankStart, r);
            multiselect(input, ranks, k+1, inputEnd, r+1, rankEnd);
        }
    }
}



int main(){

    int n, p;
    cin >> n >> p;

    vector<int> ranks(p), input(n);

    lectura(ranks, p);
    lectura(input, n);

    multiselect(input, ranks, 0, n-1, 0, p-1);

    escritura(ranks, p, input);

}


