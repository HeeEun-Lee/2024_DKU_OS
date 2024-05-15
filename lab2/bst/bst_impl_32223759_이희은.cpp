/*
*	DKU Operating System Lab
*	    Lab2 (Concurrent Data Structure)
*	    Student id : 32223759
*	    Student name : 이희은
*/
#include "bst_impl.h"
#include <iostream>
using namespace std;

void BST::insert(int key, int value){
	// Todo
	if(root == nullptr){
		root = createNode(key, value);
		return;
	}
	current = root;
	while(current){
		if(key == current->key){
			current->value += value;
			current->upd_cnt++;
			return;
		}
		else if (key < current->key){
			if(current->left == nullptr){
				current->left = createNode(key, value);
				return;
			}
			current = current->left;
		} else if (key > current->key){
			if(current->right == nullptr){
				current->right = createNode(key, value);
				return;
			}
			current = current->right;
		}
	}
}

int BST::lookup(int key) {
	// Todo
	current = root;
	while (current)
	{
		/* code */
		if(key == current->key){
			return current->value;
		} 
		else {
			if(key < current->key) {
				current = current->left;
			} else {
				current = current->right;
			}
		}
	}
	return 0;
} 

void BST::remove(int key){
	// Todo
	current = root;
	parent = nullptr;

	while(current != nullptr && current->key != key){
		parent = current;
		if(key < current->key){
			current = current->left;
		} else {
			current = current->right;
		}
	}
	// 삭제할 노드가 없는 경우
	if(current == nullptr){
		return;
	}
	
	// 자식 노드가 없거나 한 개인 경우
	if(current->left == nullptr){
		if(parent == nullptr){
			root = current->right;
		} else if(parent->left == current){
			parent->left = current->right;
		} else {
			parent->right = current->right;
		}
		delete current;
		return;
	} else if(current->right == nullptr){
		if(parent == nullptr){
			root = current->left;
		} else if(parent->left == current){
			parent->left = current->left;
		} else {
			parent->right = current->left;
		}
		delete current;
		return;
	}

	// 자식 노드가 두 개인 경우
	Node* s_parent = current;
	Node* s = current->right;
	while (s->left != nullptr)
	{
		s_parent = s;
		s = s->left;
	}
	current->key = s->key;
	current->value = s->value;
	current->upd_cnt = s->upd_cnt;

	if(s_parent->left == s){
		s_parent->left = s->right;
	} else {
		s_parent->right = s->right;
	}

	delete s;
	return;
	
}

void BST::traversal(KVC* traverse_arr) {
    // Todo
	current = root;
	previous = nullptr;
	while(current != nullptr){

		if(current->left == nullptr){
			traverse_arr[arr_index_] = createKVC(current);
			arr_index_++;
			current = current -> right;
		} else {
			previous = current->left;
			while (previous->right != nullptr && previous->right != current)
			{
				previous = previous->right;
			}
			if(previous->right == nullptr){
				previous->right = current;
				current = current->left;
			}
			else {
				traverse_arr[arr_index_] = createKVC(current);
				arr_index_++;
				previous->right = nullptr;
				current = current->right;
			}
			
		}
	}
}

void CoarseBST::insert(int key, int value){
	// Todo
	pthread_mutex_lock(&mutex_lock);
	if(root == nullptr){
		root = createNode(key, value);
		pthread_mutex_unlock(&mutex_lock);
		return;
	}
	current = root;
	while(current){
		if(key == current->key){
			current->value += value;
			current->upd_cnt++;
			break;;
		}
		else if (key < current->key){
			if(current->left == nullptr){
				current->left = createNode(key, value);
				break;
			}
			current = current->left;
		} else if (key > current->key){
			if(current->right == nullptr){
				current->right = createNode(key, value);
				break;
			}
			current = current->right;
		}
	}
	pthread_mutex_unlock(&mutex_lock);
	return;
}

int CoarseBST::lookup(int key) {
	// Todo
	pthread_mutex_lock(&mutex_lock);
	current = root;
	while (current)
	{
		/* code */
		if(key == current->key){
			int value = current->value;
			pthread_mutex_unlock(&mutex_lock);
			return value;
		} 
		else {
			if(key < current->key) {
				current = current->left;
			} else {
				current = current->right;
			}
		}
	}
	pthread_mutex_unlock(&mutex_lock);
	return 0;
} 

void CoarseBST::remove(int key){
	// Todo
	pthread_mutex_lock(&mutex_lock);
	current = root;
	parent = nullptr;

	while(current != nullptr && current->key != key){
		parent = current;
		if(key < current->key){
			current = current->left;
		} else {
			current = current->right;
		}
	}
	if(current == nullptr){
		pthread_mutex_unlock(&mutex_lock);
		return;
	}
	if(current->left == nullptr){
		if(parent == nullptr){
			root = current->right;
		} else if(parent->left == current){
			parent->left = current->right;
		} else {
			parent->right = current->right;
		}
		delete current;
		pthread_mutex_unlock(&mutex_lock);
		return;
	} else if(current->right == nullptr){
		if(parent == nullptr){
			root = current->left;
		} else if(parent->left == current){
			parent->left = current->left;
		} else {
			parent->right = current->left;
		}
		delete current;
		pthread_mutex_unlock(&mutex_lock);
		return;
	}

	Node* s_parent = current;
	Node* s = current->right;
	while (s->left != nullptr)
	{
		s_parent = s;
		s = s->left;
	}
	current->key = s->key;
	current->value = s->value;
	current->upd_cnt = s->upd_cnt;

	if(s_parent->left == s){
		s_parent->left = s->right;
	} else {
		s_parent->right = s->right;
	}

	delete s;
	pthread_mutex_unlock(&mutex_lock);
	return;
}

void CoarseBST::traversal(KVC* traverse_arr) {
    // Todo
	pthread_mutex_lock(&mutex_lock);
	current = root;
	previous = nullptr;
	while(current != nullptr){

		if(current->left == nullptr){
			traverse_arr[arr_index_] = createKVC(current);
			arr_index_++;
			current = current -> right;
		} else {
			previous = current->left;
			while (previous->right != nullptr && previous->right != current)
			{
				previous = previous->right;
			}
			if(previous->right == nullptr){
				previous->right = current;
				current = current->left;
			}
			else {
				traverse_arr[arr_index_] = createKVC(current);
				arr_index_++;
				previous->right = nullptr;
				current = current->right;
			}
			
		}
	}
	pthread_mutex_unlock(&mutex_lock);
	return;
}

void FineBST::insert(int key, int value) {
    FineNode* new_node = createNode(key, value);

    if (root == nullptr){
        root = new_node;
        return;
    }
    FineNode* current = root;
    FineNode* parent = nullptr;
    while (true) {
        pthread_mutex_lock(&current->mutex_lock); 

        if (current->key > key) {
            if (current->left == nullptr) {
                current->left = new_node;
                pthread_mutex_unlock(&current->mutex_lock);
                return;
            }
            parent = current->left;    
        } else if (current->key < key) {
            if (current->right == nullptr) {
                current->right = new_node;
                pthread_mutex_unlock(&current->mutex_lock);
                return;
            }
            parent = current->right;
        } else { 
            current->value += value;
            current->upd_cnt++;
            pthread_mutex_unlock(&current->mutex_lock);
            return;
        }

        if(parent == nullptr){
            break;
        }
        pthread_mutex_unlock(&current->mutex_lock);
        current = parent;  
    }
}  

int FineBST::lookup(int key) {
    FineNode* current = root;

    // 루트가 비어 있으면 바로 0을 반환
    if (current == nullptr) {
        return 0;
    }

    pthread_mutex_lock(&current->mutex_lock);  // 첫 노드의 락 획득

    while (current != nullptr) {
        if (key == current->key) {
            int value = current->value;
            pthread_mutex_unlock(&current->mutex_lock);  // 키가 일치하면 락 해제 후 값 반환
            return value;
        }
        
        // 다음 노드 선택
        FineNode* next = key < current->key ? current->left : current->right;

        if (next != nullptr) {
            pthread_mutex_lock(&next->mutex_lock);  // 다음 노드의 락 획득
        }

        pthread_mutex_unlock(&current->mutex_lock);  // 현재 노드의 락 해제

        if (next == nullptr) {
            return 0;  // 다음 노드가 없으면 0 반환
        }

        current = next;  // 다음 노드로 이동
    }

    return 0;  // 노드가 nullptr인 경우를 대비하여 0 반환
}

void FineBST::remove(int key) {
    FineNode* current = root;
    FineNode* parent = nullptr;
    bool found = false;

    // 루트 노드부터 시작해 해당 키를 갖는 노드 탐색
    while (current != nullptr && !found) {
        if (current->key == key) {
            found = true;
        } else {
            parent = current;
            if (key < current->key) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
    }

    if (!found) {
        return; // 키에 해당하는 노드를 찾지 못한 경우 함수 종료
    }

    // 해당 노드와 부모 노드 잠금
    if (parent) {
        pthread_mutex_lock(&parent->mutex_lock);
    }
    pthread_mutex_lock(&current->mutex_lock);

    // 삭제 연산 처리
    if (current->left == nullptr || current->right == nullptr) {
        // 하나의 자식만 있는 경우
        FineNode* temp = (current->left != nullptr) ? current->left : current->right;
        if (parent == nullptr) {
            root = temp;
        } else if (parent->left == current) {
            parent->left = temp;
        } else {
            parent->right = temp;
        }
    } else {
        // 두 개의 자식이 있는 경우
        FineNode* successor = getSuccessor(current);  // 후계자 탐색
        pthread_mutex_lock(&successor->mutex_lock);  // 후계자 잠금
        if (parent == nullptr) {
            root = successor;
        } else if (parent->left == current) {
            parent->left = successor;
        } else {
            parent->right = successor;
        }
        successor->left = current->left;
        pthread_mutex_unlock(&successor->mutex_lock);  // 후계자 잠금 해제
    }

    pthread_mutex_unlock(&current->mutex_lock);
    if (parent) {
        pthread_mutex_unlock(&parent->mutex_lock);
    }
    pthread_mutex_destroy(&current->mutex_lock);
    delete current;
}


void FineBST::traversal(KVC* traverse_arr) {
    FineNode* current = root;
    FineNode* previous = nullptr;
    // int index = 0;

    pthread_mutex_lock(&tree_lock);
    while (current != nullptr) {
        if (current->left == nullptr) {
            // 왼쪽 자식이 없으면 현재 노드를 방문하고 오른쪽으로 이동
            pthread_mutex_lock(&current->mutex_lock);
            traverse_arr[arr_index_] = createKVC(current);
            arr_index_++;
            pthread_mutex_unlock(&current->mutex_lock);
            current = current->right;
        } else {
            // 왼쪽 서브트리의 가장 오른쪽 노드를 찾음
            previous = current->left;
            while (previous->right != nullptr && previous->right != current) {
                previous = previous->right;
            }

            if (previous->right == nullptr) {
                // 현재 노드로 돌아오기 위한 임시 링크를 설정
                previous->right = current;
                current = current->left;
            } else {
                // 임시 링크를 해제하고 현재 노드를 방문한 후 오른쪽으로 이동
                previous->right = nullptr;
                pthread_mutex_lock(&current->mutex_lock);
                traverse_arr[arr_index_] = createKVC(current);
                arr_index_++;
                pthread_mutex_unlock(&current->mutex_lock);
                current = current->right;
            }
        }
    }
    pthread_mutex_unlock(&tree_lock);
}
