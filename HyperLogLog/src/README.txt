cardest.cc:

Es el encargado de estimar la cardinalidad de una secuencia.

Se compila de la siguiente manera (creando un ejecutable con el nombre 'exe'):

g++ -o exe cardest.cc

El programa tiene dos modos de ejecucion:

1) con el tamaño de la memoria auxiliar predefinada (con el tamaño por defecto es 256):

./exe < all/D1.dat


2) definiendo el tamaño de la memoria auxiliar (utilizaremos el flag -M para activar este modo):

./exe -M valor < all/D1.dat

valor define el tamaño de la memoria auxiliar, por ejemplo, si queremos una
memoria de 1024 bytes, ejecutariamos el programa asi:

./exe -M 1024 < all/D1.dat



Script.py:

Es el encargado de calcular las estimaciones de los diferentes datasets.
Al ejecutarse, crea una carpeta llamada "Tablas Datasets", dentro estaran 
las tablas creadas.
Para funcionar, requiere que en su misma carpeta este el programa "cardest.cc",
y una carpeta, llamada "all", donde esten los datasets.
El solo se encarga de compilar el programa "cardest".

El script se ejecuta con el siguiente comando: 

python script.py K

K representa el número de iteraciones, por ejemplo, para 100 iteraciones, la 
ejecución se haria asi:

python script.py 100



ScriptM.py:

Es el encargado de calcular las estimaciones del Dataset1 variando el valor
de la memoria auxiliar.

Al ejecutarse, crea un archivo llamado "dataset1_Tabla_base_M.csv", que contiene
la tabla creada con los valores obtenidos.

Para funcionar, requiere que en su misma carpeta este el programa "cardest.cc",
y una carpeta, llamada "all", donde este el "dataset1".

El solo se encarga de compilar el programa "cardest".

El script se ejecuta con el siguiente comando: 

python script.py K

K representa el número de iteraciones, por ejemplo, para 100 iteraciones, la 
ejecución se haria asi:

python script.py 100
