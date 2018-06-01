# O ambiente de desenvolvimento que utilizaremos será baseado no ubuntu 16.04
# como possui suporte ao opencv e ao systemc, e estamos habituados a utilizá-lo.
# A versão do OpenCV utilzada é a 3.4.1.
# A versão do SystemC utilizada é a 2.3.2.

FROM ubuntu:16.04

# Definimos então algumas variáveis de ambiente para facilitar o acesso do
# sistema ao systemc.

ENV SYSTEMC_HOME /usr/local/systemc-2.3.2
ENV TARGET_ARCH linux64
ENV CXX g++-5

# Atualizar as referências e os sistema

RUN apt update && apt upgrade -y

# Instalando compilador c++-5 e outras dependências para o OpenCV

RUN apt install g++-5 -y
RUN apt install build-essential libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev -y
RUN apt install python3.5-dev python3-numpy libtbb2 libtbb-dev -y
RUN apt install libjpeg-dev libpng-dev libtiff5-dev libjasper-dev libdc1394-22-dev \
  libeigen3-dev libtheora-dev libvorbis-dev libxvidcore-dev libx264-dev sphinx-common \
  libtbb-dev yasm libfaac-dev libopencore-amrnb-dev libopencore-amrwb-dev libopenexr-dev \
  libgstreamer-plugins-base1.0-dev libavutil-dev libavfilter-dev libavresample-dev -y
RUN apt install libcurl3 libcurl4-openssl-dev libssl-dev -y

# Necessitamos do camke para compilar o OpenCV e o SystemC

COPY cmake-3.11.2 /opt/cmake
COPY opencv-3.4.1 /opt/
COPY systemc-2.3.2 /opt/systemc

# Em seguida instalamos o cmake

WORKDIR /opt/cmake
RUN ./bootstrap --system-curl
RUN make -j4 && make install

# Para compilar o OpenCV criamos um diretório e usamos o cmake para gerar Makefile

RUN mkdir -p /opt/opencv/release
WORKDIR /opt/opencv/release
RUN cmake -D CMAKE_CXX_COMPILER=g++-5 -D BUILD_TIFF=ON -D WITH_CUDA=OFF -D ENABLE_AVX=OFF -D WITH_OPENGL=OFF \
  -D WITH_OPENCL=OFF -D WITH_IPP=OFF -D WITH_TBB=ON -D BUILD_TBB=ON -D WITH_EIGEN=OFF \
  -D WITH_V4L=OFF -D WITH_VTK=OFF -D BUILD_TESTS=OFF -D BUILD_PERF_TESTS=OFF \
  -D ENABLE_PRECOMPILED_HEADERS=ON -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local \
  -D OPENCV_EXTRA_MODULES_PATH=/opt/opencv_contrib/modules /opt/opencv/

# E finalmente compilamos o OpenCV

RUN make -j4 && make install && ldconfig

# Copiamos então o systemc para a imagem

RUN mkdir -p /opt/systemc/objdir
WORKDIR /opt/systemc/objdir

# E então configuramos o instalador para em seguida compilar, checar e instalar

RUN ../configure --prefix=/usr/local/systemc-2.3.2
RUN make && make check && make install

# Com tudo instalado, limpamos os diretórios desnecessários

WORKDIR /opt
RUN rm -rf *

# Para finalizar, sobrescreve um arquivo do opencv para compatibilidade do SystemC com o OpenCV

COPY interface.h /usr/local/include/opencv2/core/hal/interface.h
WORKDIR /home
COPY initialize.run initialize.run

# Definimos o script initialize como o comando initicial da imagem

CMD ["./initialize.run", "&&", "/bin/bash"]
