#!/bin/bash

# Authors: Antonio Cabrera y Alejandro Gómez
# Group: 3º MAIS 2023/2024
# Description: Script para compilar el cliente y el servidor de mulmatrix

# Compilar
g++ serverMatrix.cpp operacionesMatrix.h multmatrix.h multmatrix.cpp utils.h utils.cpp conexion_cliente.h -o servidor
g++ main_multMatrix.cpp clientMatrix.cpp clientMatrix.h multmatrix.cpp multmatrix.h operacionesMatrix.h utils.cpp utils.h -o cliente

# Dar permisos
chmod +x servidor
chmod +x cliente
