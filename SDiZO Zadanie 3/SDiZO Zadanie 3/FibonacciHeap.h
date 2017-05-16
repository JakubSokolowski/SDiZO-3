#pragma once

#include "stdafx.h"
#include <cstddef>
#include <math.h>
#include <limits>
#include <iostream>

namespace DTS
{

	/*Implementation of fibonacci heap from 
	T.H. Cormen, C.E. Leiserson, R.L. Rivest, C. Stein, “Fibonacci Heaps” 
	in "Introduction to Algorithms" ch. 19, pp.505-530  */


	//Fibonacci heap implementation
	//T - value stored in heap
	//P - key
	//The value with the smallest key is the minimum node of a heap
	template<class T, class P>
	class FibonacciHeap
	{
	public:

		class FibNode
		{
		public:
			FibNode(T val,P k, void *pl)
				: key(k)
				, value(val)
				, mark(false)
				, p(nullptr)
				, left(nullptr)
				, right(nullptr)
				, child(nullptr)
				, degree(-1)
				, payload(pl)
			{}

			~FibNode()
			{}

			//The element stored in heap
			T value;
			//The comparator
			P key;
			bool mark;
			FibNode *p;
			FibNode *left;
			FibNode *right;
			FibNode *child;
			int degree;
			void *payload;
		};

		FibonacciHeap()
			:size_(0), min_node_(nullptr)
		{}
		~FibonacciHeap()
		{
			// delete all nodes.
			DeleteFibnodes(min_node_);
		}	

		//Returns true if size of heap is equal to 0
		bool IsEmpty() const
		{
			return size_ == 0;
		}	
		
		//Inserts the element into the heap. Internally - creates new heap with one element and merges
		void Insert(T val, P key)
		{
			Push(val, key);
		}

		//Removes the first node of fib heap, calls extract minimum to fix the heap
		void PopMin()
		{
			if (IsEmpty())
				return;
			FibNode *x = ExtractMin();
			if (x)
				delete x;
		}

		//Returns the value with minimum key - first node of fibonacci heap
		T GetMin()
		{
			return GetMinNode()->value;
		}

		//Searches for node by it's value
		FibNode * FindValue(T val)
		{
			FibNode* n = min_node_;
			if (n == NULL)
				return NULL;
			do {
				if (n->value == val)
					return n;
				FibNode* ret = FindValueRecursive(n->child, val);
				if (ret)
					return ret;
				n = n->right;
			} while (n != min_node_);
			return NULL;
		}

		//Returns the size of a heap
		uint GetSize()
		{
			return size_;
		}
	
		//Searches for node by value, and decreases it's key
		void DecreaseKey(T val, P key)
		{
			FibNode* node = FindValue(val);
			if(node)
				DecreaseKey(node, key);			
		}

	private:

		//Searches for node in sub-heap
		FibNode * FindValueRecursive(FibNode *heap, T val)
		{
			FibNode* n = heap;
			if (n == NULL)
				return NULL;
			do {
				if (n->value == val)
					return n;
				FibNode* ret = FindValueRecursive(n->child, val);
				if (ret)
					return ret;
				n = n->right;
			} while (n != min_node_);
			return NULL;
		}

		FibNode* Push(T val, P key, void *pl)
		{
			FibNode *x = new FibNode(val, key, pl);
			Insert(x);
			return x;
		}

		FibNode* Push(T val, P key)
		{
			return Push(val, key, nullptr);
		}
		
		//The minimum node of the heap.	
		FibNode* GetMinNode()
		{
			return min_node_;
		}

		//Inserts a node int the heap
		void Insert(FibNode *x)
		{

			/*
			* insert(x)
			* 1. x.degree = 0
			* 2. x.p = NIL
			* 3. x.child = NIL
			* 4. x.mark = FALSE
			* 5. if H.min == NIL
			* 6. 	create a root list for H containing just x
			* 7. 	H.min = x
			* 8. else insert x into H's root list
			* 9. 	if x.key < H.min.key
			*10. 		H.min = x
			*11. H.n = H.n + 1
			*/

			// 1
			x->degree = 0;
			// 2
			x->p = nullptr;
			// 3
			x->child = nullptr;
			// 4
			x->mark = false;
			// 5
			if (min_node_ == nullptr)
			{
				// 6, 7
				min_node_ = x->left = x->right = x;
			}
			else
			{
				// 8
				min_node_->left->right = x;
				x->left = min_node_->left;
				min_node_->left = x;
				x->right = min_node_;
				// 9
				if (x->key < min_node_->key)
				{
					// 10
					min_node_ = x;
				}
			}
			// 11
			++size_;
		}

		//Deletes all child nodes of node
		void DeleteFibnodes(FibNode *x)
		{
			if (!x)
				return;

			FibNode *cur = x;
			while (true)
			{
				if (cur->left && cur->left != x)
				{
					FibNode *tmp = cur;
					cur = cur->left;
					if (tmp->child)
						DeleteFibnodes(tmp->child);
					delete tmp;
				}
				else
				{
					if (cur->child)
						DeleteFibnodes(cur->child);
					delete cur;
					break;
				}
			}
		}
		
		//Merges 2 heaps with each other
		static FibonacciHeap* Merge(FibonacciHeap *H1, FibonacciHeap *H2)
		{
			/*
			* union_fibheap(H1,H2)
			* 1. H = MAKE-FIB-HEAP()
			* 2. H.min = H1.min
			* 3. concatenate the root list of H2 with the root list of H
			* 4. if (H1.min == NIL) or (H2.min != NIL and H2.min.key < H1.min.key)
			* 5. 	H.min = H2.min
			* 6. H.n = H1.n + H2.n
			* 7. return H
			*/

			// 1
			FibonacciHeap* H = new FibonacciHeap();
			// 2
			H->min_node_ = H1->min_node_;
			// 3
			if (H->min_node_ != nullptr && H2->min_node_ != nullptr)
			{
				H->min_node_->right->left = H2->min_node_->left;
				H2->min_node_->left->right = H->min_node_->right;
				H->min_node_->right = H2->min_node_;
				H2->min_node_->left = H->min_node_;
			}
			// 4
			if (H1->min_node_ == nullptr || (H2->min_node_ != nullptr && H2->min_node_->key < H1->min_node_->key))
			{
				// 5
				H->min_node_ = H2->min_node_;
			}
			// 6
			H->size_ = H1->size_ + H2->size_;
			// 7
			return H;
		}

		//Extracts the node with min key from the heap, returns the pointer to it
		FibNode* ExtractMin()
		{
			/*
			* extract_min
			* 1. z = H.min
			* 2. if z != NIL
			* 3. 	for each child x of z
			* 4. 		add x to the root list of H
			* 5. 		x.p = NIL
			* 6. 	remove z from the root list of H
			* 7.		if z == z.right
			* 8. 		H.min = NIL
			* 9. 	else H.min = z.right
			*10. 		CONSOLIDATE(H)
			*11. 	H.n = H.n - 1
			*12. return z
			*/

			FibNode *z, *x, *next;
			FibNode ** childList;

			// 1
			z = min_node_;
			// 2
			if (z != nullptr)
			{
				// 3
				x = z->child;
				if (x != nullptr)
				{
					childList = new FibNode*[z->degree];
					next = x;
					for (int i = 0; i < (int)z->degree; i++)
					{
						childList[i] = next;
						next = next->right;
					}
					for (int i = 0; i < (int)z->degree; i++)
					{
						x = childList[i];
						// 4
						min_node_->left->right = x;
						x->left = min_node_->left;
						min_node_->left = x;
						x->right = min_node_;
						// 5
						x->p = nullptr;
					}
					delete[] childList;
				}
				// 6
				z->left->right = z->right;
				z->right->left = z->left;
				// 7
				if (z == z->right)
				{
					// 8
					min_node_ = nullptr;
				}
				else
				{
					// 9
					min_node_ = z->right;
					// 10
					Consolidate();
				}
				// 11
				size_--;
			}
			// 12
			return z;
		}

		
		void Consolidate()
		{
			/*
			* consolidate
			* 1. let A[0 . . D(H.n)] be a new array
			* 2. for i = 0 to D(H.n)
			* 3. 	A[i] = NIL
			* 4. for each node w in the root list of H
			* 5. 	x = w
			* 6. 	d = x.degree
			* 7. 	while A[d] != NIL
			* 8. 		y = A[d]
			* 9. 		if x.key > y.key
			*10.			exchange x with y
			*11. 		FIB-HEAP-LINK(H,y,x)
			*12. 		A[d] = NIL
			*13. 		d = d + 1
			*14. 	A[d] = x
			*15. H.min = NIL
			*16. for i = 0 to D(H.n)
			*17. 	if A[i] != NIL
			*18. 		if H.min == NIL
			*19. 			create a root list for H containing just A[i]
			*20. 			H.min = A[i]
			*21. 		else insert A[i] into H's root list
			*22. 			if A[i].key < H.min.key
			*23. 				H.min = A[i]
			*/

			FibNode* w, *next, *x, *y, *temp;
			FibNode** A, ** rootList;
			// Max degree <= log base golden ratio of n
			int d, rootSize;
			int max_degree = static_cast<int>(floor(log(static_cast<double>(size_)) / log(static_cast<double>(1 + sqrt(static_cast<double>(5))) / 2)));

			// 1
			A = new FibNode*[max_degree + 2]; // plus two both for indexing to max degree and so A[max_degree+1] == NIL
											  // 2, 3
			std::fill_n(A, max_degree + 2, nullptr);
			// 4
			w = min_node_;
			rootSize = 0;
			next = w;
			do
			{
				rootSize++;
				next = next->right;
			} while (next != w);
			rootList = new FibNode*[rootSize];
			for (int i = 0; i < rootSize; i++)
			{
				rootList[i] = next;
				next = next->right;
			}
			for (int i = 0; i < rootSize; i++)
			{
				w = rootList[i];
				// 5
				x = w;
				// 6
				d = x->degree;
				// 7
				while (A[d] != nullptr)
				{
					// 8
					y = A[d];
					// 9
					if (x->key > y->key)
					{
						// 10
						temp = x;
						x = y;
						y = temp;
					}
					// 11
					LinkHeaps(y, x);
					// 12
					A[d] = nullptr;
					// 13
					d++;
				}
				// 14
				A[d] = x;
			}
			delete[] rootList;
			// 15
			min_node_ = nullptr;
			// 16
			for (int i = 0; i < max_degree + 2; i++)
			{
				// 17
				if (A[i] != nullptr)
				{
					// 18
					if (min_node_ == nullptr)
					{
						// 19, 20
						min_node_ = A[i]->left = A[i]->right = A[i];
					}
					else
					{
						// 21
						min_node_->left->right = A[i];
						A[i]->left = min_node_->left;
						min_node_->left = A[i];
						A[i]->right = min_node_;
						// 22
						if (A[i]->key < min_node_->key)
						{
							// 23
							min_node_ = A[i];
						}
					}
				}
			}
			delete[] A;
		}
	
		void LinkHeaps(FibNode* y, FibNode* x)
		{
			/*
			* LinkHeaps(y,x)
			* 1. remove y from the root list of heap
			* 2. make y a child of x, incrementing x.degree
			* 3. y.mark = FALSE
			*/

			// 1
			y->left->right = y->right;
			y->right->left = y->left;
			// 2
			if (x->child != nullptr)
			{
				x->child->left->right = y;
				y->left = x->child->left;
				x->child->left = y;
				y->right = x->child;
			}
			else
			{
				x->child = y;
				y->right = y;
				y->left = y;
			}
			y->p = x;
			x->degree++;
			// 3
			y->mark = false;
		}

		void DecreaseKey(FibNode* x, P k)
		{
			/*
			* decrease_key(x,k)
			* 1. if k > x.key
			* 2. 	error "new key is greater than current key"
			* 3. x.key = k
			* 4. y = x.p
			* 5. if y != NIL and x.key < y.key
			* 6. 	CUT(H,x,y)
			* 7. 	CASCADING-CUT(H,y)
			* 8. if x.key < H.min.key
			* 9. 	H.min = x
			*/

			FibNode* y;

			// 1
			if (k > x->key)
			{
				// 2
				return;
			}
			// 3
			x->key = k;
			// 4
			y = x->p;
			// 5
			if (y != nullptr && x->key < y->key)
			{
				// 6
				Cut(x, y);
				// 7
				CascadingCut(y);
			}
			// 8
			if (x->key < min_node_->key)
			{
				// 9
				min_node_ = x;
			}
		}

		
		void Cut(FibNode* x, FibNode* y)
		{
			/*
			* Cut(x,y)
			* 1. remove x from the child list of y, decrementing y.degree
			* 2. add x to the root list of H
			* 3. x.p = NIL
			* 4. x.mark = FALSE
			*/

			// 1
			if (x->right == x)
			{
				y->child = nullptr;
			}
			else
			{
				x->right->left = x->left;
				x->left->right = x->right;
				if (y->child == x)
				{
					y->child = x->right;
				}
			}
			y->degree--;
			// 2
			min_node_->right->left = x;
			x->right = min_node_->right;
			min_node_->right = x;
			x->left = min_node_;
			// 3
			x->p = nullptr;
			// 4
			x->mark = false;
		}

	
		void CascadingCut(FibNode* y)
		{
			/*
			* CascadingCut(y)
			* 1. z = y.p
			* 2. if z != NIL
			* 3. 	if y.mark == FALSE
			* 4. 		y.mark = TRUE
			* 5. 	else CUT(H,y,z)
			* 6. 		CASCADING-CUT(H,z)
			*/

			FibNode* z;

			// 1
			z = y->p;
			// 2
			if (z != nullptr)
			{
				// 3
				if (y->mark == false)
				{
					// 4
					y->mark = true;
				}
				else
				{
					// 5
					Cut(y, z);
					// 6
					CascadingCut(z);
				}
			}
		}

		//Sets the key to infinity, so that it goes to the top of the heap and can be easily removed
		void RemoveNode(FibNode* x)
		{
			DecreaseKey(x, std::numeric_limits<T>::min_());
			FibNode *fn = ExtractMin();
			delete fn;
		}

		int size_;
		FibNode *min_node_;

	};
}