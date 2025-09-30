#include <stdio.h>
struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
}
int maxDepth(struct TreeNode* root){
    if (root==NULL)return 0;
    return fmax(depth(root->left),depth(root->right))+1;
}
int main()
{
    return 0;
}

