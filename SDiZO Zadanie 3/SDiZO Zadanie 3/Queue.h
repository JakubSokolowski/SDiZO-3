#pragma once

namespace DTS
{
	//First in First out doubly linked list based queue
	//T - value stored in queue
	template <typename T>
	struct QueueNode
	{
		QueueNode(const T &val)
			: data_(val)
			, next_(0)
			, prev_(0)
		{}

		T data_;
		QueueNode<T> *next_;
		QueueNode<T> *prev_;
	};

	template <typename T>
	class Queue
	{
	public:
		Queue()
			: head_(0)
			, tail_(0)
			, size_(0)
		{}
		~Queue();

		int GetSize() { return size_; }
		bool IsEmpty() { return size_ == 0; }

		void PushBack(const T &e);
		void PopFront();
		T &GetFront();

	private:

		QueueNode<T>* Insert(const T& e, QueueNode<T> *pos);
		void Remove(QueueNode<T> *position);

		QueueNode<T> *head_;
		QueueNode<T> *tail_;

		int size_;

	};

	template<typename T>
	inline Queue<T>::~Queue()
	{
		QueueNode<T> *current(head_);

		while (current)
		{
			QueueNode<T> *next(current->next_);
			current->data_.~T();
			delete current;
			current = next;
		}
	}

	template<typename T>
	inline void Queue<T>::PushBack(const T & e)
	{
		Insert(e, 0);
	}

	template<typename T>
	inline void Queue<T>::PopFront()
	{
		Remove(head_);
	}

	template<typename T>
	inline T & Queue<T>::GetFront()
	{
		return head_->data_;
	}
	template<typename T>
	inline void Queue<T>::Remove(QueueNode<T>* pos)
	{
		if (pos)
		{
			if (pos->prev_)
				pos->prev_->next_ = pos->next_;

			if (pos->next_)
				pos->next_->prev_ = pos->prev_;

			if (pos == head_)
				head_ = pos->next_;

			if (pos == tail_)
				tail_ = pos->prev_;

			delete pos;

			size_--;
		}
	}
	template<typename T>
	inline QueueNode<T>* Queue<T>::Insert(const T & e, QueueNode<T>* pos)
	{
		QueueNode<T> *n = new QueueNode<T>(e);

		size_++;

		n->next_ = pos;

		if (pos)
		{
			n->prev_ = pos->prev_;
			pos->prev_ = n;
		}
		else
		{
			// pos is null that is at the very end of the list
			n->prev_ = tail_;
			tail_ = n;
		}

		if (n->prev_)
		{
			n->prev_->next_ = n;
		}
		else
		{
			// at the very begining of the list
			head_ = n;
		}

		return n;
	}
}
		
