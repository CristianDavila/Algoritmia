//Cristian Davila
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;


void writeParagraphRec(vector<int> &depen, int size, vector<string> &paragrafo) {
    //Utilizamos el vector depen como guia para escribir la solucion, empezando por el final (size)
    if (depen[size] > 1) writeParagraphRec(depen,depen[size]-1, paragrafo);
    for (int i = depen[size]-1; i<size-1; ++i) {
        cout << paragrafo[i] << ' ';
    }
    cout << paragrafo[size-1] << endl;
}


void writeParagraph(vector<int> &depen, int size, vector<string> &paragrafo, int penalty) {
    writeParagraphRec(depen, size, paragrafo);    //Llamamos a una funcion para imprimir el paragrafo
    cout << "Penalty: " << penalty << endl;
}


/*Justificacion:
 *La funcion WordWrap utiliza una matriz de tamaño (n+1)*(n+1), donde n es el numero de palabra en un paragrafo. *
 *Dicha matriz es utilizada para conocer el coste optimo de colocar las palabras paragrafo[i..j] en una linia.
 *La forma de calcular el coste es la siguiente: el coste de colocar una palabra será el coste más pequeño de
 *colocar ésta y todas sus anteriores.
 *
 *Coste:
 *El coste del algoritmo WordWrap es O(n²). El principal coste de la funcion viene al recorrer la matriz 'penalty',
 *de tamaño n*n. El resto de costes son los accesos a los vectores auxiliares y a la propia matriz, por lo tanto,
 *esos costes son constantes y sigue predominando el coste del recorrido a la matriz.
 *

 *Pre:
 *
 *Post: La funcion WordWrap formatea correctamente los paragrafos optimamente, y los imprime, junto al penalty sufrido.
*/
void WordWrap(vector<string> &paragrafo, int W) {
    int size = paragrafo.size();
    vector < vector<int> >  penalty(size+1,vector<int> (size+1,0));
    //Indica el penalty de colocar en una linia las palabras de paragrafo[i..j]
    vector<int> optim(size+1,-1);           //Indica el coste optimo de colocar las palabras paragrafo[i..j]
    vector<int> depen(size+1);              //Indica la palabra de la que depende (la anterior a ella en la misma linia)
    //Sumamos uno para no acceder a posiciones invalidas de memoria

    //Calculamos el penalty (mas adelante lo elevaremos al cuadrado)
    //Calculamos los espacios que sobran o faltan respecto a W
    for (int i = 1; i<=size; ++i) {
        penalty[i][i] = W - paragrafo[i-1].size();
        for (int j = i+1; j<=size; ++j) {
            penalty[i][j] = penalty[i][j-1] - paragrafo[j-1].size() - 1;
        }
    }

    //Calculamos la sequencia optima para reducir el coste al minimo
    //El vector optim tiene sus variables iniciadas a -1 para indicar que no ha sido visitado.
    optim[0] = 0;
    for (int i = 1; i<=size; ++i) {
        for (int j = 1; j<=i; ++j) {
            int opt = optim[j-1] + penalty[j][i]*penalty[j][i];
            if (optim[i] == -1 or opt < optim[i]) {
                optim[i] = opt;
                depen[i] = j;
            }
        }
    }
    //Escribimos el paragrafo y el penalty
    writeParagraph(depen, size, paragrafo, optim[size]);
}


void read() {
    int W;
    bool endline;
    bool first = true;
    vector<string> paragrafo;
    string s, word;
    cin >> W;                               //Leemos W
    while(getline(cin, s)) {                //leemos la linia
        istringstream ss(s);
        endline = true;                     //Nos avisara de si es un salto de linia
        while(ss >> word) {
            paragrafo.push_back(word);      //Introducimos las palabras de la linia en el vector de palabras
            endline = false;
        }
        if(endline) {                       //Si hay un salto de linia, pasamos a analizar el parrado
            if(!paragrafo.empty()) {
                if(!first) cout << endl;
                else first = false;
                WordWrap(paragrafo,W);      //Como estamos en el final del paragrafo, lo procesamos
            }
            paragrafo.clear();              //Lo eliminamos (ya ha sido tratado)
        }
    }
    if(!paragrafo.empty()) WordWrap(paragrafo,W);   //Puede quedar por tratar el ultimo paragrafo
}


int main() {   
    read();
}
