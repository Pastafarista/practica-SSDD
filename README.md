# Práctica Sistemas Distribuidos

## Resumen
La práctica está compuesta de 3 aplicaciones: **broker**, **filemanager** y **multmatrix**

- **broker** se encarga de poner en contacto a los clientes con los clientes de sus respectivos objetos
- **filemanager** es un servidor que permite manipular los archivos de una máquina en remoto
- **multmatrix** es un servidor que permite a los clientes realizar operaciones con matrices

## Instalación
Para que funcionen las aplicaciones hay que cambiar las ips, puertos y rutas a los de las máquinas que se usen:

### Filemanager
- En `filemanager/servidor/conexion_cliente.h` hay que cambiar la ruta del directorio `files` para que sea la ruta completa de la máquina en el que se ejecuta la aplicación
- En `filemanager/servidor/servidor.cpp` hay que cambiar la ip y puerto en la que se ejecuta la aplicación, también hay que indicar la ip y el puerto de la máquina **broker**
- En `filemanager/cliente/cliente.cpp` hay que cambiar la ruta del directorio de `files` a la ruta completa en la que se ejecuta la aplicación, también hay que cambiar la ip y puerto de la máquina **broker**

### Multmatrix
- En `multmatrix/servidor/serverMatrix.cpp` hay que cambiar la ip y puerto de la máquina en la que se ejecuta, lo mismo con la ip y puerto de la máquina **broker**
- En `multmatrix/cliente/clientMatrix.cpp` hay que cambiar la ip y el puerto de la máquina **broker** 

### Broker
- En `broker/broker.cpp` hay que cambiar el puerto en el que queremos que se ejecute la aplicación

## Recompilar
Si se desea volver a compilar las aplicaciones, en cada carpeta hay un archivo `compile.sh` que ejecuta los comandos necesarios para la compilación

## Autores:
Antonio Cabrera y Alejandro Gómez

