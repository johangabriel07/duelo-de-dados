# Duelo de Dados

Projeto desenvolvido para a disciplina de Sistemas Operacionais.

Duelo de Dados é um jogo multiplayer de terminal feito em C, utilizando sockets TCP (Winsock) no modelo cliente-servidor. Dois jogadores se conectam ao servidor local e disputam partidas baseadas na rolagem de dados, acumulando pontos a cada rodada.

O objetivo principal do projeto foi praticar programação em rede, comunicação entre processos, uso de threads e sincronização, aplicando conceitos vistos em aula de forma prática.

## Como o jogo funciona

Dois jogadores se conectam ao servidor.
Cada jogador rola o dado na sua vez.
Quem tirar o maior valor vence a rodada.
Se houver empate, ambos jogam novamente.
A partida possui três rodadas no total.
Na última rodada existe um evento surpresa onde um jogador pode escolher trocar sua pontuação com a do oponente.
Ao final, vence quem tiver mais pontos.

## Tecnologias utilizadas

* Linguagem C
* Winsock2 (TCP/IP)
* Sockets
* Threads (CreateThread)
* Mutex para sincronização
* Execução via terminal

## Estrutura do projeto

```
duelo-de-dados/
├── server.c
├── client.c
└── README.md
```

## Como compilar (Windows)

Usando GCC ou MinGW:

Compilar o servidor:

```
gcc server.c -o server -lws2_32
```

Compilar o cliente:

```
gcc client.c -o client -lws2_32
```

## Como executar

Primeiro execute o servidor:

```
server.exe
```

Depois abra dois terminais diferentes e execute o cliente em cada um:

```
client.exe
client.exe
```

Cada terminal representará um jogador.

## Conceitos praticados

* Comunicação cliente-servidor
* Programação em rede com TCP
* Concorrência e múltiplas threads
* Sincronização com mutex
* Lógica de jogo em C

## Autores

Eduarda Pereira de Moraes
Johan Gabriel da Silva dos Santos

Projeto acadêmico da disciplina de Sistemas Operacionais.
