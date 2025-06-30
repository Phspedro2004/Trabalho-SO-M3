// filesystem.c - versão final com árvore B balanceada e suporte a diretórios com ponteiro para o pai

#include "filesystem.h"

void btree_split_child(BTreeNode* parent, int i, BTreeNode* full_child) {
    int t = BTREE_ORDER;
    BTreeNode* new_child = malloc(sizeof(BTreeNode));
    new_child->leaf = full_child->leaf;
    new_child->num_keys = t - 1;

    for (int j = 0; j < t - 1; j++)
        new_child->keys[j] = full_child->keys[j + t];

    if (!full_child->leaf) {
        for (int j = 0; j < t; j++)
            new_child->children[j] = full_child->children[j + t];
    }

    full_child->num_keys = t - 1;

    for (int j = parent->num_keys; j >= i + 1; j--)
        parent->children[j + 1] = parent->children[j];

    parent->children[i + 1] = new_child;

    for (int j = parent->num_keys - 1; j >= i; j--)
        parent->keys[j + 1] = parent->keys[j];

    parent->keys[i] = full_child->keys[t - 1];
    parent->num_keys++;
}

void btree_insert_nonfull(BTreeNode* node, TreeNode* key) {
    int i = node->num_keys - 1;

    if (node->leaf) {
        while (i >= 0 && strcmp(key->name, node->keys[i]->name) < 0) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->num_keys++;
    } else {
        while (i >= 0 && strcmp(key->name, node->keys[i]->name) < 0)
            i--;
        i++;
        if (node->children[i]->num_keys == 2 * BTREE_ORDER - 1) {
            btree_split_child(node, i, node->children[i]);
            if (strcmp(key->name, node->keys[i]->name) > 0)
                i++;
        }
        btree_insert_nonfull(node->children[i], key);
    }
}

BTree* btree_create() {
    BTree* tree = malloc(sizeof(BTree));
    tree->root = NULL;
    return tree;
}

void btree_insert(BTree* tree, TreeNode* key) {
    if (!tree->root) {
        tree->root = malloc(sizeof(BTreeNode));
        tree->root->num_keys = 0;
        tree->root->leaf = 1;
        for (int i = 0; i < 2 * BTREE_ORDER; i++)
            tree->root->children[i] = NULL;
    }

    BTreeNode* r = tree->root;
    if (r->num_keys == 2 * BTREE_ORDER - 1) {
        BTreeNode* s = malloc(sizeof(BTreeNode));
        s->leaf = 0;
        s->num_keys = 0;
        s->children[0] = r;
        tree->root = s;
        btree_split_child(s, 0, r);
        btree_insert_nonfull(s, key);
    } else {
        btree_insert_nonfull(r, key);
    }
}

TreeNode* btree_search_node(BTreeNode* node, const char* name) {
    int i = 0;
    while (i < node->num_keys && strcmp(name, node->keys[i]->name) > 0)
        i++;
    if (i < node->num_keys && strcmp(name, node->keys[i]->name) == 0)
        return node->keys[i];
    if (node->leaf)
        return NULL;
    return btree_search_node(node->children[i], name);
}

TreeNode* btree_search(BTree* tree, const char* name) {
    if (!tree || !tree->root) return NULL;
    return btree_search_node(tree->root, name);
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

TreeNode* create_directory(const char* name, Directory* parent) {
    Directory* dir = malloc(sizeof(Directory));
    dir->tree = btree_create();
    dir->parent = parent;

    TreeNode* node = malloc(sizeof(TreeNode));
    node->name = strdup(name);
    node->type = DIRECTORY_TYPE;
    node->data.directory = dir;
    return node;
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
    root->parent = NULL;
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
