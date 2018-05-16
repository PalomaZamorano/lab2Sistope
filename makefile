entrega: main lectura conversion binarizacion clasificacion resultados

main: main.c
	gcc -Wall -o main main.c -I.
lectura: lectura.c 
	gcc -Wall -o lectura lectura.c -I.
conversion: conversion.c
	gcc -Wall -o conversion conversion.c -I.
binarizacion: binarizacion.c 
	gcc -Wall -o binarizacion binarizacion.c -I.
clasificacion: clasificacion.c
	gcc -Wall -o clasificacion clasificacion.c -I.
resultados: resultados.c
	gcc -Wall -o resultados resultados.c -I.    
