#!/bin/bash
#Authors: Antonio Cabrera y Alejandro Gómez
#Group: 3º MAIS 2023/20244
#Description: Compilar el servidor de fileManager

# Compilar
g++ servidor.cpp operaciones.h utils.h utils.cpp conexion_cliente.h filemanager.h filemanager.cpp -o servidor

# Dar permisos
chmod +x servidor
