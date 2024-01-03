#!/bin/bash
#Authors: Antonio Cabrera y Alejandro Gómez
#Group: 3º MAIS 2023/20244
#Description: Compilar el cliente de fileManager

# Compilar
g++ cliente.cpp operaciones.h utils.h utils.cpp filemanager.h filemanager.cpp -o cliente

# Dar permisos
chmod +x cliente
