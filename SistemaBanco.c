#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CONTAS 100
#define MAX_MOVIMENTACOES 100

// Definição da estrutura da conta bancária
typedef struct {
    char nome[30];
    int numero_conta;
    char cpf[12];
    char senha[20];
    float saldo;
    char movimentacoes[MAX_MOVIMENTACOES][50];
    int qtdMovimentacoes;
    bool ativa;  
} Conta;

// Array de contas
Conta contas[MAX_CONTAS];
int qtdContas = 0; // Quantidade de contas registradas
int numero_conta_global=1; // Variável global para definir número da conta

// Protótipos das funções
void menuPrincipal();
void registrarConta();
int acessarConta();
void menuConta(Conta *conta);
void depositar(Conta *conta);
void sacar(Conta *conta);
void consultarSaldo(Conta *conta);
void listarMovimentacoes(Conta *conta);
void desativarConta(Conta *conta);
int buscarConta(char *cpf);
void ativarConta(Conta *conta);
void mostrarDados(Conta *conta);

int main() {
    int opcao;

    do {
        menuPrincipal();
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer de entrada

        switch (opcao) {
            case 1:
                acessarConta();
                break;
            case 2:
                registrarConta();
                break;
            case 3:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 3);

    return 0;
}

// Função para exibir o menu principal
void menuPrincipal() {
    printf("\n===== MENU PRINCIPAL =====\n1. Acessar conta\n2. Registrar conta\n3. Sair\n==========================\n");
}

// Função para registrar uma nova conta
void registrarConta() {
    if (qtdContas >= MAX_CONTAS) {
        printf("Limite de contas registradas atingido!\n");
        return;
    }

    Conta novaConta;
    printf("Digite o seu nome:");
    fgets(novaConta.nome, 30, stdin);
    strtok(novaConta.nome, "\n"); // Remove o \n do final
    
    printf("Digite o CPF (somente números): ");
    fgets(novaConta.cpf, 12, stdin);
    strtok(novaConta.cpf, "\n"); // Remove o \n do final
    
    
    novaConta.numero_conta = numero_conta_global++;

    if (buscarConta(novaConta.cpf) != -1) {
        printf("Conta já registrada com esse CPF.\n");
        return;
    }

    printf("Digite uma senha: ");
    fgets(novaConta.senha, 20, stdin);
    strtok(novaConta.senha, "\n"); // Remove o \n do final

    novaConta.saldo = 0.0;
    novaConta.qtdMovimentacoes = 0;

    contas[qtdContas++] = novaConta;
    printf("%s sua conta foi registrada com sucesso!\n",novaConta.nome);

    ativarConta(&contas[qtdContas - 1]); // Ativa a conta imediatamente após o registro
}

// Função para ativar uma conta
void ativarConta(Conta *conta) {
    conta->ativa = true; // Conta ativa
    printf("Conta ativada com sucesso!\n");
}

// Função para acessar uma conta
int acessarConta() {
    char cpf[12], senha[20];
    printf("Digite o CPF: ");
    fgets(cpf, 12, stdin);
    strtok(cpf, "\n");

    int indiceConta = buscarConta(cpf);
    if (indiceConta == -1) {
        printf("Conta não encontrada.\n");
        return -1;
    }

    printf("Digite a senha: ");
    fgets(senha, 20, stdin);
    strtok(senha, "\n");

    if (strcmp(contas[indiceConta].senha, senha) == 0) {
        if (contas[indiceConta].ativa) {
            printf("Bem-vindo %s!\n",contas[indiceConta].nome);
            menuConta(&contas[indiceConta]);
        } else {
            printf("Conta não ativada. Você deve ativá-la primeiro.\n");
        }
    } else {
        printf("Senha incorreta.\n");
    }
    return 0;
}

// Menu das operações da conta bancária
void menuConta(Conta *conta) {
    int opcao;
    do {
        printf("\n===== MENU DA CONTA =====\n1. Depositar dinheiro\n2. Sacar dinheiro\n3. Consultar saldo\n4. Listar movimentações\n5. Mostrar seus dados\n6. Desativar conta\n7. Sair\n=========================\nEscolha uma opção: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                depositar(conta);
                break;
            case 2:
                sacar(conta);
                break;
            case 3:
                consultarSaldo(conta);
                break;
            case 4:
                listarMovimentacoes(conta);
                break;
            case 5:
                mostrarDados(conta);
                break;
            case 6:
                desativarConta(conta);
                opcao=7;
                break;
            case 7:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 7);
}

// Função para depositar dinheiro na conta
void depositar(Conta *conta) {
    float valor;
    printf("Digite o valor para depósito: R$ ");
    scanf("%f", &valor);
    getchar();

    if (valor > 0) {
        conta->saldo += valor;
        sprintf(conta->movimentacoes[conta->qtdMovimentacoes], "Depósito de R$ %.2f", valor);
        conta->qtdMovimentacoes++;
        printf("Depósito realizado com sucesso!\n");
    } else {
        printf("Valor inválido para depósito.\n");
    }
}

// Função para sacar dinheiro da conta
void sacar(Conta *conta) {
    float valor;
    printf("Digite o valor para saque: R$ ");
    scanf("%f", &valor);
    getchar();

    if (valor > 0 && valor <= conta->saldo) {
        conta->saldo -= valor;
        sprintf(conta->movimentacoes[conta->qtdMovimentacoes], "Saque de R$ %.2f", valor);
        conta->qtdMovimentacoes++;
        printf("Saque realizado com sucesso!\n");
    } else {
        printf("Saldo insuficiente ou valor inválido.\n");
    }
}

// Função para consultar o saldo da conta
void consultarSaldo(Conta *conta) {
    printf("Saldo atual: R$ %.2f\n", conta->saldo);
}

// Função para listar todas as movimentações
void listarMovimentacoes(Conta *conta) {
    printf("\n=== Movimentações ===\n");
    int i;
    for (i = 0; i < conta->qtdMovimentacoes; i++) {
        printf("%d. %s\n", i + 1, conta->movimentacoes[i]);
    }
    if (conta->qtdMovimentacoes == 0) {
        printf("Nenhuma movimentação registrada.\n");
    }
}

// Função para desativar a conta
void desativarConta(Conta *conta) {
    if (conta->saldo == 0.0) {
        conta->ativa = false;
        printf("Conta desativada com sucesso!\n");
        
    } else {
        printf("Não é possível desativar a conta. Saldo atual: R$ %.2f\n", conta->saldo);
    }
}

// Função para buscar uma conta pelo CPF
int buscarConta(char *cpf) {
    int i;
    for (i = 0; i < qtdContas; i++) {
        if (strcmp(contas[i].cpf, cpf) == 0) {
            return i;
        }
    }
    return -1; // Conta não encontrada
}

// Função para mostrar os dados do usuário
void mostrarDados(Conta *conta){
    printf("Nome: %s\n",conta->nome);
    printf("Número da conta: %d\n", conta->numero_conta);
    printf("CPF: %s\n", conta->cpf);
}

