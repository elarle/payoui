#!/bin/sh

echo "Compilando main.cpp"
if [[ ! -e build ]]; then
	mkdir build
fi
g++ main.cpp -o build/main
echo "Compilado."
echo 
echo 
echo " === Salida del programa === "
./build/main
