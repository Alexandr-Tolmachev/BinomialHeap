#include<iostream>
#include<stdio.h>
#include<algorithm>
#include "Array.h"

template <typename Key>

class BinomialHeap {
public:
	struct node;
	class Pointer;

	class Pointer {
	public:
		friend class BinomialHeap<Key>;
		Pointer(node* ptr) : element(ptr) {}
	private:
		node* element;
	};

	struct node {
		node *child1, *child2;
		node *parent, *prev, *next;
		Pointer* back_ptr;

		int deg = 0;
		bool less = false;
		Key elem;
		node(Key elem1) {
			elem = elem1;
			back_ptr = nullptr;
			child1 = nullptr;
			child2 = nullptr;
			parent = nullptr;
			prev = nullptr;
			next = nullptr;
		};

		node() {
			back_ptr = nullptr;
			child1 = nullptr;
			child2 = nullptr;
			parent = nullptr;
			next = nullptr;
			prev = nullptr;
		};
	};

	BinomialHeap() {};
	~BinomialHeap() {
		while (child_first != nullptr) {
			node* new_node = child_first->next;
			DeleteHeap(child_first);
			child_first = new_node;
		}
	};

	void merge(BinomialHeap &heap) {
		node *ptr1 = child_first, *ptr2 = heap.child_first;
		Array <node* > arr;

		while (ptr1 != nullptr && ptr2 != nullptr) {
			if (ptr1->deg < ptr2->deg) {
				arr.add(ptr1);
				ptr1->parent = nullptr;
				ptr1->prev = nullptr;
				ptr1 = ptr1->next;
				if (ptr1 != nullptr) {
					ptr1->prev->next = nullptr;
				}
			}
			else {
				arr.add(ptr2);
				ptr2->parent = nullptr;
				ptr2->prev = nullptr;
				ptr2 = ptr2->next;
				if (ptr2 != nullptr) {
					ptr2->prev->next = nullptr;
				}
			}
		}
		while (ptr1 != nullptr) {
			arr.add(ptr1);
			ptr1->parent = nullptr;
			ptr1->prev = nullptr;
			ptr1 = ptr1->next;
			if (ptr1 != nullptr) {
				ptr1->prev->next = nullptr;
			}
		}
		while (ptr2 != nullptr) {
			arr.add(ptr2);
			ptr2->parent = nullptr;
			ptr2->prev = nullptr;
			ptr2 = ptr2->next;
			if (ptr2 != nullptr) {
				ptr2->prev->next = nullptr;
			}
		}
		int curr = 0;
		node* node1 = new node();
		node* start = node1;

		while (curr < arr.size()) {
			while (curr < arr.size() - 2 && arr[curr]->deg == arr[curr + 2]->deg) {
				arr[curr + 2] = merge(arr[curr], arr[curr + 2]);
				node1->next = arr[curr + 1];
				node1->next->prev = node1;
				node1 = node1->next;
				curr += 2;
			}
			int flag = 0;

			while (curr + 1 < arr.size() && arr[curr]->deg == arr[curr + 1]->deg) {
				if (curr + 2 < arr.size() && arr[curr]->deg == arr[curr + 2]->deg) {
					flag = 1;
					break;
				}
				arr[curr + 1] = merge(arr[curr], arr[curr + 1]);
				++curr;
			}

			if (flag == 1)
				continue;
			node1->next = arr[curr];
			node1->next->prev = node1;
			node1 = node1->next;
			++curr;
		}
		child_first = start->next;
		if (child_first != nullptr)
			child_first->prev = nullptr;
		heap.child_first = nullptr;
	}

	bool is_empty() const {
		return (child_first == nullptr);
	}

	Pointer* insert(Key key) {
		BinomialHeap<Key> heap;
		heap.child_first = new node(key);
		Pointer* pointer;
		if (DeletePtr != nullptr) {
			pointer = DeletePtr;
			pointer->element = heap.child_first;
		}
		else {
			pointer = new Pointer(heap.child_first);
		}
		heap.child_first->back_ptr = pointer;
		this->merge(heap);
		return pointer;
	}

	Key extract_min() {
		try {
			if (this->is_empty())
				throw std::logic_error("Binomial heap is empty.");
		}
		catch (logic_error& error) {
			std::cerr << error.what() << std::endl;
			exit(1);
		}

		node* cur_node = child_first;
		node* minimum = cur_node;
		while (cur_node != nullptr) {
			if (cur_node->elem < minimum->elem || cur_node->less)
				minimum = cur_node;
			cur_node = cur_node->next;
		}

		Key res = minimum->elem;
		BinomialHeap<Key> heap1;
		heap1.child_first = minimum->child1;
		if (minimum->prev != nullptr && minimum->next != nullptr) {
			minimum->prev->next = minimum->next;
			minimum->next->prev = minimum->prev;
		}
		else if (minimum->prev != nullptr) {
			minimum->prev->next = nullptr;
		}
		else if (minimum->next != nullptr) {
			child_first = minimum->next;
			minimum->next->prev = nullptr;
		}
		else
			child_first = nullptr;

		if (DeletePtr == nullptr)
			delete minimum->back_ptr;
		delete minimum;
		this->merge(heap1);
		return res;
	}

	void Delete(Pointer* pointer) {
		pointer->element->less = true;
		decreaseKey(pointer, pointer->element->elem);
		//delete pointer;
		this->extract_min();
	}

	Key get_min() {
		try {
			if (this->is_empty())
				throw std::logic_error("Binomial heap is empty.");
		}
		catch (logic_error& error) {
			std::cerr << error.what() << std::endl;
			exit(1);
		}
		node* cur_node = child_first;
		Key minimum = cur_node->elem;
		while (cur_node != nullptr) {
			if (cur_node->elem < minimum)
				minimum = cur_node->elem;
			cur_node = cur_node->next;
		}
		return minimum;
	}

	void change(Pointer* pointer, Key key) {
		if (key > pointer->element->elem) {
			DeletePtr = pointer;
			this->Delete(pointer);
			insert(key);
			DeletePtr = nullptr;
		}
		else
			decreaseKey(pointer, key);
	}

private:
	node* child_first = nullptr;
	Pointer* DeletePtr = nullptr;

	node* merge(node* heap1, node* heap2) {
		if (heap1->elem >= heap2->elem) {
			node* new_node = heap1;
			heap1 = heap2;
			heap2 = new_node;
		}
		if (heap1->child2 == nullptr) {
			heap1->child2 = heap1->child1 = heap2;
			heap2->parent = heap1;
			heap2->next = nullptr;
			heap2->prev = nullptr;
			++(heap1->deg);
			return heap1;
		}

		heap1->child2->next = heap2;
		heap2->next = nullptr;
		heap2->prev = heap1->child2;
		heap1->child2 = heap1->child2->next;
		heap1->parent = heap1;
		++(heap1->deg);
		return heap1;
	}

	void decreaseKey(Pointer* pointer, int key) {
		node* cur = pointer->element;
		cur->elem = key;
		while (cur->parent && (cur->elem < cur->parent->elem || cur->less)) {
			swap_up(cur);
			cur = cur->parent;
		}
	}

	void DeleteHeap(node* Node) {
		if (Node == nullptr)
			return;
		node* new_node = Node -> child1;
		while (new_node != Node -> child2) {
			node* example = new_node->next;
			DeleteHeap(new_node);
			new_node = example;
		}
		DeleteHeap(new_node);
		delete Node->back_ptr;
		delete Node;
	}

	void swap_up(node*& ptr) {
		Key key1 = ptr->elem;
		ptr->elem = ptr->parent->elem;
		ptr->parent->elem = key1;

		Pointer* ptr0 = ptr->back_ptr;
		ptr->back_ptr = ptr->parent->back_ptr;
		ptr->parent->back_ptr = ptr0;

		ptr->back_ptr->element = ptr->parent;
		ptr->parent->back_ptr->element = ptr;
	}
};