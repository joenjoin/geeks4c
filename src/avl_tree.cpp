#include <stdio.h>

#define MAX(x, y) ((x)>(y)?(x):(y))
#define DEFAULT_NODE {0, NULL, NULL, -1}

typedef struct __node_t {
    int data;

    struct __node_t* left;
    struct __node_t* right;

    int height;

} node_t;

void pre_order_traverse(node_t* root) {
    if (!root)
        return;

    printf("%d(%d) ", root->data, root->height);

    if (root->left)
        pre_order_traverse(root->left);

    if (root->right)
        pre_order_traverse(root->right);
}

void in_order_traverse(node_t* root) {
    if (!root)
        return;

    if (root->left)
        in_order_traverse(root->left);

    printf("%d(%d) ", root->data, root->height);

    if (root->right)
        in_order_traverse(root->right);
}

int get_height(node_t* root) {
    if (!root)
        return -1;

    int left_height = root->left == NULL ? -1 : root->left->height;
    int right_height = root->right == NULL ? -1 : root->right->height;

    int height = MAX(left_height, right_height) + 1;

    return height;
}

node_t* rotate_left(node_t* node) {
    printf("---- rotate left (%d)\n", node->data);

    node_t* pivot = node->right;
    node_t* pivot_left = pivot->left;

    node->right = pivot_left;
    pivot->left = node;

    node->height = (pivot_left == NULL ? -1 : pivot_left->height) + 1;
    pivot->height = get_height(pivot);

    return pivot;
}

node_t* rotate_right(node_t* node) {
    printf("---- rotate right (%d)\n", node->data);

    node_t* pivot = node->left;
    node_t* pivot_right = pivot->right;

    node->left = pivot_right;
    pivot->right = node;

    node->height = (pivot_right == NULL ? -1 : pivot_right->height) + 1;
    pivot->height = get_height(pivot);

    return pivot;
}

/**
 * Balance a normal tree
 */
node_t* make_balance(node_t* root) {
    if (!root) {
        return NULL;
    }

    printf(".........balancing........%p (%d)\n", root, root->data);

    root->left = make_balance(root->left);
    root->right = make_balance(root->right);

    int left_height = get_height(root->left);
    int right_height = get_height(root->right);

    if (left_height - right_height > 1) {
        int left_left_height = get_height(root->left->left);
        int left_right_height = get_height(root->left->right);

        if (left_right_height - left_left_height > 0) {
            root->left = rotate_left(root->left);
            root = rotate_right(root);
        } else {
            root = rotate_right(root);
        }
    } else if (right_height - left_height > 1) {
        int right_left_height = get_height(root->right->left);
        int right_right_height = get_height(root->right->right);

        if (right_left_height - right_right_height > 0) {
            root->right = rotate_right(root->right);
            root = rotate_left(root);
        } else {
            root = rotate_left(root);
        }
    }
    root->height = get_height(root);
    printf(".........root(%d).....%d\n", root->data, root->height);

    return root;
}

int main() {
    node_t n1 = DEFAULT_NODE,
           n2 = DEFAULT_NODE,
           n3 = DEFAULT_NODE,
           n4 = DEFAULT_NODE,
           n5 = DEFAULT_NODE,
           n6 = DEFAULT_NODE;

    n1.right = &n2;
    n1.data = 1;

    n2.right = &n3;
    n2.data = 2;

    n3.right = &n4;
    n3.data = 3;

    n4.right = &n5;
    n4.data = 4;

    n5.right = &n6;
    n5.data = 5;

    n6.data = 6;

    in_order_traverse(&n1);
    printf("\n");
    pre_order_traverse(&n1);
    printf("\n");

    node_t* t = make_balance(&n1);

    in_order_traverse(t);
    printf("\n");
    pre_order_traverse(t);
    printf("\n");
    return 0;
}
