#include "filesystem.h"

BTree* btree_create() {
    BTree* tree = malloc(sizeof(BTree));
    tree->root = NULL;
    return tree;
}

TreeNode* create_txt_file(const char* name, const char* content) {
    File* file = malloc(sizeof(File));
    file->name = strdup(name);
    file->content = strdup(content);
    file->size = strlen(content);

    TreeNode* node = malloc(sizeof(TreeNode));
    node->name = strdup(name);
    node->type = FILE_TYPE;
    node->data.file = file;
    return node;
}

TreeNode* create_directory(const char* name) {
    Directory* dir = malloc(sizeof(Directory));
    dir->tree = btree_create();

    TreeNode* node = malloc(sizeof(TreeNode));
    node->name = strdup(name);
    node->type = DIRECTORY_TYPE;
    node->data.directory = dir;
    return node;
}

void btree_insert(BTree* tree, TreeNode* node) {
    if (!tree->root) {
        tree->root = malloc(sizeof(BTreeNode));
        tree->root->num_keys = 0;
        tree->root->leaf = 1;
        for (int i = 0; i < 2 * BTREE_ORDER; i++)
            tree->root->children[i] = NULL;
    }

    BTreeNode* r = tree->root;

    if (r->num_keys == 2 * BTREE_ORDER - 1) {
        printf("Erro: Nó cheio. Divisão de nós não implementada.\n");
        return;
    }

    int i = r->num_keys - 1;
    while (i >= 0 && strcmp(node->name, r->keys[i]->name) < 0) {
        r->keys[i + 1] = r->keys[i];
        i--;
    }
    r->keys[i + 1] = node;
    r->num_keys++;
}

TreeNode* btree_search(BTree* tree, const char* name) {
    if (!tree || !tree->root) return NULL;
    BTreeNode* node = tree->root;

    for (int i = 0; i < node->num_keys; i++) {
        if (strcmp(name, node->keys[i]->name) == 0)
            return node->keys[i];
    }
    return NULL;
}

void btree_delete(BTree* tree, const char* name) {
    if (!tree || !tree->root) return;
    BTreeNode* node = tree->root;

    for (int i = 0; i < node->num_keys; i++) {
        if (strcmp(name, node->keys[i]->name) == 0) {
            free(node->keys[i]->name);
            if (node->keys[i]->type == FILE_TYPE) {
                free(node->keys[i]->data.file->content);
                free(node->keys[i]->data.file);
            } else {
                // Apenas libera o ponteiro (não libera recursivamente)
                free(node->keys[i]->data.directory);
            }
            free(node->keys[i]);

            for (int j = i; j < node->num_keys - 1; j++)
                node->keys[j] = node->keys[j + 1];

            node->num_keys--;
            return;
        }
    }
}

void btree_traverse(BTree* tree) {
    if (!tree || !tree->root) return;
    BTreeNode* node = tree->root;

    for (int i = 0; i < node->num_keys; i++) {
        TreeNode* item = node->keys[i];
        printf("- %s (%s)\n", item->name, item->type == FILE_TYPE ? "Arquivo" : "Diretório");
    }
}

void delete_txt_file(BTree* tree, const char* name) {
    TreeNode* node = btree_search(tree, name);
    if (node && node->type == FILE_TYPE) {
        btree_delete(tree, name);
        printf("Arquivo '%s' deletado com sucesso.\n", name);
    } else {
        printf("Arquivo '%s' não encontrado.\n", name);
    }
}

void delete_directory(BTree* tree, const char* name) {
    TreeNode* node = btree_search(tree, name);
    if (node && node->type == DIRECTORY_TYPE) {
        Directory* dir = node->data.directory;
        if (dir->tree->root && dir->tree->root->num_keys > 0) {
            printf("Diretório '%s' não está vazio.\n", name);
        } else {
            btree_delete(tree, name);
            printf("Diretório '%s' deletado com sucesso.\n", name);
        }
    } else {
        printf("Diretório '%s' não encontrado.\n", name);
    }
}

Directory* get_root_directory() {
    Directory* root = malloc(sizeof(Directory));
    root->tree = btree_create();
    return root;
}

void change_directory(Directory** current, const char* path) {
    TreeNode* node = btree_search((*current)->tree, path);
    if (node && node->type == DIRECTORY_TYPE) {
        *current = node->data.directory;
        printf("Diretório alterado para '%s'\n", path);
    } else {
        printf("Diretório '%s' não encontrado.\n", path);
    }
}

void list_directory_contents(Directory* dir) {
    if (!dir) return;
    printf("Conteúdo do diretório:\n");
    btree_traverse(dir->tree);
}
