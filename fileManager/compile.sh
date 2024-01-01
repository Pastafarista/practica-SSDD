#!/bin/bash
#Description: Compilar los programas de cliente y servidor

# Generar ejecutables
g++ cliente.cpp operaciones.h utils.h utils.cpp conexion_cliente.h filemanager.h filemanager.cpp -o cliente
g++ servidor.cpp operaciones.h utils.h utils.cpp conexion_cliente.h filemanager.h filemanager.cpp -o servidor

# Dar permisos
chmod +x cliente
chmod +x servidor
