#include "filesystem.h"

int main() {
    Directory* root = get_root_directory();

    TreeNode* dirA = create_directory("A");
    TreeNode* dirB = create_directory("B");
    TreeNode* file1 = create_txt_file("readme.txt", "Conteúdo do arquivo");

    btree_insert(root->tree, dirA);
    btree_insert(root->tree, dirB);
    btree_insert(root->tree, file1);

    printf("\n[ROOT]\n");
    list_directory_contents(root);

    change_directory(&root, "A"); // teste de navegação

    return 0;
}