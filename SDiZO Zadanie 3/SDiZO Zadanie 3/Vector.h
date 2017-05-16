#pragma once
#include <cstddef>
#include <cstring>
#include <utility>
#include <iterator>
#include <stdexcept>


#define DTS_VECTOR_MAX_SIZE 1000000000

namespace DTS {

	//Reweritten STL Vector container, with higher growth factor
	//T - value stored in Vector
	template <typename T>
	class Vector {
	public:
		// types:
		typedef T                                     value_type;
		typedef T &                                   reference;
		typedef const T &                             const_reference;
		typedef T *                                   pointer;
		typedef const T *                             const_pointer;
		typedef T *                                   iterator;
		typedef const T *                             const_iterator;
		typedef std::reverse_iterator<iterator>       reverse_iterator;
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
		typedef ptrdiff_t                             difference_type;
		typedef unsigned int                          size_type;

		//Construct, copy , destroy
		Vector() noexcept;
		explicit Vector(size_type n);
		Vector(size_type n, const T &val);
		Vector(typename Vector<T>::iterator first, typename Vector<T>::iterator last);
		Vector(std::initializer_list<T>);
		Vector(const Vector<T> &);
		Vector(Vector<T> &&) noexcept;
		~Vector();
		Vector<T> & operator = (const Vector<T> &);
		Vector<T> & operator = (Vector<T> &&);
		Vector<T> & operator = (std::initializer_list<T>);
		void assign(size_type, const T &value);
		void assign(typename Vector<T>::iterator, typename Vector<T>::iterator);
		void assign(std::initializer_list<T>);

		// iterators:
		iterator begin() noexcept;
		const_iterator cbegin() const noexcept;
		iterator end() noexcept;
		const_iterator cend() const noexcept;
		reverse_iterator rbegin() noexcept;
		const_reverse_iterator crbegin() const noexcept;
		reverse_iterator rend() noexcept;
		const_reverse_iterator crend() const noexcept;

		//Capacity
		bool empty() const noexcept;
		size_type size() const noexcept;
		size_type max_size() const noexcept;
		size_type capacity() const noexcept;
		void resize(size_type);
		void resize(size_type, const T &);
		void reserve(size_type);
		void shrink_to_fit();

		// element access
		reference operator [](size_type);
		const_reference operator [](size_type) const;
		reference at(size_type);
		const_reference at(size_type) const;
		reference front();
		const_reference front() const;
		reference back();
		const_reference back() const;

		//Data access
		T * data() noexcept;
		const T * data() const noexcept;

		//Modifiers
		template <class ... Args> void emplace_back(Args && ... args);
		void push_back(const T &);
		void push_back(T &&);
		void pop_back();

		template <class ... Args> iterator emplace(const_iterator, Args && ...);
		iterator insert(const_iterator, const T &);
		iterator insert(const_iterator, T &&);
		iterator insert(const_iterator, size_type, const T&);
		template <class InputIt> iterator insert(const_iterator, InputIt, InputIt);
		iterator insert(const_iterator, std::initializer_list<T>);
		iterator erase(const_iterator);
		iterator erase(const_iterator, const_iterator);
		void swap(Vector<T> &);
		void clear() noexcept;

		//Comparision operators
		bool operator == (const Vector<T> &) const;
		bool operator != (const Vector<T> &) const;
		bool operator < (const Vector<T> &) const;
		bool operator <= (const Vector<T> &) const;
		bool operator > (const Vector<T> &) const;
		bool operator >= (const Vector<T> &) const;
	private:
		size_type rsrv_sz = 4;
		size_type vec_sz = 0;
		T *arr;

		inline void reallocate();

	};



	template <typename T>
	Vector<T>::Vector() noexcept {
		arr = new T[rsrv_sz];
	}

	template <typename T>
	inline Vector<T>::Vector(typename Vector<T>::size_type n) {
		size_type i;
		rsrv_sz = n << 2;
		arr = new T[rsrv_sz];
		for (i = 0; i < n; ++i)
			arr[i] = T();
		vec_sz = n;
	}

	template <typename T>
	inline Vector<T>::Vector(typename Vector<T>::size_type n, const T &value) {
		size_type i;
		rsrv_sz = n << 2;
		arr = new T[rsrv_sz];
		for (i = 0; i < n; ++i)
			arr[i] = value;
		vec_sz = n;
	}

	template <typename T>
	inline Vector<T>::Vector(typename Vector<T>::iterator first, typename Vector<T>::iterator last) {
		size_type i, count = last - first;
		rsrv_sz = count << 2;
		arr = new T[rsrv_sz];
		for (i = 0; i < count; ++i, ++first)
			arr[i] = *first;
		vec_sz = count;
	}

	template <typename T>
	inline Vector<T>::Vector(std::initializer_list<T> lst) {
		rsrv_sz = lst.size() << 2;
		arr = new T[rsrv_sz];
		for (auto &item : lst)
			arr[vec_sz++] = item;
	}

	template <typename T>
	inline Vector<T>::Vector(const Vector<T> &other) {
		size_type i;
		rsrv_sz = other.rsrv_sz;
		arr = new T[rsrv_sz];
		for (i = 0; i < other.vec_sz; ++i)
			arr[i] = other.arr[i];
		vec_sz = other.vec_sz;
	}

	template <typename T>
	inline Vector<T>::Vector(Vector<T> &&other) noexcept {
		size_type i;
		rsrv_sz = other.rsrv_sz;
		arr = new T[rsrv_sz];
		for (i = 0; i < other.vec_sz; ++i)
			arr[i] = std::move(other.arr[i]);
		vec_sz = other.vec_sz;
	}

	template <typename T>
	inline Vector<T>::~Vector() {
		delete[] arr;
	}

	template <typename T>
	inline Vector<T> & Vector<T>::operator = (const Vector<T> &other) {
		size_type i;
		if (rsrv_sz < other.vec_sz) {
			rsrv_sz = other.vec_sz << 2;
			reallocate();
		}
		for (i = 0; i < other.vec_sz; ++i)
			arr[i] = other.arr[i];
		vec_sz = other.vec_sz;
		return *this;
	}

	template <typename T>
	inline Vector<T> & Vector<T>::operator = (Vector<T> &&other) {
		size_type i;
		if (rsrv_sz < other.vec_sz) {
			rsrv_sz = other.vec_sz << 2;
			reallocate();
		}
		for (i = 0; i < other.vec_sz; ++i)
			arr[i] = std::move(other.arr[i]);
		vec_sz = other.vec_sz;
		return *this;
	}

	template <typename T>
	inline Vector<T> & Vector<T>::operator = (std::initializer_list<T> lst) {
		if (rsrv_sz < lst.size()) {
			rsrv_sz = lst.size() << 2;
			reallocate();
		}
		vec_sz = 0;
		for (auto &item : lst)
			arr[vec_sz++] = item;
	}

	template <typename T>
	inline void Vector<T>::assign(typename Vector<T>::size_type count, const T &value) {
		size_type i;
		if (count > rsrv_sz) {
			rsrv_sz = count << 2;
			reallocate();
		}
		for (i = 0; i < count; ++i)
			arr[i] = value;
		vec_sz = count;
	}

	template <typename T>
	inline void Vector<T>::assign(typename Vector<T>::iterator first, typename Vector<T>::iterator last) {
		size_type i, count = last - first;
		if (count > rsrv_sz) {
			rsrv_sz = count << 2;
			reallocate();
		}
		for (i = 0; i < count; ++i, ++first)
			arr[i] = *first;
		vec_sz = count;
	}

	template <typename T>
	inline void Vector<T>::assign(std::initializer_list<T> lst) {
		size_type i, count = lst.size();
		if (count > rsrv_sz) {
			rsrv_sz = count << 2;
			reallocate();
		}
		i = 0;
		for (auto &item : lst)
			arr[i++] = item;
	}


	template <typename T>
	inline typename Vector<T>::iterator Vector<T>::begin() noexcept {
		return arr;
	}

	template <typename T>
	inline typename Vector<T>::const_iterator Vector<T>::cbegin() const noexcept {
		return arr;
	}

	template <typename T>
	inline typename Vector<T>::iterator Vector<T>::end() noexcept {
		return arr + vec_sz;
	}

	template <typename T>
	inline typename Vector<T>::const_iterator Vector<T>::cend() const noexcept {
		return arr + vec_sz;
	}

	template <typename T>
	inline typename Vector<T>::reverse_iterator Vector<T>::rbegin() noexcept {
		return reverse_iterator(arr + vec_sz);
	}

	template <typename T>
	inline typename Vector<T>::const_reverse_iterator Vector<T>::crbegin() const noexcept {
		return reverse_iterator(arr + vec_sz);
	}

	template <typename T>
	inline typename Vector<T>::reverse_iterator Vector<T>::rend() noexcept {
		return reverse_iterator(arr);
	}

	template <typename T>
	inline typename Vector<T>::const_reverse_iterator Vector<T>::crend() const noexcept {
		return reverse_iterator(arr);
	}


	template <typename T>
	inline void Vector<T>::reallocate() {
		T *tarr = new T[rsrv_sz];
		memcpy(tarr, arr, vec_sz * sizeof(T));
		delete[] arr;
		arr = tarr;
	}


	template <typename T>
	inline bool Vector<T>::empty() const noexcept {
		return vec_sz == 0;
	}

	template <typename T>
	inline typename Vector<T>::size_type Vector<T>::size() const noexcept {
		return vec_sz;
	}

	template <typename T>
	inline typename Vector<T>::size_type Vector<T>::max_size() const noexcept {
		return DTS_VECTOR_MAX_SIZE;
	}

	template <typename T>
	inline typename Vector<T>::size_type Vector<T>::capacity() const noexcept {
		return rsrv_sz;
	}

	template <typename T>
	inline void Vector<T>::resize(typename Vector<T>::size_type sz) {
		if (sz > vec_sz) {
			if (sz > rsrv_sz) {
				rsrv_sz = sz;
				reallocate();
			}
		}
		else {
			size_type i;
			for (i = vec_sz; i < sz; ++i)
				arr[i].~T();
		}
		vec_sz = sz;
	}

	template <typename T>
	inline void Vector<T>::resize(typename Vector<T>::size_type sz, const T &c) {
		if (sz > vec_sz) {
			if (sz > rsrv_sz) {
				rsrv_sz = sz;
				reallocate();
			}
			size_type i;
			for (i = vec_sz; i < sz; ++i)
				arr[i] = c;
		}
		else {
			size_type i;
			for (i = vec_sz; i < sz; ++i)
				arr[i].~T();
		}
		vec_sz = sz;
	}

	template <typename T>
	inline void Vector<T>::reserve(typename Vector<T>::size_type _sz) {
		if (_sz > rsrv_sz) {
			rsrv_sz = _sz;
			reallocate();
		}
	}

	template <typename T>
	inline void Vector<T>::shrink_to_fit() {
		rsrv_sz = vec_sz;
		reallocate();
	}


	template <typename T>
	inline typename Vector<T>::reference Vector<T>::operator [](typename Vector<T>::size_type idx) {
		return arr[idx];
	}

	template <typename T>
	inline typename Vector<T>::const_reference Vector<T>::operator [](typename Vector<T>::size_type idx) const {
		return arr[idx];
	}

	template <typename T>
	inline typename Vector<T>::reference Vector<T>::at(size_type pos) {
		if (pos < vec_sz)
			return arr[pos];
		else
			throw std::out_of_range("accessed position is out of range");
	}

	template <typename T>
	inline typename Vector<T>::const_reference Vector<T>::at(size_type pos) const {
		if (pos < vec_sz)
			return arr[pos];
		else
			throw std::out_of_range("accessed position is out of range");
	}

	template <typename T>
	inline typename Vector<T>::reference Vector<T>::front() {
		return arr[0];
	}

	template <typename T>
	inline typename Vector<T>::const_reference Vector<T>::front() const {
		return arr[0];
	}

	template <typename T>
	inline typename Vector<T>::reference Vector<T>::back() {
		return arr[vec_sz - 1];
	}

	template <typename T>
	inline typename Vector<T>::const_reference Vector<T>::back() const {
		return arr[vec_sz - 1];
	}


	template <typename T>
	inline T * Vector<T>::data() noexcept {
		return arr;
	}

	template <typename T>
	inline const T * Vector<T>::data() const noexcept {
		return arr;
	}


	template <typename T>
	template <class ... Args>
	inline void Vector<T>::emplace_back(Args && ... args) {
		if (vec_sz == rsrv_sz) {
			rsrv_sz <<= 2;
			reallocate();
		}
		arr[vec_sz] = std::move(T(std::forward<Args>(args) ...));
		++vec_sz;
	}

	template <typename T>
	inline void Vector<T>::push_back(const T &val) {
		if (vec_sz == rsrv_sz) {
			rsrv_sz <<= 2;
			reallocate();
		}
		arr[vec_sz] = val;
		++vec_sz;
	}

	template <typename T>
	inline void Vector<T>::push_back(T &&val) {
		if (vec_sz == rsrv_sz) {
			rsrv_sz <<= 2;
			reallocate();
		}
		arr[vec_sz] = std::move(val);
		++vec_sz;
	}

	template <typename T>
	inline void Vector<T>::pop_back() {
		--vec_sz;
		arr[vec_sz].~T();
	}


	template <typename T>
	template <class ... Args>
	inline typename Vector<T>::iterator Vector<T>::emplace(typename Vector<T>::const_iterator it, Args && ... args) {
		iterator iit = &arr[it - arr];
		if (vec_sz == rsrv_sz) {
			rsrv_sz <<= 2;
			reallocate();
		}
		memmove(iit + 1, iit, (vec_sz - (it - arr)) * sizeof(T));
		(*iit) = std::move(T(std::forward<Args>(args) ...));
		++vec_sz;
		return iit;
	}

	template <typename T>
	inline typename Vector<T>::iterator Vector<T>::insert(typename Vector<T>::const_iterator it, const T &val) {
		iterator iit = &arr[it - arr];
		if (vec_sz == rsrv_sz) {
			rsrv_sz <<= 2;
			reallocate();
		}
		memmove(iit + 1, iit, (vec_sz - (it - arr)) * sizeof(T));
		(*iit) = val;
		++vec_sz;
		return iit;
	}

	template <typename T>
	inline typename Vector<T>::iterator Vector<T>::insert(typename Vector<T>::const_iterator it, T &&val) {
		iterator iit = &arr[it - arr];
		if (vec_sz == rsrv_sz) {
			rsrv_sz <<= 2;
			reallocate();
		}
		memmove(iit + 1, iit, (vec_sz - (it - arr)) * sizeof(T));
		(*iit) = std::move(val);
		++vec_sz;
		return iit;
	}

	template <typename T>
	inline typename Vector<T>::iterator Vector<T>::insert(typename Vector<T>::const_iterator it, typename Vector<T>::size_type cnt, const T &val) {
		iterator f = &arr[it - arr];
		if (!cnt) return f;
		if (vec_sz + cnt > rsrv_sz) {
			rsrv_sz = (vec_sz + cnt) << 2;
			reallocate();
		}
		memmove(f + cnt, f, (vec_sz - (it - arr)) * sizeof(T));
		vec_sz += cnt;
		for (iterator it = f; cnt--; ++it)
			(*it) = val;
		return f;
	}

	template <typename T>
	template <class InputIt>
	inline typename Vector<T>::iterator Vector<T>::insert(typename Vector<T>::const_iterator it, InputIt first, InputIt last) {
		iterator f = &arr[it - arr];
		size_type cnt = last - first;
		if (!cnt) return f;
		if (vec_sz + cnt > rsrv_sz) {
			rsrv_sz = (vec_sz + cnt) << 2;
			reallocate();
		}
		memmove(f + cnt, f, (vec_sz - (it - arr)) * sizeof(T));
		for (iterator it = f; first != last; ++it, ++first)
			(*it) = *first;
		vec_sz += cnt;
		return f;
	}

	template <typename T>
	inline typename Vector<T>::iterator Vector<T>::insert(typename Vector<T>::const_iterator it, std::initializer_list<T> lst) {
		size_type cnt = lst.size();
		iterator f = &arr[it - arr];
		if (!cnt) return f;
		if (vec_sz + cnt > rsrv_sz) {
			rsrv_sz = (vec_sz + cnt) << 2;
			reallocate();
		}
		memmove(f + cnt, f, (vec_sz - (it - arr)) * sizeof(T));
		iterator iit = f;
		for (auto &item : lst) {
			(*iit) = item;
			++iit;
		}
		vec_sz += cnt;
		return f;
	}

	template <typename T>
	inline typename Vector<T>::iterator Vector<T>::erase(typename Vector<T>::const_iterator it) {
		iterator iit = &arr[it - arr];
		(*iit).~T();
		memmove(iit, iit + 1, (vec_sz - (it - arr) - 1) * sizeof(T));
		--vec_sz;
		return iit;
	}

	template <typename T>
	inline typename Vector<T>::iterator Vector<T>::erase(typename Vector<T>::const_iterator first, typename Vector<T>::const_iterator last) {
		iterator f = &arr[first - arr];
		if (first == last) return f;
		for (; first != last; ++first)
			(*first).~T();
		memmove(f, last, (vec_sz - (last - arr)) * sizeof(T));
		vec_sz -= last - first;
		return f;
	}

	template <typename T>
	inline void Vector<T>::swap(Vector<T> &rhs) {
		size_t tvec_sz = vec_sz,
			trsrv_sz = rsrv_sz;
		T *tarr = arr;

		vec_sz = rhs.vec_sz;
		rsrv_sz = rhs.rsrv_sz;
		arr = rhs.arr;

		rhs.vec_sz = tvec_sz;
		rhs.rsrv_sz = trsrv_sz;
		rhs.arr = tarr;
	}

	template <typename T>
	inline void Vector<T>::clear() noexcept {
		size_type i;
		for (i = 0; i < vec_sz; ++i)
			arr[i].~T();
		vec_sz = 0;
	}


	template <typename T>
	inline bool Vector<T>::operator == (const Vector<T> &rhs) const {
		if (vec_sz != rhs.vec_sz) return false;
		size_type i;
		for (i = 0; i < vec_sz; ++i)
			if (arr[i] != rhs.arr[i])
				return false;
		return true;
	}

	template <typename T>
	inline bool Vector<T>::operator != (const Vector<T> &rhs) const {
		if (vec_sz != rhs.vec_sz) return true;
		size_type i;
		for (i = 0; i < vec_sz; ++i)
			if (arr[i] != rhs.arr[i])
				return true;
		return false;
	}

	template <typename T>
	inline bool Vector<T>::operator < (const Vector<T> &rhs) const {
		size_type i, j, ub = vec_sz < rhs.vec_sz ? vec_sz : rhs.vec_sz;
		for (i = 0; i < ub; ++i)
			if (arr[i] != rhs.arr[i])
				return arr[i] < rhs.arr[i];
		return vec_sz < rhs.vec_sz;
	}

	template <typename T>
	inline bool Vector<T>::operator <= (const Vector<T> &rhs) const {
		size_type i, j, ub = vec_sz < rhs.vec_sz ? vec_sz : rhs.vec_sz;
		for (i = 0; i < ub; ++i)
			if (arr[i] != rhs.arr[i])
				return arr[i] < rhs.arr[i];
		return vec_sz <= rhs.vec_sz;
	}

	template <typename T>
	inline bool Vector<T>::operator > (const Vector<T> &rhs) const {
		size_type i, j, ub = vec_sz < rhs.vec_sz ? vec_sz : rhs.vec_sz;
		for (i = 0; i < ub; ++i)
			if (arr[i] != rhs.arr[i])
				return arr[i] > rhs.arr[i];
		return vec_sz > rhs.vec_sz;
	}

	template <typename T>
	inline bool Vector<T>::operator >= (const Vector<T> &rhs) const {
		size_type i, j, ub = vec_sz < rhs.vec_sz ? vec_sz : rhs.vec_sz;
		for (i = 0; i < ub; ++i)
			if (arr[i] != rhs.arr[i])
				return arr[i] > rhs.arr[i];
		return vec_sz >= rhs.vec_sz;
	}



	template <>
	inline void Vector<bool>::resize(typename Vector<bool>::size_type sz) {
		if (sz > rsrv_sz) {
			rsrv_sz = sz;
			reallocate();
		}
		vec_sz = sz;
	}	
}