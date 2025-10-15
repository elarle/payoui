#!/bin/sh

echo "Compilando..."
if [[ ! -e build ]]; then
	mkdir build
fi
g++ main.cpp -o build/main
echo "Compilado."
