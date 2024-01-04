#!/usr/bin/python3
# Authors: Antonio Cabrera y Alejandro Gómez
# Description: Script para instalar cada una de las aplicaciones

import os
import subprocess
import sys

# Obtenemos el parametro que nos indica que aplicacion instalar

if len(sys.argv) < 2:
    print("Uso: python3 install.py <broker, filemanager o multmatrix>")
    exit()

app = sys.argv[1] # (broker, filemanager, multmatrix)

if app == "broker":
    print("Instalando broker...")
    
    # Si no estamos en la carpeta raiz del proyecto, clonamos el repositorio
    if not os.getcwd().split("/")[-1] == "remote-objects":
        if os.path.isdir("remote-objects"):
            os.system("rm -rf remote-objects")

        os.system("git clone https://github.com/Pastafarista/remote-objects.git")
        os.chdir("remote-objects")
   
    # Eliminamos la carpeta del broker si existe
    if os.path.isdir("../broker"):
        os.system("rm -rf ../broker")

    # Creamos la carpeta donde se instalará el broker
    os.mkdir("../broker")
    
    # Movemos los archivos del broker a la carpeta creada
    os.system("cp -r broker/* ../broker")
    os.chdir("../broker")
    
    # Eliminamos los archivos innecesarios
    os.system("rm -rf ../remote-objects")

    # Compilamos el broker
    os.system("chmod +x ./compile.sh")
    os.system("./compile.sh")

    # Ejecutamos el broker
    os.system("./broker")

elif app == "filemanager":


    if(len(sys.argv) != 4):
        print("Uso: python3 install.py filemanager <ip_broker> <ip_filemanager>")
        exit()
    
    ip_broker = sys.argv[2]
    ip = sys.argv[3]
    port = "15000"
    port_broker = "15015"

    print("Instalando filemanager...")

    # Si no estamos en la carpeta raiz del proyecto, clonamos el repositorio
    if not os.getcwd().split("/")[-1] == "remote-objects":
        if os.path.isdir("remote-objects"):
            os.system("rm -rf remote-objects")

        os.system("git clone https://github.com/Pastafarista/remote-objects.git")
        os.chdir("remote-objects")

    # Eliminamos la carpeta del filemanager si existe
    if os.path.isdir("../filemanager"):
        os.system("rm -rf ../filemanager")

    # Creamos la carpeta donde se instalará el filemanager
    os.mkdir("../filemanager")

    # Movemos los archivos del filemanager a la carpeta creada
    os.system("cp -r fileManager/servidor/* ../filemanager")
    os.chdir("../filemanager")

    # Eliminamos los archivos innecesarios
    os.system("rm -rf ../remote-objects")

    # Modificamos el archivo de configuracion del filemanager
    f = open("data.txt", "w")
    f.write(ip + "\n")
    f.write(port + "\n")
    f.write(ip_broker + "\n")
    f.write(port_broker + "\n")
    f.close()

    # Compilamos el filemanager
    os.system("chmod +x ./compile.sh")
    os.system("./compile.sh")

    # Ejecutamos el filemanager
    os.system("./servidor")

elif app == "multmatrix":

    if(len(sys.argv) != 5):
        print("Uso: python3 install.py multmatrix <ip_broker> <ip_multmatrix>")
        exit()

    ip_broker = sys.argv[3]
    ip = sys.argv[4]
    port = "15030"
    port_broker = "15015"

    print("Instalando multmatrix...")

    # Si no estamos en la carpeta raiz del proyecto, clonamos el repositorio
    if not os.getcwd().split("/")[-1] == "remote-objects":
        if os.path.isdir("remote-objects"):
            os.system("rm -rf remote-objects")

        os.system("git clone https://github.com/Pastafarista/remote-objects.git")
        os.chdir("remote-objects")

    # Eliminamos la carpeta del multmatrix si existe
    if os.path.isdir("../multmatrix"):
        os.system("rm -rf ../multmatrix")

    # Creamos la carpeta donde se instalará el multmatrix
    os.mkdir("../multmatrix")

    # Movemos los archivos del multmatrix a la carpeta creada
    os.system("cp -r multMatrix/servidor/* ../multmatrix")
    os.chdir("../multmatrix")

    # Eliminamos los archivos innecesarios
    os.system("rm -rf ../remote-objects")

    # Modificamos el archivo de configuracion del multmatrix
    f = open("data.txt", "w")
    f.write(ip + "\n")
    f.write(port + "\n")
    f.write(ip_broker + "\n")
    f.write(port_broker + "\n")
    f.close()

    # Compilamos el multmatrix
    os.system("chmod +x ./compile.sh")
    os.system("./compile.sh")

    # Ejecutamos el multmatrix
    os.system("./servidor")
