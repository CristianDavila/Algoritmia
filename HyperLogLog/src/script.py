import os
import subprocess
import math
import time
import csv
import sys
from time import sleep

global iterador

def convertirEnLista(argument):                     # busca los dos valores del string que se le da, utilizando como
    i=0;                                            # marca el espacio
    lista = []
    while (argument[i] != ' '):                     # busca la posicion de la marca
        i = i+1
    aux = len(argument)                             # largo del argumento
    element1 = argument[0:i]                        # obtiene el primer elemento
    element2 = argument[i+1:aux-1]                  # obtiene el segundo elemento
    element1 = float(element1)                        # el element1 es un string, lo pasamos a int
    element2 = float(element2)                        # el element2 es un string, lo pasamos a int
    return [element1,element2]





def procesarDataset(datasetTarget, k, n, N):

    tiempoAcumulado = 0
    # definimos la variable tiempoAcumulado, que se utilizara para sumar los tiempos

    estimacionAcumulada = 0
    # definimos la variable estimacionAcumulada, que se utilizara para sumar las estimaciones

    estimacionAcumuladaCuadrado = 0
    # definimos la variable estimacionAcumuladaCuadrado, que se utilizara para sumar el cuardado de las estimaciones

    datasetfile  = open('Tablas Datasets/'+datasetTarget+"_Tabla.csv", "wb")
    # abrimos o creamos un fichero csv, donde escribiremos la tabla de resultados

    datasetWriter = csv.writer(datasetfile)                 # creamos un writer para el fichero

    datasetWriter.writerows([['i','Estimacion','Error relativo','tiempo (ms)']])
    # escribimos la cabecera de la tabla

    # inicializamos las variables que usaremos para contar el numero de datasets que esta en cada franja de % de errores
    errorRelativoMenor1 = 0
    errorRelativoMenor1y5 = 0
    errorRelativoMenor5y10 = 0
    errorRelativoMenor10y15 = 0
    errorRelativoMenor15y25 = 0
    errorRelativoMenor25y40 = 0
    errorRelativoMenor40y55 = 0
    errorRelativoMenor55y70 = 0
    errorRelativoMenor70y85 = 0
    errorRelativoMenor85y100 = 0
    i = 1
    while (i <= k):
        sleep(1)
        # entre ejecucion y ejecucion, hacemos que se pause durante 1 segundo, para asegurar que los numeros random
        # escogidos en el algoritmo sea realmente aleatorios (dichos numeros se basan en el tiempo del procesador,
        # por eso al dejar el margen de 1 segundo, nos aseguramos de que la semilla del generador ha variado)


        ejecucion = "./exe < all/"+datasetTarget+".dat"     # comando para ejecutar

        tiempoIni = time.time()
        resultado = subprocess.check_output(ejecucion, shell=True)      # ejecutamos el programa pasandole el dataset
        tiempoFin = time.time()

        tiempoTotal = tiempoFin - tiempoIni                 # calculamos el tiempo de ejecucion
        tiempoTotal = tiempoTotal*1000                      # pasamos a milisegundos
        tiempoTotal = round(tiempoTotal, 4)                 # truncamos a 4 digitos por detras de la coma


        tiempoAcumulado += tiempoTotal                      # calculamos el tiempo Acumulado de ejecuciones en ms
        tiempoAcumulado = round(tiempoAcumulado, 4)         # truncamos a 4 digitos por detras de la coma

        [estimacion,numeroTotalElementosLeidos] = convertirEnLista(resultado)
        # obtenemos la estimacion y el numeroTotalElementosLeidos

        estimacionAcumulada += estimacion                   # sumamos a la estimacionAcumulada
        estimacionAcumuladaCuadrado += estimacion*estimacion    # sumamos el cuadrado de la estimacion a su Acumulada

        errorRelativo = float(abs((1.0*n)-(1.0*estimacion))/(1.0*n))        
        errorRelativo = round(errorRelativo, 4)
        # calculamos el error relativo y truncamos a 4 digitos por detras de la coma

        info = [i,estimacion,errorRelativo,tiempoTotal]

        # preparamos una lista, llamada 'info' para escribirla en la tabla

        datasetWriter.writerows([info])                     #escribimos la fila en la tabla
        if errorRelativo < 0.01 :
            errorRelativoMenor1 = errorRelativoMenor1+1
        elif errorRelativo < 0.05 :
            errorRelativoMenor1y5 = errorRelativoMenor1y5+1
        elif errorRelativo < 0.1:
            errorRelativoMenor5y10 = errorRelativoMenor5y10+1
        elif errorRelativo < 0.15 :
            errorRelativoMenor10y15 = errorRelativoMenor10y15+1
        elif errorRelativo < 0.25 :
            errorRelativoMenor15y25 = errorRelativoMenor15y25+1
        elif errorRelativo < 0.40 :
            errorRelativoMenor25y40 = errorRelativoMenor25y40+1
        elif errorRelativo < 0.55 :
            errorRelativoMenor40y55 = errorRelativoMenor40y55+1
        elif errorRelativo < 0.70 :
            errorRelativoMenor55y70 = errorRelativoMenor55y70+1
        elif errorRelativo < 0.85 :
            errorRelativoMenor70y85 = errorRelativoMenor70y85+1
        elif errorRelativo < 1.0 :
            errorRelativoMenor85y100 = errorRelativoMenor85y100+1

        i = i+1


    datasetfile.close()                                     # cerramos el fichero csv

    estimacionMedia = round(estimacionAcumulada/k, 4)
    # calculamos la estimacion media y truncamos a 4 digitos por detras de la coma

    errorEstandard = round(math.sqrt((estimacionAcumuladaCuadrado/k)-(estimacionMedia*estimacionMedia))/n, 4)
    # calculamos el error estandard y truncamos a 4 digitos por detras de la coma

    tiempoMedio = tiempoAcumulado/(k*1.0)                    # calculamos el tiempo medio (en milisegundos)
    tiempoMedio = round(tiempoMedio, 4)                      # truncamos a 4 digitos por detras de la coma

    tiempoMedioElemento = float(tiempoMedio*1000/(N*1.0))    # calculamos el tiempo medio por elemento (en microsegundos)
    tiempoMedioElemento = round(tiempoMedioElemento, 4)      # truncamos a 4 digitos por detras de la coma


    print 'Error relativo de la estimacion menor del 1%: ' , errorRelativoMenor1
    print 'Error relativo de la estimacion entre el 1% y el 5%: ' , errorRelativoMenor1y5
    print 'Error relativo de la estimacion entre el 5% y el 10%: ' , errorRelativoMenor5y10
    print 'Error relativo de la estimacion entre el 10% y el 15% ' , errorRelativoMenor10y15
    print 'Error relativo de la estimacion entre el 15% y el 25% ' , errorRelativoMenor15y25
    print 'Error relativo de la estimacion entre el 25% y el 40% ' , errorRelativoMenor25y40
    print 'Error relativo de la estimacion entre el 40% y el 55% ' , errorRelativoMenor40y55
    print 'Error relativo de la estimacion entre el 55% y el 70% ' , errorRelativoMenor55y70
    print 'Error relativo de la estimacion entre el 70% y el 85% ' , errorRelativoMenor70y85
    print 'Error relativo de la estimacion entre el 85% y el 100% ' , errorRelativoMenor85y100

    print ' '


    global tablaResumen 
    tablaResumen.append([datasetTarget,estimacionMedia,errorEstandard,tiempoMedio,tiempoMedioElemento])






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

error = subprocess.call(compilar, shell=True)           # compilamos el programa
if error == 0:
    print '--Compilacion completada correctamente--'
else:
    print '--Compilacion fallida--'
    sys.exit()

try:
    os.makedirs('./Tablas Datasets')            # creamos la carpeta donde pondremos las tablas
except OSError:
    pass                                        # si llegamos al error, no hacemos nada (existe la carpeta)

print '--Creacion de carpeta destino de tablas completada--\n'

tablaResumen = []

n = [3185,23134,5893,5760,9517,6319,8995,496317,17620]
# cardinalidad exacta del dataset

N = [17219,384222,61882,27266,166428,124623,157262,7052495,575284]
# numero total de elementos de los datasets

i = 1                                           # la 'i' se utiliza para iterar entre los datasets
while (i<10):
    print ' '
    print 'Inicio de la ejecucion del dataset',i
    data = 'D'+str(i)                           # creamos el nombre del dataset
    procesarDataset(data, k, n[i-1], N[i-1])              # llamamos a la funcion que procesara dicho dataset
    print 'Dataset',i,' ejecutado correctamente\n'
    i = i+1


datasetfile  = open('Tablas Datasets/Tabla_Resumen.csv', "wb")
# abrimos o creamos un fichero csv, donde escribiremos la tabla de resumen

datasetWriter = csv.writer(datasetfile)                 # creamos un writer para el fichero
datasetWriter.writerows([['Dataset','Estimacion','Error estandard','tiempo medio (ms)', 'tiempo medio elemento (micro secs)']])

for row in tablaResumen:                                # recorremos la tabla de resumen
    datasetWriter.writerows([row])                      # escribimos la informacion del dataset
datasetfile.close()                                     # cerramos el fichero


tiempoFinEje = time.time()
tiempoTotalEje = tiempoFinEje-tiempoIniEje
print 'Tiempo total de ejecucion del script: ', int(tiempoTotalEje/60), ' minutos y ',int(tiempoTotalEje%60), ' segundos'



