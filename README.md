# 🎲 Duelo de Dados

### Projeto desenvolvido para a disciplina de Sistemas Operacionais

O **Duelo de Dados** é um jogo multiplayer de terminal feito em **C**, utilizando **Sockets TCP (Winsock)** no modelo **cliente-servidor**.

A ideia é simples: dois jogadores se conectam ao servidor, rolam dados por turnos e disputam quem faz mais pontos. Apesar de ser um jogo leve, ele foi criado para praticar conceitos importantes como **redes, threads, sincronização e concorrência**, simulando comunicação real entre processos.

Além das rodadas normais, o jogo possui bônus de sorte e uma **rodada surpresa**, que pode virar o placar de última hora.

---

## 🕹️ Como o jogo funciona

* Dois jogadores entram na partida
* Cada um rola o dado na sua vez
* Quem tirar o maior valor vence a rodada
* Se empatar, jogam novamente
* A partida tem 3 rodadas
* Na última rodada, um jogador pode escolher trocar a pontuação com o oponente
* Quem tiver mais pontos no final ganha

Simples, mas com aquele fator surpresa 😄

---

## ⚙️ Tecnologias utilizadas

* Linguagem C
* Winsock2 (TCP/IP)
* Programação com sockets
* Threads (CreateThread)
* Mutex para sincronização
* Execução via terminal (console)

---

## 📁 Estrutura do projeto

```
duelo-de-dados/
│
├── server.c   -> responsável por gerenciar a partida
├── client.c   -> programa usado pelos jogadores
└── README.md
```

---

## 💻 Como compilar (Windows)

Usando **GCC / MinGW**:

Servidor:

```
gcc server.c -o server -lws2_32
```

Cliente:

```
gcc client.c -o client -lws2_32
```

---

## ▶️ Como executar

1. Inicie o servidor:

```
server.exe
```

2. Abra dois terminais diferentes e execute:

```
client.exe
client.exe
```

Cada terminal será um jogador conectado ao servidor.

---

## 🎮 Controles

* Pressione **Enter** para rolar o dado
* Na rodada surpresa:

  * `s` → trocar pontos
  * `n` → manter pontos

---

## 📚 O que foi praticado neste projeto

Durante o desenvolvimento foram aplicados conceitos de:

* Cliente-servidor
* Comunicação em rede com TCP
* Concorrência e múltiplas threads
* Sincronização com mutex
* Organização de lógica de jogo em C
* Interação via terminal

---

---

## 👨‍💻 Autores

* Eduarda Pereira de Moraes
* Johan Gabriel da Silva dos Santos

Projeto acadêmico desenvolvido para a disciplina de **Sistemas Operacionais**.
