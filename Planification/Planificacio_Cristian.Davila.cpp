//Cristian Davila

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <queue>

using namespace std;


struct InfoTask {                       //Informacion de la tarea
    int listAdPosition;                 //Posicion de la tarea en la lista de adyacencias
    int duration;                       //Duración de la tarea
    int inPos;                          //Orden de entrada
    bool hecha, encoladoET, encoladoLT;
    vector<string> dependen;            //Contiene los ID's de las tareas de las cual depende
    InfoTask(){
        encoladoET = encoladoLT = hecha = false;
    }
};

struct Sol {               // Estructura para guardar las soluciones de cada tarea
    int ES;
    int EF;
    int LS;
    int LF;

    Sol() {
        EF = ES = 0;
        LS = LF = 1000000;          //Damos a LS y LF un valor muy elevado, que no se pueda cumplir en ningun
                                    //juego de pruebas (tiene 7 caracteres, y las columnas son de 6)
    }

};

typedef map<string,InfoTask> InformationTasks;  //Map con la informacion de cada tarea
                    
typedef vector< vector<string> > listAd;        //Lista de adyacencias

typedef vector<Sol> Solution;                   //Vector con las soluciones


/*La funcion read se encarga de leer las tareas y de crear la lista de adyacencias.
 *Ademas, añade en un map la informacion de cada tarea, donde su clave es su identificador.
 *Para mantener el orden, guarda en un vector de ID's el orden de entrada.
*/
void read(vector<string> & ids, listAd & adyacencias, InformationTasks & infoTasks) {

    vector<string>  dependences(adyacencias.size()); //Utilizado para encontrar las dependencias de cada tarea
    string ID, ID2;
    int posORDEN = 1;                   //Utilizado para conocer el orden y guardarlo en el map de tareas
    int time;
    ids.push_back("START");             //Añadimos START al principio
    InfoTask aux;                       //Utilizado para añadir START y END
    int numTasks = adyacencias.size();     //numTasks = 'n' + 2 (START y END)
    map<string,bool> firstTask;            //map auxiliar utilizado para saber las tareas dependientes de START
    for(int i=1; i < numTasks-1; ++i){ //Utilizaremos 'i' para recorrer la lista de adyacencias, sin pasar por START
        cin >> ID >> time;
        dependences[i] = ID;          //Mantenemos el orden de las tareas, para saber en que tarea estamos
        infoTasks[ID].duration = time;
        if (firstTask.find(ID)== firstTask.end()) {   //Si la tarea no esta en el 'map'
            firstTask[ID] = true;                  //Añadimos su dependencia de START (hasta demostrar lo contrario)
            ids.push_back(ID);                  //Introducimos la nueva tarea en el vector de id's, manteniendo el orden
            infoTasks[ID].inPos = posORDEN;   //Añadimos la posicion de entrada
            ++posORDEN;
        }
        infoTasks[ID].listAdPosition = i;           //Añadimos la posicion en la lista
        cin >> ID2;                              //Leemos la primera tarea de la que es prerequisito
        if (ID2 == "@") adyacencias[i].push_back("END");      // Si la primera tarea que entra es '@' esa tarea corresponde a una tarea END (END depende de dicha tarea)
        while (ID2 != "@") {
            adyacencias[i].push_back(ID2);
            if (firstTask.find(ID2)== firstTask.end()) {      //Si la tarea no esta en el 'map'
                ids.push_back(ID2);                     //Introducimos la nueva tarea en el vector de id's, manteniendo el orden
                infoTasks[ID2].inPos = posORDEN;      //Añadimos la posicion de entrada
                ++posORDEN;                
            }
            firstTask[ID2] = false;      // Si una tarea depende de otra, ya no puede ser dependiente de START
            cin >> ID2;
        }
    }

    aux.inPos = 0; aux.listAdPosition = 0; aux.duration = 0;
    infoTasks["START"] = aux;                         //Inicializamos la tarea START
    aux.inPos = ids.size(); aux.listAdPosition = numTasks-1;
    infoTasks["END"] = aux;                           //Inicializamos la tarea START
    ids.push_back("END");                               //Introducimos la tarea END
    dependences[0] = "START";
    dependences[numTasks-1] = "END";

    map<string,bool>::iterator it;
    for (it = firstTask.begin(); it != firstTask.end(); ++it)     //Añadimos las dependecias de START en la lista de adyacencias
        if (it->second) adyacencias[0].push_back(it->first);

    //Añadimos las tareas de las que depende    
    for (int i = 0; i<adyacencias.size(); ++i) {
        for (int j = 0; j<adyacencias[i].size(); ++j) {
            infoTasks[adyacencias[i][j]].dependen.push_back(dependences[i]);
        }
    }

}


/*Justificacion:
 *searchDependences busca si alguna de las tareas de las que depende todavia no ha sido completada.
 *Si alguna de las tareas de las que depende no ha sido visitada, no podemos actualizar la tarea actual.
 *Coste:
 *El coste de la funcion es log(n) en el acceso de la tarea, y log(n)*m del bucle, en total es log(n)*(m+1),
 *donde size es el numero de dependencias.
*/
bool searchDependences(string tarea, InformationTasks & informacion) {
    InfoTask info = informacion[tarea]; 
    int m = info.dependen.size();
    for (int i = 0; i<m; ++i) {          //Consultamos sus tareas de las que depende
        string dependiente = info.dependen[i];              //ID de la siguiente tarea
        if(!informacion[dependiente].hecha) return false;   //Si no ha sido compeltada, no se puede visitar
    }
    return true;
}




/*Justificacion:
 *La primera tarea por la que empieza es el primer nodo del grafo: la tarea START. Primero encola las tareas que dependen
 *de el (las adyacentes) en una cola. Si no existen adyacentes, tendremos un ciclo, puesto que 'n' sera mayor a 0.
 *A continuacion empezamos a visitar las tareas encoladas. Para cada tarea, el algorismo comprueba que las tareas de las
 *que depende hayan sido vistadas, y si no es asi, las encola para visitarlas mas tarde, de esta forma nos aseguramos
 *tener encoladas todas las tareas dependientes de la tarea actual, para tratarlas posteriormente. Si sus dependencias
 *estan visitadas, encolamos las tareas de las que es prerequisito, se comprueba que no hayan sido actualizadas
 *(para evitar ciclos), y actualizamos el ES. Al finalizar actualizamos el EF.
 *
 *Coste:
 *El algoritmo utilizado para recorrer el grafo es un BFS (breadth-first search). Con el BFS, visitaremos todos los
 *vertices del grafo, y sus adyacentes, por lo tanto, el coste del BFS es n+m (numero de tareas + numero de dependencias).
 *El algoritmo emplea accesos al map de tareas, y tambien ejecuta la funcion que busca las dependencias de cada tarea,
 *por lo tanto el coste total del algoritmo aumenta a (n+m) * log n (coste de los accesos al map).
 *

 *Pre:  soluciones.size() = adyacencias.size() > 0;
 *      infoTasks: informacion de las tareas
 *      soluciones: vector con las soluciones de cada tarea
 *
 *Post: La funcion devuelve true si el grafo no es ciclico, o false si lo es. Actualiza el vector de soluciones
 *      y el map que contiene la informacion de cada tarea
*/
bool ET(listAd & adyacencias, InformationTasks & infoTasks, Solution & solutions) {

    queue<string> cola;                                 //Utilizado para visitar las tareas que dependientes
    InfoTask task;

    for (int i = 0; i<adyacencias[0].size(); ++i) {     // Guardado en la cola de las tareas adyacentes a START
        cola.push(adyacencias[0][i]);                   //Encolamos las tareas dependientes de START
        infoTasks[adyacencias[0][i]].encoladoET = true;
    }
    infoTasks["START"].encoladoET = true;
    infoTasks["START"].hecha = true;                    //La tarea START ya esta hecha
    if (cola.empty()) return false;                     //Si no hay adyacentes, estamos delante de un ciclo

    while (!cola.empty()) {                             //Mientras queden tareas por visitar
        string tarea = cola.front();                    //Siguiente tarea
        cola.pop();                                     //Quitamos de la cola

        while (!searchDependences(tarea,infoTasks)) {//Mientras la tarea no tenga tareas dependientes sin visitar
            //En este bucle interno, comprobamos si existen ciclos
            //Si ya esta marcada como repetida, significa que tenemos un ciclo
            if(infoTasks[tarea].hecha) return false;
            infoTasks[tarea].hecha = true;
            cola.push(tarea);           //Añadimos la tarea
            tarea = cola.front();       //Obtenemos la siguinte tarea
            cola.pop();                 //La desencolamos
        }

        task = infoTasks[tarea];        //Continuamos con la tarea

        for (int i = 0; i<adyacencias[task.listAdPosition].size(); ++i) {   //Recorremes las tareas de la que es prerequisito

            if(infoTasks[adyacencias[task.listAdPosition][i]].hecha) return false;
            //Si ha sido visitada, estamos en un ciclo

            if(!infoTasks[adyacencias[task.listAdPosition][i]].encoladoET){         //Si no ha sido encolado
                infoTasks[adyacencias[task.listAdPosition][i]].encoladoET = true;   //Y lo marcamos como encolado
                cola.push(adyacencias[task.listAdPosition][i]);                     //Lo encolamos
            }
            if (solutions[infoTasks[adyacencias[task.listAdPosition][i]].inPos].ES < solutions[task.inPos].ES + task.duration)   //Si es menor el tiempo
                solutions[infoTasks[adyacencias[task.listAdPosition][i]].inPos].ES = solutions[task.inPos].ES + task.duration;   //Cogemos el nuevo tiempo
        }
        solutions[task.inPos].EF = solutions[task.inPos].ES + task.duration;   // EF = ES + tarea.duration
        infoTasks[tarea].hecha = true;    //Tarea completada, evitaremos visitarla de nuevo
    }
    return true;
}


/*Justificacion:
 *La funcion latestTime se basa en la funcion earliestTime, pero de forma inversa: comienza por la tarea END, y va
 *visitando las tareas de las que depende, encolando las no visitadas, y actualizando los tiempos de LF y LS.
 *En este caso no cal comprobar ciclos ni repeticiones, porque ya han sido comprobadas en la funcion earliestTime.
 *
 *Coste:
 *El algoritmo utilizado para recorrer el grafo es un BFS (breadth-first search). Con el BFS, visitaremos todos los
 *vertices del grafo, y sus adyacentes, por lo tanto, el coste del BFS es n+m (numero de tareas + numero de dependencias).
 *El algoritmo emplea accesos al map de tareas, y tambien ejecuta la funcion que busca las dependencias de cada tarea,
 *por lo tanto el coste total del algoritmo aumenta a (n+m) * log n (coste de los accesos al map).
 *

 *Pre:
 *      infoTasks: informacion de las tareas
 *      solutions: vector con las soluciones de cada tarea
 *
 *Post: Actualiza el LF y LS de cada tarea en el vector de soluciones
 *
*/
void LT(Solution & solutions, InformationTasks & infoTasks) {

    queue<string> cola;                                 //Utilizado para visitar las tareas de las que depende
    InfoTask task, task2;
    task = infoTasks["END"];                             //Tarea 'END'
    for (int i = 0; i<task.dependen.size(); ++i) {      //empieza por el 'END'
        task2 = infoTasks[task.dependen[i]];            //Visitamos las tareas de las que depende
        cola.push(task.dependen[i]);                    //La añadimos a la cola
        task2.encoladoLT = true;          //la marcamos como visitada
        solutions[task2.inPos].LF = solutions[solutions.size()-1].EF;
        //Las tareas de las que depende END acaban al mismo tiempo que END
    }
    while (!cola.empty()) {                             //Mientras tengamos tareas por visitar
        task2 = infoTasks[cola.front()];                //Obtenemos dicha tarea
        cola.pop();                                     //Se elimina de la cola
        for (int i = 0; i<task2.dependen.size(); ++i) { //Recorremos las tareas de las que depende

            task = infoTasks[task2.dependen[i]];        //Tarea
            if (!task.encoladoLT) {       //Si es una nueva tarea, la añadimos
                cola.push(task2.dependen[i]);           //Se añade
                task.encoladoLT = true;   //La marcamos como añadida
            }
            if (solutions[task.inPos].LF > solutions[task2.inPos].LF - task2.duration) {
                //Si la nueva tarea tarda mas
                if (task.encoladoLT) cola.push(task2.dependen[i]);
                //Es necesario encolarla de nuevo, su nuevo tiempo puede afectar a sus dependientes
                solutions[task.inPos].LF = solutions[task2.inPos].LF - task2.duration;
            }
        }
        solutions[task2.inPos].LS = solutions[task2.inPos].LF - task2.duration;
    }
}


//writeColumn escribe un dato de la solucion, asegurandose escribir el dato con 6 caracteres
void writeColumn(int solution) {
   int size = 1;
   cout << solution << " ";
   while(solution > 9){
       ++size;
       solution = solution/10;
   }
   if(size > 4) size = 4;               //evitamos pasarnos de anchura
   for(int i=4-size; 0<=i; --i)
       cout << " ";
}


//writeRow escribe la fila de la solucion a la tarea
void writeRow(Sol & singleSolution){
    writeColumn(singleSolution.ES);
    writeColumn(singleSolution.EF);
    writeColumn(singleSolution.LS);
    writeColumn(singleSolution.LF);
}


//writeSolution escribe las soluciones, dejado el margen correcto
void writeSolution(Solution & solution, vector<string> &ids) {
    for (int i = 0; i<solution.size()-1; ++i) {
        int size = ids[i].size();
        cout << ids[i];
        for(int j=5-size; 0<=j; --j)
            cout << " ";

        writeRow(solution[i]);

        if (solution[i].ES == solution[i].LS) cout << '*';
        cout << endl;
    }
    cout << "END" << "   ";
    Sol solutionEND = solution[solution.size()-1];
    solutionEND.LF = solutionEND.LS = solutionEND.EF = solutionEND.ES;
    writeRow(solutionEND);

    cout << "*" << endl;
}


int main() {
    int numTask;
    cin >> numTask;
    if (numTask == 0) {       //Si no hay tareas
        cout << "START " << 0 << "     " << 0 << "     " << 0 << "     " << 0 << "     " << '*' << endl;
        cout << "END   " << 0 << "     " << 0 << "     " << 0 << "     " << 0 << "     " << '*' << endl;
    }
    else{
        vector<string> IDs;                                 // Vector que contendrá las tareas por orden de entrada
        listAd listaady(numTask+2);                         // Lista de adyacencias
        Solution solutions(numTask+2);                      // Vector solución
        InformationTasks infoTask;                       //Contiene la informacion de cada tarea
        read(IDs,listaady,infoTask);                     //Procedemos a la elctura y preparacion de los datos

        if (ET(listaady,infoTask, solutions)) { //Ejecutamos earliestTime, y si no contiene ciclos, proseguimos
            LT(solutions,infoTask);
            writeSolution(solutions,IDs);                   //escribimos las soluciones
        }
        else cout << "Proyecto contiene ciclos" << endl;
    }
}

