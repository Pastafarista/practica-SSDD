#!/bin/bash
#Authors: Antonio Cabrera y Alejandro Gómez
#Group: 3º MAIS 2023/20244
#Description: Compilar los programas de cliente y servidor

# Compilar
g++ cliente.cpp operaciones.h utils.h utils.cpp conexion_cliente.h filemanager.h filemanager.cpp -o cliente
g++ servidor.cpp operaciones.h utils.h utils.cpp conexion_cliente.h filemanager.h filemanager.cpp -o servidor

# Dar permisos
chmod +x cliente
chmod +x servidor
