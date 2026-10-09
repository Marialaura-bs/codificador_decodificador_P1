# Codificador e Decodificador de Boletos

## 1. Autora

**Maria Laura Barbosa da Silva**

## 2. Descrição do projeto

Este projeto foi desenvolvido em C++ para trabalhar com a codificação e a decodificação de códigos de barras de boletos bancários, seguindo as regras da FEBRABAN e a documentação técnica utilizada como referência no trabalho.

O projeto é dividido em dois programas:

* **Codificador:** recebe informações do boleto, como banco, moeda, data de vencimento, valor e campo livre. A partir desses dados, calcula os dígitos verificadores e gera o código de barras e a linha digitável.
* **Decodificador:** recebe um código de barras e procura validar sua estrutura, separar seus campos e apresentar as informações que podem ser extraídas, como banco, moeda, fator de vencimento, valor e campo livre.

O desenvolvimento foi realizado utilizando funções para separar as responsabilidades do programa, facilitando a organização e a compreensão do código.

## 3. Compilação e execução

### Requisitos

* Linux ou ambiente Linux acessível pelo terminal, como o Git Bash com um compilador configurado ou o WSL.
* Compilador C++ `g++`.
* Arquivos-fonte do codificador e do decodificador.

### Verificar a instalação do compilador

No terminal, execute:

```bash
g++ --version
```

Se o compilador estiver instalado, será exibida a versão disponível.

### Compilar o codificador

Entre na pasta onde estão os arquivos do projeto e execute:

```bash
g++ -Wall -Wextra -std=c++17 codificador.cpp -o codificador
```

Para executar:

```bash
./codificador
```

### Compilar o decodificador

```bash
g++ -Wall -Wextra -std=c++17 decodificador.cpp -o decodificador
```

Para executar:

```bash
./decodificador
```

**Observação:** os nomes `codificador.cpp` e `decodificador.cpp` são exemplos. Caso os arquivos tenham outros nomes, substitua-os pelos nomes utilizados no projeto.

As opções `-Wall` e `-Wextra` habilitam avisos adicionais do compilador, ajudando a identificar possíveis problemas no código. A opção `-std=c++17` seleciona o padrão C++17.

## 4. Limitações conhecidas

Durante o desenvolvimento, algumas simplificações foram adotadas. Elas influenciam a quantidade de informações que o programa consegue identificar e a precisão de determinadas interpretações.

### 4.1. Ausência do campo de agência e identificação dos convênios

O programa não recebe o nosso número como uma informação separada. Em vez disso, os dados relacionados ao convênio são tratados por meio do campo livre do código de barras.

Essa decisão simplificou a entrada e a organização dos dados, mas também limitou a identificação dos diferentes tipos de convênio. Como o Nosso número não é informado separadamente e o programa não recebe todas as informações necessárias para distinguir os formatos, não é possível identificar com precisão todos os tamanhos de convênio apenas pelo conteúdo do campo livre.

O programa consegue reconhecer alguns formatos por características específicas da sua estrutura, como o preenchimento fixo de seis posições utilizado no formato de sete posições e o sufixo `21` adotado na implementação do formato livre de 17 posições. Entretanto, essas características não garantem a identificação correta em todos os casos, pois estruturas semelhantes podem gerar ambiguidades.

### 4.2. Identificação do fator de vencimento

O decodificador não consegue determinar com precisão, em todos os casos, se o código de barras contém um fator de vencimento ou se as mesmas posições estão sendo utilizadas para representar integralmente o valor do boleto, pois os documentos não dão informações o suficiente para fazer essa diferenciação e o fator de vencimento pode acabar sendo confundido com o início do valor de 14 dígitos.

Para fazer essa verificação da forma mais eficiente o programa utiliza uma regra simplificada para interpretar o campo: quando os primeiros dígitos do campo analisado começam com `0`, assume-se que não há fator de vencimento e que as 14 posições correspondem ao valor. Nos demais casos, as quatro primeiras posições são interpretadas como fator de vencimento e as dez seguintes como valor.

Essa regra é uma aproximação adotada para permitir a decodificação, mas não é uma forma universalmente confiável de distinguir os dois casos. Dependendo do valor e da estrutura do código recebido, a interpretação pode estar incorreta.

### 4.3. Fator de vencimento e datas

O cálculo do fator de vencimento utiliza uma data-base e a diferença em dias entre essa data e o vencimento informado. Entretanto, a interpretação do fator em códigos recebidos exige atenção às mudanças de regra e aos ciclos de reinício do fator de vencimento.

Assim, a conversão entre fator e data deve ser considerada dentro das regras implementadas no programa, não sendo garantida para todos os períodos e formatos possíveis.

### 4.4. Escopo dos formatos implementados

O programa utiliza regras específicas para montar e interpretar os campos do código de barras e da linha digitável. Embora alguns formatos de convênio sejam tratados por condições próprias, a identificação automática realizada pelo decodificador possui limitações.

Dessa forma, o programa deve ser considerado uma implementação acadêmica das regras estudadas, e não uma ferramenta de validação bancária para uso em operações financeiras reais.

## 5. Dificuldades de aprendizagem

Durante o desenvolvimento, alguns dos principais desafios foram compreender a estrutura do código de barras e da linha digitável, organizar os dados nos campos corretos e implementar os cálculos dos dígitos verificadores.

Também foi necessário compreender o cálculo do fator de vencimento, a conversão do valor para centavos e a utilização de funções para dividir o problema em etapas menores.

Outra dificuldade foi interpretar a documentação técnica e transformar suas regras em instruções C++, além de testar os resultados para identificar possíveis erros na montagem dos códigos.

O desenvolvimento contribuiu para praticar o uso de funções, strings, estruturas de repetição, condições, conversão de dados e organização de programas em C++.
