#include <iostream>
#include "List.h"

template<typename keyType, typename valueType>

class Map {
public:
	class Node {
	public:
		bool color; // 1 -> Red, 0 -> Black
		keyType key; // holds the key
		valueType value;
		Node* left; // pointer to left child
		Node* right; // pointer to right child
		Node* parent; // pointer to the parent

		Node(keyType key, valueType value, Node* parent, bool color, Node* left = NULL, Node* right = NULL) {
			this->key = key;
			this->value = value;
			this->parent = parent;
			this->color = color;
			this->left = left;
			this->right = right;
		}
	};
private:

	Node* root;
	Node* nil;

	void leftRotate(Node* x) {
		Node* y = x->right;
		x->right = y->left;

		if (y->left != nil)
			y->left->parent = x;

		y->parent = x->parent;

		if (x->parent == NULL)
			this->root = y;
		else
			if (x == x->parent->left)
				x->parent->left = y;
			else
				x->parent->right = y;

		y->left = x;
		x->parent = y;
	}

	void rightRotate(Node* x) {
		Node* y = x->left;
		x->left = y->right;

		if (y->right != nil)
			y->right->parent = x;

		y->parent = x->parent;

		if (x->parent == NULL)
			this->root = y;
		else
			if (x == x->parent->right)
				x->parent->right = y;
			else
				x->parent->left = y;

		y->right = x;
		x->parent = y;
	}

	void replacement(Node* oldNode, Node* newNode) {
		if (oldNode->parent == nullptr)
			root = newNode;
		else
			if (oldNode == oldNode->parent->left)
				oldNode->parent->left = newNode;
			else
				oldNode->parent->right = newNode;

		newNode->parent = oldNode->parent;
	}

	void deleteHelper(Node* node) {
		if (node != nil) {
			if (node->right != NULL && node->right != nil)
				deleteHelper(node->right);
			if (node->left != NULL && node->left != nil)
				deleteHelper(node->left);
			delete node;
		}
	}

	List <keyType> getKeysHelper(Node* node, List <keyType> list) {
		if (node != nil) {
			if (node->right != NULL && node->right != nil)
				list = getKeysHelper(node->right, list);
			if (node->left != NULL && node->left != nil)
				list = getKeysHelper(node->left, list);
			list.push_back(node->key);
			return list;
		}
	}

	List <valueType> getValuesHelper(Node* node, List <valueType> list) {
		if (node != nil) {
			if (node->right != NULL && node->right != nil)
				list = getValuesHelper(node->right, list);
			if (node->left != NULL && node->left != nil)
				list = getValuesHelper(node->left, list);
			list.push_back(node->value);
			return list;
		}
	}

	void printHelper(Node* node) {
		if (node != nil) {
			if (node->right != NULL && node->right != nil)
				printHelper(node->right);
			if (node->left != NULL && node->left != nil)
				printHelper(node->left);
			std::cout << node->key << ": " << node->value << std::endl;
		}
	}

public:

	Map() {
		nil = new Node(NULL, NULL, NULL, 0); //common tree leaf
		root = nil;//tree is empty -> root = nil
	}
	~Map() {
		deleteHelper(this->root);
		delete nil;
	}

	void insert(keyType key, valueType value) {
		// new node, color=1(red), parent = Null (will be find later), kids = nil (will be find later)
		Node* newNode = new Node(key, value, NULL, 1, nil, nil);

		if (this->root == nil) {
			//if tree is empty -> newNode is root
			root = newNode;
			newNode->color = 0;
			return;
		}
		else {
			Node* newNodeParent = NULL;

			//find the newNodeParent
			Node* currNode = this->root;

			while (currNode != nil) {
				newNodeParent = currNode;
				if (newNode->key < currNode->key) {
					currNode = currNode->left;
				}
				else {
					currNode = currNode->right;
				}
			}

			//add newNode and newNodeParent relationship
			newNode->parent = newNodeParent;

			if (newNode->key < newNodeParent->key)
				newNodeParent->left = newNode;
			else
				newNodeParent->right = newNode;
		}

		// return the properties of the red-black tree
		if (newNode->parent->parent == NULL)
			return;

		Node* uncle;
		while (newNode->parent->color == 1) {
			bool isAtRight = newNode->parent->parent->right == newNode->parent;

			uncle = isAtRight ? (newNode->parent->parent->left) : (newNode->parent->parent->right);

			if (uncle->color == 1) {
				uncle->color = 0;
				newNode->parent->color = 0;
				newNode->parent->parent->color = 1;
				newNode = newNode->parent->parent;
			}
			else {
				if (newNode == (isAtRight ? (newNode->parent->left) : (newNode->parent->right))) {
					newNode = newNode->parent;
					isAtRight ? rightRotate(newNode) : leftRotate(newNode);
				}
				newNode->parent->color = 0;
				newNode->parent->parent->color = 1;
				isAtRight ? leftRotate(newNode->parent->parent) : rightRotate(newNode->parent->parent);
			}

			if (newNode == root) {
				break;
			}
		}
		root->color = 0;
	}

	void clear() {
		deleteHelper(this->root);
		this->root = nil;
	}

	List <keyType> getKeys() {
		List <keyType> list;
		return getKeysHelper(this->root, list);
	}

	List <valueType> getValues() {
		List <valueType> list;
		return getValuesHelper(this->root, list);
	}

	void print() {
		if (this->root == nil)
			std::cout << "Map is empty" << std::endl;
		else
			printHelper(this->root);
	}

	//if node is not found returns NULL
	Node* findNode(keyType key) {
		Node* findNode = NULL;
		Node* currNode = this->root;

		while (currNode != nil) {
			if (currNode->key == key) {
				findNode = currNode;
				break;
			}
			if (currNode->key <= key)
				currNode = currNode->right;
			else
				currNode = currNode->left;
		}
		return findNode;
	}

	//if node is not found returns 0, else returns 1
	bool removeNode(keyType key) {
		Node* nodeToBeDeleted = findNode(key);
		if (nodeToBeDeleted == NULL) //if nodeToBeDeleted is not found
			return 0;

		else { //if nodeToBeDeleted found
			Node* x = NULL;
			Node* y = NULL;

			int originalColor = nodeToBeDeleted->color;

			y = nodeToBeDeleted;

			if (nodeToBeDeleted->left == nil) {
				x = nodeToBeDeleted->right;
				replacement(nodeToBeDeleted, nodeToBeDeleted->right);
			}
			else
				if (nodeToBeDeleted->right == nil) {
					x = nodeToBeDeleted->left;
					replacement(nodeToBeDeleted, nodeToBeDeleted->left);
				}
				else {
					y = nodeToBeDeleted->right;

					while (y->left != nil) {
						y = y->left;
					}

					originalColor = y->color;
					x = y->right;

					if (y->parent == nodeToBeDeleted)
						x->parent = y;
					else {
						replacement(y, y->right);
						y->right = nodeToBeDeleted->right;
						y->right->parent = y;
					}

					replacement(nodeToBeDeleted, y);
					y->left = nodeToBeDeleted->left;
					y->left->parent = y;
					y->color = nodeToBeDeleted->color;
				}

			delete nodeToBeDeleted;

			if (originalColor == 0) {
				Node* brother;
				while (x != root && x->color == 0) {
					if (x == x->parent->left) {
						brother = x->parent->right;

						if (brother->color == 1) {
							brother->color = 0;
							x->parent->color = 1;
							leftRotate(x->parent);
							brother = x->parent->right;
						}

						if (brother->left->color == 0 && brother->right->color == 0) {
							brother->color = 1;
							x = x->parent;
						}
						else {
							if (brother->right->color == 0) {
								brother->left->color = 0;
								brother->color = 1;
								rightRotate(brother);
								brother = x->parent->right;
							}
							brother->color = x->parent->color;
							x->parent->color = 0;
							brother->right->color = 0;
							leftRotate(x->parent);
							x = root;
						}
					}
					else {
						brother = x->parent->left;
						if (brother->color == 1) {
							brother->color = 0;
							x->parent->color = 1;
							rightRotate(x->parent);
							brother = x->parent->left;
						}
						if (brother->right->color == 0 && brother->right->color == 0) {
							brother->color = 1;
							x = x->parent;
						}
						else {
							if (brother->left->color == 0) {
								brother->right->color = 0;
								brother->color = 1;
								leftRotate(brother);
								brother = x->parent->left;
							}
							brother->color = x->parent->color;
							x->parent->color = 0;
							brother->left->color = 0;
							rightRotate(x->parent);
							x = root;
						}
					}
				}
				x->color = 0;
			}
		}
		return 1;
	}
};
