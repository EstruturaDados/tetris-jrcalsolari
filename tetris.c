#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Estrutura para representar uma peça
typedef struct {
    char nome[2];  // Tipo da peça: 'I', 'O', 'T', 'L', etc.
    int id;        // Identificador único
} Peca;

// Estrutura para a fila circular
typedef struct {
    Peca pecas[5];
    int frente;
    int tras;
    int tamanho;
    int capacidade;
} FilaCircular;

// Estrutura para a pilha
typedef struct {
    Peca pecas[3];
    int topo;
    int capacidade;
} Pilha;

// Variáveis globais para controle de IDs
int proximoId = 1;

// Função para gerar uma peça automaticamente
Peca gerarPeca() {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L', 'S', 'Z', 'J'};
    int indice = rand() % 7;
    
    nova.nome[0] = tipos[indice];
    nova.nome[1] = '\0';
    nova.id = proximoId++;
    
    return nova;
}

// ========== FUNÇÕES DA FILA CIRCULAR ==========

void inicializarFila(FilaCircular *fila) {
    fila->frente = 0;
    fila->tras = 0;
    fila->tamanho = 0;
    fila->capacidade = 5;
}

int filaVazia(FilaCircular *fila) {
    return fila->tamanho == 0;
}

int filaCheia(FilaCircular *fila) {
    return fila->tamanho == fila->capacidade;
}

void enqueue(FilaCircular *fila, Peca peca) {
    if (filaCheia(fila)) {
        printf("Erro: Fila cheia!\n");
        return;
    }
    
    fila->pecas[fila->tras] = peca;
    fila->tras = (fila->tras + 1) % fila->capacidade;
    fila->tamanho++;
}

Peca dequeue(FilaCircular *fila) {
    Peca pecaVazia = {"", -1};
    
    if (filaVazia(fila)) {
        printf("Erro: Fila vazia!\n");
        return pecaVazia;
    }
    
    Peca peca = fila->pecas[fila->frente];
    fila->frente = (fila->frente + 1) % fila->capacidade;
    fila->tamanho--;
    
    return peca;
}

void mostrarFila(FilaCircular *fila) {
    printf("\n=== FILA DE PEÇAS FUTURAS ===\n");
    if (filaVazia(fila)) {
        printf("Fila vazia.\n");
        return;
    }
    
    int i = fila->frente;
    int count = 0;
    printf("Ordem: ");
    while (count < fila->tamanho) {
        printf("[%s(id:%d)] ", fila->pecas[i].nome, fila->pecas[i].id);
        i = (i + 1) % fila->capacidade;
        count++;
    }
    printf("\n");
}

// ========== FUNÇÕES DA PILHA ==========

void inicializarPilha(Pilha *pilha) {
    pilha->topo = -1;
    pilha->capacidade = 3;
}

int pilhaVazia(Pilha *pilha) {
    return pilha->topo == -1;
}

int pilhaCheia(Pilha *pilha) {
    return pilha->topo == pilha->capacidade - 1;
}

void push(Pilha *pilha, Peca peca) {
    if (pilhaCheia(pilha)) {
        printf("Erro: Pilha cheia! Capacidade máxima: 3 peças.\n");
        return;
    }
    
    pilha->topo++;
    pilha->pecas[pilha->topo] = peca;
}

Peca pop(Pilha *pilha) {
    Peca pecaVazia = {"", -1};
    
    if (pilhaVazia(pilha)) {
        printf("Erro: Pilha vazia!\n");
        return pecaVazia;
    }
    
    Peca peca = pilha->pecas[pilha->topo];
    pilha->topo--;
    
    return peca;
}

void mostrarPilha(Pilha *pilha) {
    printf("\n=== PILHA DE RESERVA ===\n");
    if (pilhaVazia(pilha)) {
        printf("Pilha vazia.\n");
        return;
    }
    
    printf("Topo -> ");
    for (int i = pilha->topo; i >= 0; i--) {
        printf("[%s(id:%d)] ", pilha->pecas[i].nome, pilha->pecas[i].id);
    }
    printf("\n");
}

// ========== FUNÇÕES PARA NÍVEL MESTRE ==========

// Trocar peça da frente da fila com o topo da pilha
int trocarFrenteComTopo(FilaCircular *fila, Pilha *pilha) {
    if (filaVazia(fila)) {
        printf("Erro: Fila vazia! Não é possível trocar.\n");
        return 0;
    }
    
    if (pilhaVazia(pilha)) {
        printf("Erro: Pilha vazia! Não é possível trocar.\n");
        return 0;
    }
    
    Peca temp = fila->pecas[fila->frente];
    fila->pecas[fila->frente] = pilha->pecas[pilha->topo];
    pilha->pecas[pilha->topo] = temp;
    
    printf("Troca realizada: peça da frente da fila com o topo da pilha.\n");
    return 1;
}

// Desfazer última jogada (adicionar peça de volta à frente da fila)
int desfazerUltimaJogada(FilaCircular *fila, Peca ultimaPeca) {
    if (filaCheia(fila)) {
        printf("Erro: Fila cheia! Não é possível desfazer.\n");
        return 0;
    }
    
    // Insere na frente da fila
    fila->frente = (fila->frente - 1 + fila->capacidade) % fila->capacidade;
    fila->pecas[fila->frente] = ultimaPeca;
    fila->tamanho++;
    
    printf("Última jogada desfeita: peça [%s(id:%d)] retornou à fila.\n", 
           ultimaPeca.nome, ultimaPeca.id);
    return 1;
}

// Inverter fila com pilha (trocar 3 primeiros da fila com os 3 da pilha)
int inverterFilaComPilha(FilaCircular *fila, Pilha *pilha) {
    if (pilha->topo != 2) {
        printf("Erro: A pilha deve ter exatamente 3 peças para inverter.\n");
        return 0;
    }
    
    if (fila->tamanho < 3) {
        printf("Erro: A fila deve ter pelo menos 3 peças para inverter.\n");
        return 0;
    }
    
    // Troca os 3 primeiros da fila com os 3 da pilha
    Peca temp[3];
    
    // Salva os 3 primeiros da fila
    int idx = fila->frente;
    for (int i = 0; i < 3; i++) {
        temp[i] = fila->pecas[idx];
        idx = (idx + 1) % fila->capacidade;
    }
    
    // Coloca os 3 da pilha na fila
    idx = fila->frente;
    for (int i = 0; i < 3; i++) {
        fila->pecas[idx] = pilha->pecas[2 - i];
        idx = (idx + 1) % fila->capacidade;
    }
    
    // Coloca os 3 da fila na pilha
    for (int i = 0; i < 3; i++) {
        pilha->pecas[i] = temp[2 - i];
    }
    
    printf("Inversão realizada: 3 primeiros da fila trocados com os 3 da pilha.\n");
    return 1;
}

// ========== NÍVEIS DO JOGO ==========

void nivelNovato() {
    FilaCircular fila;
    inicializarFila(&fila);
    
    // Inicializar fila com 5 peças
    printf("Inicializando fila com 5 peças...\n");
    for (int i = 0; i < 5; i++) {
        enqueue(&fila, gerarPeca());
    }
    
    int opcao;
    do {
        mostrarFila(&fila);
        printf("\n=== MENU - NÍVEL NOVATO ===\n");
        printf("1 - Jogar peça (remover da frente)\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        
        if (opcao == 1) {
            if (!filaVazia(&fila)) {
                Peca pecaJogada = dequeue(&fila);
                printf("\nPeça jogada: [%s(id:%d)]\n", pecaJogada.nome, pecaJogada.id);
                
                // Insere nova peça automaticamente
                enqueue(&fila, gerarPeca());
                printf("Nova peça adicionada à fila.\n");
            } else {
                printf("Fila vazia! Não há peças para jogar.\n");
            }
        } else if (opcao != 0) {
            printf("Opção inválida!\n");
        }
    } while (opcao != 0);
    
    printf("\nObrigado por jogar!\n");
}

void nivelAventureiro() {
    FilaCircular fila;
    Pilha pilha;
    
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    
    // Inicializar fila com 5 peças
    printf("Inicializando fila com 5 peças...\n");
    for (int i = 0; i < 5; i++) {
        enqueue(&fila, gerarPeca());
    }
    
    int opcao;
    do {
        mostrarFila(&fila);
        mostrarPilha(&pilha);
        
        printf("\n=== MENU - NÍVEL AVENTUREIRO ===\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça (enviar da fila para pilha)\n");
        printf("3 - Usar peça reservada (remover do topo da pilha)\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        
        if (opcao == 1) {
            if (!filaVazia(&fila)) {
                Peca pecaJogada = dequeue(&fila);
                printf("\nPeça jogada: [%s(id:%d)]\n", pecaJogada.nome, pecaJogada.id);
                
                // Mantém a fila sempre com 5 peças
                enqueue(&fila, gerarPeca());
                printf("Nova peça adicionada à fila.\n");
            } else {
                printf("Fila vazia! Não há peças para jogar.\n");
            }
        } else if (opcao == 2) {
            if (!filaVazia(&fila)) {
                if (!pilhaCheia(&pilha)) {
                    Peca pecaReservada = dequeue(&fila);
                    push(&pilha, pecaReservada);
                    printf("\nPeça [%s(id:%d)] reservada na pilha.\n", 
                           pecaReservada.nome, pecaReservada.id);
                    
                    // Mantém a fila sempre com 5 peças
                    enqueue(&fila, gerarPeca());
                    printf("Nova peça adicionada à fila.\n");
                } else {
                    printf("Pilha cheia! Capacidade máxima: 3 peças.\n");
                }
            } else {
                printf("Fila vazia! Não há peças para reservar.\n");
            }
        } else if (opcao == 3) {
            if (!pilhaVazia(&pilha)) {
                Peca pecaUsada = pop(&pilha);
                printf("\nPeça reservada usada: [%s(id:%d)]\n", 
                       pecaUsada.nome, pecaUsada.id);
            } else {
                printf("Pilha vazia! Não há peças reservadas.\n");
            }
        } else if (opcao != 0) {
            printf("Opção inválida!\n");
        }
    } while (opcao != 0);
    
    printf("\nObrigado por jogar!\n");
}

void nivelMestre() {
    FilaCircular fila;
    Pilha pilha;
    
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    
    // Inicializar fila com 5 peças
    printf("Inicializando fila com 5 peças...\n");
    for (int i = 0; i < 5; i++) {
        enqueue(&fila, gerarPeca());
    }
    
    // Histórico para desfazer (armazena última peça jogada)
    Peca ultimaPecaJogada = {"", -1};
    int temHistorico = 0;
    
    int opcao;
    do {
        mostrarFila(&fila);
        mostrarPilha(&pilha);
        
        printf("\n=== MENU - NÍVEL MESTRE ===\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("4 - Trocar peça da frente com topo da pilha\n");
        printf("5 - Desfazer última jogada\n");
        printf("6 - Inverter fila com pilha\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        
        if (opcao == 1) {
            if (!filaVazia(&fila)) {
                ultimaPecaJogada = dequeue(&fila);
                temHistorico = 1;
                printf("\nPeça jogada: [%s(id:%d)]\n", 
                       ultimaPecaJogada.nome, ultimaPecaJogada.id);
                
                // Mantém a fila sempre com 5 peças
                enqueue(&fila, gerarPeca());
                printf("Nova peça adicionada à fila.\n");
            } else {
                printf("Fila vazia! Não há peças para jogar.\n");
            }
        } else if (opcao == 2) {
            if (!filaVazia(&fila)) {
                if (!pilhaCheia(&pilha)) {
                    Peca pecaReservada = dequeue(&fila);
                    push(&pilha, pecaReservada);
                    printf("\nPeça [%s(id:%d)] reservada na pilha.\n", 
                           pecaReservada.nome, pecaReservada.id);
                    
                    // Mantém a fila sempre com 5 peças
                    enqueue(&fila, gerarPeca());
                    printf("Nova peça adicionada à fila.\n");
                } else {
                    printf("Pilha cheia! Capacidade máxima: 3 peças.\n");
                }
            } else {
                printf("Fila vazia! Não há peças para reservar.\n");
            }
        } else if (opcao == 3) {
            if (!pilhaVazia(&pilha)) {
                Peca pecaUsada = pop(&pilha);
                printf("\nPeça reservada usada: [%s(id:%d)]\n", 
                       pecaUsada.nome, pecaUsada.id);
            } else {
                printf("Pilha vazia! Não há peças reservadas.\n");
            }
        } else if (opcao == 4) {
            trocarFrenteComTopo(&fila, &pilha);
        } else if (opcao == 5) {
            if (temHistorico) {
                if (desfazerUltimaJogada(&fila, ultimaPecaJogada)) {
                    temHistorico = 0;
                }
            } else {
                printf("Não há jogada anterior para desfazer.\n");
            }
        } else if (opcao == 6) {
            inverterFilaComPilha(&fila, &pilha);
        } else if (opcao != 0) {
            printf("Opção inválida!\n");
        }
    } while (opcao != 0);
    
    printf("\nObrigado por jogar!\n");
}

// ========== FUNÇÃO PRINCIPAL ==========

int main() {
    srand(time(NULL)); // Inicializa gerador de números aleatórios
    
    printf("╔═══════════════════════════════════════════════════════╗\n");
    printf("║     DESAFIO TETRIS STACK - CONTROLE DE PEÇAS         ║\n");
    printf("║              ByteBros - Estruturas de Dados          ║\n");
    printf("╚═══════════════════════════════════════════════════════╝\n\n");
    
    printf("Escolha o nível de dificuldade:\n");
    printf("1 - Nível Novato (Fila Circular)\n");
    printf("2 - Nível Aventureiro (Fila + Pilha)\n");
    printf("3 - Nível Mestre (Integração Total)\n");
    printf("0 - Sair\n");
    printf("\nEscolha uma opção: ");
    
    int nivel;
    scanf("%d", &nivel);
    
    switch (nivel) {
        case 1:
            printf("\n=== INICIANDO NÍVEL NOVATO ===\n");
            nivelNovato();
            break;
        case 2:
            printf("\n=== INICIANDO NÍVEL AVENTUREIRO ===\n");
            nivelAventureiro();
            break;
        case 3:
            printf("\n=== INICIANDO NÍVEL MESTRE ===\n");
            nivelMestre();
            break;
        case 0:
            printf("\nAté logo!\n");
            break;
        default:
            printf("\nOpção inválida!\n");
            break;
    }
    
    return 0;
}
