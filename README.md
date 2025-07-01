# Projeto de Sistema de Arquivos com Árvore B

A principal característica do projeto é o uso de uma *Árvore B* como estrutura de dados para organizar hierarquicamente os diretórios e arquivos, permitindo operações eficientes de busca, inserção e remoção

---

## Estrutura do Código

O projeto está organizado nos seguintes arquivos:

* main.c: Contém a interface do usuário, com um menu interativo para manipulação do sistema de arquivos.
* filesystem.c: Implementa toda a lógica do sistema.Aqui estão as funções da Árvore B e as operações de manipulação de arquivos e diretórios.
* filesystem.h: O arquivo de cabeçalho que define todas as estruturas de dados (File, Directory, TreeNode, BTree, etc.) e os protótipos das funções.
* Makefile: Script para compilação e limpeza do projeto.

---

## Como Compilar e Executar

### Pré-requisitos
* Um compilador C (como o GCC).
* O utilitário make.

### Passos

1.  *Compilar o projeto:*
    No terminal, na raiz do diretório do projeto, execute o comando:
    bash
    make
    
    Isso irá compilar os arquivos-fonte e gerar um executável chamado main.

2.  *Executar o programa:*
    Para iniciar o sistema de arquivos virtual, execute:
    bash
    ./main
    

3.  *Limpar os arquivos compilados:*
    Para remover o executável e os arquivos objeto (.o), use o comando:
    bash
    make clean
    

---

## Funcionalidades Implementadas

O sistema oferece as seguintes operações através de um menu no console:

* *1. Criar diretório*: Cria um novo diretório dentro da pasta atual.
* *2. Criar arquivo*: Cria um novo arquivo de texto com conteúdo.
* *3. Excluir diretório*: Remove um diretório, somente se ele estiver vazio.
* *4. Excluir arquivo*: Remove um arquivo pelo nome.
* *5. Mudar para subdiretório*: Navega para uma pasta filha.
* *6. Listar conteúdo*: Exibe os arquivos e subdiretórios da pasta atual.
* *7. Voltar para o diretório anterior*: Retorna à pasta pai.
* *0. Sair*: Encerra o programa.