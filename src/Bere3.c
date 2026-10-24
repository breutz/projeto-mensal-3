/* Sistema de Gerenciamento Automaziado de Mercearia com cadastros flexíveis, estruturas dinâmicas de dados,
vendas com carrinho, pagamentos combinados, sangria, controle de caixa, relatórios e persistência em
arquivo. */

/* --------------     Bibliotecas     -------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --------------     Estruturas de dados     -------------- */
typedef struct {
    int codigo;
    char nome[60];
    char nomeSocial[60];
    char cpf[15];
    char endereco[80];
    char bairro[40];
    char telefone[20];
} Cliente;

typedef struct {
    int codigo;
    char descricao[50];
    char categoria[30];
    float precoCompra;
    float margemLucro;
    float precoVenda;
    int estoque;
    int estoqueMinimo;
} Produto;

typedef struct {
    int codigo;
    char descricao[50];
    float precoVenda;
    int quantidade;
    float valorTotalItem;
} ItemCarrinho;

typedef struct {
    int numeroVenda;
    int codigoCliente;
    ItemCarrinho *itens;
    int totalItens;
    float valorTotal;
    char situacao; // 'a' = aberto, 'p' = pago
} Venda;

typedef struct {
    int numeroVenda;
    float valorPago; // total da venda
    float valorEntregue; // para pagamento em dinheiro ou misto
    float troco; // para pagamento em dinheiro ou misto
    char tipo[3]; // "d" = dinheiro, "c" = cartao, "dc" = misto
} Pagamento;

/* --------------     Agrupamento das structs principais     -------------- */

typedef struct {
    Cliente *dados;
    int total;
} ListaClientes;

typedef struct {
    Produto *dados;
    int total;
} ListaProdutos;

typedef struct {
    Venda *dados;
    int total;
} ListaVendas;

typedef struct {
    Pagamento *dados;
    int total;
} ListaPagamentos;

/* --------------     Struct central que contem todos os dados     -------------- */

typedef struct {
    ListaClientes clientes;
    ListaProdutos produtos;
    ListaVendas vendas;
    ListaPagamentos pagamentos;

    float saldoAbertura; // valor que foi colocado na abertura do caixa
    float saldoCaixa; // valor disponivel na gaveta do caixa
    float totalSangrias; // soma das sangrias realizadas
    int caixaAberto; // 0 = fechado, 1 = aberto
} Sistema;

Sistema s = {0}; // Struct global para ser usada como único parâmetro entre funções e ter acesso aos dados de todas as structs do sistema

/* --------------     Protótipos de função     -------------- */

/* Menus */
void menuPrincipal();
void menuCadastros();
void menuVendas();
void menuRelatorios();

/* Cadastros */
void cadastrarCliente();
void cadastrarProduto();
void adicionarCliente(Cliente);
void adicionarProduto(Produto);
void adicionarVenda(Venda);
void adicionarPagamento(Pagamento);

/* Operações de caixa */
void aberturaCaixa();
void fechamentoCaixa();
void realizarSangria();

/* Vendas */
void novaVenda();
void processarPagamento(Venda*);
void recuperarVendaAberta();

/* Relatórios */
void listarClientes();
void listarProdutos();
void listarVendas();

/* Persistência de dados */
void salvarArquivo();
void carregarArquivo();

/* Funções auxiliares */
int buscarProdutoPorCodigo(int);
int buscarClientePorCodigo(int);
void exibirCatalogoProdutos();
void pagamentoDinheiro(Venda*, float, float); // Auxiliar para pagamento em dinheiro
void pagamentoCartao(Venda*, float, float); // Auxiliar para pagamento em cartao | Juntas essas duas tratam o pagamento misto

/* --------------     Implementação do código     -------------- */
int main() {
    char resposta;

    printf("\n================== BERE IS NICE ================== <- e minimalista (:\n");
    do {
        printf("\nCarregar os dados salvos anteriormente? (s/n): ");

        if (scanf(" %c", &resposta) != 1){
            printf("\nERRO: Digite apenas s ou n!\n");
            while (getchar() != '\n'); // Limpa a letra digitada do buffer
            continue;
        }

        if (resposta == 's' || resposta == 'S') {
            break;
        } else if (resposta == 'n' || resposta == 'N'){
            break;
        } else {
            printf("\nERRO: Digite apenas s ou n!\n");
            while (getchar() != '\n'); // Limpa a letra digitada do buffer
            continue;
        }
    } while (resposta != 's' || resposta != 'S' || resposta != 'n' || resposta != 'N');

    if (resposta == 's' || resposta == 'S') {
        carregarArquivo();
    }

    menuPrincipal();

    printf("\n==================================================\n");
    do {
        printf("\nSalvar os dados antes de fechar? (s/n): ");

        if (scanf(" %c", &resposta) != 1){
            printf("\nERRO: Digite apenas s ou n!\n");
            while (getchar() != '\n'); // Limpa a letra digitada do buffer
            continue;
        }

        if (resposta == 's' || resposta == 'S') {
            break;
        } else if (resposta == 'n' || resposta == 'N'){
            break;
        } else {
            printf("\nERRO: Digite apenas s ou n!\n");
            while (getchar() != '\n'); // Limpa a letra digitada do buffer
            continue;
        }
    } while (resposta != 's' || resposta != 'S' || resposta != 'n' || resposta != 'N');

    if (resposta == 's' || resposta == 'S') {
        salvarArquivo();
    }

    printf("\nEncerrando!\nBere is Nice!\n");
    return 0;
}

/* --------------     Menu Principal     -------------- */

void menuPrincipal() {
    int opcao = -1;
    do {
        printf("\n================== BERE IS NICE ==================\n");
        printf("1. %47s\n", "Cadastros");
        printf("2. %47s\n", "Vendas");
        printf("3. %47s\n", "Abertura de Caixa");
        printf("4. %47s\n", "Fechamento de Caixa");
        printf("5. %47s\n", "Relatorios");
        printf("0. %47s\n", "Sair");
        printf("\nDigite o codigo do submenu em que deseja entrar: ");
        if (scanf("%d", &opcao) != 1) {
            printf("\nERRO: Digite apenas numeros!\n");
            while (getchar() != '\n'); // Limpa a letra digitada do buffer
            opcao = -1; // Força a repetir o loop
            continue;
        }

        switch (opcao) {
        case 1:
            menuCadastros();
            break;
        case 2:
            if (s.caixaAberto == 0) {
                printf("\nErro: O caixa precisa ser aberto antes!\n");
            } else {
                menuVendas();
            }
            break;
        case 3:
            aberturaCaixa();
            break;
        case 4:
            fechamentoCaixa();
            break;
        case 5:
            menuRelatorios();
            break;
        case 0:
            if (s.caixaAberto == 1) {
                printf("\n==================================================\n");
                printf("AVISO: O sistema esta fechando com caixa ABERTO!\n");
                printf("Os dados de faturamento foram preservados.\n");
                printf("Sera necessario reabrir o caixa no proximo inicio.\n");
                printf("==================================================\n");
            } else {
                printf("\nEncerrando o sistema...\n");
            }
            break;
        default:
            printf("\nErro: Opcao Invalida!\n");
        }
    } while (opcao != 0);
}

/* --------------     Submenus     -------------- */

void menuCadastros() {
    int opcao = -1;
    do {
        printf("\n================ MENU DE CADASTROS ===============\n");
        printf("1. %47s\n", "Cadastro de clientes");
        printf("2. %47s\n", "Cadastro de produtos");
        printf("0. %47s\n", "Voltar ao Menu Principal");
        printf("\nDigite o codigo do submenu em que deseja entrar: ");
        if (scanf("%d", &opcao) != 1) {
            printf("\nERRO: Digite apenas numeros!\n");
            while (getchar() != '\n'); // Limpa a letra digitada do buffer
            opcao = -1; // Força a repetir o loop
            continue;
        }

        switch (opcao) {
            case 1: 
                cadastrarCliente();
                break;
            case 2:
                cadastrarProduto();
                break;
            case 0:
                printf("\nVoltando...\n");
                break;
            default:
                printf("\nErro: Opcao Invalida!\n");
        }
    } while (opcao != 0);
}

void menuVendas() {
    int opcao = -1;
    do {
        printf("\n================== MENU DE VENDAS ================\n");
        printf("1. %47s\n", "Nova Venda");
        printf("2. %47s\n", "Sangria");
        printf("3. %47s\n", "Pagamento");
        printf("0. %47s\n", "Voltar ao menu principal");
        printf("\nDigite o codigo do submenu em que deseja entrar: ");
        if (scanf("%d", &opcao) != 1) {
            printf("\nERRO: Digite apenas numeros!\n");
            while (getchar() != '\n'); // Limpa a letra digitada do buffer
            opcao = -1; // Força a repetir o loop
            continue;
        }

        switch (opcao) {
            case 1: 
                novaVenda();
                break;
            case 2:
                realizarSangria();
                break;
            case 3:
                recuperarVendaAberta();
                break;
            case 0:
                printf("\nVoltando...\n");
                break;
            default:
                printf("\nErro: Opcao Invalida!\n");
        }
    } while (opcao != 0);
}

void menuRelatorios() {
    int opcao = -1;
    do {
        printf("\n=============== MENU DE RELATORIOS ===============\n");
        printf("1. %47s\n", "Listagem de Clientes");
        printf("2. %47s\n", "Listagem de Produtos");
        printf("3. %47s\n", "Listagem de Vendas");
        printf("0. %47s\n", "Voltar ao Menu Principal");
        printf("\nDigite o codigo do submenu em que deseja entrar: ");
        if (scanf("%d", &opcao) != 1) {
            printf("\nERRO: Digite apenas numeros!\n");
            while (getchar() != '\n'); // Limpa a letra digitada do buffer
            opcao = -1; // Força a repetir o loop
            continue;
        }

        switch (opcao) {
            case 1:
                listarClientes();
                break;
            case 2:
                listarProdutos();
                break;
            case 3:
                listarVendas();
                break;
            case 0:
                printf("\nRetornando ao Menu Principal...\n");
                break;
            default:
                printf("Erro: Opcao invalida.\n");
        }
    } while (opcao != 0);
}

/* --------------     Funções de registro dos cadastros     -------------- */

void cadastrarCliente() {
    Cliente novo;
    int opcao = 0;

    do {
        printf("\nDigite o codigo do cliente: ");

        if (scanf("%d", &novo.codigo) != 1) {
            printf("\nERRO: Digite apenas numeros!\n");
            while (getchar() != '\n'); // Limpa a letra digitada do buffer
            opcao = 0;
            continue;
        } 

        if (novo.codigo <= 0) {
            printf("\nErro: O codigo deve ser maior que zero!\n");
            opcao = 0;
            continue;
        }
        
        // Se retornar indice diferente de -1, o código já existe
        if (buscarClientePorCodigo(novo.codigo) != -1) {
            printf("\nERRO: Codigo ja existente! Digite um codigo diferente.\n");
            opcao = 0;
            continue;
        } else {
            opcao = 1;
        } 
    } while (opcao == 0 || novo.codigo <= 0);

    // Limpa o buffer do teclado
    while (getchar() != '\n'); 

    printf("Digite o nome completo: ");
    scanf(" %59[^\n]", novo.nome);
    printf("Digite o nome social: ");
    scanf(" %59[^\n]", novo.nomeSocial);
    printf("Digite o CPF: ");
    scanf(" %14[^\n]", novo.cpf);
    printf("Digite o endereco (rua e numero): ");
    scanf(" %79[^\n]", novo.endereco);
    printf("Digite o bairro: ");
    scanf(" %39[^\n]", novo.bairro);
    printf("Digite o telefone/WhatsApp: ");
    scanf(" %19[^\n]", novo.telefone);

    adicionarCliente(novo);
    printf("\nCliente cadastrado com sucesso!\n");
}

void cadastrarProduto() {
    Produto novo;
    int opcao = 0;

    do {
        printf("\nDigite o codigo do produto: ");

        if (scanf("%d", &novo.codigo) != 1) {
            printf("\nERRO: Digite apenas numeros!\n");
            while (getchar() != '\n'); // Limpa a letra digitada do buffer
            opcao = 0;
            continue;
        } 

        if (novo.codigo <= 0) {
            printf("\nErro: Codigo deve ser maior que zero!\n");
            opcao = 0;
            continue;
        }
        
        // Se retornar o indice diferente de -1, o codigo já existe
        if (buscarProdutoPorCodigo(novo.codigo) != -1) {
            printf("\nERRO: Codigo existente! Digite novamente.\n");
            opcao = 0;
            continue;
        } else {
            opcao = 1;
        } 
        
        
    } while (opcao == 0 || novo.codigo <= 0);

    // Limpa o buffer do teclado
    while (getchar() != '\n');

    printf("Digite a descricao: ");
    scanf(" %59[^\n]", novo.descricao);
    while (getchar() != '\n');

    printf("Digite a categoria: ");
    scanf(" %59[^\n]", novo.categoria);
    while (getchar() != '\n');
    
    do {
        printf("Digite o preco de compra: ");
        if (scanf("%f", &novo.precoCompra) != 1) {
            printf("\nERRO: Digite apenas numeros!\n");
            while (getchar() != '\n'); // Limpa a letra digitada do buffer
            opcao = 0;
            continue;
        } else {
            opcao = 1;
        }
        if (novo.precoCompra <= 0) {
            printf("\nErro: Preco de compra deve ser maior que zero!\n");
        }
    } while (opcao == 0 || novo.precoCompra <= 0);

    do {
        printf("Digite a margem de lucro (%%): ");
        if (scanf("%f", &novo.margemLucro) != 1) {
            printf("\nERRO: Digite apenas numeros!\n");
            while (getchar() != '\n'); // Limpa a letra digitada do buffer
            opcao = 0;
            continue;
        } else {
            opcao = 1;
        }
        if (novo.margemLucro < 0) novo.margemLucro = 0;
        novo.precoVenda = novo.precoCompra * (1.0 + novo.margemLucro / 100.0);
    } while (opcao == 0);

    do {
        printf("Digite o estoque inicial: ");
        if (scanf("%d", &novo.estoque) != 1) {
            printf("\nERRO: Digite apenas numeros!\n");
            while (getchar() != '\n'); // Limpa a letra digitada do buffer
            opcao = 0;
            continue;
        } else {
            opcao = 1;
        }
        if (novo.estoque < 0) novo.estoque = 0;
    } while (opcao == 0);

    do {
        printf("Digite o estoque minimo: ");
        if (scanf("%d", &novo.estoqueMinimo) != 1) {
            printf("\nERRO: Digite apenas numeros!\n");
            while (getchar() != '\n'); // Limpa a letra digitada do buffer
            opcao = 0;
            continue;
        } else {
            opcao = 1;
        }
        if (novo.estoqueMinimo < 0) novo.estoqueMinimo = 0;
    } while (opcao == 0);

    adicionarProduto(novo);
    printf("\nProduto cadastrado com sucesso!\n%-39s R$ %7.2f\n", "Preco de venda:", novo.precoVenda);
}

/* --------------     Funções de cadastro dinânimo com gerenciamento de memória     -------------- */

void adicionarCliente(Cliente novo) {
    int novoTotal = s.clientes.total + 1;
    Cliente *temp = realloc(s.clientes.dados, novoTotal * sizeof(Cliente));

    if (temp == NULL) {
        printf("\nErro ao alocar memoria para cliente!\n");
        return;
    }

    s.clientes.dados = temp;
    s.clientes.dados[s.clientes.total] = novo; // Grava os novos dados registrados dentro da nova vaga livre na lista de structs (copia a struct inteira com os dados registrados pra nova vaga que acabou de ser liberada)
    s.clientes.total = novoTotal;
}

void adicionarProduto(Produto novo) {
    int novoTotal = s.produtos.total + 1;
    Produto *temp = realloc(s.produtos.dados, novoTotal * sizeof(Produto));

    if (temp == NULL) {
        printf("\nErro ao alocar memória para produto!\n");
        return;
    }

    s.produtos.dados = temp;
    s.produtos.dados[s.produtos.total] = novo; // Grava os novos dados registrados dentro da nova vaga livre na lista de structs (copia a struct inteira com os dados registrados pra nova vaga que acabou de ser liberada)
    s.produtos.total = novoTotal;
}

void adicionarVenda(Venda nova) {
    int novoTotal = s.vendas.total + 1;
    Venda *temp = realloc(s.vendas.dados, novoTotal * sizeof(Venda));

    if (temp == NULL) {
        printf("\nErro ao alocar memória para venda!\n");
        return;
    }

    s.vendas.dados = temp;
    s.vendas.dados[s.vendas.total] = nova; // Grava os novos dados registrados dentro da nova vaga livre na lista de structs (copia a struct inteira com os dados registrados pra nova vaga que acabou de ser liberada)
    s.vendas.total = novoTotal;
}

void adicionarPagamento(Pagamento novo) {
    int novoTotal = s.pagamentos.total + 1;
    Pagamento *temp = realloc(s.pagamentos.dados, novoTotal * sizeof(Pagamento));

    if (temp == NULL) {
        printf("\nErro ao alocar memória para pagamento!\n");
        return;
    }

    s.pagamentos.dados = temp;
    s.pagamentos.dados[s.pagamentos.total] = novo;
    s.pagamentos.total = novoTotal;
}

/* --------------     Funções de gerenciamento de caixa     -------------- */

void aberturaCaixa() {
    if (s.caixaAberto == 1) {
        printf("\nAVISO: O caixa ja esta aberto!\n");
        return;
    }
    float valor;
    do {
        printf("\nDigite o valor de abertura (fundo de troco): R$ ");
        scanf("%f", &valor);
        if (valor < 0) {
            printf("\nERRO: O valor de abertura nao pode ser negativo!\n");
        }
    } while (valor < 0);

    s.saldoAbertura = s.saldoCaixa = valor;
    s.caixaAberto = 1;

    printf("\nCaixa aberto com sucesso! Saldo: R$ %.2f\n", s.saldoCaixa);
}

void fechamentoCaixa() {
    if (s.caixaAberto == 0) {
        printf("\nAVISO: O caixa ja esta fechado!\n");
        return;
    }

    float pagaD = 0.0, pagaC = 0.0, pagaDC = 0.0;
    float faturamento = 0.0;
    int qtdPagas = 0;

    // Contabiliza a quantidade de vendas pagas para o relatório
    for (int i = 0; i < s.vendas.total; i++) {
        if (s.vendas.dados[i].situacao == 'p') {
            faturamento += s.vendas.dados[i].valorTotal;
            qtdPagas++;
        }
    }

    // Totaliza os pagamentos por tipo
    for (int i = 0; i < s.pagamentos.total; i++) {
        if (strcmp(s.pagamentos.dados[i].tipo, "d") == 0) {
            pagaD += s.pagamentos.dados[i].valorPago;
        } else if (strcmp(s.pagamentos.dados[i].tipo, "c") == 0) {
            pagaC += s.pagamentos.dados[i].valorPago;
        } else if (strcmp(s.pagamentos.dados[i].tipo, "dc") == 0){
            pagaDC += s.pagamentos.dados[i].valorPago;
        }
    }

    float conciliacao = faturamento - s.saldoAbertura - pagaD - pagaC - pagaDC;
    float ajuste = conciliacao;

    printf("\n============== FECHAMENTO DE CAIXA ===============\n");
    printf("%-35s %14d\n", "Quantidade de vendas pagas:", qtdPagas);
    printf("%-35s R$ %11.2f\n", "Faturamento total do dia:", faturamento);
    printf("%-35s R$ %11.2f\n", "Valor de abertura:", s.saldoAbertura);
    printf("%-35s R$ %11.2f\n", "Valor pago em Dinheiro:", pagaD);
    printf("%-35s R$ %11.2f\n", "Valor pago em Cartao:", pagaC);
    printf("%-35s R$ %11.2f\n", "Valor pago em Dinheiro e Cartao:", pagaDC);
    printf("%-35s R$ %11.2f\n", "Total de sangrias:", s.totalSangrias);
    printf("%-35s R$ %11.2f\n", "Saldo fisico na gaveta:", s.saldoCaixa);
    printf("--------------------------------------------------\n");
    printf("%-35s %14.2f\n",  "Resultado da conciliacao:", conciliacao);
    printf("==================================================\n");

    // Tratamento de fechamento divergente
    if (conciliacao != 0.0) {
        if (ajuste > 0.0) {
            printf("\nAjuste do Caixa: SOBRA\nDivergencia positiva de R$ %.2f\n", ajuste);
        } else {
            printf("\nAjuste do Caixa: FALTA\nDivergencia negativa de R$ %.2f\n", ajuste);
        }

        char confirma;
        
        printf("\n==================================================\n");
        printf("ATENCAO: O caixa nao podera ser fechado!\n");
        printf("Ha divergencia de valores no fechamento.\n");
        printf("--------------------------------------------------\n");
        printf("Deseja registrar o ajuste e fechar? (s/n): ");
        scanf(" %c", &confirma);

        if (confirma != 's' && confirma != 'S') {
            printf("\nFechamento IMPEDIDO conforme regras do sistema.\nAjuste os valores antes de fechar.\n");
            return; // Impede o fechamento do caixa
        }
        
        printf("\nAjuste de %.2f registrado com sucesso!\n", ajuste);
    } else {
        printf("\nAjuste do Caixa: ZERO\nFechamento correto, sem divergencias.\n");
    }

    // Zera tudo
    s.caixaAberto = 0;
    s.saldoCaixa = 0.0;
    s.saldoAbertura = 0.0;
    s.totalSangrias = 0.0;

    printf("\nCaixa fechado com sucesso!\n");
}

void realizarSangria() {
    if (s.caixaAberto == 0) {
        printf("\nERRO: Abra o caixa antes de realizar sangria!\n");
        return;
    }

    printf("\n=========== RETIRADA DE CAIXA (SANGRIA) ==========\n");
    printf("Saldo atual em dinheiro na gaveta: R$ %.2f\n", s.saldoCaixa);
    printf("Reserva obrigatoria para troco: R$ 50.00\n");

    if (s.saldoCaixa <= 50.0) {
        printf("\nATENCAO: Saldo insuficiente.\nO valor minimo de R$ 50.00 deve ser preservado!\n");
        return;
    }

    float valorRetirada = 0.0;
    printf("\nDigite o valor da retirada: R$ ");
    scanf("%f", &valorRetirada);

    if (valorRetirada <= 0.0) {
        printf("\nERRO: Valor da retirada deve ser maior que zero!\n");
        return;
    }

    if ((s.saldoCaixa - valorRetirada) < 50.0) {
        printf("\nERRO: Retirada nao permitida!\nDeixaria menos de R$ 50.00 no caixa.\n");
        printf("Valor maximo permitido para retirada: R$ %.2f\n", s.saldoCaixa - 50.0);
        return;
    }

    s.saldoCaixa -= valorRetirada;
    s.totalSangrias += valorRetirada;

    printf("\nSangria realizada com sucesso!\n");
    printf("Valor retirado: R$ %.2f\nNovo saldo em caixa: R$ %.2f\n", valorRetirada, s.saldoCaixa);
}

/* --------------     Funções de gerenciamento de venda e pagamento     -------------- */

void novaVenda() {
    if (s.produtos.total == 0) {
        printf("\nErro: Nao ha produtos cadastrados!\n");
        return;
    }

    Venda v = {0};
    v.numeroVenda = s.vendas.total + 1;
    int opcao = 0;

    do {
        printf("\nDigite o codigo do cliente (0 = cliente avulso): ");

        if (scanf("%d", &v.codigoCliente) != 1) {
            printf("\nERRO: Digite apenas numeros!\n");
            while (getchar() != '\n'); // Limpa a letra digitada do buffer
            opcao = 0;
            continue;
        } 

        if (v.codigoCliente < 0) {
            printf("\nErro: O codigo deve ser maior ou igual a zero!\n");
            opcao = 0;
            continue;
        }
        
        if (v.codigoCliente == 0) {
            printf("\n==================================================\n");
            printf("%-7s%-2d | %-8s %12.12s | %-7s %6s\n",
            "Venda #", v.numeroVenda,
            "Cliente:", "AVULSO",
            "Status:", "ABERTO");
            printf("--------------------------------------------------");
            break;
        }

        if (buscarClientePorCodigo(v.codigoCliente) == -1) {
            printf("\nCliente nao encontrado!\n");
            opcao = 0;
            continue;
        } else {
            printf("\n==================================================\n");
            printf("%-7s%-2d | %-8s %12.12s | %-7s %6s\n",
            "Venda #", v.numeroVenda,
            "Cliente:", s.clientes.dados[buscarClientePorCodigo(v.codigoCliente)].nome,
            "Status:", "ABERTO");
            printf("--------------------------------------------------");
            opcao = 1;
        } 
    } while (opcao == 0 || v.codigoCliente <= 0);

    ItemCarrinho *itens = NULL;
    int totalItens = 0;
    float subtotal = 0.0;
    char resposta = 's';

    do {
        exibirCatalogoProdutos();
        int codigoProduto, posicaoProduto;

        printf("\nDigite o codigo do produto (0 para voltar): ");
        if (scanf("%d", &codigoProduto) != 1) {
            printf("\nERRO: Digite apenas numeros!\n");
            while (getchar() != '\n'); // Limpa a letra digitada do buffer
            continue;
        } 

        if (codigoProduto == 0) {
            break;
        }

        posicaoProduto = buscarProdutoPorCodigo(codigoProduto);
        if (posicaoProduto == -1) {
            printf("\nProduto nao encontrado!\n"); 
            continue; 
        }

        if (s.produtos.dados[posicaoProduto].estoque == 0) {
            printf("\nALERTA: Estoque zerado! Produto Indisponivel.\n");
            continue;
        }

        int quantidade;
        printf("\nDigite a quantidade desejada: ");
        scanf("%d", &quantidade);

        if (quantidade <= 0) {
                printf("\nQuantidade invalida!\n");
                continue;
        }
        
        if (quantidade > s.produtos.dados[posicaoProduto].estoque) {
            printf("\nALERTA: Quantidade solicidada (%d) indisponivel!\nEstoque disponivel: %d\n", quantidade, s.produtos.dados[posicaoProduto].estoque);
            char confirma;
            printf("\nDeseja comprar as %d un. disponiveis? (s/n)\n", s.produtos.dados[posicaoProduto].estoque);
            scanf(" %c", &confirma);

            if (confirma == 's' || confirma == 'S') {
                quantidade = s.produtos.dados[posicaoProduto].estoque;
            } else {
                printf("\nItem descartado.\n");
                continue;
            }
        }

        int novoTotalItens = totalItens + 1;
        ItemCarrinho *temp = (ItemCarrinho *) realloc(itens, novoTotalItens * sizeof(ItemCarrinho));

        if (temp == NULL) {
            printf("\nErro ao alocar memoria para o item do carrinho!\n");
            break;
        }

        itens = temp;
        itens[totalItens].codigo = s.produtos.dados[posicaoProduto].codigo;
        strcpy(itens[totalItens].descricao, s.produtos.dados[posicaoProduto].descricao);
        itens[totalItens].precoVenda = s.produtos.dados[posicaoProduto].precoVenda;
        itens[totalItens].quantidade = quantidade;
        itens[totalItens].valorTotalItem = itens[totalItens].precoVenda * quantidade;

        // Atualiza o subtotal da venda e incrementa o contador do carrinho
        subtotal += itens[totalItens].valorTotalItem;
        totalItens = novoTotalItens;

        // Deduz a quantidade vendida do estoque do produto
        s.produtos.dados[posicaoProduto].estoque -= quantidade;

        // Alerta se o produto atingiu ou ficou abaixo do estoque mínimo
        if (s.produtos.dados[posicaoProduto].estoque <= s.produtos.dados[posicaoProduto].estoqueMinimo) {
            printf("\nAVISO: '%s' alcancou o limite minimo de estoque!\nEstoque atual: %d.\n", s.produtos.dados[posicaoProduto].descricao, s.produtos.dados[posicaoProduto].estoque);
        }

        printf("\nItem adicionado com sucesso!\nSubtotal atual: R$ %.2f\n", subtotal);

        do {
            printf("\nDeseja adicionar outro item? (s/n): ");

            if (scanf(" %c", &resposta) != 1){
                printf("\nERRO: Digite apenas s ou n!\n");
                while (getchar() != '\n'); // Limpa a letra digitada do buffer
                continue;
            }

            if (resposta == 's' || resposta == 'S') {
                break;
            } else if (resposta == 'n' || resposta == 'N'){
                break;
            } else {
                printf("\nERRO: Digite apenas s ou n!\n");
                while (getchar() != '\n'); // Limpa a letra digitada do buffer
                continue;
            }
        } while (resposta != 's' || resposta != 'S' || resposta != 'n' || resposta != 'N');

    } while (resposta == 's' || resposta == 'S');

    if (totalItens == 0) {
        printf("\nNenhum item adicionado. Venda cancelada.\n");
        return;
    }

    // Aplica desconto personalizado
    float descPercentual = 0.0;
    printf("\nDigite o percentual de desconto: ");
    scanf("%f", &descPercentual);
    if (descPercentual > 0.0) {
        if (descPercentual > 100.0) {
            descPercentual = 100.0;
        } else if (descPercentual < 0.0) {
        descPercentual = 0.0;
        }

        subtotal = subtotal * (1.0 - descPercentual / 100.0);
        printf("\nDesconto aplicado com sucesso!\nNovo total da venda: R$ %.2f\n", subtotal);
    }

    v.itens = itens;
    v.totalItens = totalItens;
    v.valorTotal = subtotal;
    v.situacao = 'a';

    adicionarVenda(v);
    processarPagamento(&s.vendas.dados[s.vendas.total - 1]);
}

void processarPagamento(Venda *v) {
    int opcaoPagamento = -1;
    float valorDinheiro = 0.0;
    float falta = 0.0;

    do {
        printf("\n============== PROCESSAR PAGAMENTO ===============\n");
        printf("%-39s R$ %7.2f\n", "Valor total a pagar: ", v->valorTotal);
        printf("--------------------------------------------------\n");
        printf("1. %47s\n", "Dinheiro");
        printf("2. %47s\n", "Cartao");
        printf("3. %47s\n", "Misto (Dinheiro + Cartao)");
        printf("0. %47s\n", "Deixar venda em aberto");
        printf("\nDigite a forma de pagamento desejada: ");
        if (scanf("%d", &opcaoPagamento) != 1) {
            printf("\nERRO: Digite apenas numeros!\n");
            while (getchar() != '\n'); // Limpa a letra digitada do buffer
            opcaoPagamento = -1; // Força a repetir o loop
            continue;
        }

        switch (opcaoPagamento) {
            case 1: /* --------------     Pagamento em dinheiro     -------------- */
                pagamentoDinheiro(v, v->valorTotal, 0.0);
                break;
            case 2: /* --------------     Pagamento em Cartao     -------------- */
                pagamentoCartao(v, v->valorTotal, 0.0);
                break;
            case 3: /* --------------     Pagamento misto (Dinheiro + Cartao)     -------------- */
                do {
                    printf("Digite a parte que sera paga em dinheiro: R$ ");
                    if (scanf("%f", &valorDinheiro) != 1) {
                        printf("Erro! Digite apenas numeros.\n");
                        while (getchar() != '\n');
                        valorDinheiro = -1;
                        continue;
                    }
                } while (valorDinheiro <= 0 || valorDinheiro >= v->valorTotal);
                // Executa o fluxo passando o dinheiro inserido e calcula a falta para o cartão
                falta = v->valorTotal - valorDinheiro;
                printf("Restante a ser pago no cartao: R$ %.2f\n", falta);
                pagamentoCartao(v, falta, valorDinheiro);
                break;
            case 0: /* --------------     Deixar venda em aberto     -------------- */
                printf("\nVenda deixada em aberto para pagamento posterior.\n");
                break;
            default:
                printf("\nERRO: Opcao Invalida!\n");

        }

    } while (opcaoPagamento < 0 || opcaoPagamento > 3);

}

// Função auxiliar para pagamento em dinheiro
void pagamentoDinheiro(Venda *v, float valorAPagar, float valorCartao) {
    float valorEntregue = 0.0, troco = 0.0;

    do { 
        printf("Digite o valor recebido em dinheiro: R$ "); 
        if (scanf("%f", &valorEntregue) != 1) {
            printf("Erro! Digite apenas numeros.\n");
            while (getchar() != '\n'); // Limpa o buffer
            valorEntregue = -1;
            continue;
        }
        if (valorEntregue <= 0) {
            printf("Erro! Valor Invalido!\n");
        }
    } while (valorEntregue <= 0);

    if (valorEntregue >= valorAPagar) {
        troco = valorEntregue - valorAPagar;
        s.saldoCaixa += valorAPagar;

        Pagamento p = { 
            .numeroVenda = v->numeroVenda, 
            .valorPago = v->valorTotal, 
            .valorEntregue = valorEntregue, 
            .troco = troco
        };

        if (valorCartao > 0.0) {
            strcpy(p.tipo, "dc");
        } else {
            strcpy(p.tipo, "d");
        }

        adicionarPagamento(p);
        v->situacao = 'p'; 
        printf("Pagamento concluido! Troco: R$ %.2f\n", troco);
    } else {
        if (valorCartao == 0.0) { // Se nao for misto ainda, oferece iniciar o misto
            float falta = valorAPagar - valorEntregue;
            char resposta;
            do {
                printf("Valor Insuficiente. Faltam R$ %.2f. \nUsar cartao para o restante? (s/n): ", falta);

                if (scanf(" %c", &resposta) != 1){
                    printf("\nERRO: Digite apenas s ou n!\n");
                    while (getchar() != '\n'); // Limpa a letra digitada do buffer
                    continue;
                }

                if (resposta == 's' || resposta == 'S') {
                    break;
                } else if (resposta == 'n' || resposta == 'N'){
                    break;
                } else {
                    printf("\nERRO: Digite apenas s ou n!\n");
                    while (getchar() != '\n'); // Limpa a letra digitada do buffer
                    continue;
                }
            } while (resposta != 's' || resposta != 'S' || resposta != 'n' || resposta != 'N');

            if (resposta == 's' || resposta == 'S') {
                // Chama a funcao do cartao passando apenas o valor que falta e o dinheiro entregue
                pagamentoCartao(v, falta, valorEntregue);
            } else {
                printf("\nOperacao cancelada. A venda ficara aberta.\n");
            }
        } else {
            printf("\nErro: Valor insuficiente para a parte mista.\n");
        }
    }
}

// Função auxiliar para pagamento em cartao
void pagamentoCartao(Venda *v, float valorCartao, float valorDinheiro) {
    int statusCartao = -1;

    printf("\nValor a cobrar na maquininha: R$ %.2f\n", valorCartao);
    
    do {
        printf("\nCartao aprovado (1) ou recusado (2)? ");
        if (scanf("%d", &statusCartao) != 1) {
            printf("\nErro! Digite apenas 1 ou 2.\n");
            while (getchar() != '\n');
            statusCartao = -1;
            continue;
        }

        if (statusCartao == 1) {
            if (valorDinheiro > 0.0) {
                s.saldoCaixa += valorDinheiro; // So entra na gaveta o dinheiro fisico
            }

            Pagamento p = {
                .numeroVenda = v->numeroVenda,
                .valorPago = v->valorTotal,
                .valorEntregue = valorDinheiro,
                .troco = 0.0
            };

            // Se teve dinheiro junto eé "dc" (misto), se nao eé "c" (cartao)
            if (valorDinheiro > 0.0) {
                strcpy(p.tipo, "dc");
            } else {
                strcpy(p.tipo, "c");
            }

            adicionarPagamento(p);
            v->situacao = 'p';

            printf("\nPagamento concluido com sucesso!\n");
        } else if (statusCartao == 2) {
            printf("\nCartao recusado! A venda ficara aberta.\n");
        } else {
            printf("\nErro! Digite apenas 1 ou 2.\n");
        }
    } while (statusCartao != 1 && statusCartao != 2);
}

// Localiza vendas pendentes com situacao 'a' e permite pagar posteriormente (RF11 / RN04)
void recuperarVendaAberta() {
    if (s.vendas.total == 0) {
        printf("\nNao existem vendas registradas no sistema.\n");
        return;
    }

    int totalAbertas = 0;
    printf("\n=========== VENDAS PENDENTES (ABERTAS) ===========\n");

    for (int i = 0; i < s.vendas.total; i++) {
        if (s.vendas.dados[i].situacao != 'a') {
            continue; 
        }
        totalAbertas++;

        char cliente[50];

        if (s.vendas.dados[i].codigoCliente == 0 ) {
            strcpy(cliente, "AVULSO");
        } else {
            int posCliente = buscarClientePorCodigo(s.vendas.dados[i].codigoCliente);
            if (posCliente != -1) {
                strcpy(cliente, s.clientes.dados[posCliente].nome);
            } else {
                strcpy(cliente, "N/A");
            }
        }

        printf("%-7s%-2d | %-8s %12.12s | %-7s %6s\n",
            "Venda #", s.vendas.dados[i].numeroVenda,
            "Cliente:", cliente,
            "Status:", "ABERTO");
        printf("--------------------------------------------------\n");
        printf("%-9.9s | %-9s | %-9s | %-14s\n", "DESCRICAO", "QTD", "PRECO", "TOTAL");
        printf("--------------------------------------------------\n");
        
        for (int j = 0; j < s.vendas.dados[i].totalItens; j++) {
            printf("%-9.9s | %9d | R$ %6.2f | R$ %11.2f\n", s.vendas.dados[i].itens[j].descricao, s.vendas.dados[i].itens[j].quantidade, s.vendas.dados[i].itens[j].precoVenda, s.vendas.dados[i].itens[j].valorTotalItem);
        }
        printf("--------------------------------------------------\n");
        printf("%-35s R$ %11.2f\n", "VALOR FINAL DA COMPRA:", s.vendas.dados[i].valorTotal);
        printf("==================================================\n");
    }

    if (totalAbertas == 0) {
        printf("\nNao existem vendas em aberto no momento!\n");
        return;
    }

    int opcao = 0;
    do {printf("\nDigite o numero da venda a pagar (0 = voltar): ");
        if (scanf("%d", &opcao) != 1) {
            printf("\nERRO: Digite apenas numeros!\n");
            while (getchar() != '\n'); // Limpa a letra digitada do buffer
            opcao = -1; // Força a repetir o loop
            continue;
        }
    } while (opcao == -1);
    

    if (opcao == 0) {
        return;
    }

    int posicaoVenda = -1;
    for (int i = 0; i < s.vendas.total; i++) {
        if (s.vendas.dados[i].numeroVenda == opcao && s.vendas.dados[i].situacao == 'a') {
            posicaoVenda = i;
            break;
        }
    }

    if (posicaoVenda == -1) {
        printf("\nVenda nao encontrada ou ja se encontra paga!\n");
        return;
    }

    // Encaminha a venda encontrada para processar o pagamento
    processarPagamento(&s.vendas.dados[posicaoVenda]);
}

/* --------------     Funções auxiliares de busca por código     -------------- */

// Busca a posicao de um produto e retorna o código
int buscarProdutoPorCodigo(int codigo) {
    for (int i = 0; i < s.produtos.total; i++) {
        if (s.produtos.dados[i].codigo == codigo) {
            return i;
        }
    }
    return -1; // Produto nao encontrado = codigo livre para registro
}

// Busca a posicao de um cliente e retorna o código
int buscarClientePorCodigo(int codigo) {
    for (int i = 0; i < s.clientes.total; i++) {
        if (s.clientes.dados[i].codigo == codigo) {
            return i;
        }
    }
    return -1; // Cliente nao encontrado = codigo livre para registro
}

// Exibe a tabela de produtos disponiveis antes de adicionar itens ao carrinho
void exibirCatalogoProdutos() {
    if (s.produtos.total == 0) {
        printf("\nNao existem produtos cadastrados!\n");
        return;
    }

    printf("\n============== CATALOGO DE PRODUTOS ==============\n");
    printf("%-4s | %-9s | %-9s | %-9s | %-7s\n", "COD", "DESCRICAO", "CATEGORIA", "PRECO", "ESTOQUE");
    printf("--------------------------------------------------\n");

    for (int i = 0; i < s.produtos.total; i++) {
        printf("%-4d | %-9.9s | %-9.9s | R$ %6.2f | %7d\n",
               s.produtos.dados[i].codigo,
               s.produtos.dados[i].descricao,
               s.produtos.dados[i].categoria,
               s.produtos.dados[i].precoVenda,
               s.produtos.dados[i].estoque);
    }
    printf("==================================================\n");
}

/* --------------     Funções de relatórios     -------------- */

void listarClientes() {
    if (s.clientes.total == 0) {
        printf("\nNao existem clientes cadastrados.\n");
        return;
    }

    printf("\n============= RELATORIO DE CLIENTES ==============\n");

    for (int i = 0; i < s.clientes.total; i++) {
        printf("%-46s#%3d\n", "Cliente:", i + 1);
        printf("%-35s%15d\n", "Codigo:", s.clientes.dados[i].codigo);
        printf("%-15s %34.34s\n", "Nome:", s.clientes.dados[i].nome);
        printf("%-15s %34.34s\n", "Nome Social:", s.clientes.dados[i].nomeSocial);
        printf("%-15s %34.34s\n", "CPF:", s.clientes.dados[i].cpf);
        printf("%-15s %34.34s\n", "Endereco:", s.clientes.dados[i].endereco);
        printf("%-15s %34.34s\n", "Bairro:", s.clientes.dados[i].bairro);
        printf("%-15s %34.34s\n", "Telefone:", s.clientes.dados[i].telefone);
        printf("--------------------------------------------------\n");
    }
}

void listarProdutos() {
    if (s.produtos.total == 0) {
        printf("\nNao existem produtos cadastrados.\n");
        return;
    }

    printf("\n============= RELATORIO DE PRODUTOS ==============\n");

    for (int i = 0; i < s.produtos.total; i++) {
        printf("%-46s#%3d\n", "Produto:", i + 1);
        printf("%-35s%15d\n", "Codigo:", s.produtos.dados[i].codigo);
        printf("%-15s %34.34s\n", "Descricao:", s.produtos.dados[i].descricao);
        printf("%-15s %34.34s\n", "Categoria:", s.produtos.dados[i].categoria);
        printf("%-41sR$ %6.2f\n", "Preco de compra:", s.produtos.dados[i].precoCompra);
        printf("%-35s%14.2f%%\n", "Margem de lucro:", s.produtos.dados[i].margemLucro);
        printf("%-41sR$ %6.2f\n", "Preco de venda:", s.produtos.dados[i].precoVenda);
        printf("%-35s%15d\n", "Estoque atual:", s.produtos.dados[i].estoque);
        printf("%-35s%15d\n", "Estoque minimo:", s.produtos.dados[i].estoqueMinimo);
        printf("--------------------------------------------------\n");
    }
}

void listarVendas() {
    if (s.vendas.total == 0) {
        printf("\nNao existem vendas registradas no sistema.\n");
        return;
    }

    printf("\n=============== RELATORIO DE VENDAS ==============\n");

    for (int i = 0; i < s.vendas.total; i++) {
        char statusTexto[10];
        char cliente[50];

        if (s.vendas.dados[i].codigoCliente == 0 ) {
            strcpy(cliente, "AVULSO");
        } else {
            strcpy(cliente, s.clientes.dados[buscarClientePorCodigo(s.vendas.dados[i].codigoCliente)].nome);
        }
        if (s.vendas.dados[i].situacao == 'p') {
            strcpy(statusTexto, "PAGO");
        } else {
            strcpy(statusTexto, "ABERTO");
        }

        printf("%-7s%-2d | %-8s %12.12s | %-7s %6s\n",
            "Venda #", s.vendas.dados[i].numeroVenda,
            "Cliente:", cliente,
            "Status:", statusTexto);
        printf("--------------------------------------------------\n");
        printf("%-9.9s | %-9s | %-9s | %-14s\n", "DESCRICAO", "QTD", "PRECO", "TOTAL");
        printf("--------------------------------------------------\n");
        
        for (int j = 0; j < s.vendas.dados[i].totalItens; j++) {
            printf("%-9.9s | %9d | R$ %6.2f | R$ %11.2f\n", s.vendas.dados[i].itens[j].descricao, s.vendas.dados[i].itens[j].quantidade, s.vendas.dados[i].itens[j].precoVenda, s.vendas.dados[i].itens[j].valorTotalItem);
        }
        printf("--------------------------------------------------\n");
        printf("%-35s R$ %11.2f\n", "VALOR FINAL DA COMPRA:", s.vendas.dados[i].valorTotal);

        // Se a venda estiver paga, exibe as informacoes gravadas do pagamento
        if (s.vendas.dados[i].situacao == 'p') {
            for (int k = 0; k < s.pagamentos.total; k++) {
                if (s.pagamentos.dados[k].numeroVenda == s.vendas.dados[i].numeroVenda) {
                    if (strcmp(s.pagamentos.dados[k].tipo, "d") == 0) {
                        printf("%-20s%30s\n%-35s R$ %11.2f\n", "Forma de Pagamento: ", "Dinheiro", "Valor da Venda:", s.pagamentos.dados[k].valorPago);
                        printf("%-35s R$ %11.2f\n%-35s R$ %11.2f\n", "Valor Entregue:", s.pagamentos.dados[k].valorEntregue, "Troco:", s.pagamentos.dados[k].troco);
                    } else if (strcmp(s.pagamentos.dados[k].tipo, "c") == 0) {
                        printf("%-20s%30s\n%-35s R$ %11.2f\n", "Forma de Pagamento: ", "Cartao", "Valor da Venda:", s.pagamentos.dados[k].valorPago);
                    } else if (strcmp(s.pagamentos.dados[k].tipo, "dc") == 0) {
                        printf("%-20s%30s\n%-35s R$ %11.2f\n", "Forma de Pagamento: ", "Misto (Dinheiro + Cartao)", "Valor da Venda:", s.pagamentos.dados[k].valorPago);
                        printf("%-35s R$ %11.2f\n%-35s R$ %11.2f\n", "Recebido em Dinheiro:", s.pagamentos.dados[k].valorEntregue, "Restante no Cartao:", s.pagamentos.dados[k].valorPago - s.pagamentos.dados[k].valorEntregue);
                    }
                    break;
                }
            }
        }
        printf("==================================================\n");
    }
}

/* --------------     Funções de gerenciamento de persistência em arquivo     -------------- */

void salvarArquivo() {
    FILE *fClientes = fopen("clientes.dat", "wb");
    FILE *fProdutos = fopen("produtos.dat", "wb");
    FILE *fVendas   = fopen("vendas.dat", "wb");
    FILE *fPagamentos = fopen("pagamentos.dat", "wb");

    if (fClientes == NULL || fProdutos == NULL || fVendas == NULL || fPagamentos == NULL) {
        printf("\nERRO: Não foi possível abrir os arquivos para gravação binária!\n");
        if (fClientes) fclose(fClientes);
        if (fProdutos) fclose(fProdutos);
        if (fVendas) fclose(fVendas);
        if (fPagamentos) fclose(fPagamentos);
        return;
    }

    // Grava Clientes (Salva o vetor inteiro de uma vez só)
    if (s.clientes.total > 0) {
        fwrite(s.clientes.dados, sizeof(Cliente), s.clientes.total, fClientes);
    }

    // Grava Produtos (Salva o vetor inteiro de uma vez só)
    if (s.produtos.total > 0) {
        fwrite(s.produtos.dados, sizeof(Produto), s.produtos.total, fProdutos);
    }

    // Grava Vendas e seus respectivos Carrinhos
    for (int i = 0; i < s.vendas.total; i++) {
        // Primeiro grava a struct da Venda em si sem os itens
        fwrite(&s.vendas.dados[i], sizeof(Venda), 1, fVendas);
        
        // Se houver itens no carrinho, grava na sequência
        if (s.vendas.dados[i].totalItens > 0) {
            fwrite(s.vendas.dados[i].itens, sizeof(ItemCarrinho), s.vendas.dados[i].totalItens, fVendas);
        }
    }

    // Grava Pagamentos
    if (s.pagamentos.total > 0) {
        fwrite(s.pagamentos.dados, sizeof(Pagamento), s.pagamentos.total, fPagamentos);
    }

    fclose(fClientes);
    fclose(fProdutos);
    fclose(fVendas);
    fclose(fPagamentos);

    printf("\nDados gravados com sucesso no formato.dat!\n");
}

void carregarArquivo() {
    FILE *fClientes = fopen("clientes.dat", "rb");
    FILE *fProdutos = fopen("produtos.dat", "rb");
    FILE *fVendas   = fopen("vendas.dat", "rb");
    FILE *fPagamentos = fopen("pagamentos.dat", "rb");

    // Se nenhum arquivo existir, inicia o sistema zerado
    if (fClientes == NULL && fProdutos == NULL && fVendas == NULL && fPagamentos == NULL) {
        printf("\nNenhum cadastro anterior encontrado.\nIniciando sistema vazio.\n");
        return;
    }

    // Carrega Clientes
    if (fClientes != NULL) {
        Cliente c;
        while (fread(&c, sizeof(Cliente), 1, fClientes) == 1) {
            adicionarCliente(c); // Reutiliza a função que faz realloc automaticamente
        }
        fclose(fClientes);
    }

    // Carrega Produtos
    if (fProdutos != NULL) {
        Produto p;
        while (fread(&p, sizeof(Produto), 1, fProdutos) == 1) {
            adicionarProduto(p); // Reutiliza a função que faz realloc automaticamente
        }
        fclose(fProdutos);
    }

    // Carrega Vendas e reconstrói os Carrinhos Dinâmicos
    if (fVendas != NULL) {
        Venda v;
        while (fread(&v, sizeof(Venda), 1, fVendas) == 1) {
            if (v.totalItens > 0) {
                v.itens = (ItemCarrinho *) malloc(v.totalItens * sizeof(ItemCarrinho));
                fread(v.itens, sizeof(ItemCarrinho), v.totalItens, fVendas);
            } else {
                v.itens = NULL;
            }
            adicionarVenda(v); // Adiciona na lista global s.vendas
        }
        fclose(fVendas);
    }

    // Carrega Pagamentos
    if (fPagamentos != NULL) {
        Pagamento pag;
        while (fread(&pag, sizeof(Pagamento), 1, fPagamentos) == 1) {
            adicionarPagamento(pag);
        }
        fclose(fPagamentos);
    }

    printf("\nDados carregados com sucesso!\n");
    printf("%-35s%15d\n%-35s%15d\n%-35s%15d\n%-35s%15d\n", 
        "Clientes:", s.clientes.total,
        "Produtos:", s.produtos.total,
        "Vendas:", s.vendas.total,
        "Pagamentos:", s.pagamentos.total);
}



