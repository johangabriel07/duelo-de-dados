Jogo Duelo de Dados - Realizado para a disciplina de Sistemas Operacionais

Duelo de Dados é um jogo multiplayer de terminal desenvolvido em **linguagem C**, utilizando **Sockets TCP (Winsock)** no modelo **cliente-servidor**.

Dois jogadores se conectam ao servidor local, rolam dados por turnos e disputam rodadas para ver quem faz mais pontos. O jogo possui bônus aleatórios e uma rodada surpresa estratégica que pode mudar totalmente o resultado da partida.

Projeto criado para praticar **programação em C, redes de computadores, threads e concorrência**.

---

## 🕹️ Como funciona

* 2 jogadores se conectam ao servidor
* Cada rodada ambos rolam um dado
* Maior valor vence a rodada
* Empate → rolam novamente
* São 3 rodadas no total
* Na última rodada existe um evento surpresa onde um jogador pode trocar os pontos com o oponente
* Quem tiver mais pontos no final vence

---

## ⚙️ Tecnologias utilizadas

* Linguagem C
* Winsock2 (TCP/IP)
* Sockets
* Threads (CreateThread)
* Mutex (sincronização)
* Execução via terminal (console)

---

## 📁 Estrutura do projeto

```
duelo-de-dados/
│
├── server.c   -> servidor do jogo
├── client.c   -> cliente do jogador
└── README.md
```

---

## 💻 Como compilar (Windows)

### Usando GCC / MinGW

Compile o servidor:

```
gcc server.c -o server -lws2_32
```

Compile o cliente:

```
gcc client.c -o client -lws2_32
```

---

## ▶️ Como executar

### 1. Inicie o servidor

```
server.exe
```

### 2. Abra dois terminais e execute o cliente em cada um

```
client.exe
client.exe
```

Cada janela será um jogador.

---

## 🎮 Controles

* Pressione **Enter** para rolar o dado
* Na rodada surpresa:

  * `s` → trocar pontos
  * `n` → manter pontos

---

## 📚 Conceitos praticados

Este projeto envolve:

* Comunicação cliente-servidor
* Programação em rede com TCP
* Sincronização de múltiplas threads
* Manipulação de sockets
* Lógica de jogo
* Estruturas básicas em C

---

## 📸 Demonstração

Você pode adicionar prints do jogo aqui:

```
![Gameplay](prints/jogo.png)
```

---

## 👨‍💻 Autores

Eduarda Pereira de Morares
Johan Gabriel da Silva dos Santos
Projeto acadêmico para a disciplina de Sistemas Operacionais
