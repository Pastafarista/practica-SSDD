#!/bin/bash
# Authors: Antonio Cabrera y Alejandro Gómez
# Group: 3º MAIS 2023/2024
# Description: Script para compilar el broker

# Compilar
g++ -pthread broker.cpp operaciones.h utils.h utils.cpp -o broker

# Dar persmisos
chmod +x broker
