#!/bin/bash

# Authors: Antonio Cabrera y Alejandro Gómez
# Group: 3º MAIS 2023/2024
# Description: Script para compilar el cliente y de mulmatrix

# Compilar
g++ main_multMatrix.cpp clientMatrix.cpp clientMatrix.h multmatrix.cpp multmatrix.h operacionesMatrix.h utils.cpp utils.h -o cliente

# Dar permisos
chmod +x cliente
