
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX 100

// Definição da estrutura da conta bancária
typedef struct {
    char nome[30];
    int numero_conta;
    char cpf[12];
    char senha[20];
    float saldo;
    char movimentacoes[MAX][50];
    int qtdMovimentacoes;
    bool ativa;  
} Conta;


Conta contas[MAX]; // Array de contas
int qtdContas = 0; // Quantidade de contas registradas
int numero_conta_global=1; // Variável global para definir número da conta

// Protótipos das funções
void menuPrincipal();
void registrarConta();
int acessarConta();
void menuConta(Conta *conta);
void depositar(Conta *conta);
void sacar(Conta *conta);
void transferir(Conta *conta);
void consultarSaldo(Conta *conta);
void listarMovimentacoes(Conta *conta);
void desativarConta(Conta *conta);
int buscarConta(char *cpf);
void ativarConta(Conta *conta);
void mostrarDados(Conta *conta);
void carregarContas();
void salvarContas();

void limparBuffer(){
	char c;
	do{
		c = getchar();
		
	}while( c != '\n');
}

int main() {
    carregarContas();
    int opcao;

    do {
        menuPrincipal();
        printf("Escolha uma opcao: ");
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
                printf("Opcao invalida!\n");
        }
    } while (opcao != 3);
    
    salvarContas();

    return 0;
}

// Função para exibir o menu principal
void menuPrincipal() {
    printf("\n===== MENU PRINCIPAL =====\n1. Acessar conta\n2. Registrar conta\n3. Sair\n==========================\n");
}

// Função para registrar uma nova conta
void registrarConta() {
    if (qtdContas >= MAX) {
        printf("Limite de contas registradas atingido!\n");
        return;
    }

    Conta novaConta;
    printf("Digite o seu nome:");
    fgets(novaConta.nome, 30, stdin);
    strtok(novaConta.nome, "\n"); // Remove o \n do final
    
    printf("Digite o CPF (somente numeros): ");
    fgets(novaConta.cpf, 12, stdin);
    strtok(novaConta.cpf, "\n"); 

    novaConta.numero_conta = numero_conta_global++;

    if (buscarConta(novaConta.cpf) != -1) {
        printf("Conta ja registrada com esse CPF.\n");
        return;
    }

    printf("Digite uma senha: ");
    limparBuffer();
    fgets(novaConta.senha, 20, stdin);
    strtok(novaConta.senha, "\n"); 

    novaConta.saldo = 0.0;
    novaConta.qtdMovimentacoes = 0;
    novaConta.ativa = false;

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
        printf("Conta nao encontrada.\n");
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
            printf("Conta nao ativada. Voce deve ativa-la primeiro.\n");
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
        printf("\n===== MENU DA CONTA =====\n1. Depositar dinheiro\n2. Sacar dinheiro\n3. Transferir\n4. Consultar saldo\n5. Listar movimentacoes\n6. Mostrar dados\n7. Desativar conta\n8. Sair\n=========================\nEscolha uma opcao: ");
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
                transferir(conta);
                break;
            case 4:
            	consultarSaldo(conta);
                break;
            case 5:
            	listarMovimentacoes(conta);
                break;
            case 6:
            	mostrarDados(conta);
                break;
            case 7:
            	desativarConta(conta);
                if (conta->saldo != 0){
					opcao=8;
				}
                break;
            case 8:
          	    printf("Saindo...\n");
          	    break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 8);
}

// Função para depositar dinheiro na conta
void depositar(Conta *conta) {
    float valor;
    if(conta->ativa==true){
		printf("Digite o valor para deposito: R$ ");
		scanf("%f", &valor);
    	getchar();
	  	if (valor > 0) {
	        conta->saldo += valor;
	        sprintf(conta->movimentacoes[conta->qtdMovimentacoes], "Deposito de R$ %.2f", valor);
	        conta->qtdMovimentacoes++;
	        printf("Deposito realizado com sucesso!\n");printf("Saindo...\n");
        }
		else {
        	printf("Valor inválido para deposito.\n");
 		}
	} 
	else{
		printf("Nao e possivel movimentar uma conta desativada.");
	}
}

// Função para sacar dinheiro da conta
void sacar(Conta *conta) {
    float valor;
    if(conta->ativa==true){
	    printf("Digite o valor para saque: R$ ");
	    scanf("%f", &valor);
	    getchar();
	    if (valor > 0 && valor <= conta->saldo) {
	        conta->saldo -= valor;
	        sprintf(conta->movimentacoes[conta->qtdMovimentacoes], "Saque de R$ %.2f", valor);
	        conta->qtdMovimentacoes++;
	        printf("Saque realizado com sucesso!\n");
	    } 
		else {
	        printf("Saldo insuficiente ou valor invalido.\n");
	    }
	}
	else{
		printf("Nao e possivel movimentar uma conta desativada.");
	}
}
// Função para transferir dinheiro entre contas
void transferir(Conta *conta) {
    char cpf_destino[12];
    float valor;

    // Solicita o CPF da conta de destino
    printf("Digite o CPF da conta de destino: ");
    fgets(cpf_destino, 12, stdin);
    strtok(cpf_destino, "\n");

    // Busca a conta de destino
    int indiceContaDestino = buscarConta(cpf_destino);
    if (indiceContaDestino == -1) {
        printf("Conta de destino não encontrada.\n");
        return;
    }

    Conta *contaDestino = &contas[indiceContaDestino];

    // Verifica se a conta de destino está ativa
    if (!contaDestino->ativa) {
        printf("A conta de destino não está ativa.\n");
        return;
    }

    // Solicita o valor da transferência
    printf("Digite o valor para transferir: R$ ");
    scanf("%f", &valor);
    getchar();

    // Verifica se o valor é positivo e se há saldo suficiente na conta de origem
    if (valor > 0 && conta->saldo >= valor) {
        conta->saldo -= valor;
        contaDestino->saldo += valor;

        // Registra a movimentação nas duas contas
        sprintf(conta->movimentacoes[conta->qtdMovimentacoes], "Transferencia de R$ %.2f para CPF %s", valor, cpf_destino);
        conta->qtdMovimentacoes++;
        
        sprintf(contaDestino->movimentacoes[contaDestino->qtdMovimentacoes], "Transferencia recebida de R$ %.2f de CPF %s", valor, conta->cpf);
        contaDestino->qtdMovimentacoes++;

        printf("Transferência de R$ %.2f para %s realizada com sucesso!\n", valor, contaDestino->nome);
    } else {
        printf("Saldo insuficiente ou valor inválido para transferência.\n");
    }
}


// Função para consultar o saldo da conta
void consultarSaldo(Conta *conta) {
    printf("Saldo atual: R$ %.2f\n", conta->saldo);
}

// Função para listar todas as movimentações
void listarMovimentacoes(Conta *conta) {
    printf("\n=== Movimentacoes ===\n");
    int i;
    for (i = 0; i < conta->qtdMovimentacoes; i++) {
        printf("%d. %s\n", i + 1, conta->movimentacoes[i]);
    }
    if (conta->qtdMovimentacoes == 0) {
        printf("Nenhuma movimentacao registrada.\n");
    }
}

// Função para desativar a conta
void desativarConta(Conta *conta) {
    if (conta->saldo == 0.0) {
        conta->ativa = false;
        printf("Conta desativada com sucesso!\n");
        #define FILE_NAME "contas.txt"
    } else {
        printf("Não e possivel desativar a conta. Saldo atual: R$ %.2f\n", conta->saldo);
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
    printf("Numero da conta: %d\n", conta->numero_conta);
    printf("CPF: %s\n", conta->cpf);
}

// Função para salvar as contas no arquivo txt
void salvarContas() {
	int i;
	int j;
    FILE *file = fopen("contas.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para salvar os dados.\n");
        return;
    }

    // Salva a quantidade de contas registradas e o número da conta global
    fprintf(file, "%d\n", qtdContas);
    fprintf(file, "%d\n", numero_conta_global);

    for (i = 0; i < qtdContas; i++) {
        Conta conta = contas[i];
        fprintf(file, "%s\n", conta.nome);
        fprintf(file, "%d\n", conta.numero_conta);
        fprintf(file, "%s\n", conta.cpf);
        fprintf(file, "%s\n", conta.senha);
        fprintf(file, "%.2f\n", conta.saldo);
        fprintf(file, "%d\n", conta.qtdMovimentacoes);
        for (j = 0; j < conta.qtdMovimentacoes; j++) {
            fprintf(file, "%s\n", conta.movimentacoes[j]);
        }
        fprintf(file, "%d\n", conta.ativa);
    }

    fclose(file);
    printf("Dados das contas salvos com sucesso.\n");
}

// Função para carregar as contas registradas no arquivo txt
void carregarContas() {
	int i;
	int j;
    FILE *file = fopen("contas.txt", "r");
    if (file == NULL) {
        printf("Nenhum dado de conta encontrado para carregar.\n");
        return;
    }

    // Carrega a quantidade de contas registradas e o número da conta global
    fscanf(file, "%d\n", &qtdContas);
    fscanf(file, "%d\n", &numero_conta_global);

    for (i = 0; i < qtdContas; i++) {
        Conta conta;
        fgets(conta.nome, 30, file);
        strtok(conta.nome, "\n");
        fscanf(file, "%d\n", &conta.numero_conta);
        fgets(conta.cpf, 12, file);
        strtok(conta.cpf, "\n");
        fgets(conta.senha, 20, file);
        strtok(conta.senha, "\n");
        fscanf(file, "%f\n", &conta.saldo);
        fscanf(file, "%d\n", &conta.qtdMovimentacoes);
        for (j = 0; j < conta.qtdMovimentacoes; j++) {
            fgets(conta.movimentacoes[j], 50, file);
            strtok(conta.movimentacoes[j], "\n");
        }
        fscanf(file, "%d\n", &conta.ativa);

        contas[i] = conta;
    }

    fclose(file);
    printf("Dados das contas carregados com sucesso.\n");
}
