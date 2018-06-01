CryptoDIP
=========

CryptoDIP é uma aplicação C++ SystemC Open Source que simula um dispositivo capaz de receber imagens criptografadas da rede em pacotes, fazer a descriptografia, realizar um processamento de imagem (rotação da imagem 90° horário), criptografar novamente e enviar a imagem criptografada para a rede conectada, em pacotes.

Suporte
-------

CryptoDIP é desenvolvido e testado no `Ubuntu 16.04`. Qualquer outro sistema operacional não foi testado e não possui suporte.

Dependências
------------

O CryptoDIP **necessita** das bibliotecas `SystemC` e do `OpenCV` installados no sistema. Também utiliza-se do `cmake` para gerar Makefiles.

* [SystemC 2.3.2](http://www.accellera.org/downloads/standards/systemc "Accellera SystemC")
* [OpenCV 3.4.1](https://opencv.org/opencv-3-4-1.html "OpenCV 3.4.1")
* [CMake](https://cmake.org/download/ "Download CMake")

É recomendado que o OpenCV seja instalado com os pacotes de [contribuidores (opencv_contrib)](https://github.com/opencv/opencv_contrib/releases/tag/3.4.1). Recomenda-se também a versão `3.11.3` do `cmake`.

Docker
------

Para os familiarizados ao `Docker`, compilamos uma imagem com o sistema operacional `Ubuntu 16.04`, com todas as dependências instaladas incluindo as versões destacadas do `OpenCV` e `SystemC`. A imagem está disponível em [johanneslpc/systemcv](https://hub.docker.com/r/johanneslpc/systemcv/ "SystemC OpenCV Docker environment").

O `Dockerfile` gerador da imagem está disponível no diretório [environment](/environment).

Compilando e Executando
-----------------------

Todo o código fonte está no diretório [src](https://github.com/elciusferreira/CryptoDIP/tree/docker-env-test/src "src"), onde há tudo necessário para compilar e executar a aplicação.

Utilizamos do `cmake` para gerar arquivos de compilação (_Makefiles_). O uso do `cmake` e `Makefile` agiliza o desenvolvido em aplicações com volumes grandes de arquivos fonte.

#### Compilação

Para compilar criamos o diretório `debug` em `src` e geramos um `Makefile` no diretório com o comando do `cmake` e o parâmetro de _Debug_

```
mkdir -p src/debug && cd src/debug
cmake -D CMAKE_BUILD_TYPE=Debug ..
```

Com essa chamada, o `cmake` busca o `CMakeFile.txt` presente no diretório superior, e baseado nele gera um `Makefile` no diretório atual.

Com o `Makefile` gerado, basta executá-lo para compilar a aplicação

```
make
```

#### Execução

::TODO

#### Scripts Auxiliares

Alguns scripts auxiliares são providos para facilitar o acesso, teste e gerenciamento da aplicação.

Problemas Conhecidos
--------------------

::TODO
