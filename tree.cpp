#include <bits/stdc++.h>
using namespace std;

/* RED = 0
 * BLACK = 1
 */
enum Color {RED, BLACK};

// Estrutura de Nó
struct Node
{
	int key;
	bool color;
	Node *left, *right, *parent;

	// Construtor
	Node(int key)
	{
	this->key = key;
	left = right = parent = NULL;
	}
};

// Classe da Árvore Rubro-Negra
class ArvRB
{
private:
	Node *root;
protected:
	void rotateLeft(Node *&, Node *&);
	void rotateRight(Node *&, Node *&);
	void fixViolation(Node *&, Node *&);
public:
	// Constructor
	ArvRB() { root = NULL; }
	void insert(const int &n);
	void inorder();
	void levelOrder();
};

// Travessia em Ordem
void inorderHelper(Node *root)
{
	if (root == NULL)
		return;

	inorderHelper(root->left);
	cout << root->key << " ";
	inorderHelper(root->right);
}

/* Inserir um no na arvore dado uma chave */
Node* TREEInsert(Node* root, Node *newNode)
{
	/* If the tree is empty, return a new node */
	if (root == NULL)
	return newNode;

	/* Otherwise, recur down the tree */
	if (newNode->key < root->key)
	{
		root->left = TREEInsert(root->left,newNode);
		root->left->parent = root;
	}
	else if (newNode->key > root->key)
	{
		root->right = TREEInsert(root->right,newNode);
		root->right->parent = root;
	}

	/* return the (unchanged) node pointer */
	return root;
}

// Utility function to do level order traversal
void levelOrderHelper(Node *root)
{
	if (root == NULL){
		return;
    }
	int c=0, count=0;
	char colorChar;
	Node *nodeNULL = NULL;
	std::queue<Node *> q;
	q.push(root);
	while (!q.empty())
	{
		Node *temp = q.front();
		if(temp == nodeNULL){
			q.pop();
			count++;
			continue;
		}

		if(temp->color == 1)
			printf("\033[1m\033[30m (%d%c) \033[0m",temp->key, colorChar);
		else
			printf("\033[31m (%d%c) \033[0m",temp->key, colorChar);
		//cout << temp->key<<" "<<temp->color << " ";
		q.pop();
		count++;
		if((int)log2(count)==c){
			cout<<endl;
			count=0;
			c++;
		}
        
        if (temp->left == NULL || temp->right == NULL)
            q.push(nodeNULL);
		if (temp->left != NULL)
			q.push(temp->left);

		if (temp->right != NULL)
			q.push(temp->right);
	}
}

void ArvRB::rotateLeft(Node *&root, Node *&newNode)
{
	Node *newNode_right =newNode->right;
	newNode->right =newNode_right->left;
	if (newNode->right != NULL)
		newNode->right->parent =newNode;

	newNode_right->parent =newNode->parent;

	if (newNode->parent == NULL)
		root =newNode_right;

	else if (newNode==newNode->parent->left)
		newNode->parent->left =newNode_right;

	else
		newNode->parent->right =newNode_right;

	newNode_right->left =newNode;
	newNode->parent =newNode_right;
}

void ArvRB::rotateRight(Node *&root, Node *&newNode)
{
	Node *newNode_left =newNode->left;

	newNode->left =newNode_left->right;

	if (newNode->left != NULL)
		newNode->left->parent =newNode;

	newNode_left->parent =newNode->parent;

	if (newNode->parent == NULL)
		root =newNode_left;

	else if (newNode==newNode->parent->left)
		newNode->parent->left =newNode_left;

	else
		newNode->parent->right =newNode_left;

	newNode_left->right =newNode;
	newNode->parent =newNode_left;
}

// This function fixes violations caused by TREE insertion
void ArvRB::fixViolation(Node *&root, Node *&newNode)
{
	Node *parent_newNode= NULL;
	Node *grand_parent_newNode= NULL;

	while ((newNode!= root) && (newNode->color != BLACK) &&
		(newNode->parent->color == RED))
	{

		parent_newNode=newNode->parent;
		grand_parent_newNode=newNode->parent->parent;

		/* Case : A
			Parent ofnewNode is left child of Grand-parent ofnewNode */
		if (parent_newNode== grand_parent_newNode->left)
		{

			Node *uncle_newNode= grand_parent_newNode->right;

			/* Case : 1
			The uncle ofnewNode is also red
			Only Recoloring required */
			if (uncle_newNode!= NULL && uncle_newNode->color == RED)
			{
				printf("Caso 1\n");
				grand_parent_newNode->color = RED;
				parent_newNode->color = BLACK;
				uncle_newNode->color = BLACK;
				newNode= grand_parent_newNode;
			}

			else
			{
				/* Case : 2
				newNodeis right child of its parent
				Left-rotation required */
				if (newNode== parent_newNode->right)
				{
					printf("Caso 2\n");
					rotateLeft(root, parent_newNode);
					newNode= parent_newNode;
					parent_newNode=newNode->parent;
				}

				/* Case : 3
				newNodeis left child of its parent
				Right-rotation required */
				printf("Caso 3\n");
				rotateRight(root, grand_parent_newNode);
				swap(parent_newNode->color, grand_parent_newNode->color);
				newNode= parent_newNode;
			}
		}

		/* Case : B
		Parent ofnewNode is right child of Grand-parent ofnewNode */
		else
		{
			Node *uncle_newNode= grand_parent_newNode->left;

			/* Case : 1
				The uncle ofnewNode is also red
				Only Recoloring required */
			if ((uncle_newNode!= NULL) && (uncle_newNode->color == RED))
			{
				grand_parent_newNode->color = RED;
				parent_newNode->color = BLACK;
				uncle_newNode->color = BLACK;
				newNode= grand_parent_newNode;
			}
			else
			{
				/* Case : 2
				newNodeis left child of its parent
				Right-rotation required */
				if (newNode== parent_newNode->left)
				{
					rotateRight(root, parent_newNode);
					newNode= parent_newNode;
					parent_newNode=newNode->parent;
				}

				/* Case : 3
				newNodeis right child of its parent
				Left-rotation required */
				rotateLeft(root, grand_parent_newNode);
				swap(parent_newNode->color, grand_parent_newNode->color);
				newNode= parent_newNode;
			}
		}
	}

	root->color = BLACK;
}

// Function to insert a new node with given key
void ArvRB::insert(const int &key)
{
	Node *newNode= new Node(key);

	// Do a normal TREE insert
	root = TREEInsert(root,newNode);

	// fix Red Black Tree violations
	fixViolation(root,newNode);
}

// Function to do inorder and level order traversals
void ArvRB::inorder()	 { inorderHelper(root);}
void ArvRB::levelOrder() { levelOrderHelper(root); }


int main()
{
	ArvRB tree;
	int choice=-1;
	int newKey;
	cout << "\033[2J\033[1;1H";
	do{
		
		cout<<"\n1 - Inserir novo numero\n"<<"2 - Printar arvore por nivel\n"<<"3 - Printar arvore em ordem\n"<<"Escolha uma opcao: ";
		cin>>choice;
		cout<<endl;
		switch(choice){
			case 1:	cout<<"Digite o numero:";
					cin>>newKey;
					tree.insert(newKey);
					tree.levelOrder();
					break;
			case 2: tree.levelOrder();
					break;
			case 3: tree.inorder();				
					break;
			}
	}while(choice!=0);

/*
	tree.insert(7);
	tree.insert(6);
	tree.insert(5);
	tree.insert(4);
	tree.insert(3);
	tree.insert(2);
	tree.insert(1);

	cout << "\nInoder Traversal of Created Tree\n";
	tree.inorder();

	cout << "\n\nLevel Order Traversal of Created Tree\n";
	tree.levelOrder();
*/
	return 0;
}

