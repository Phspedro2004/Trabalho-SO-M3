#include "filesystem.h"
#include <stdio.h>

int main() {
    Directory* root = get_root_directory();
    Directory* current = root;
    int opcao;
    char nome[100];
    char conteudo[1024];

    do {
        printf("\n=== MENU ===\n");
        printf("1. Criar diretório\n");
        printf("2. Criar arquivo\n");
        printf("3. Excluir diretório\n");
        printf("4. Excluir arquivo\n");
        printf("5. Mudar para subdiretório\n");
        printf("6. Listar conteúdo do diretório atual\n");
        printf("7. Voltar para o diretório anterior\n");
        printf("0. Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);
        getchar(); // consumir '\n'

        switch (opcao) {
            case 1:
                printf("Nome do diretório: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = 0;
                btree_insert(current->tree, create_directory(nome, current));
                break;

            case 2:
                printf("Nome do arquivo: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = 0;
                printf("Conteúdo do arquivo: ");
                fgets(conteudo, sizeof(conteudo), stdin);
                conteudo[strcspn(conteudo, "\n")] = 0;
                btree_insert(current->tree, create_txt_file(nome, conteudo));
                break;

            case 3:
                printf("Nome do diretório a excluir: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = 0;
                delete_directory(current->tree, nome);
                break;

            case 4:
                printf("Nome do arquivo a excluir: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = 0;
                delete_txt_file(current->tree, nome);
                break;

            case 5:
                printf("Nome do subdiretório: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = 0;
                change_directory(&current, nome);
                break;

            case 6:
                list_directory_contents(current);
                break;

            case 7:
                if (current->parent != NULL) {
                    current = current->parent;
                    printf("Voltando para o diretório pai.\n");
                } else {
                    printf("Você já está no diretório raiz.\n");
                }
                break;

            case 0:
                printf("Encerrando...\n");
                break;

            default:
                printf("Opção inválida.\n");
        }

    } while (opcao != 0);

    return 0;
}