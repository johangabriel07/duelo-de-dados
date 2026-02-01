#define _WIN32_WINNT 0x0600
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#pragma comment(lib, "Ws2_32.lib")
#define SERVER "127.0.0.1"
#define PORT "12347"
#define BUF 512

void limparBuffer(){
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
  {
  }
}

int main(){
  SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
  WSADATA wsa;
  WSAStartup(MAKEWORD(2, 2), &wsa);
  srand((unsigned int)time(NULL));

  struct addrinfo hints = {0}, *res = NULL;
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  getaddrinfo(SERVER, PORT, &hints, &res);

  SOCKET s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  connect(s, res->ai_addr, (int)res->ai_addrlen);
  freeaddrinfo(res);

  printf("Conectado ao servidor.\n");

  char buf[BUF];
  int id = 0, gameOver = 0, rodada = 1;

  while (!gameOver){
    int r = recv(s, buf, BUF - 1, 0);
    if (r <= 0)
      break;
    buf[r] = '\0';

    if (strncmp(buf, "CLEAR", 5) == 0){
      system("cls");
      continue;
    }

    if(strncmp(buf, "ASSIGN|", 7) == 0){
      sscanf(buf, "ASSIGN|%d", &id);
      printf("\nVocê é o jogador %d.\n", id);
      continue;
    }

    if(strncmp(buf, "TURN|", 5) ==0){
      int turno;
      char msg[128];
      sscanf(buf, "TURN|%d|%[^\n]", &turno, msg);
      printf("\n%s\n", msg);
      printf("Pressione Enter para jogar o dado...");
      getchar();

      int valor = rand() % 20 + 1;
      if (valor == 5){
        printf("\nBônus de sorte! Dobro de Pontos!\n");
        printf("Você tirou D5 e evoluiu para D10!\n");
        valor = 10;
      }else if (valor == 10){
        printf("\nBônus de sorte! Dobro de Pontos!\n");
        printf("Você tirou D10 e evoluiu para D20!\n");
        valor = 20;
      }

      printf("\nVocê tirou D%d!\n", valor);

      char msgRoll[32];
      sprintf(msgRoll, "ROLL|%d|%d\n", id, valor);
      send(s, msgRoll, strlen(msgRoll), 0);
      continue;
    }

    if(strncmp(buf, "INFO|", 5) == 0){
      printf("\n%s\n", buf + 5);
      continue;
    }

    if (strncmp(buf, "SURPRESA_ESCOLHIDO", 18)== 0){
      printf("\n--RODADA SURPRESA: TROCA TROCA DE PONTOS--\n");
      printf("\nNesta rodada, você pode escolher trocar sua pontuação com a do oponente.\n");
      printf("\nDeseja trocar sua pontuação com a do oponente? (s/n): ");
      char escolha = 0;
      while (escolha != 's' && escolha != 'S' && escolha != 'n' && escolha != 'N')
        escolha = getchar();
      limparBuffer();

      char msg[32];
      sprintf(msg, "SURPRESA|%d|%c\n", id, escolha);
      send(s, msg, strlen(msg), 0);
      continue;
    }

    if (strncmp(buf, "SURPRESA_OUTRO", 14) == 0)
    {
      printf("\n--RODADA SURPRESA: TROCA TROCA DE PONTOS--\n");
      printf("\nNesta rodada, seu oponente foi sorteado e está decidindo se vai trocar os pontos, aguarde...\n");
      continue;
    }

    if (strstr(buf, "Resultado da Rodada Surpresa:") != NULL){
      printf("%s", buf);
      printf("\nPressione Enter para continuar...");
      limparBuffer();
      getchar();
      continue;
    }

    if (strncmp(buf, "ROUND_RESULT|", 13) ==0){
      int vencedor, p1, p2, r1, r2;
      sscanf(buf, "ROUND_RESULT|%d|%d|%d|%d|%d", &vencedor, &p1, &p2, &r1, &r2);

      printf("\n------------------------------\n");
      printf("Placar da Rodada!\n");
      printf("\nJogador 1 tirou: D%d\n", r1);
      printf("Jogador 2 tirou: D%d\n", r2);

      if (r1 == r2) {
        printf("\nEmpate! Jogadores vão jogar novamente...\n");
      }
      else{
        printf("\nVencedor da rodada: Jogador %d\n", vencedor);
        printf("\nPlacar: Jogador 1 = %d | Jogador 2 = %d\n", p1, p2);
        if (vencedor == id)
          printf("\nVocê venceu esta rodada!\n");
        else
          printf("\nVocê perdeu esta rodada.\n");
        rodada++;
      }

      printf("\nPressione Enter para continuar...");
      limparBuffer();
      getchar();
      if (rodada <= 3)
        system("cls");
      continue;
    }

    if (strncmp(buf, "FIM DE JOGO|", 12) == 0)
    {
      int vencedorFinal, p1, p2;
      if (sscanf(buf, "FIM DE JOGO|%d|%d|%d", &vencedorFinal, &p1, &p2) == 3)
      {
        system("cls");
        printf("\n");
        printf("\nPlacar final:\n");
        printf("Jogador 1: %d\n", p1);
        printf("Jogador 2: %d\n", p2);

        if (vencedorFinal == id)
          printf("\nParabéns! Você venceu o jogo!\n");
        else
          printf("\nVocê perdeu. Boa sorte na próxima!\n");

        printf("\nPressione Enter para encerrar...");
        limparBuffer();
        getchar();
      }
      gameOver = 1;
      continue;
    }

    printf("%s", buf);
  }

  if (!gameOver){
    system("cls");
    printf("\nO servidor encerrou a conexão.\n");
    printf("Se você não viu o placar final, ele pode não ter sido recebido a tempo.\n");
  }

  printf("\nPressione Enter para sair...");
  limparBuffer();
  getchar();
  closesocket(s);
  WSACleanup();
  return 0;
}
