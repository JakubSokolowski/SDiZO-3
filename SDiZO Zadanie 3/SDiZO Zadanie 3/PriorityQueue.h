#pragma once
#include <iostream>
#include <utility>
#include "Vector.h"

namespace DTS {

	//Priority queue that uses std::pair as Queue Node and min binary heap as internal data structure
	//T - type of node
	//P - priority, P must overload operator "<"
	//First element of the queue is the element of lowest priority
	template <typename T, typename P>
	class PriorityQueue {

		Vector<std::pair<T, P>> list;

		void ShiftUp(int index);
		void ShiftDown(int index);
		void Swap(int child, int parent);
		int GetLeftChild(int parent);
		int GetRightChild(int parent);
		int GetParent(int child);
		void MakeHeap();

	public:

		PriorityQueue();
		void Insert(T, P);

		void UpdatePriority(int index, P priority);
		P GetPriority(int index);
		void PopFront();
		T GetFront();
		int GetSize();
		bool IsEmpty();

	};

	//Base constructor
	template <typename T, typename P>
	PriorityQueue<T, P> ::PriorityQueue() {

	}

	//Returns the number of elements in queue
	template <typename T, typename P>
	inline int PriorityQueue<T, P> ::GetSize() {
		return list.size();
	}

	//Returns true if the number of elements in queue is 0
	template<typename T, typename P>
	inline bool PriorityQueue<T, P>::IsEmpty()
	{
		return GetSize() == 0;
	}

	//Swaps two elements, restores the property of binary heap
	template <typename T, typename P>
	inline void PriorityQueue<T, P>::Swap(int child, int parent) {
		std::pair<T, P> temp;
		temp = list[child];
		list[child] = list[parent];
		list[parent] = temp;
	}

	//Returns the parent node index
	template <typename T, typename P>
	inline int PriorityQueue<T, P>::GetParent(int child) {
		if (child % 2 == 0)
			return (child / 2) - 1;
		else
			return child / 2;
	}

	//Restores the property of heap
	template<typename T, typename P>
	inline void PriorityQueue<T, P>::MakeHeap()
	{
		for (int i = (GetSize() - 2) / 2; i >= 0; --i)
			ShiftDown(i);
	}

	//Returns the index of a left child
	template <typename T, typename P>
	inline int PriorityQueue<T, P>::GetLeftChild(int parent) {
		return 2 * parent + 1;
	}

	//Returns the index of a right child
	template <typename T, typename P>
	inline int PriorityQueue<T, P>::GetRightChild(int parent) {
		return 2 * parent + 2;
	}

	//Inserts the element into queue, the element is placed in according to its priority
	template <typename T, typename P>
	inline void PriorityQueue<T, P>::Insert(T value, P priority) {
		std::pair<T, P> new_pair(value, priority);
		list.push_back(new_pair);
		ShiftUp(GetSize() - 1);
	}

	//Updates the priority of element, calls MakeHeap after
	template<typename T, typename P>
	inline void PriorityQueue<T, P>::UpdatePriority(int index, P priority)
	{
		list[index].second = priority;
		MakeHeap();
	}

	//Returns the priority of element at index
	template<typename T, typename P>
	inline P PriorityQueue<T, P>::GetPriority(int index)
	{
		return list[index].second;
	}

	//Swaps last element with first, and removes the last (previously first) element
	template<typename T, typename P>
	inline void PriorityQueue<T, P>::PopFront()
	{
		int last_node_index = list.size() - 1;
		Swap(last_node_index, 0);
		list.pop_back();
		ShiftDown(0);
	}

	//Shifts the element up, if needed
	template <typename T, typename P>
	inline void PriorityQueue <T, P>::ShiftUp(int node_index)
	{
		if (node_index != 0)
		{
			int parent_index = GetParent(node_index);
			if (list[parent_index].second > list[node_index].second)
			{
				Swap(node_index, parent_index);
				ShiftUp(parent_index);
			}
		}
	}

	//Returns the first element in queue
	template <typename T, typename P>
	inline T PriorityQueue<T, P>::GetFront()
	{
		return list.front().first;
	}

	//Shifts the element down, if needed
	template <typename T, typename P>
	inline void PriorityQueue<T, P>::ShiftDown(int node_index) {
		int lci, rci, min_index;

		lci = GetLeftChild(node_index);
		rci = GetRightChild(node_index);

		if (rci >= GetSize())
		{
			if (lci >= GetSize())
				return;
			else
			{
				min_index = lci;
			}
		}
		else
		{
			if (list[lci].second <= list[rci].second)
				min_index = lci;
			else
				min_index = rci;
		}
		if (list[node_index].second > list[min_index].second)
		{
			Swap(min_index, node_index);
			ShiftDown(min_index);
		}
	}
}