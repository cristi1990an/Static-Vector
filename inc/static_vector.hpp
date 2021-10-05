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
class static_vector;

template<typename T, std::size_t Capacity> 
constexpr void swap(static_vector<T, Capacity>& lhs, static_vector<T, Capacity>& rhs) noexcept (std::is_nothrow_swappable_v<T> && (std::is_move_constructible_v<T> || std::is_copy_constructible_v<T>) && std::is_nothrow_destructible_v<T>);

template <typename T, size_t Capacity>
class static_vector
{
	std::aligned_storage_t<sizeof(T), alignof(T)> _data[Capacity];
	std::size_t _size = 0;

public:

	template<typename U, std::size_t Other_Size>
	friend class static_vector;

	friend constexpr void swap<>(static_vector& lhs, static_vector& rhs) noexcept (std::is_nothrow_swappable_v<T> && (std::is_move_constructible_v<T> || std::is_copy_constructible_v<T>) && std::is_nothrow_destructible_v<T>);

	struct const_iterator;
	struct iterator
	{
		using difference_type = ptrdiff_t;
		using value_type = T;
		using element_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::contiguous_iterator_tag;

		friend struct const_iterator;

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

		friend struct iterator;

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

	struct const_reverse_iterator;
	struct reverse_iterator
	{
		using difference_type = ptrdiff_t;
		using value_type = T;
		using element_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::contiguous_iterator_tag;

		friend struct const_reverse_iterator;

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

		friend struct reverse_iterator;

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

	static constexpr bool nothrow_move_constructor_requirements = (
		// If we can't move either because move throws or isn't available, the move constructor depends on the copy constructible being nothrow, since that's what's going to be called instead
	((!std::is_nothrow_move_constructible_v<T> || !std::is_move_constructible_v<T>) && std::is_nothrow_copy_constructible_v<T>) ||
	// Otherwise we simply check if T is nothrow move constructible
		(std::is_nothrow_move_constructible_v<T>));

	// Both the move constructor and the move assignment operator are nothrow
		static constexpr bool is_both_nothrow_move_constructible_and_move_assignable = std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_assignable_v<T>;

	// Both the move constructor and the move assignment operator are available
		static constexpr bool both_move_constructible_and_move_assignable = std::is_move_constructible_v<T> && std::is_move_assignable_v<T>;

	// Both the copy constructor and the copy assignment operator are nothrow
		static constexpr bool is_both_nothrow_copy_constructible_and_copy_assignable = std::is_nothrow_constructible_v<T> && std::is_nothrow_copy_assignable_v<T>;

		static constexpr bool nothrow_move_assignment_requirements =
	// All nothrow cases require the destructor to be nothrow since any surplus elements in the original array will be destructed
		std::is_nothrow_destructible_v<T> &&
	// If we can't move either because move throws or isn't available, the move assignment depends on the copy assignment/construction being nothrow, since that's what's going to be called instead.
	// Here we check for both the move constructor and the move assignment operator, since both *could be called.
		(((!is_both_nothrow_move_constructible_and_move_assignable || !both_move_constructible_and_move_assignable) && is_both_nothrow_copy_constructible_and_copy_assignable) ||
	// Otherwise we simply check if T is nothrow move constructible and assignable
		is_both_nothrow_move_constructible_and_move_assignable);

	constexpr static_vector() noexcept = default;

	constexpr static_vector(std::size_t count, const T& value) 
		requires (std::is_copy_constructible_v<T>)
	{
		if (count > Capacity)
		{
			throw std::runtime_error("Static vector lacks the capacity for so many elements!\n");
		}

		std::uninitialized_fill_n(begin(), count, value);

		_size = count;
	}

	constexpr static_vector(std::size_t count)  
		requires (std::is_default_constructible_v<T>)
	{
		if (count > Capacity)
		{
			throw std::runtime_error("Static vector lacks the capacity for so many elements!\n");
		}

		std::uninitialized_default_construct_n(begin(), count);

		_size = count;
	}

	template<typename Iterator> requires std::constructible_from<T, typename Iterator::value_type>
	constexpr static_vector(Iterator first, Iterator last) 
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
	constexpr static_vector(std::initializer_list<U> values)
	{
		const auto count = values.size();

		if (count > Capacity)
		{
			throw std::runtime_error("Static vector lacks the capacity for so many elements!\n");
		}

		std::uninitialized_copy_n(values.begin(), count, begin());

		_size = count;
	}

	constexpr static_vector(const static_vector& other) noexcept (std::is_nothrow_copy_constructible_v<T>) requires (std::is_copy_constructible_v<T>)
		: _size(other.size())
	{
		std::uninitialized_copy_n(other.cbegin(), other.size(), begin());
	}

	template<std::size_t Other_Capacity> requires (std::copy_constructible<T> && (Capacity != Other_Capacity))
	constexpr static_vector(const static_vector<T, Other_Capacity>& other) noexcept (std::is_nothrow_copy_constructible_v<T> && (Other_Capacity < Capacity))
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

	template<std::size_t Other_Capacity> requires ((std::move_constructible<T> || std::copy_constructible<T>) && (Capacity != Other_Capacity))
	constexpr static_vector(static_vector<T, Other_Capacity>&& other) noexcept (nothrow_move_constructor_requirements && Capacity > Other_Capacity)
	{
		if constexpr (Other_Capacity > Capacity)
		{
			if (other.size() > Capacity)
			{
				throw std::runtime_error("Static vector lacks the capacity to store the data of the other vector!\n");
			}
		}

		if constexpr ((std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) && std::is_move_constructible_v<T>)
		{
			std::uninitialized_move_n(other.begin(), other.size(), begin());
		}
		else
		{
			std::uninitialized_copy_n(other.begin(), other.size(), begin());
		}

		_size = other.size();

		other.clear();
	}

	constexpr static_vector(static_vector&& other) noexcept (nothrow_move_constructor_requirements) requires (std::move_constructible<T> || std::copy_constructible<T>)
	{
		if constexpr ((std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) && std::is_move_constructible_v<T>)
		{
			std::uninitialized_move_n(other.begin(), other.size(), begin());
		}
		else
		{
			std::uninitialized_copy_n(other.begin(), other.size(), begin());
		}

		_size = other.size();

		other.clear();
	}

	constexpr static_vector& operator= (const static_vector& other) noexcept (std::is_nothrow_copy_constructible_v<T>&& std::is_nothrow_copy_assignable_v<T>&& std::is_nothrow_destructible_v<T>) requires (std::is_copy_constructible_v<T>&& std::is_copy_assignable_v<T>)
	{
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

	template<std::size_t Other_Capacity> requires (std::copy_constructible<T> && std::is_copy_assignable_v<T> && (Capacity != Other_Capacity))
	constexpr static_vector& operator= (const static_vector<T, Other_Capacity>& other) noexcept (std::is_nothrow_copy_constructible_v<T> && std::is_nothrow_copy_assignable_v<T> && std::is_nothrow_destructible_v<T> && (Other_Capacity < Capacity))
	{
		if constexpr (Other_Capacity > Capacity)
		{
			if (other.size() > Capacity)
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
 
	constexpr static_vector& operator= (static_vector&& other) noexcept (nothrow_move_assignment_requirements) requires ((std::copy_constructible<T>&& std::is_copy_assignable_v<T>) || (std::move_constructible<T> && std::is_move_assignable_v<T>))
	{
		// If T isn't both move_constructible_and_move_assignable or if they aren't nothrow, we'll just do a copy
		if constexpr (!both_move_constructible_and_move_assignable || !is_both_nothrow_move_constructible_and_move_assignable)
		{
			(*this) = other; // This line does nothing?
			other.clear();
			return *this;
		}
		else
		{
			if (this == &other)
			{
				return *this;
			}

			if constexpr (std::is_trivially_move_assignable_v<T>)
			{
				std::copy_n(std::make_move_iterator(other.begin()), other.size(), begin());
			}
			else
			{
				if (_size <= other.size())
				{
					std::copy_n(std::make_move_iterator(other.begin()), _size, begin());
					std::uninitialized_move_n(other.begin() + _size, other.size() - _size, begin() + _size);
				}
				else
				{
					std::copy_n(std::make_move_iterator(other.begin()), other.size(), begin());
					if constexpr (!std::is_trivially_destructible_v<T>)
					{
						std::destroy_n(begin() + other.size(), _size - other.size());
					}
				}
			}

			_size = other.size();
			other.clear();

			return *this;
		}
	}

	template<std::size_t Other_Capacity> requires ((std::copy_constructible<T>&& std::is_copy_assignable_v<T>) || (std::move_constructible<T> && std::is_move_assignable_v<T>) && (Capacity != Other_Capacity))
	constexpr static_vector& operator= (static_vector<T, Other_Capacity>&& other) noexcept (nothrow_move_assignment_requirements && (Other_Capacity < Capacity))
	{
		// If T isn't both move_constructible_and_move_assignable or if they aren't nothrow, we'll just do a copy
		if constexpr (!both_move_constructible_and_move_assignable || !is_both_nothrow_move_constructible_and_move_assignable)
		{
			(*this) = other; // This line does nothing?
			other.clear();
			return *this;
		}
		else
		{
			if constexpr (Other_Capacity > Capacity)
			{
				if (other.size() > Capacity)
				{
					throw std::runtime_error("Static vector lacks the capacity to store the data of the other vector!\n");
				}
			}

			if constexpr (std::is_trivially_move_assignable_v<T>)
			{
				std::copy_n(std::make_move_iterator(other.begin()), other.size(), begin());
			}
			else
			{
				if (_size <= other.size())
				{
					std::copy_n(std::make_move_iterator(other.begin()), _size, begin());
					std::uninitialized_move_n(other.begin() + _size, other.size() - _size, begin() + _size);
				}
				else
				{
					std::copy_n(std::make_move_iterator(other.begin()), other.size(), begin());
					if constexpr (!std::is_trivially_destructible_v<T>)
					{
						std::destroy_n(begin() + other.size(), _size - other.size());
					}
				}
			}

			_size = other.size();
			other.clear();

			return *this;
		}
	}

	template <typename U> requires (std::constructible_from<T, U> && std::is_copy_assignable_v<T> && std::copy_constructible<T>)
	constexpr static_vector& operator=(std::initializer_list<U> values)
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

	template <typename U> requires (std::is_constructible_v<T, U> && std::is_copy_assignable_v<T>&& std::is_copy_constructible_v<T>)
	constexpr void assign(std::initializer_list<U> values)
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
	}

	constexpr void assign(std::size_t count, const T& value)
	{
		if (count > Capacity)
		{
			throw std::runtime_error("Static vector lacks the capacity for so many elements!\n");
		}

		if constexpr (std::is_trivially_copyable_v<T>)
		{
			std::fill_n(begin(), count, value);
		}
		else
		{
			if (_size <= count)
			{
				std::fill_n(begin(), count, value);
				std::uninitialized_fill_n(begin() + _size, count - _size, value);
			}
			else
			{
				std::fill_n(begin(), count, value);
				if constexpr (!std::is_trivially_copyable_v<T>)
				{
					std::destroy_n(begin() + count, _size - count);
				}
			}
		}

		_size = count;
	}

	constexpr void swap(static_vector& other) noexcept(std::is_nothrow_swappable_v<T>) requires (std::is_swappable_v<T>)
	{
		::swap(*this, other);
	}

	template <size_t Other_Capacity> requires (Capacity != Other_Capacity && std::is_swappable_v<T>)
	constexpr void swap(static_vector<T, Other_Capacity>& other)
	{
		if constexpr (Other_Capacity > Capacity)
		{
			if (other.size() > Capacity)
			{
				throw std::runtime_error("Static vector lacks the capacity for so many elements!\n");
			}
		}
		else
		{
			if (size() > Other_Capacity)
			{
				throw std::runtime_error("Static vector lacks the capacity for so many elements!\n");
			}
		}

		auto this_it = begin();
		auto other_it = other.begin();

		for (; this_it != end() && other_it != other.end(); ++this_it, ++other_it)
		{
			std::swap(*this_it, *other_it);
		}

		if (this_it != end())
		{
			if constexpr (std::is_move_constructible_v<T>)
			{
				std::uninitialized_move(this_it, end(), other.end());
			}
			else
			{
				std::uninitialized_copy(this_it, end(), other.end());
			}
			if constexpr (!std::is_trivially_destructible_v<T>)
			{
				std::destroy_n(this_it, std::distance(this_it, end() - 1));
			}
		}
		if (other_it != other.end())
		{
			if constexpr (std::is_move_constructible_v<T>)
			{
				std::uninitialized_move(other_it, other.end(), end());
			}
			else
			{
				std::uninitialized_copy(other_it, other.end(), end());
			}
			if constexpr (!std::is_trivially_destructible_v<T>)
			{
				std::destroy_n(other_it, std::distance(other_it, other.end() - 1));
			}
		}

		std::swap(_size, other._size);
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
			if constexpr (std::is_nothrow_destructible_v<T>)
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

	constexpr iterator erase(const_iterator pos) noexcept(std::is_nothrow_move_assignable_v<T>&& std::is_nothrow_destructible_v<T>)
	{
		for (iterator it = *reinterpret_cast<iterator*>(std::addressof(pos)); it != (end() - 1); ++it)
		{
			*it = std::move(*(it + 1));
		}
		std::destroy_at(std::to_address(end() - 1));

		_size--;

		return *reinterpret_cast<iterator*>(std::addressof(pos));
	}

	constexpr iterator erase(const_iterator from, const_iterator to) noexcept(std::is_nothrow_move_assignable_v<T>&& std::is_nothrow_destructible_v<T>)
	{
		iterator it_1 = *reinterpret_cast<iterator*>(std::addressof(from));
		auto to_2 = to + 1;
		iterator it_2 = *reinterpret_cast<iterator*>(std::addressof(to_2));

		while (it_2 != end())
		{
			*it_1 = std::move(*it_2);
			++it_1;
			++it_2;
		}

		if (it_1 < to)
		{
			std::destroy(it_1, *reinterpret_cast<iterator*>(std::addressof(to_2)));
		}

		_size = std::distance(begin(), it_1);

		return *reinterpret_cast<iterator*>(std::addressof(from));
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

template <typename T, std::size_t lc, std::size_t rc> requires (std::equality_comparable<T>)
constexpr bool operator==(const static_vector<T, lc>& lhs, const static_vector<T, rc> rhs) noexcept
{
	return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}

template <typename T, std::size_t lc, std::size_t rc> requires (std::equality_comparable<T>)
constexpr auto operator<=>(const static_vector<T, lc>& lhs, const static_vector<T, rc> rhs) noexcept
{
	return std::lexicographical_compare_three_way(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}

template <typename T, std::size_t LCapacity, std::size_t RCapacity> requires (LCapacity != RCapacity)
constexpr void swap(static_vector<T, LCapacity>& lhs, static_vector<T, RCapacity>& rhs)
{
	lhs.swap(rhs);
}

template <typename T, std::size_t Capacity>
constexpr void swap(static_vector<T, Capacity>& lhs, static_vector<T, Capacity>& rhs) noexcept (std::is_nothrow_swappable_v<T> && (std::is_move_constructible_v<T> || std::is_copy_constructible_v<T>) && std::is_nothrow_destructible_v<T>)
{
	if (&lhs == &rhs)
	{
		return;
	}

	auto left_it = lhs.begin();
	auto right_it = rhs.begin();

	for (; left_it != lhs.end() && right_it != rhs.end(); ++left_it, ++right_it)
	{
		std::swap(*left_it, *right_it);
	}

	if (left_it != lhs.end())
	{
		if constexpr (std::is_move_constructible_v<T>)
		{
			std::uninitialized_move(left_it, lhs.end(), rhs.end());
		}
		else
		{
			std::uninitialized_copy(left_it, lhs.end(), rhs.end());
		}
		if constexpr (!std::is_trivially_destructible_v<T>)
		{
			std::destroy_n(left_it, std::distance(left_it, lhs.end() - 1));
		}
	}
	if (right_it != rhs.end())
	{
		if constexpr (std::is_move_constructible_v<T>)
		{
			std::uninitialized_move(right_it, rhs.end(), lhs.end());
		}
		else
		{
			std::uninitialized_copy(right_it, rhs.end(), lhs.end());
		}
		if constexpr (!std::is_trivially_destructible_v<T>)
		{
			std::destroy_n(right_it, std::distance(right_it, rhs.end() - 1));
		}
	}

	std::swap(lhs._size, rhs._size);
}

namespace static_vector_static_assertions
{
	template<bool IS_NO_THROW>
	struct NO_THROW_COPYABLE
	{
		NO_THROW_COPYABLE() noexcept = default;
		NO_THROW_COPYABLE(const NO_THROW_COPYABLE&) noexcept(IS_NO_THROW) {}
		NO_THROW_COPYABLE(NO_THROW_COPYABLE&&) = delete;
		NO_THROW_COPYABLE& operator=(const NO_THROW_COPYABLE&) noexcept(IS_NO_THROW) {}
		NO_THROW_COPYABLE& operator=(NO_THROW_COPYABLE&&) = delete;
		~NO_THROW_COPYABLE() noexcept(IS_NO_THROW) {}
	};

	template<bool IS_NO_THROW>
	struct NO_THROW_MOVE
	{
		NO_THROW_MOVE() noexcept = default;
		NO_THROW_MOVE(const NO_THROW_MOVE&) noexcept {}
		NO_THROW_MOVE(NO_THROW_MOVE&&) noexcept(IS_NO_THROW) {}
		NO_THROW_MOVE& operator=(const NO_THROW_MOVE&) noexcept {}
		NO_THROW_MOVE& operator=(NO_THROW_MOVE&&) noexcept(IS_NO_THROW) {}
		~NO_THROW_MOVE() noexcept(IS_NO_THROW) {}
	};

	template<bool IS_NO_THROW>
	struct NO_THROW_COPYABLE_WITH_NO_MOVE
	{
		NO_THROW_COPYABLE_WITH_NO_MOVE() noexcept(IS_NO_THROW) = default;
		NO_THROW_COPYABLE_WITH_NO_MOVE(const NO_THROW_COPYABLE_WITH_NO_MOVE&) noexcept(IS_NO_THROW) {}
		NO_THROW_COPYABLE_WITH_NO_MOVE(NO_THROW_COPYABLE_WITH_NO_MOVE&&) = delete;
		NO_THROW_COPYABLE_WITH_NO_MOVE& operator=(const NO_THROW_COPYABLE_WITH_NO_MOVE&) noexcept(IS_NO_THROW) {}
		NO_THROW_COPYABLE_WITH_NO_MOVE& operator=(NO_THROW_COPYABLE_WITH_NO_MOVE&&) = delete;
		~NO_THROW_COPYABLE_WITH_NO_MOVE() noexcept(IS_NO_THROW) {}
	};

	static_assert(!std::is_nothrow_move_constructible_v<static_vector<NO_THROW_COPYABLE_WITH_NO_MOVE<false>, 10>>);
	static_assert(std::is_nothrow_move_constructible_v<static_vector<NO_THROW_COPYABLE_WITH_NO_MOVE<true>, 10>>);
	static_assert(!std::is_nothrow_move_assignable_v<static_vector<NO_THROW_COPYABLE_WITH_NO_MOVE<false>, 10>>);
	static_assert(std::is_nothrow_move_assignable_v<static_vector<NO_THROW_COPYABLE_WITH_NO_MOVE<true>, 10>>);
	static_assert(!std::is_nothrow_move_assignable_v<static_vector<NO_THROW_MOVE<false>, 10>>);
	static_assert(std::is_nothrow_move_assignable_v<static_vector<NO_THROW_MOVE<true>, 10>>);
	static_assert(!std::is_nothrow_move_constructible_v<static_vector<NO_THROW_MOVE<false>, 10>>);
	static_assert(std::is_nothrow_move_constructible_v<static_vector<NO_THROW_MOVE<true>, 10>>);
	static_assert(!std::is_nothrow_copy_constructible_v<static_vector<NO_THROW_COPYABLE<false>, 10>>);
	static_assert(std::is_nothrow_copy_constructible_v<static_vector<NO_THROW_COPYABLE<true>, 10>>);
	static_assert(std::is_nothrow_copy_assignable_v<static_vector<NO_THROW_COPYABLE<true>, 10>>);
	static_assert(!std::is_nothrow_copy_assignable_v<static_vector<NO_THROW_COPYABLE<false>, 10>>);
	static_assert(std::is_trivially_destructible_v<static_vector<int, 10>>);
	static_assert(!std::is_trivially_destructible_v<static_vector<std::string, 10>>);
	static_assert(std::is_nothrow_constructible_v<static_vector<int, 10>>);
	static_assert(!std::is_nothrow_constructible_v<static_vector<int, 10>, size_t>);
	static_assert(!std::is_nothrow_constructible_v<static_vector<int, 10>, size_t, int>);
	static_assert(!std::is_nothrow_constructible_v<static_vector<int, 10>, std::initializer_list<int>>);
}