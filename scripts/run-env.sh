#!/bin/bash

cd ..
REPOPATH=$(pwd)
echo "Repository path: " $REPOPATH

rm -rf $REPOPATH/src/debug/* $REPOPATH/src/release/*
docker run -it --rm -v $REPOPATH/src:/home/CryptoDIP johanneslpc/systemcv
