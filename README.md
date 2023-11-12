# Qualidade da Água

> Projeto de microcontroladores

Mono repositório do primeiro projeto de microcontroladores com ESP32 e com o Firebase.

O ESP32 usado para esse projeto é o Heltec v3 WiFi LoRa.

O projeto tem duas pastas, a pasta `microcontroller` contém o código do ESP32 e a pasta `site` contém o código do site para a exibição dos gráficos.

Para este projeto é necessário o Platformio e o Node.JS.

## Executar o projeto

Para executar o projeto na sua máquina é necessário clonar o repositório. Para executar o repositório é necessário ter o [Git](https://git-scm.com/) instalado.

Com o Git, execute no seu terminal:

```sh
$ git clone https://github.com/gabirum/water-quality.git
```

A partir disso, abra o projeto no editor e o Platformio configurará o projeto do ESP32, já para baixar as dependências do site, execute no terminal o seguinte:

```sh
$ cd site
$ npm i
```

Baixadas as dependências, para visualizar o site em modo de desenvolvimento, execute no terminal:

```sh
$ npm run dev
```

E só abrir http://localhost:5173

## Recursos usados

Os recursos usados do Firebase foram: o Firestore (banco NoSQL) e o Firebase Hosting (serviço de lançamento de sites).

> Não é do escopo desse README a configuração do projeto no firebase
