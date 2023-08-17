#include<bits/stdc++.h>
using namespace std;

// Defining the node Structure for the AVL Tree
class Node {
    public:
        int height;
        string meaning;
        string word;
        Node* left;
        Node* right;
        Node(string word,string meaning){
            this->word = word;
            this->meaning = meaning;
            height = 1;
            left = NULL;
            right = NULL;
        }
};

//AVL Tree Class
class Avl_tree{
    private:
        Node* root;

        // Helper functions
        int height(Node*);
        Node* insertt(string,string, Node*);
        int b_factor(Node*);
        void inorder(Node*);
        void descending(Node*);
        Node* rol(Node*);
        Node* ror(Node*);
        int src(string ,Node*);
        Node* deletee(Node*,string);
        void update(string,string, Node*);
    public: 

        // Public Interface methods
        void updat(string, string);
        void search(string);
        void traversal();
        void insrt(string,string);
        void delet(string);
        Avl_tree(){
            root = NULL;        
            // Open the dictionary.txt file
            ifstream inputFile("dictionary.txt");
            if (!inputFile.is_open()) {
                cout << "Error opening dictionary.txt" << endl;
                return;
            }

            // Read lines from the file and insert words and meanings into the AVL tree
            string line;
            while (getline(inputFile, line)) {
                size_t colonPos = line.find(":");
                if(colonPos != string::npos){
                    string word = line.substr(0, colonPos);
                    string meaning = line.substr(colonPos + 1);
                    root = insertt(word,meaning, root);
                } 
                else{
                    cout << "Error parsing line: " << line << endl;
                }
            }
            inputFile.close();  // Close the file
        }
};

void Avl_tree:: updat(string s,string mean){
    // Update Node in the AVL Tree
    update(s,mean,root);
    
    // Update the entry in the text file
    // Open the dictionary.txt file for reading and writing
    ifstream inputFile("dictionary.txt");
    ofstream tempFile("temp.txt");
    if (!inputFile || !tempFile) {
        cout << "Error opening files" << endl;
        return ;
    }
    string line;
    while (getline(inputFile, line)) {
        int colonPos = line.find(":");
        if (colonPos != string::npos) {
            if (line.find(s + ":") == string::npos) {
                tempFile << line << endl;
            }
            else{
                line = s + ": "+ mean;
                tempFile << line << endl;
            }
        }
    }
    inputFile.close();
    tempFile.close();

    // Replace the original file with updated data
    if (remove("dictionary.txt") != 0) {
        cout << "Error deleting original dictionary.txt" << endl;
        return ;
    }
    if (rename("temp.txt", "dictionary.txt") != 0) {
        cout << "Error renaming temp.txt to dictionary.txt" << endl;
        return ;
    }
}

// Update meaning recursively
void Avl_tree:: update(string s,string mean, Node* root){
    if(root == NULL){
        cout<<"      ****WORD NOT FOUND****"<<endl;
    }
    else if(root->word.compare(s)==0){
        cout<<"      WORD FOUND"<<endl;
        root->meaning = mean;
        cout<<"      MEANING UPDATED"<<endl;
        cout<<"      Word: "<<root->word<<endl<<"      Meaning: "<<root->meaning<<endl;
    }
    else{
        if(s.compare(root->word)<0){
            // cout<<"->left ";
            update(s,mean,root->left);
        }
        else{
            // cout<<"->right ";
            update(s,mean,root->right);
        }
    }
}


// Delete a word
void Avl_tree::delet(string key){
    // Delete Node from the AVL Tree
    root = deletee(root,key);

    // Delete the entry from Dictionary
    // Open the dictionary.txt file for reading and writing
    ifstream inputFile("dictionary.txt");
    ofstream tempFile("temp.txt");
    if (!inputFile || !tempFile) {
        cout << "Error opening files" << endl;
        return ;
    }
    string line;
    while (getline(inputFile, line)) {
        int colonPos = line.find(":");
        if (colonPos != string::npos) {
            if (line.find(key + ":") == string::npos) {
                tempFile << line << endl;
            }
        }
    }
    inputFile.close();
    tempFile.close();
    // Replace the original file with updated data
    if (remove("dictionary.txt") != 0) {
        cout << "Error deleting original dictionary.txt" << endl;
        return ;
    }
    if (rename("temp.txt", "dictionary.txt") != 0) {
        cout << "Error renaming temp.txt to dictionary.txt" << endl;
        return ;
    }
}

// Delete Node Recursively
Node* Avl_tree::deletee(Node*root, string key){
    static bool a = 1;
    if(root==NULL){
        cout<<"***WORD NOT FOUND***"<<endl;
        return NULL;
    }
    else if(root->word.compare(key)>0){
        // cout<<"left->";
        root->left = deletee(root->left,key);
    }
    else if(root->word.compare(key)<0){
        // cout<<"right->";
        root->right = deletee(root->right,key);
    }

    else{
        if(root->left==NULL && root->right==NULL){
            if(a){
                cout<<"      "<<key<<" deleted successfully";
            }
            return NULL;
        }
        else if(root->right==NULL){
            if(a){
                cout<<"      "<<key<<" deleted successfully";
            }
            return root->left;
        }
        else if(root->left==NULL){
            if(a){
                cout<<"      "<<key<<" deleted successfully";
            }
            return root->right;
        }
        else{
            Node* temp = root->right;
            while(temp->left!=NULL){
                temp = temp->left;
            }
            root->word = temp->word;
            cout<<"      "<<key<<" deleted successfully";
            a=0;
            temp = deletee(root->right,temp->word);
            a=1;
            return temp;
        }
    }
    // Calculate the balance factor to check if tree is unbalanced
    if(b_factor(root)>1){
        if(b_factor(root->left)==-1){
            root->left = rol(root->left);
            // cout<<"->rol";
        }
        root = ror(root);
        // cout<<"->ror"<<endl;
    }
    else if(b_factor(root)<-1){
        if(b_factor(root->right)==1){
            root->right = ror(root->right);
            // cout<<"->ror"<<endl;
        }
        root = rol(root);
        // cout<<"->rol"<<endl;
    }
    root-> height = height(root);
    return root;
}

// Search for a word and return the number of comparisons performed
int Avl_tree:: src(string s, Node* root){
    int a=1;
    if(root == NULL){
        cout<<"      ****WORD NOT FOUND****"<<endl;
    }
    else if(root->word.compare(s)==0){
        cout<<"      WORD FOUND"<<endl;
        cout<<"      "<<root->word<<endl<<"      Meaning: "<<root->meaning<<endl;
    }
    else{
        if(s.compare(root->word)<0){
            // cout<<"->left ";
            a+= src(s,root->left);
        }
        else{
            // cout<<"->right ";
            a+= src(s,root->right);
        }
    }
    return a;
}

// Search for a word and display the number of comparisons
void Avl_tree:: search(string s){
    cout<<"      Number of comparisons: "<<src(s,root);
}


// Perform right rotation
Node* Avl_tree:: ror(Node* a){
    Node* b = a->left;
    Node* temp = b->right;
    a->left = temp;
    b->right = a;
    a->height = height(a);
    b->height = height(b);
    return b;
}

// Perform left rotation
Node* Avl_tree:: rol(Node* a){
    Node* b = a->right;
    Node* temp = b->left;
    a->right = temp;
    b->left = a;
    a->height = height(a);
    b->height = height(b);
    return b;
}

// Calculate height of a node
int Avl_tree:: height(Node* root){
    int a=0 ,b=0;
    if(root->left){
        a = root->left->height;
    }
    if(root->right){
        b = root->right->height;
    }
    return 1+max(a,b);
}

// Calculate balance factor of a node
int Avl_tree:: b_factor(Node* root){
    int a=0 ,b=0;
    if(root->left){
        a = root->left->height;
    }
    if(root->right){
        b = root->right->height;
    }
    return a-b;
}

// Insert a word and meaning into the AVL tree
void Avl_tree::insrt(string key,string mean){
    //Insert the new node into the Tree
    root = insertt(key,mean,root);

    // Add data entry in the txt file
    // Open the dictionary.txt file in append mode
    ofstream outputFile("dictionary.txt", ios::app);
    if (!outputFile.is_open()) {
        cout << "Error opening dictionary.txt for appending" << endl;
        return;
    }
    cout<<"   Insertion successful"<<endl;
    
    // Append the new word and meaning to the file
    outputFile << key << ": " << mean << endl;
    
    outputFile.close(); // Close the file
}

// Insert a node into the AVL tree recursively
Node* Avl_tree:: insertt(string key,string mean, Node* root){
    if(!root){
        Node* temp = new Node(key,mean);
        return temp;
    }
    else{
        if(root->word.compare(key)>0){
            root->left = insertt(key,mean,root->left);
        }
        else{
            root->right = insertt(key,mean,root->right);
        }
    }

    // Balance the tree if necessary
    if(b_factor(root)>1){
        if(b_factor(root->left)==-1){
            root->left = rol(root->left);
            // cout<<"->rol";
        }
        root = ror(root);
        // cout<<"->ror"<<endl;
    }
    else if(b_factor(root)<-1){
        if(b_factor(root->right)==1){
            root->right = ror(root->right);
            // cout<<"->ror"<<endl;
        }
        root = rol(root);
        // cout<<"->rol"<<endl;
    }
    root-> height = height(root);
    return root;
}

// Traverse the tree to print all the words and meanings
void Avl_tree:: traversal(){
    bool b;
    if(root!=NULL){
        cout<<"      0:Ascending\n      1:Descending\n      Enter your choice: ";
    }
    else{
        cout<<"      ***TREE EMPTY***";
        return;
    }
    cin>>b;
    if(!b){
        inorder(root);
    }
    else{
        descending(root);
    }
}

// Using recursive Inorder Traversal to print Dictionary in Ascending alphabetical order
void Avl_tree:: inorder(Node* root){
    if(root ==NULL){
        return;
    }
    inorder(root->left);
    cout<<"         Word:    "<<root->word<<endl<<"         Meaning:    "<<root->meaning<<endl<<endl;
    // cout<<"  Height: "<< root->height<<endl;
    inorder(root->right);
}


// Using recursive Inorder Traversal(reversed) to print Dictionary in Descending alphabetical order
void Avl_tree:: descending(Node* root){
    if(root ==NULL){
        return;
    }
    descending(root->right);
    cout<<"         Word:        "<<root->word<<endl<<"         Meaning:    "<<root->meaning<<endl<<endl;
    // cout<<"  Height: "<< root->height<<endl;
    descending(root->left);
}


// Display the menu 
void menu(){
    cout<<endl<<endl<<"*************************  Menu  *************************"<<endl<<endl;
    cout<<"   1: Insert to Dictionary"<<endl;
    cout<<"   2: Display all words"<<endl; 
    cout<<"   3: Search the Dictionary"<<endl;
    cout<<"   4: Remove a word"<<endl;
    cout<<"   5: Update meaning"<<endl;
    cout<<"   6: Exit"<<endl<<endl;
    cout<<"   >>>Enter your choice: "; 
}

int main(){
    Avl_tree* tree = new Avl_tree();
    bool working=1;
    int choice;
    string s,m;
    while(working){
        menu();
        cin>>choice;
        // Handling user input
        switch (choice)
        {
        case 1:
            cout<<"   Enter the word to be inserted: ";
            cin>>s;
            cout<<"   Enter the meaning: ";
            cin.ignore();
            getline(cin,m);
            tree->insrt(s,m);
            break;  
        case 2:
            tree->traversal();
            break;
        case 3:
            cout<<"   Enter the word: ";
            cin>>s;
            tree->search(s);
            break;
        case 4:
            cout<<"   Enter the word to be removed: ";
            cin>>s;
            tree->delet(s);
            break;
        case 5:
            cout<<"   Enter the word: ";
            cin>>s;
            cout<<"   Enter the new meaning: ";
            cin.ignore();
            getline(cin,m);
            tree->updat(s,m);
            break;

        case 6:
            working = 0;
            break;
        }
    }
}   

