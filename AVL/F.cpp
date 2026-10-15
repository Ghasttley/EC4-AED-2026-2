#include <bits/stdc++.h>

// Implementación de AVL:
// https://github.com/Ghasttley/EC4-AED-2026-2/blob/main/AVL/AVL.cpp

template<typename data_type>
struct AVL {
    struct TreeNode {
        int height;
        data_type data;
        TreeNode* left;
        TreeNode* right;

        TreeNode* parent; // adición para los suces y preds

        TreeNode(data_type data = data_type(), int height = 0,
                    TreeNode* left = nullptr,
                    TreeNode* right = nullptr,
                    TreeNode* parent = nullptr)
                    : data(data), height(height), left(left), right(right), parent(parent) {}
    };

    TreeNode* root; AVL() { root = nullptr; }

    void update(TreeNode* u) { 
        if(u) u -> height = 1 + std::max(h(u -> left) , h(u -> right)); 
    }

    int h(TreeNode* u) {
        return u == nullptr ? -1 : u -> height;
    }

    int FB(TreeNode* u) {
        return h(u -> left) - h(u -> right);
    }

    bool search(data_type key) {
        TreeNode* current = root;
        while (current != nullptr) {
            if (current -> data == key) { return true; }
                if (current -> data > key) { current = current -> left; }
                    else { current = current -> right; }
        } return false;
    }

    TreeNode* min_element(TreeNode* u) {
        if (u == nullptr) return nullptr;
        while (u -> left != nullptr) {
            u = u -> left;
        } return u;
    }

    TreeNode* max_element(TreeNode* u) {
        if (u == nullptr) return nullptr;
        while (u -> right != nullptr) {
            u = u -> right;
        } return u;
    }

    TreeNode* min_element() { return min_element(root); }
    TreeNode* max_element() { return max_element(root); }

    TreeNode* rotate_left(TreeNode* u) {
        TreeNode* v = u -> right;
        u -> right = v -> left;
        if(v -> left) v -> left -> parent = u;
            v -> left = u; 
        // parent ads
        v -> parent = u -> parent;
        u -> parent = v;
        update(u); update(v);
        return v;
    }

    TreeNode* rotate_right(TreeNode* u) {
        TreeNode* v = u -> left;
        u -> left = v -> right;
        if(v -> right) v -> right -> parent = u;
            v -> right = u; 
        // parent ads
        v -> parent = u -> parent;
        u -> parent = v;
        update(u); update(v);
        return v;
    }

    TreeNode* rebalance(TreeNode* u) {
        update(u);
        if (FB(u) > 1) {
            // L
            if (FB(u -> left) < 0) {
                // LR
                u -> left = rotate_left(u -> left);
                // parent ads
                u -> left -> parent = u; 
            }

            TreeNode* new_root = rotate_right(u);
            return new_root;
        }

        if (FB(u) < -1) {
            // R
            if (FB(u -> right) > 0) {
                // RL
                u -> right = rotate_right(u -> right);
                // parent ads
                u -> right -> parent = u; 
            }

            TreeNode* new_root = rotate_left(u);
            return new_root;
        } return u;
    }

    TreeNode* insert(TreeNode* u, data_type value, TreeNode* parent = nullptr) {
        if (u == nullptr) {
            TreeNode* new_node = new TreeNode(value, 0);
            new_node -> parent = parent; 
            return new_node;
        } if (value < u -> data) {
            u -> left = insert(u -> left, value, u);
            u -> left -> parent = u; 
        } else if (value > u -> data){
            u -> right = insert(u -> right, value, u);
            u -> right -> parent = u; 
        } else {
            return u; // para no duplicarlos
        }

        TreeNode* new_root = rebalance(u);
        if(new_root -> parent == nullptr) root = new_root;
        return new_root;
    }

    void insert(data_type value) {
        if(root == nullptr){
            root = new TreeNode(value, 0);
            return; 
        }

        root = insert(root, value, nullptr);
        root -> parent = nullptr;
    }

    TreeNode* find(data_type value){
        TreeNode* cur = root;
        while(cur){
            if(cur -> data == value) return cur; 
            if(value < cur -> data) cur = cur -> left;
            else { cur = cur -> right; }
        } return nullptr;
    }

    // sucesor inorder
    TreeNode* successor(TreeNode* x) {
        if(x -> right == nullptr) return nullptr;

        if (x -> right != nullptr) { return min_element(x -> right); }

        TreeNode* y = x -> parent;
        while (y != nullptr && y -> right == x) {
            x = y; y = y -> parent;
        } return y;
    }

    // predecesor inorder
    TreeNode* predecessor(TreeNode* x) {
        if(x -> left == nullptr) return nullptr;

        if (x -> left != nullptr) { return max_element(x -> left); }
        TreeNode* y = x -> parent;
        while (y != nullptr && y -> left == x) {
            x = y; y = y -> parent;
        } return y;
    }

    // ads for succ/preds
    TreeNode* successor_value(data_type value){
        TreeNode* cur = root; TreeNode* r = nullptr;
        while(cur){
            if(cur -> data > value){
                r = cur; cur = cur -> left;
            } else { cur = cur -> right; }
        } return r;
    }

    TreeNode* predecessor_value(data_type value){
        TreeNode* cur = root; TreeNode* r = nullptr;
        while(cur){
            if(cur -> data < value){
                r = cur; cur = cur -> right;
            } else { cur = cur -> left; }
        } return r;
    }

    void transplant(TreeNode* u, TreeNode* v) {
        if (u -> parent == nullptr) { root = v; }
        else if (u -> parent -> left == u) {
            u -> parent -> left = v;
        } else {
            u -> parent -> right = v;
        }
        if (v != nullptr) {
            v -> parent = u -> parent;
        }
    }

    void erase(data_type value) {
       TreeNode* z = find(value);
       if(z == nullptr) return;

       if(z -> left == nullptr){
            transplant(z, z -> right);
       } else if(z -> right == nullptr){
        transplant(z, z -> left);
       } else {
        TreeNode* y = min_element(z -> right);
        if(y -> parent != z){
            transplant(y, y -> right);
            y -> right = z -> right;
            y -> right -> parent = y;
        } transplant(z, y);
        y -> left = z -> left;
        y -> left -> parent = y;
       }

       TreeNode* p = (z -> parent ? z -> parent: nullptr);
       rebalance_up(p);
    }

    void rebalance_up(TreeNode* u){
        while(u != nullptr){
            TreeNode* father = u -> parent;
            TreeNode* new_bal = rebalance(u);
            if(father == nullptr){
                root = new_bal;
                if(new_bal) new_bal -> parent = nullptr;
                return;
            } else if(father -> left == u){
                father -> left = new_bal;
            } else {
                father -> right = new_bal;
            }

            if(new_bal) new_bal -> parent = father; 
            u = father;
        }
    }
};

int main(){
    std::ios::sync_with_stdio(false); 
    std::cin.tie(nullptr);

int q; std::cin >> q;
AVL<int> T; 

for(int i=0; i<q; i++){
    int x, y; 
    std::cin >> x >> y;

    switch(x){
        case 1: 
            T.insert(y); break;
        case 2:
            T.erase(y); break;
        case 3: 
            if(T.search(y)){
                std::cout << y << std::endl; break;
            }

            AVL<int>::TreeNode* pred = T.predecessor_value(y); 
            AVL<int>::TreeNode* succ = T.successor_value(y); 

            int d_pred = pred ? abs(pred->data - y) : INT_MAX;
            int d_succ = succ ? abs(succ->data - y) : INT_MAX;

            if (d_pred <= d_succ) {
                std::cout << pred -> data << '\n';
            } else {
                std::cout << succ -> data << '\n';
            }
    }
}

    return 0; 
}