#pragma once

#include <memory>
#include <utility>
#include <algorithm>
#include <new>

#ifdef _DEBUG
	constexpr static bool STATIC_VECTOR_DEBUGGING = true;
#else
	constexpr static bool STATIC_VECTOR_DEBUGGING = false;
#endif // _DEBUG

template <typename T, size_t Capacity>
class static_vector
{
	std::aligned_storage_t<sizeof(T), alignof(T)> _data[Capacity];
	std::size_t _size = 0;

public:

	struct iterator
	{
		using difference_type = ptrdiff_t;
		using value_type = T;
		using element_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::contiguous_iterator_tag;

		constexpr iterator() noexcept : ptr{} {};
		constexpr iterator(const iterator&) noexcept = default;
		constexpr iterator(iterator&&) noexcept = default;
		constexpr iterator(pointer other) noexcept : ptr{ other } {}
		constexpr iterator(bool) = delete;

		constexpr iterator& operator=(const iterator&) = default;

		constexpr iterator& operator=(iterator&&) = default;

		constexpr reference operator* () const noexcept
		{
			return *ptr;
		}

		constexpr pointer operator-> () const noexcept
		{
			return ptr;
		}

		constexpr iterator& operator++ () noexcept
		{
			++ptr;
			return *this;
		}

		constexpr iterator operator++(int) noexcept
		{
			return iterator(ptr++);
		}

		constexpr iterator& operator-- () noexcept
		{
			--ptr;
			return *this;
		}

		constexpr iterator operator--(int) noexcept
		{
			return iterator(ptr--);
		}

		constexpr iterator& operator+=(const ptrdiff_t offset) noexcept
		{
			ptr += offset;
			return *this;
		}

		constexpr iterator& operator-=(const ptrdiff_t offset) noexcept
		{
			ptr -= offset;
			return *this;
		}

		constexpr reference operator[](const size_t offset) const noexcept
		{
			return ptr[offset];
		}

		constexpr friend bool operator==(const iterator it_a, const iterator it_b) noexcept
		{
			return it_a.ptr == it_b.ptr;
		}

		constexpr friend bool operator!=(const iterator it_a, const iterator it_b) noexcept
		{
			return it_a.ptr != it_b.ptr;
		}

		constexpr friend iterator operator+(const iterator it, const size_t offset) noexcept
		{
			T* result = it.ptr + offset;
			return iterator(result);
		}

		constexpr friend iterator operator+(const size_t offset, const iterator& it) noexcept
		{
			auto aux = offset + it.ptr;
			return iterator(aux);
		}

		constexpr friend iterator operator-(const iterator it, const size_t offset) noexcept
		{
			T* aux = it.ptr - offset;
			return iterator(aux);
		}

		constexpr friend difference_type operator-(const iterator a, const iterator b) noexcept
		{
			return a.ptr - b.ptr;
		}

		constexpr friend auto operator<=>(const iterator a, const iterator b) noexcept
		{
			return a.ptr <=> b.ptr;
		}

	private:
		pointer ptr;
	};
	struct const_iterator
	{
		using difference_type = ptrdiff_t;
		using value_type = T;
		using element_type = T;
		using pointer = const T*;
		using reference = const T&;
		using iterator_category = std::contiguous_iterator_tag;

		constexpr const_iterator() noexcept : ptr{} {};
		constexpr const_iterator(const const_iterator&) noexcept = default;
		constexpr const_iterator(const_iterator&&) noexcept = default;
		constexpr const_iterator(pointer other) noexcept : ptr{ other } {}

		constexpr const_iterator(bool) = delete;

		constexpr const_iterator(const iterator other) noexcept : ptr{ other.ptr } {}

		constexpr const_iterator& operator=(const const_iterator&) noexcept = default;

		constexpr const_iterator& operator=(const_iterator&&) noexcept = default;

		constexpr const_iterator& operator= (const pointer other) noexcept
		{
			ptr = other;
			return (*this);
		}

		constexpr reference operator* () const noexcept
		{
			return *ptr;
		}

		constexpr pointer operator-> () const noexcept
		{
			return ptr;
		}

		constexpr const_iterator& operator++ () noexcept
		{
			++ptr;
			return *this;
		}

		constexpr const_iterator operator++(int) noexcept
		{
			return const_iterator(ptr++);
		}

		constexpr const_iterator& operator-- () noexcept
		{
			--ptr;
			return *this;
		}

		constexpr const_iterator operator--(int) noexcept
		{
			return const_iterator(ptr--);
		}

		constexpr const_iterator& operator+=(const size_t offset) noexcept
		{
			ptr += offset;
			return *this;
		}

		constexpr const_iterator& operator-=(const size_t offset) noexcept
		{
			ptr -= offset;
			return *this;
		}

		constexpr reference operator[](const size_t offset) const noexcept
		{
			return ptr[offset];
		}

		constexpr friend bool operator==(const const_iterator it_a, const const_iterator it_b) noexcept
		{
			return it_a.ptr == it_b.ptr;
		}

		constexpr friend bool operator==(const const_iterator it, const pointer adr) noexcept
		{
			return it.ptr == adr;
		}

		constexpr friend bool operator!=(const const_iterator it_a, const const_iterator it_b) noexcept
		{
			return it_a.ptr != it_b.ptr;
		}

		constexpr friend bool operator!=(const const_iterator it, const pointer adr) noexcept
		{
			return it.ptr != adr;
		}

		constexpr friend const_iterator operator+(const const_iterator& it, const size_t offset) noexcept
		{
			return const_iterator(it.ptr + offset);
		}

		constexpr friend const_iterator operator+(const size_t offset, const const_iterator it) noexcept
		{
			return const_iterator(offset + it.ptr);
		}

		constexpr friend const_iterator operator-(const const_iterator it, const size_t offset) noexcept
		{
			return const_iterator(it.ptr - offset);
		}

		constexpr friend difference_type operator-(const const_iterator a, const const_iterator b) noexcept
		{
			return a.ptr - b.ptr;
		}

		constexpr friend auto operator<=>(const const_iterator a, const const_iterator b) noexcept
		{
			return a.ptr <=> b.ptr;
		}

	private:

		pointer ptr;
	};
	struct reverse_iterator
	{
		using difference_type = ptrdiff_t;
		using value_type = T;
		using element_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::contiguous_iterator_tag;

		constexpr reverse_iterator() noexcept : ptr{} {};
		constexpr reverse_iterator(const reverse_iterator&) noexcept = default;
		constexpr reverse_iterator(reverse_iterator&&) noexcept = default;
		constexpr reverse_iterator(pointer other) noexcept : ptr{ other } {}
		constexpr reverse_iterator(bool) = delete;

		constexpr reverse_iterator& operator=(const reverse_iterator&) = default;

		constexpr reverse_iterator& operator=(reverse_iterator&&) = default;

		constexpr reference operator* () const noexcept
		{
			return *ptr;
		}

		constexpr pointer operator-> () const noexcept
		{
			return ptr;
		}

		constexpr reverse_iterator& operator++ () noexcept
		{
			++ptr;
			return *this;
		}

		constexpr reverse_iterator operator++(int) noexcept
		{
			return iterator(ptr--);
		}

		constexpr reverse_iterator& operator-- () noexcept
		{
			++ptr;
			return *this;
		}

		constexpr reverse_iterator operator--(int) noexcept
		{
			return iterator(ptr++);
		}

		constexpr reverse_iterator& operator+=(const ptrdiff_t offset) noexcept
		{
			ptr -= offset;
			return *this;
		}

		constexpr reverse_iterator& operator-=(const ptrdiff_t offset) noexcept
		{
			ptr += offset;
			return *this;
		}

		constexpr reverse_iterator operator[](const size_t offset) const noexcept
		{
			return ptr[offset];
		}

		constexpr friend bool operator==(const reverse_iterator it_a, const reverse_iterator it_b) noexcept
		{
			return it_a.ptr == it_b.ptr;
		}

		constexpr friend bool operator!=(const reverse_iterator it_a, const reverse_iterator it_b) noexcept
		{
			return it_a.ptr != it_b.ptr;
		}

		constexpr friend reverse_iterator operator+(const reverse_iterator it, const size_t offset) noexcept
		{
			T* result = it.ptr - offset;
			return reverse_iterator(result);
		}

		constexpr friend reverse_iterator operator+(const size_t offset, const reverse_iterator& it) noexcept
		{
			auto aux = offset - it.ptr;
			return reverse_iterator(aux);
		}

		constexpr friend reverse_iterator operator-(const reverse_iterator it, const size_t offset) noexcept
		{
			T* aux = it.ptr + offset;
			return iterator(aux);
		}

		constexpr friend difference_type operator-(const reverse_iterator a, const reverse_iterator b) noexcept
		{
			return a.ptr + b.ptr;
		}

		constexpr friend auto operator<=>(const reverse_iterator a, const reverse_iterator b) noexcept
		{
			return b.ptr <=> a.ptr;
		}

	private:
		pointer ptr;
	};
	struct const_reverse_iterator
	{
		using difference_type = ptrdiff_t;
		using value_type = T;
		using element_type = T;
		using pointer = const T*;
		using reference = const T&;
		using iterator_category = std::contiguous_iterator_tag;

		constexpr const_reverse_iterator() noexcept : ptr{} {};
		constexpr const_reverse_iterator(const const_reverse_iterator&) noexcept = default;
		constexpr const_reverse_iterator(const_reverse_iterator&&) noexcept = default;
		constexpr const_reverse_iterator(pointer other) noexcept : ptr{ other } {}

		constexpr const_reverse_iterator(bool) = delete;

		constexpr const_reverse_iterator(const iterator other) noexcept : ptr{ other.ptr } {}

		constexpr const_reverse_iterator& operator=(const const_reverse_iterator&) noexcept = default;

		constexpr const_reverse_iterator& operator=(const_reverse_iterator&&) noexcept = default;

		constexpr const_reverse_iterator& operator= (const pointer other) noexcept
		{
			ptr = other;
			return (*this);
		}

		constexpr reference operator* () const noexcept
		{
			return *ptr;
		}

		constexpr pointer operator-> () const noexcept
		{
			return ptr;
		}

		constexpr const_reverse_iterator& operator++ () noexcept
		{
			--ptr;
			return *this;
		}

		constexpr const_reverse_iterator operator++(int) noexcept
		{
			return const_reverse_iterator(ptr--);
		}

		constexpr const_reverse_iterator& operator-- () noexcept
		{
			++ptr;
			return *this;
		}

		constexpr const_reverse_iterator operator--(int) noexcept
		{
			return const_reverse_iterator(ptr++);
		}

		constexpr const_reverse_iterator& operator+=(const size_t offset) noexcept
		{
			ptr -= offset;
			return *this;
		}

		constexpr const_reverse_iterator& operator-=(const size_t offset) noexcept
		{
			ptr += offset;
			return *this;
		}

		constexpr reference operator[](const size_t offset) const noexcept
		{
			return ptr[offset];
		}

		constexpr friend bool operator==(const const_reverse_iterator it_a, const const_reverse_iterator it_b) noexcept
		{
			return it_a.ptr == it_b.ptr;
		}

		constexpr friend bool operator==(const const_reverse_iterator it, const pointer adr) noexcept
		{
			return it.ptr == adr;
		}

		constexpr friend bool operator!=(const const_reverse_iterator it_a, const const_reverse_iterator it_b) noexcept
		{
			return it_a.ptr != it_b.ptr;
		}

		constexpr friend bool operator!=(const const_reverse_iterator it, const pointer adr) noexcept
		{
			return it.ptr != adr;
		}

		constexpr friend const_reverse_iterator operator+(const const_reverse_iterator& it, const size_t offset) noexcept
		{
			return const_reverse_iterator(it.ptr - offset);
		}

		constexpr friend const_reverse_iterator operator+(const size_t offset, const const_reverse_iterator it) noexcept
		{
			return const_reverse_iterator(offset - it.ptr);
		}

		constexpr friend const_reverse_iterator operator-(const const_reverse_iterator it, const size_t offset) noexcept
		{
			return const_reverse_iterator(it.ptr + offset);
		}

		constexpr friend difference_type operator-(const const_reverse_iterator a, const const_reverse_iterator b) noexcept
		{
			return a.ptr + b.ptr;
		}

		constexpr friend auto operator<=>(const const_reverse_iterator a, const const_reverse_iterator b) noexcept
		{
			return b.ptr <=> a.ptr;;
		}

	private:

		pointer ptr;
	};

	using value_type = T;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = T*;
	using const_pointer = const T*;

	constexpr iterator begin() noexcept
	{
		return iterator(std::launder(reinterpret_cast<T*>(&_data[0])));
	}
	constexpr iterator end() noexcept
	{
		return iterator(std::launder(reinterpret_cast<T*>(&_data[_size])));
	}
	constexpr const_iterator begin() const noexcept
	{
		return const_iterator(std::launder(reinterpret_cast<const T*>(&_data[0])));
	}
	constexpr const_iterator end() const noexcept
	{
		return const_iterator(std::launder(reinterpret_cast<const T*>(&_data[_size])));
	}
	constexpr const_iterator cbegin() const noexcept
	{
		return const_iterator(std::launder(reinterpret_cast<const T*>(&_data[0])));
	}
	constexpr const_iterator cend() const noexcept
	{
		return const_iterator(std::launder(reinterpret_cast<const T*>(&_data[_size])));
	}
	constexpr reverse_iterator rbegin() noexcept
	{
		return reverse_iterator(std::launder(reinterpret_cast<T*>(&_data[_size - 1])));
	}
	constexpr reverse_iterator rend() noexcept
	{
		return reverse_iterator(std::launder(reinterpret_cast<T*>(&_data[-1])));
	}
	constexpr const_reverse_iterator rbegin() const noexcept
	{
		return const_reverse_iterator(std::launder(reinterpret_cast<const T*>(&_data[_size - 1])));
	}
	constexpr const_reverse_iterator rend() const noexcept
	{
		return const_reverse_iterator(std::launder(reinterpret_cast<const T*>(&_data[-1])));
	}
	constexpr const_reverse_iterator crbegin() const noexcept
	{
		return const_reverse_iterator(std::launder(reinterpret_cast<const T*>(&_data[_size - 1])));
	}
	constexpr const_reverse_iterator crend() const noexcept
	{
		return const_reverse_iterator(std::launder(reinterpret_cast<const T*>(&_data[-1])));
	}

	constexpr static_vector() noexcept = default;

	constexpr static_vector(std::size_t count, const T& value) noexcept (std::is_nothrow_copy_constructible_v<T>)
	{
		if (count > Capacity)
		{
			throw std::runtime_error("Static vector lacks the capacity for so many elements!\n");
		}

		std::uninitialized_fill_n(begin(), count, value);

		_size = count;
	}

	constexpr static_vector(std::size_t count) noexcept (std::is_nothrow_default_constructible_v<T>)
	{
		if (count > Capacity)
		{
			throw std::runtime_error("Static vector lacks the capacity for so many elements!\n");
		}

		std::uninitialized_default_construct_n(begin(), count);

		_size = count;
	}

	template<typename Iterator>
	constexpr static_vector(Iterator first, Iterator last) noexcept (std::is_nothrow_copy_constructible_v<T>)
	{
		const auto count = std::distance(first, last);

		if (count > Capacity)
		{
			throw std::runtime_error("Static vector lacks the capacity for so many elements!\n");
		}

		std::uninitialized_copy(first, last, begin());

		_size = count;
	}

	template <typename U> requires std::constructible_from<T, U>
	constexpr static_vector(std::initializer_list<U> values) noexcept (std::is_nothrow_constructible_v<T, U>)
	{
		const auto count = values.size();

		if (count > Capacity)
		{
			throw std::runtime_error("Static vector lacks the capacity for so many elements!\n");
		}

		std::uninitialized_copy_n(values.begin(), count, begin());

		_size = count;
	}

	template<std::size_t Other_Capacity>
	constexpr static_vector(const static_vector<T, Other_Capacity>& other) noexcept (std::is_nothrow_copy_constructible_v<T> && (Other_Capacity <= Capacity))
	{
		if constexpr (Other_Capacity > Capacity)
		{
			if (other.size() > Capacity)
			{
				throw std::runtime_error("Static vector lacks the capacity to store the data of the other vector!\n");
			}
		}

		std::uninitialized_copy_n(other.cbegin(), other.size(), begin());

		_size = other.size();
	}

	template<std::size_t Other_Capacity>
	constexpr static_vector& operator =(const static_vector<T, Other_Capacity>& other) noexcept (std::is_nothrow_copy_assignable_v<T> && std::is_nothrow_destructible_v<T> && (Other_Capacity <= Capacity))
	{
		if constexpr (Other_Capacity > Capacity)
		{
			if (other._size > Capacity)
			{
				throw std::runtime_error("Static vector lacks the capacity to store the data of the other vector!\n");
			}
		}

		if constexpr (std::is_trivially_copyable_v<T>)
		{
			std::copy_n(other.cbegin(), other.size(), begin());
		}
		else
		{
			if (_size <= other.size())
			{
				std::copy_n(other.cbegin(), _size, begin());
				std::uninitialized_copy_n(other.cbegin() + _size, other.size() - _size, begin() + _size);
			}
			else
			{
				std::copy_n(other.cbegin(), other.size(), begin());
				if constexpr (!std::is_trivially_destructible_v<T>)
				{
					std::destroy_n(begin() + other.size(), _size - other.size());
				}
			}
		}

		_size = other.size();

		return *this;
	}

	constexpr static_vector& operator=(std::initializer_list<T> values)
	{
		const auto count = values.size();

		if (count > Capacity)
		{
			throw std::runtime_error("Static vector lacks the capacity for so many elements!\n");
		}

		if constexpr (std::is_trivially_copyable_v<T>)
		{
			std::copy_n(values.begin(), count, begin());
		}
		else
		{
			if (_size <= count)
			{
				std::copy_n(values.begin(), _size, begin());
				std::uninitialized_copy_n(values.begin() + _size, count - _size, begin() + _size);
			}
			else
			{
				std::copy_n(values.begin(), count, begin());
				if constexpr (!std::is_trivially_destructible_v<T>)
				{
					std::destroy_n(begin() + count, _size - count);
				}
			}
		}

		_size = count;

		return *this;
	}

	constexpr reference operator[] (std::size_t index) noexcept(!STATIC_VECTOR_DEBUGGING)
	{
		if (STATIC_VECTOR_DEBUGGING)
		{
			if (index > _size - 1)
			{
				throw std::out_of_range("Index out of bounds!\n");
			}
		}

		return *reinterpret_cast<T*>(&_data[index]);
	}

	constexpr const_reference operator[] (std::size_t index) const noexcept(!STATIC_VECTOR_DEBUGGING)
	{
		if (STATIC_VECTOR_DEBUGGING)
		{
			if (index > _size - 1)
			{
				throw std::out_of_range("Index out of bounds!\n");
			}
		}

		return *reinterpret_cast<const T*>(&_data[index]);
	}

	constexpr reference at(std::size_t index) 
	{
		if (index > _size - 1)
		{
			throw std::out_of_range("Index out of bounds!\n");
		}

		return *reinterpret_cast<T*>(&_data[index]);
	}

	constexpr const_reference at(std::size_t index) const 
	{
		if (index > _size - 1)
		{
			throw std::out_of_range("Index out of bounds!\n");
		}

		return *reinterpret_cast<const T*>(&_data[index]);
	}

	constexpr void push_back(const T& val)
	{
		if (_size == Capacity)
		{
			throw std::runtime_error("Vector is at full capacity, push back not allowed!\n");
		}

		std::construct_at(std::to_address(end()), val);
		_size++;
	}

	constexpr void push_back(T&& val)
	{
		if (_size == Capacity)
		{
			throw std::runtime_error("Vector is at full capacity, push back not allowed!\n");
		}

		std::construct_at(std::to_address(end()), std::forward<T>(val));
		_size++;
	}

	constexpr void pop_back() noexcept(!STATIC_VECTOR_DEBUGGING && std::is_nothrow_destructible_v<T>)
	{
		if (STATIC_VECTOR_DEBUGGING)
		{
			if (empty())
			{
				throw std::runtime_error("Can't pop from empty vector!\n");
			}
		}

		if constexpr (!std::is_trivially_destructible_v<T>)
		{
			std::destroy_at(reinterpret_cast<T*>(&_data[_size - 1]));
		}

		_size--;
	}

	template <typename ... Args>
	constexpr void emplace_back(Args&& ... args)
	{
		if (_size == Capacity)
		{
			throw std::runtime_error("Vector is at full capacity, push back not allowed!\n");
		}

		std::construct_at(std::to_address(end()), std::forward<T>(args)...);
		_size++;
	}

	constexpr void clear() noexcept (std::is_nothrow_destructible_v<T>)
	{
		if constexpr (std::is_trivially_destructible_v<T>)
		{
			_size = 0;
		}
		else
		{
			if (std::is_nothrow_destructible_v<T>)
			{
				std::destroy_n(begin(), size());
				_size = 0;
			}
			else
			{
				while (!empty())
				{
					pop_back();
				}
			}
		}
	}

	constexpr iterator insert(iterator pos, const T& value)
	{
		if (_size == Capacity)
		{
			throw std::runtime_error("Vector is at full capacity, insertion not allowed!\n");
		}
		
		std::construct_at(std::to_address(end()), std::move(*(end() - 1)));

		std::move_backward(pos, end() - 1, end());

		*pos = value;

		_size++;

		return pos;
	}

	constexpr iterator insert(const_iterator pos, const T& value)
	{
		auto mutable_pos = *reinterpret_cast<iterator*>(&pos);
		return this->insert(mutable_pos, value);
	}

	// If T is trivially_destructible, static_vector<T> should be too.
	// To get trivial destructibility, a class needs to default ~static_vector
	// Use a requires clause to select the right version (default vs destroy_n)
	//
	// Citation--https://en.cppreference.com/w/cpp/language/destructor#Trivial_destructor
	// See also
	// - https://www.sandordargo.com/blog/2021/06/16/multiple-destructors-with-cpp-concepts
	// - Sy Brand's talk "How to write well-behaved value wrappers",
	//    https://youtu.be/sQcPte0JNmE?t=1147
	//
	// Unfortunately, this won't work on clang yet
	constexpr ~static_vector() = default;
	constexpr ~static_vector() noexcept (std::is_nothrow_destructible_v<T>)
		requires(!std::is_trivially_destructible_v<T>)
	{
		std::destroy_n(begin(), _size);
	}

	static_assert(std::is_trivially_destructible_v<T>
			    ==std::is_trivially_destructible_v<T>);

	constexpr std::size_t size() const noexcept
	{
		return _size;
	}

	constexpr void resize(std::size_t new_size) noexcept (std::is_nothrow_default_constructible_v<T> && std::is_nothrow_destructible_v<T>)
	{
		if (new_size > Capacity)
			throw std::runtime_error("Can't resize beyond capacity!\n");

		if (new_size > _size)
		{
			const auto difference = new_size - _size;
			std::uninitialized_value_construct_n(begin() + difference, difference);
		}
		else
		{
			const auto difference = _size - new_size;

			if constexpr (!std::is_trivially_destructible_v<T>)
			{
				std::destroy_n(begin() + difference, difference);
			}
		}

		_size = new_size;
	}

	consteval std::size_t max_size() const noexcept
	{
		return Capacity;
	}

	consteval std::size_t capacity() const noexcept
	{
		return Capacity;
	}

	constexpr bool empty() const noexcept
	{
		return _size == 0;
	}

	constexpr std::size_t free_space() const noexcept
	{
		return Capacity - _size;
	}

	constexpr reference front() noexcept
	{
		return *reinterpret_cast<T*>(&_data[0]);
	}

	constexpr const_reference front() const noexcept
	{
		return *reinterpret_cast<const T*>(&_data[0]);
	}

	constexpr reference back() noexcept
	{
		return *reinterpret_cast<T*>(&_data[_size - 1]);
	}

	constexpr const_reference back() const noexcept
	{
		return *reinterpret_cast<const T*>(&_data[_size - 1]);
	}

	constexpr pointer data() noexcept
	{
		return reinterpret_cast<T*>(&_data[0]);
	}

	constexpr const_pointer data() const noexcept
	{
		return reinterpret_cast<const T*>(&_data[0]);
	}
};
