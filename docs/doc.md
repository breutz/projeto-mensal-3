# Sistema de Gerenciamento Automatizado de Mercearia - Bere is Nice

## Objetivo

Profissionalizar o sistema de gerenciamento automatizado da mercearia da Dona Berê em linguagem C através de cadastros dinâmicos flexíveis, estruturas heterogêneas, gerenciamento e alocação dinâmica de memória, vendas com carrinho progressivo, pagamentos combinados, controle de sangria de caixa com reserva de troco, conciliação contábil no fechamento de caixa, relatórios estruturados e persistência de dados em arquivos de texto.

---

## Problemas Identificados

- Limitação de tamanho fixo em vetores rígidos, impedindo o crescimento livre do negócio;
- Inconsistência de cadastros com dados espalhados em matrizes e vetores paralelos;
- Falta de carrinho dinâmico para compras de múltiplos itens na mesma transação;
- Inexistência de pagamento misto quando o cliente possui dinheiro insuficiente;
- Ausência de controle de sangria seguro durante o expediente;
- Falta de conciliação financeira automatizada no fechamento de caixa com conferência de divergências;
- Perda total dos dados cadastrados e vendas ao encerrar o sistema no console.

---

## Requisitos Funcionais

- RF01: Implementação em linguagem C compilada e executada em terminal de console.
- RF02: Menu Principal profissional com navegação em submenus (Cadastros, Vendas, Abertura, Fechamento, Relatórios e Sair) com retorno seguro.
- RF03: Cadastro dinâmico de clientes sem limite predeterminado de registros.
- RF04: Cadastro dinâmico de produtos com código, descrição, categoria, preço de compra, margem de lucro, preço de venda, estoque e estoque mínimo.
- RF05: Preço de venda calculado automaticamente no ato da venda ou cadastro a partir da margem de lucro vigente.
- RF06: Consulta e catálogo estruturado de produtos e estoques antes de iniciar a venda.
- RF07: Carrinho de compras progressivo aceitando múltiplos itens e totalização progressiva da compra.
- RF08: Desconto informado pelo operador em percentual aplicado sobre o subtotal do carrinho.
- RF09: Processamento de pagamento em dinheiro com cálculo de troco e suporte automático a pagamento misto (dinheiro + cartão).
- RF10: Processamento de pagamento em cartão com simulação de maquininha (1 aprovado / 0 recusado).
- RF11: Recuperação e quitação de vendas esquecidas no caixa com situação em aberto.
- RF12: Sangria de caixa com regra de segurança que preserva obrigatoriamente no mínimo R$ 50,00 na gaveta para troco.
- RF13: Abertura de caixa com validação obrigatória de valores não negativos.
- RF14: Fechamento de caixa com conciliação matemática contábil e tratamento de divergências (sobra/falta).
- RF15: Relatórios legíveis e estruturados de clientes, produtos e vendas.
- RF16: Persistência em arquivo de texto com rotinas completas de gravação e carregamento.

---

## Modelos de Dados Heterogêneos (Structs)

### Cliente
- `int codigo`: Código identificador único do cliente
- `char nome[60]`: Nome completo
- `char nomeSocial[60]`: Nome social
- `char cpf[15]`: CPF formatado
- `char endereco[80]`: Rua e número
- `char bairro[40]`: Bairro
- `char telefone[20]`: Celular ou WhatsApp

### Produto
- `int codigo`: Código do produto
- `char descricao[50]`: Descrição do produto
- `char categoria[30]`: Categoria (Alimento, Material de Limpeza ou Panificação)
- `float precoCompra`: Preço de custo/compra
- `float margemLucro`: Margem de lucro em percentual
- `float precoVenda`: Preço de venda calculado
- `int estoque`: Quantidade disponível em estoque
- `int estoqueMinimo`: Estoque mínimo de segurança

### Item do Carrinho
- `int codigo`: Código do produto comprado
- `char descricao[50]`: Descrição copiada do produto
- `float precoVenda`: Preço praticado no momento da venda
- `int quantidade`: Quantidade de unidades compradas
- `float valorTotalItem`: Subtotal do item (`precoVenda * quantidade`)

### Venda
- `int numeroVenda`: Identificador sequencial da venda
- `int codigoCliente`: Código do cliente vinculado (ou 0 para avulso)
- `ItemCarrinho *itens`: Ponteiro para o vetor dinâmico de itens no carrinho
- `int totalItens`: Quantidade de itens distintos no carrinho
- `float valorTotal`: Valor final da compra com desconto aplicado
- `char situacao`: Situação da venda (`'a'` para Aberto / `'p'` para Pago)

### Pagamento
- `int numeroVenda`: Número da venda associada
- `float valorPago`: Valor financeiro quitado
- `char tipo[3]`: Forma de pagamento (`"d"`, `"c"` ou `"dc"`)
