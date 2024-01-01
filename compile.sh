#!/bin/bash
# Description: Como no se usar CMAKE voy a compilar con un script de bash

g++ cliente.cpp include/* -o output/cliente
g++ servidor.cpp include/* -o output/servidor
g++ broker.cpp include/* -o output/broker
