/*
*	DKU Operating System Lab
*	    Lab2 (Concurrent Data Structure)
*	    Student id : 32223759
*	    Student name : 이희은
*/
#include "bst.h"

#ifndef BST_IMPI_H
#define BST_IMPI_H

/**
 * @brief BST without lock
 * DefaultBST의 함수를 오버라이드하여, 클래스를 완성한다.
 * 구현에 필요한 멤버 변수/함수를 추가하여 구현한다.
 */
class BST : public DefaultBST {
	private:
		// 멤버 변수 추가 선언 가능
		Node* root;
		Node* current;
		Node* parent;
		Node* previous;

	public:
		// 멤버 함수 추가 선언 가능
		Node* createNode(int key, int value){
			Node* node = new Node;
			node->key = key;
			node->value = value;
			node->upd_cnt = 0;
			node->left = nullptr;
			node->right = nullptr;
			return node;
		}
		KVC createKVC(Node* node){
			KVC kvc;
			kvc.key = node->key;
			kvc.value = node->value;
			kvc.upd_cnt = node->upd_cnt;
			return kvc;
		}
		void insert(int key, int value) override;
		int lookup(int key) override;
		void remove(int key) override;
		void traversal(KVC* arr) override;
};

/**
 * @brief BST with coarse-grained lock
 * BST 전체를 critical section으로 가정하여, 하나의 lock으로 이를 관리한다.
 * DefaultBST의 함수를 오버라이드하여, 클래스를 완성한다.
 * 구현에 필요한 멤버 변수/함수를 추가하여 구현한다.
 */
class CoarseBST : public DefaultBST {
	private:
		// 멤버 변수 추가 선언 가능
		Node* root;
		Node* current;
		Node* parent;
		Node* previous;
		pthread_mutex_t mutex_lock;
	public:
		// 멤버 함수 추가 선언 가능
		Node* createNode(int key, int value){
			Node* node = new Node;
			node->key = key;
			node->value = value;
			node->upd_cnt = 0;
			node->left = nullptr;
			node->right = nullptr;
			return node;
		}
		KVC createKVC(Node* node){
			KVC kvc;
			kvc.key = node->key;
			kvc.value = node->value;
			kvc.upd_cnt = node->upd_cnt;
			return kvc;
		}
		void insert(int key, int value) override;
		int lookup(int key) override;
		void remove(int key) override;
		void traversal(KVC* arr) override;
};

/// @brief FineBST는 FineNode를 정의하여 사용하길 권장한다.
struct FineNode : public Node {
    // 멤버 변수 추가 가능
	// 키
    int key;
    // 밸류
    int value;
    // 업데이트 횟수
    int upd_cnt;
    // left child 노드
    FineNode* left;
    // right child 노드
    FineNode* right;
	// lock
	pthread_mutex_t mutex_lock;
};

/**
 * @brief BST with fine-grained lock
 * BST 내부의 critical section을 Node 단위로 lock으로 관리한다.
 * Node를 상속받은 FineNode를 정의하여 사용하길 권장한다.
 * DefaultBST의 함수를 오버라이드하여, 클래스를 완성한다.
 * 구현에 필요한 멤버 변수/함수를 추가하여 구현한다.
 */
class FineBST : public DefaultBST {
	private:
		// 멤버 변수 추가 선언 가능
		FineNode* root;
		FineNode* nextNode = nullptr;
		FineNode* current = nullptr;
		FineNode* parent = nullptr;
		FineNode* previous = nullptr;
		pthread_mutex_t tree_lock;

	public:
		// 멤버 함수 추가 선언 가능
		FineNode* createNode(int key, int value){
			FineNode* node = new FineNode();
			node->key = key;
			node->value = value;
			node->upd_cnt = 0;
			node->left = nullptr;
			node->right = nullptr;
			return node;
		}
		KVC createKVC(FineNode* node){
			KVC kvc;
			kvc.key = node->key;
			kvc.value = node->value;
			kvc.upd_cnt = node->upd_cnt;
			return kvc;
		}
		
		FineNode* getSuccessor(FineNode* node) {
			FineNode* successorParent = node;
			FineNode* successor = node;
			FineNode* current = node->right;
			while (current != nullptr) {
				successorParent = successor;
				successor = current;
				current = current->left;
			}
			if (successor != node->right) {
				successorParent->left = successor->right;
				successor->right = node->right;
			}
			return successor;
		}
		void insert(int key, int value) override;
		int lookup(int key) override;
		void remove(int key) override;
		void traversal(KVC* arr) override;
};
#endif