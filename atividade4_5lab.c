#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define LARGURA_JANELA 800
#define ALTURA_JANELA 600
#define RAIO_JOGADOR 20.0f
#define MAX_ENTIDADES 15
typedef enum {
    ENTIDADE_JOGADOR,
    ENTIDADE_INIMIGO,
    ENTIDADE_ITEM
} TipoEntidade;
typedef union {
    int dano;
    int valor;
} ExtraEntidade;
typedef struct {
    TipoEntidade tipo;
    Vector2 pos;
    float raio;
    int vida;
    Color cor;
    ExtraEntidade extra;
} Entidade;
Entidade *vetorEntidades[MAX_ENTIDADES];
int totalEntidades = 0;
Entidade *criarEntidade(TipoEntidade tipo, Vector2 pos) {
    Entidade *e = (Entidade *)malloc(sizeof(Entidade));
    if (e == NULL)
        return NULL;
    e->tipo = tipo;
    e->pos = pos;
    e->raio = (tipo == ENTIDADE_JOGADOR) ?
              RAIO_JOGADOR :
              (tipo == ENTIDADE_INIMIGO) ? 15.0f : 8.0f;
    switch (tipo) {
        case ENTIDADE_JOGADOR:
            e->vida = 100;
            e->cor = BLUE;
            break;
        case ENTIDADE_INIMIGO:
            e->vida = 40;
            e->cor = MAROON;
            e->extra.dano = GetRandomValue(5, 15);
            break;
        case ENTIDADE_ITEM:
            e->vida = 1;
            e->cor = GOLD;
            e->extra.valor = GetRandomValue(5, 20);
            break;
    }
    return e;
}
void adicionarEntidade(Entidade *e) {
    if (e == NULL)
        return;
    if (totalEntidades >= MAX_ENTIDADES) {
        free(e);
        return;
    }
    vetorEntidades[totalEntidades] = e;
    totalEntidades++;
}
void removerEntidade(int indice) {
    if (indice < 0 || indice >= totalEntidades)
        return;
    free(vetorEntidades[indice]);
    vetorEntidades[indice] =
        vetorEntidades[totalEntidades - 1];
    totalEntidades--;
}
void ordenarPorDistancia(Entidade *jogador) {
    for (int i = 1; i < totalEntidades - 1; i++) {

        for (int j = i + 1; j < totalEntidades; j++) {

            float dx1 =
                vetorEntidades[i]->pos.x - jogador->pos.x;

            float dy1 =
                vetorEntidades[i]->pos.y - jogador->pos.y;

            float distancia1 =
                sqrtf(dx1 * dx1 + dy1 * dy1);

            float dx2 =
                vetorEntidades[j]->pos.x - jogador->pos.x;

            float dy2 =
                vetorEntidades[j]->pos.y - jogador->pos.y;

            float distancia2 =
                sqrtf(dx2 * dx2 + dy2 * dy2);

            if (distancia2 < distancia1) {

                Entidade *tmp = vetorEntidades[i];

                vetorEntidades[i] =
                    vetorEntidades[j];

                vetorEntidades[j] = tmp;
            }
        }
    }
}

void desenharEntidade(Entidade *e) {

    DrawCircleV(e->pos, e->raio, e->cor);

    if (e->tipo == ENTIDADE_JOGADOR) {

        DrawText(
            TextFormat("HP: %d", e->vida),
            e->pos.x - 25,
            e->pos.y - 35,
            14,
            BLACK
        );
    }
    if (e->tipo == ENTIDADE_INIMIGO) {

        DrawText(
            TextFormat("HP: %d", e->vida),
            e->pos.x - 15,
            e->pos.y - 28,
            14,
            BLACK
        );
    }

    if (e->tipo == ENTIDADE_ITEM) {

        DrawText(
            TextFormat("+%d", e->extra.valor),
            e->pos.x - 10,
            e->pos.y - 25,
            14,
            BLACK
        );
    }
}

int main(void) {

    srand((unsigned int)time(NULL));

    InitWindow(
        LARGURA_JANELA,
        ALTURA_JANELA,
        "Atividade 5 - Vetores de Ponteiros para Struct"
    );

    SetTargetFPS(60);

    Entidade *jogador =
        criarEntidade(
            ENTIDADE_JOGADOR,
            (Vector2){
                LARGURA_JANELA / 2.0f,
                ALTURA_JANELA / 2.0f
            }
        );

    adicionarEntidade(jogador);

    for (int i = 0; i < 6; i++) {

        Entidade *inimigo =
            criarEntidade(
                ENTIDADE_INIMIGO,
                (Vector2){
                    GetRandomValue(30, LARGURA_JANELA - 30),
                    GetRandomValue(60, ALTURA_JANELA - 30)
                }
            );

        adicionarEntidade(inimigo);
    }

    for (int i = 0; i < 3; i++) {

        Entidade *item =
            criarEntidade(
                ENTIDADE_ITEM,
                (Vector2){
                    GetRandomValue(30, LARGURA_JANELA - 30),
                    GetRandomValue(60, ALTURA_JANELA - 30)
                }
            );

        adicionarEntidade(item);
    }

    while (!WindowShouldClose()) {

        float vel = 250.0f * GetFrameTime();

        if (IsKeyDown(KEY_RIGHT))
            jogador->pos.x += vel;

        if (IsKeyDown(KEY_LEFT))
            jogador->pos.x -= vel;

        if (IsKeyDown(KEY_UP))
            jogador->pos.y -= vel;

        if (IsKeyDown(KEY_DOWN))
            jogador->pos.y += vel;

        if (IsKeyPressed(KEY_N)) {

            if (totalEntidades < MAX_ENTIDADES) {

                Entidade *novoItem =
                    criarEntidade(
                        ENTIDADE_ITEM,
                        (Vector2){
                            GetRandomValue(30, LARGURA_JANELA - 30),
                            GetRandomValue(60, ALTURA_JANELA - 30)
                        }
                    );

                adicionarEntidade(novoItem);
            }
        }

        ordenarPorDistancia(jogador);

        BeginDrawing();

            ClearBackground(RAYWHITE);

            for (int i = 0; i < totalEntidades; i++) {

                desenharEntidade(vetorEntidades[i]);
            }

            DrawText(
                "Setas: mover jogador",
                10,
                10,
                20,
                DARKGRAY
            );

            DrawText(
                "N: criar novo item",
                10,
                35,
                20,
                DARKGRAY
            );
            DrawText(
                "Entidade mais proxima:",
                10,
                60,
                18,
                DARKGRAY
            );
            if (totalEntidades > 1) {
                if (vetorEntidades[1]->tipo == ENTIDADE_INIMIGO) {

                    DrawText(
                        "Inimigo",
                        10,
                        82,
                        18,
                        MAROON
                    );
                }
                if (vetorEntidades[1]->tipo == ENTIDADE_ITEM) {

                    DrawText(
                        "Item",
                        10,
                        82,
                        18,
                        GOLD
                    );
                }
            }

        EndDrawing();
    }
    for (int i = 0; i < totalEntidades; i++) {
        free(vetorEntidades[i]);
    }
    CloseWindow();
    return 0;
}
