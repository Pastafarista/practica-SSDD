#!/bin/bash
# Authors: Antonio Cabrera y Alejandro Gómez
# Description: Script de instalación de la aplicación

# Parametro de entrada (broker, filemanager, multmatrix)

if [ $# -eq 0 ]; then
    echo "No se ha introducido ningún parámetro"
    echo "Uso: ./install.sh [broker|filemanager|multmatrix]"
    exit 1
fi

if [ $1 = "broker" ]; then
    echo "Instalando broker..."
    
    mkdir ../broker
    cp -r broker ../broker

    cd ../broker
	
    rm -rf ../remote_objects
	
    chmod +x compile.sh
    ./compile.sh

    echo "Broker instalado"
    exit 0
fi

if [ $1 = "filemanager" ]; then
    echo "Instalando filemanager..."
    
    mkdir ../filemanager
    cp -r filemanager ../filemanager

    cd ../filemanager
    rm -rf ../remote_objects
    
    chmod +x compile.sh
    ./compile.sh

    echo "Filemanager instalado"
    exit 0
fi

if [ $1 = "multmatrix" ]; then
    echo "Instalando multmatrix..."
    
    mkdir ../multmatrix
    cp -r multmatrix ../multmatrix

    cd ../multmatrix
    rm -rf remote_objects

    chmod +x compile.sh
    ./compile.sh

    echo "Multmatrix instalado"
    exit 0
fi
