#!/bin/bash

cd ..
REPOPATH=$(pwd)
echo "Repository path: " $REPOPATH

docker run -it --name systemc_opencv_env -v $REPOPATH/src:/home/CryptoDIP johanneslpc/systemcv
