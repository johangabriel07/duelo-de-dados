#define _WIN32_WINNT 0x0600
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#pragma comment(lib, "Ws2_32.lib")
#define PORT "12347"
#define BUF 512

typedef struct
{
  SOCKET socket;
  int id;
  int roll;
  int score;
  char escolha;
} Player;

Player players[2];
int readyCount = 0;
HANDLE hMutex;

DWORD WINAPI clientHandler(LPVOID param){
  Player *p = (Player *)param;
  char buf[BUF];
  while (1){
    int r = recv(p->socket, buf, BUF - 1, 0);
    if (r <= 0)
    {
      printf("\nJogador %d desconectou.\n", p->id);
      return 0;
    }
    buf[r] = '\0';

    if (strncmp(buf, "ROLL|", 5) == 0) {
      int id, val;
      sscanf(buf, "ROLL|%d|%d", &id, &val);
      WaitForSingleObject(hMutex, INFINITE);
      p->roll = val;
      readyCount++;
      ReleaseMutex(hMutex);
      printf("Jogador %d: Tirou %d\n", p->id, val);
    }
    else if (strncmp(buf, "SURPRESA|", 9) == 0) {
      int id;
      char esc;
      sscanf(buf, "SURPRESA|%d|%c", &id, &esc);
      WaitForSingleObject(hMutex, INFINITE);
      p->escolha = esc;
      readyCount++;
      ReleaseMutex(hMutex);
    }
  }
}

int main(){
  SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

  WSADATA wsa;
  WSAStartup(MAKEWORD(2, 2), &wsa);
  srand((unsigned int)time(NULL));
  struct addrinfo hints = {0}, *res;
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  getaddrinfo(NULL, PORT, &hints, &res);

  SOCKET server = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  bind(server, res->ai_addr, (int)res->ai_addrlen);
  listen(server, 2);
  freeaddrinfo(res);
  printf("Servidor aguardando jogadores...\n\n");
  hMutex = CreateMutex(NULL, FALSE, NULL);

  for(int i = 0; i < 2; i++){
    SOCKET c = accept(server, NULL, NULL);
    players[i].socket = c;
    players[i].id = i + 1;
    players[i].score = 0;
    players[i].roll = 0;
    players[i].escolha = 'n';
    char msg[32];
    sprintf(msg, "ASSIGN|%d\n", players[i].id);
    send(c, msg, strlen(msg), 0);
    CreateThread(NULL, 0, clientHandler, &players[i], 0, NULL);
    printf("Jogador %d conectado.\n", i + 1);
  }
  for (int round = 1; round <= 3; round++) {
    printf("\n--- Rodada %d ---\n", round);
    players[0].roll = players[1].roll = 0;
    readyCount = 0;

    printf("\nVez do jogador 1\n");
    send(players[0].socket, "TURN|1|Sua vez de jogar o dado!\n", 32, 0);
    send(players[1].socket, "INFO|Jogador 1 está jogando, aguarde...\n", 42, 0);
    while (readyCount < 1)
      Sleep(100);
    readyCount = 0;

    printf("\nVez do jogador 2\n");
    send(players[1].socket, "TURN|2|Sua vez de jogar o dado!\n", 32, 0);
    send(players[0].socket, "INFO|Jogador 2 está jogando, aguarde...\n", 42, 0);
    while (readyCount < 1)
      Sleep(100);

    if (round ==3){
      int esc = rand() % 2;
      int outro = 1 - esc;
      printf("\nRODADA SURPRESA: TROCA TROCA DE PONTOS\n");
      send(players[esc].socket, "SURPRESA_ESCOLHIDO\n", 20, 0);
      send(players[outro].socket, "SURPRESA_OUTRO\n", 16, 0);
      readyCount = 0;
      while (readyCount < 1)
        Sleep(100);

      char escolha = players[esc].escolha;
      int antes1 = players[esc].roll, antes2 = players[outro].roll;

      if (escolha == 's' ||escolha == 'S'){
        int tmp = players[esc].roll;
        players[esc].roll = players[outro].roll;
        players[outro].roll = tmp;
        Sleep(2000);
        send(players[esc].socket, "CLEAR\n", 6, 0);
        send(players[outro].socket, "CLEAR\n", 6, 0);
        char msg1[512], msg2[512];
        sprintf(msg1, "\nResultado da Rodada Surpresa:\n");
        strcat(msg1, "\nVocê optou por trocar as pontuações!\n");
        sprintf(msg1 + strlen(msg1), "\nVocê tinha: D%d → ficou: D%d\n", antes1, players[esc].roll);
        sprintf(msg1 + strlen(msg1), "Seu oponente tinha: D%d → ficou: D%d\n", antes2, players[outro].roll);
        strcat(msg1, "\nTroca feita. ");
        strcat(msg1, (players[esc].roll > players[outro].roll ? "Deu bom pra você!\n" : "Não deu bom pra você!\n"));

        sprintf(msg2, "\nResultado da Rodada Surpresa:\n");
        strcat(msg2, "\nSeu oponente optou por trocar as pontuações!\n");
        sprintf(msg2 + strlen(msg2), "\nVocê tinha: D%d → ficou: D%d\n", antes2, players[outro].roll);
        sprintf(msg2 + strlen(msg2), "Seu oponente tinha: D%d → ficou: D%d\n", antes1, players[esc].roll);
        sprintf(msg2 + strlen(msg2), "\nJogador %d decidiu trocar. ", players[esc].id);
        strcat(msg2, (players[outro].roll > players[esc].roll ? "Você se deu bem!\n" : "Você se deu mal!\n"));

        send(players[esc].socket, msg1, strlen(msg1), 0);
        send(players[outro].socket, msg2, strlen(msg2), 0);
      }else{
        send(players[esc].socket, "CLEAR\n", 6, 0);
        send(players[outro].socket, "CLEAR\n", 6, 0);
        char msg1[512], msg2[512];
        sprintf(msg1, "\nResultado da Rodada Surpresa:\n");
        strcat(msg1, "\nVocê decidiu não trocar as pontuações.\n");
        sprintf(msg1 + strlen(msg1), "\nVocê ficou com D%d, o oponente com D%d.\n", players[esc].roll, players[outro].roll);
        strcat(msg1, "\nA troca teria sido vantajosa...\n");

        sprintf(msg2, "\nResultado da Rodada Surpresa:\n");
        strcat(msg2, "\nSeu oponente decidiu não trocar as pontuações.\n");
        sprintf(msg2 + strlen(msg2), "\nVocê tinha: D%d → ficou: D%d\n", antes2, players[outro].roll);
        sprintf(msg2 + strlen(msg2), "Seu oponente tinha: D%d → ficou: D%d\n", antes1, players[esc].roll);

        send(players[esc].socket, msg1, strlen(msg1), 0);
        send(players[outro].socket, msg2, strlen(msg2), 0);
      }
      continue;
    }

    while (players[0].roll == players[1].roll) {
      printf("\nEmpate! Jogadores vão jogar novamente...\n\n");
      players[0].roll = players[1].roll = 0;
      readyCount = 0;
      send(players[0].socket, "TURN|1|Empate! Jogue o dado novamente!\n", 40, 0);
      send(players[1].socket, "INFO|Jogador 1 está jogando novamente...\n", 44, 0);
      while (readyCount < 1)
        Sleep(100);
      readyCount = 0;
      send(players[1].socket, "TURN|2|Empate! Jogue o dado novamente!\n", 40, 0);
      send(players[0].socket, "INFO|Jogador 2 está jogando novamente...\n", 44, 0);
      while (readyCount < 1)
        Sleep(100);
    }

    int winner = (players[0].roll > players[1].roll) ? 1 : 2;
    if (winner == 1)
      players[0].score++;
    else
      players[1].score++;

    char result[128];
    sprintf(result, "ROUND_RESULT|%d|%d|%d|%d|%d\n", winner, players[0].score, players[1].score, players[0].roll, players[1].roll);
    for (int i = 0; i < 2; i++)
      send(players[i].socket, result, strlen(result), 0);

    printf("\nRodada %d - Vencedor: Jogador %d (placar %d-%d)\n", round, winner, players[0].score, players[1].score);
  }

  int finalWinner = (players[0].score > players[1].score) ? 1 : 2;
  Sleep(500);
  char endMsg[128];
  sprintf(endMsg, "FIM DE JOGO|%d|%d|%d\n", finalWinner, players[0].score, players[1].score);
  for (int i = 0; i < 2; i++)
    send(players[i].socket, endMsg, strlen(endMsg), 0);

  Sleep(1000);
  for (int i=0; i < 2; i++){
    shutdown(players[i].socket, SD_SEND);
    Sleep(300);
    closesocket(players[i].socket);
  }

  printf("\nJogo encerrado.\n");
  printf("Vencedor final: Jogador %d\n", finalWinner);
  closesocket(server);
  WSACleanup();
  return 0;
}
