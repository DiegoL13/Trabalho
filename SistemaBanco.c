#include <stdio.h>
#include <string.h>

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
    int ativa;  
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
    int tamanho;
    if (qtdContas >= MAX) {
        printf("Limite de contas registradas atingido!\n");
        return;
    }

    Conta novaConta;
    printf("Digite o seu nome: ");
    fgets(novaConta.nome, 30, stdin);
    strtok(novaConta.nome, "\n"); // Remove o \n do final
    
     do {
		printf("Digite o CPF (somente numeros): ");
		scanf("%s",novaConta.cpf);
    	strtok(novaConta.cpf, "\n"); 
		tamanho = strlen(novaConta.cpf);
		if(tamanho!=11){
			printf("O CPF deve conter 11 numeros.\n");
		}
	 }while(tamanho != 11);
    

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
    novaConta.ativa = 0;

    contas[qtdContas++] = novaConta;
    printf("%s sua conta foi registrada com sucesso!\n",novaConta.nome);

    ativarConta(&contas[qtdContas - 1]); // Ativa a conta após o registro
}

// Função para ativar uma conta
void ativarConta(Conta *conta) {
    conta->ativa = 1;
}

// Função para acessar uma conta
int acessarConta() {
    int ativar;
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
    limparBuffer();
    fgets(senha, 20, stdin);
    strtok(senha, "\n");

    if (strcmp(contas[indiceConta].senha, senha) == 0) {
        if (contas[indiceConta].ativa) {
            printf("Bem-vindo %s!\n",contas[indiceConta].nome);
            menuConta(&contas[indiceConta]);
        } else {
            do{
		printf("Conta desativada. Deseja reativar?\n1. Sim\n2. Nao\n");
		scanf("%d",&ativar);
		switch (ativar){
			case 1:
				contas[indiceConta].ativa = 1;
				printf("Conta reativada com sucesso!\n");
				ativar=2;
				break;
			case 2:
				printf("Voltando ao menu principal...\n");
				break;
			default:
				printf("Opcao invalida.\n");
	                }		
            } while (ativar !=2);
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
			opcao=9;
		}
		else{
			opcao=8;
		}
                break;
            case 8:
          	    printf("Saindo...\n");
          	    break;
          	case 9:
		    break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 8);
}

// Função para depositar dinheiro na conta
void depositar(Conta *conta) {
    float valor;
    if(conta->ativa==1){
		printf("Digite o valor para deposito: R$ ");
		scanf("%f", &valor);
    	getchar();
	  	if (valor > 0) {
	        conta->saldo += valor;
	        sprintf(conta->movimentacoes[conta->qtdMovimentacoes], "Deposito de R$ %.2f", valor);
	        conta->qtdMovimentacoes++;
	        printf("Deposito realizado com sucesso!\n");
        }
		else {
        	printf("Valor invalido para deposito.\n");
 		}
	} 
	else{
		printf("Nao e possivel movimentar uma conta desativada.");
	}
}

// Função para sacar dinheiro da conta
void sacar(Conta *conta) {
    float valor;
    if(conta->ativa==1){
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
        printf("Conta de destino nao encontrada.\n");
        return;
    }

    Conta *contaDestino = &contas[indiceContaDestino];

    // Verifica se a conta de destino está ativa
    if (!contaDestino->ativa) {
        printf("A conta de destino nao esta ativa.\n");
        return;
    }

	
	else{
		if (contaDestino->cpf != conta->cpf){
			// Solicita o valor da transferência
		    printf("Digite o valor para transferir: R$ ");
		    scanf("%f", &valor);
		    getchar();
			// Verifica se o valor é positivo e se há saldo suficiente na conta de origem
		    if (valor > 0 && conta->saldo >= valor) {
		        conta->saldo -= valor;
		        contaDestino->saldo += valor;
			   	printf("Transferencia de R$ %.2f para %s realizada com sucesso!\n", valor, contaDestino->nome);
			   	// Registra a movimentação nas duas contas
		        sprintf(conta->movimentacoes[conta->qtdMovimentacoes], "Transferencia de R$ %.2f para CPF %s", valor, cpf_destino);
		        conta->qtdMovimentacoes++;
		        sprintf(contaDestino->movimentacoes[contaDestino->qtdMovimentacoes], "Transferencia recebida de R$ %.2f de CPF %s", valor, conta->cpf);
		        contaDestino->qtdMovimentacoes++;		   	
           }else{
			   printf("Saldo insuficiente ou valor invalido.\n");
		   }			
	    }else{
        	printf("Nao e possivel fazer transferencia para si mesmo.\n");
    	}		
    } 
}


// Função para consultar o saldo da conta
void consultarSaldo(Conta *conta) {
    printf("Saldo atual: R$ %.2f\n", conta->saldo);
}

// Função para listar todas as movimentações
void listarMovimentacoes(Conta *conta) {
    printf("\n===== Movimentacoes =====\n");
    int i;
    if (conta->qtdMovimentacoes == 0) {
        printf("Nenhuma movimentacao registrada.\n");
    }else{
		for (i = 0; i < conta->qtdMovimentacoes; i++) {
        	printf("%d. %s\n", i + 1, conta->movimentacoes[i]);
    	}
	}
}

// Função para desativar a conta
void desativarConta(Conta *conta) {
    if (conta->saldo == 0.0) {
        conta->ativa = 0;
        printf("Conta desativada com sucesso!\n");
    } else {
        printf("Nao e possivel desativar a conta. Saldo atual: R$ %.2f\n", conta->saldo);
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
    int i, j;
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
    int i, j;
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
        fgetc(file);
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
