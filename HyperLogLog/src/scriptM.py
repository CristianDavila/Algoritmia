import os
import subprocess
import math
import time
import csv
import sys
from time import sleep


def convertirEnLista(argument):
    i=0;
    lista = []
    while (argument[i] != ' '):
        i = i+1
    aux = len(argument)
    element1 = argument[0:i]
    element2 = argument[i+1:aux-1]
    [element1,element2]
    return [element1,element2]


def procesarDataset(datasetTarget, k, n, N, M, datasetWriter):
    tiempoAcumulado = 0
    # definimos la variable tiempoAcumulado, que se utilizara para sumar los tiempos

    estimacionAcumulada = 0
    # definimos la variable estimacionAcumulada, que se utilizara para sumar las estimaciones

    estimacionAcumuladaCuadrado = 0
    # definimos la variable estimacionAcumuladaCuadrado, que se utilizara para sumar el cuardado de las estimaciones

    i = 1
    while (i <= k):
        sleep(1)
        # entre ejecucion y ejecucion, hacemos que se pause durante 1 segundo, para asegurar que los numeros random
        # escogidos en el algoritmo sea realmente aleatorios (dichos numeros se basan en el tiempo del procesador,
        # por eso al dejar el margen de 1 segundo, nos aseguramos de que la semilla del generador ha variado)

        ejecucion = "./exe -M "+str(M)+" < all/D1.dat"                     # comando para ejecutar

        tiempoIni = time.time()
        resultado = subprocess.check_output(ejecucion, shell=True)      # ejecutamos el programa pasandole el dataset
        tiempoFin = time.time()

        tiempoTotal = tiempoFin - tiempoIni                 # calculamos el tiempo de ejecucion
        tiempoTotal = round(tiempoTotal, 4)                 # truncamos a 4 digitos por detras de la coma
        tiempoTotal = tiempoTotal*1000                      # pasamos a milisegundos

        tiempoAcumulado += tiempoTotal                      # calculamos el tiempo Acumulado de ejecuciones en ms
        tiempoAcumulado = round(tiempoAcumulado, 4)         # truncamos a 4 digitos por detras de la coma

        [estimacion,numeroTotalElementosLeidos] = convertirEnLista(resultado)
        # obtenemos la estimacion y el numeroTotalElementosLeidos

        estimacion = float(estimacion)                          # la estimacion esta en string, la pasamos a int
        estimacionAcumulada += estimacion                       # sumamos a la estimacionAcumulada
        estimacionAcumuladaCuadrado += estimacion*estimacion    # sumamos el cuadrado de la estimacion a su Acumulada

        errorRelativo = float(abs((1.0*n)-(1.0*estimacion))/(1.0*n))
        errorRelativo = round(errorRelativo, 4)
        # calculamos el error relativo


        i = i+1



    estimacionMedia = round(estimacionAcumulada/k, 4)
    # calculamos la estimacion media y truncamos a 4 digitos por detras de la coma

    errorEstandard = round(math.sqrt((estimacionAcumuladaCuadrado/k)-(estimacionMedia*estimacionMedia))/n, 4)
    # calculamos el error estandard y truncamos a 4 digitos por detras de la coma

    tiempoMedio = tiempoAcumulado/(k*1.0)                    # calculamos el tiempo medio (en milisegundos)
    tiempoMedio = round(tiempoMedio, 4)                      # truncamos a 4 digitos por detras de la coma

    tiempoMedioElemento = float(tiempoMedio*1000/(N*1.0))    # calculamos el tiempo medio por elemento (en microsegundos)
    tiempoMedioElemento = round(tiempoMedioElemento, 4)      # truncamos a 4 digitos por detras de la coma


    info = [M,estimacionMedia,errorEstandard,tiempoMedio,tiempoMedioElemento]
    # preparamos una lista, llamada 'info' para escribirla en la tabla

    datasetWriter.writerows([info])                     #escribimos la fila en la tabla



# Main

if len(sys.argv) == 2:
        k = sys.argv[1]                                 # definimos el numero de iteraciones
        k = int(k)
else:
    print "Parametros incorrectos! Introduzca un valor de K"
    sys.exit()


print '-- Inicio del Script --\n'

tiempoIniEje = time.time()

compilar = "g++ -o exe cardest.cc"
subprocess.call(compilar, shell=True)
datasetTarget = 'dataset1'
datasetfile  = open(datasetTarget+"_Tabla_base_M.csv", "wb")
# abrimos o creamos un fichero csv, donde escribiremos la tabla de resultados

datasetWriter = csv.writer(datasetfile)                 # creamos un writer para el fichero

datasetWriter.writerows([['M','Estimacion','Error estandard','tiempo medio (ms)', 'tiempo medio elemento (micro secs)']])
# escribimos la cabecera de la tabla


n = 3185                # cardinalidad exacta del dataset
N = 17219               # numero total de elementos del dataset
M = 128                 # valor inicial de M
while (M <= 65536):    # 131072 = 2^16
    print 'Inicio de la ejecucion con M = ',M
    procesarDataset(datasetTarget, k, n, N, M, datasetWriter)
    M = M*2
    print 'Fin de la ejecucion'
    print ''

datasetfile.close()                                     # cerramos el fichero csv


tiempoFinEje = time.time()
tiempoTotalEje = tiempoFinEje-tiempoIniEje
print 'Tiempo total de ejecucion del script: ', int(tiempoTotalEje/60), ' minutos y ',int(tiempoTotalEje%60), ' segundos'
