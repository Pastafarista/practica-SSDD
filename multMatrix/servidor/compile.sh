#!/bin/bash

# Authors: Antonio Cabrera y Alejandro Gómez
# Group: 3º MAIS 2023/2024
# Description: Script para compilar el servidor de mulmatrix

# Compilar
g++ -pthread serverMatrix.cpp operacionesMatrix.h multmatrix.h multmatrix.cpp utils.h utils.cpp conexion_cliente.h -o servidor

# Dar permisos
chmod +x servidor
