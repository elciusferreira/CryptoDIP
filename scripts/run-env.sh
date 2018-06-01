#!/bin/bash

cd ..
REPOPATH=$(pwd)
echo "Repository path: " $REPOPATH

# Limpa builds antigos
rm -rf $REPOPATH/src/debug/* $REPOPATH/src/release/*

# Caso haja flags
while getopts ":r" opt; do
  case $opt in
  r)
    echo "Running environment once..." >&2
    docker run -it --rm -v $REPOPATH/src:/home/CryptoDIP johanneslpc/systemcv
    exit 0
    ;;
  \?)
    echo "Invalid option: -$OPTARG" >&2
    exit -1
    ;;
  esac
done

# Caso não tenha flags, verifica se já existe um container em cache
if `docker ps -a | grep -q systemc_opencv_env`
then
  # Caso já exista, inicia o container em cache
  echo "Already online. Starting environment..."
  docker start -i systemc_opencv_env
else
  # Caso contrário, inicializa um novo container interativo
  echo "Initializing environment..."
  docker run -it --name systemc_opencv_env -v $REPOPATH/src:/home/CryptoDIP johanneslpc/systemcv /bin/bash
fi
