# Temporizador de um disparo
Este projeto é um temporizador simples que controla três LEDs (vermelho, azul e verde) usando um Raspberry Pi Pico. O temporizador alterna entre os LEDs a cada 3 segundos.

## Estrutura do Projeto
.gitignore
.vscode/
build/
CMakeLists.txt
diagram.json
pico_sdk_import.cmake
Temporizador.c
wokwi.toml

## Configuração do Ambiente
1. Instale o SDK do Raspberry Pi Pico seguindo as instruções [aqui](https://github.com/raspberrypi/pico-sdk).
2. Clone este repositório:
   ```sh
   git clone <URL_DO_REPOSITORIO>
   cd Temporizador

   