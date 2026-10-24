# Casos de Teste Mínimos – Projeto Mensal 3

Este documento registra os 15 casos de teste mínimos exigidos pelo Modelo Acadêmico do Projeto Mensal 3, com os retornos e comportamentos verificados e homologados.

---

## CT01 – Cliente Dinâmico
- Cadastrar clientes sucessivamente.
    Retorno do Teste: Memória expandida dinamicamente com sucesso. Todos os registros permanecem acessíveis e com dados íntegros na listagem de clientes.

---

## CT02 – Produto e Margem
- Preço de compra R$ 10,00; margem 20%.
    Retorno do Teste: Preço de venda calculado automaticamente em R$ 12,00 no ato do cadastro e mantido consistente.

---

## CT03 – Estoque Mínimo
- Vender item com estoque no limite minimo.
    Retorno do Teste: Sistema exibe o alerta: `AVISO: O produto atingiu o estoque minimo! (Restam: X)`.

---

## CT04 – Estoque Insuficiente
- Solicitar 4 unidades com apenas 3 disponíveis em estoque.
    Retorno do Teste: Sistema exibe alerta de quantidade superior ao estoque, informa o saldo disponível e pergunta ao operador se deseja levar todo o saldo disponível de 3 unidades.

---

## CT05 – Estoque Zerado
- Selecionar produto com estoque igual a zero.
    Retorno do Teste: Sistema bloqueia a inclusão no carrinho e informa: `ATENCAO: Produto com estoque zerado! Nao eh possivel vender.`

---

## CT06 – Carrinho Progressivo
- Adicionar Café e Esponja na mesma sessão de compra.
    Retorno do Teste: Itens alocados dinamicamente no vetor `itens` da struct `Venda`. Subtotal progressivo e totalizador do carrinho calculados com exatidão.

---

## CT07 – Desconto
- Subtotal de R$ 18,39; desconto informado de 5%.
    Retorno do Teste: Desconto de R$ 0,92 deduzido com exatidão, resultando no total a pagar de R$ 17,47.

---

## CT08 – Dinheiro Insuficiente
- Total da compra R$ 50,00; valor entregue em dinheiro R$ 30,00.
    Retorno do Teste: Sistema identifica a falta de R$ 20,00 e oferece automaticamente o pagamento misto (`dc`), passando o restante no cartão.

---

## CT09 – Cartão Recusado
- Informar status 2 (Recusado) na maquininha de cartão.
    Retorno do Teste: Sistema preserva a venda em aberto com status `'a'`, não perde os itens do carrinho e permite quitação posterior.

---

## CT10 – Venda Aberta
- Finalizar o carrinho e escolher deixar a venda em aberto (opção 0 no pagamento).
    Retorno do Teste: Venda registrada com situação `'a'`. Localizada e quitada com sucesso no menu `2. Vendas -> 3. Pagamento (Venda Aberta)`.

---

## CT11 – Sangria Válida
- Saldo em gaveta de R$ 100,00; realizar retirada de R$ 30,00.
    Retorno do Teste: Retirada efetuada com sucesso, saldo da gaveta atualizado para R$ 70,00 e acumulador de sangria atualizado.

---

## CT12 – Sangria Inválida
- Saldo em gaveta de R$ 70,00; tentar retirar R$ 30,00 (deixaria R$ 40,00).
    Retorno do Teste: Retirada bloqueada imediatamente com aviso de que o saldo mínimo obrigatório de R$ 50,00 para troco deve ser preservado.

---

## CT13 – Abertura Inválida
- Informar valor negativo (ex: R$ -20,00) na abertura do caixa.
    Retorno do Teste: Sistema rejeita o valor com mensagem de erro e solicita nova digitação até que seja informado valor maior ou igual a zero.

---

## CT14 – Fechamento Divergente
- Realizar o fechamento do caixa com divergência na fórmula contábil da RN07.
    Retorno do Teste: Exibe mensagem obrigatória: `ATENCAO: O caixa nao podera ser fechado porque ha divergencia de valores!`, detalha a natureza (SOBRA ou FALTA) e exige confirmação do ajuste.

---

## CT15 – Persistência em Arquivo
- Responder "s" ao sair do sistema.
    Retorno do Teste: Dados gravados em arquivos binários separados (`clientes.dat`, `produtos.dat`, `vendas.dat` e `pagamentos.dat`), e carregados com integridade na reinicialização do sistema.
