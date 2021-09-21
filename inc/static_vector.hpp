#pragma once

#include <memory>
#include <utility>
#include <algorithm>
#include <new>

template <typename T, size_t Capacity, typename Allocator = std::allocator<T>>
class static_vector
{
	std::aligned_storage_t<sizeof(T), alignof(T)> _data[Capacity];
	std::size_t _size = 0;

#ifdef _MSC_VER 
	[[msvc::no_unique_address]] Allocator allocator_instance;
#else
	[[no_unique_address]] Allocator allocator_instance;
#endif

public:

	struct iterator
	{
		using difference_type = ptrdiff_t;
		using value_type = T;
		using element_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::contiguous_iterator_tag;

		iterator() noexcept : ptr{} {};
		iterator(const iterator&) noexcept = default;
		iterator(iterator&&) noexcept = default;
		iterator(pointer other) noexcept : ptr{ other } {}
		iterator(bool) = delete;

		explicit operator reference() const noexcept
		{
			return *ptr;
		}

		iterator& operator=(const iterator&) = default;

		iterator& operator=(iterator&&) = default;

		iterator& operator= (const pointer other) noexcept
		{
			ptr = other;
			return (*this);
		}

		reference operator* () const noexcept
		{
			return *ptr;
		}

		pointer operator-> () const noexcept
		{
			return ptr;
		}

		iterator& operator++ () noexcept
		{
			++ptr;
			return *this;
		}

		iterator operator++(int) noexcept
		{
			return iterator(ptr++);
		}

		iterator& operator-- () noexcept
		{
			--ptr;
			return *this;
		}

		iterator operator--(int) noexcept
		{
			return iterator(ptr--);
		}

		iterator& operator+=(const ptrdiff_t offset) noexcept
		{
			ptr += offset;
			return *this;
		}

		iterator& operator-=(const ptrdiff_t offset) noexcept
		{
			ptr -= offset;
			return *this;
		}

		reference operator[](const size_t offset) const noexcept
		{
			return ptr[offset];
		}

		friend bool operator==(const iterator it_a, const iterator it_b) noexcept
		{
			return it_a.ptr == it_b.ptr;
		}

		friend bool operator!=(const iterator it_a, const iterator it_b) noexcept
		{
			return it_a.ptr != it_b.ptr;
		}

		friend iterator operator+(const iterator it, const size_t offset) noexcept
		{
			T* result = it.ptr + offset;
			return iterator(result);
		}

		friend iterator operator+(const size_t offset, const iterator& it) noexcept
		{
			auto aux = offset + it.ptr;
			return iterator(aux);
		}

		friend iterator operator-(const iterator it, const size_t offset) noexcept
		{
			T* aux = it.ptr - offset;
			return iterator(aux);
		}

		friend difference_type operator-(const iterator a, const iterator b) noexcept
		{
			return a.ptr - b.ptr;
		}

		friend auto operator<=>(const iterator a, const iterator b) noexcept
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

		const_iterator() noexcept : ptr{} {};
		const_iterator(const const_iterator&) noexcept = default;
		const_iterator(const_iterator&&) noexcept = default;
		const_iterator(pointer other) noexcept : ptr{ other } {}

		const_iterator(bool) = delete;

		const_iterator(const iterator other) noexcept : ptr{ other.ptr } {}

		explicit operator reference() const noexcept
		{
			return *ptr;
		}

		const_iterator& operator=(const const_iterator&) noexcept = default;

		const_iterator& operator=(const_iterator&&) noexcept = default;

		const_iterator& operator= (const pointer other) noexcept
		{
			ptr = other;
			return (*this);
		}

		reference operator* () const noexcept
		{
			return *ptr;
		}

		pointer operator-> () const noexcept
		{
			return ptr;
		}

		const_iterator& operator++ () noexcept
		{
			++ptr;
			return *this;
		}

		const_iterator operator++(int) noexcept
		{
			return const_iterator(ptr++);
		}

		const_iterator& operator-- () noexcept
		{
			--ptr;
			return *this;
		}

		const_iterator operator--(int) noexcept
		{
			return const_iterator(ptr--);
		}

		const_iterator& operator+=(const size_t offset) noexcept
		{
			ptr += offset;
			return *this;
		}

		const_iterator& operator-=(const size_t offset) noexcept
		{
			ptr -= offset;
			return *this;
		}

		reference operator[](const size_t offset) const noexcept
		{
			return ptr[offset];
		}

		friend bool operator==(const const_iterator it_a, const const_iterator it_b) noexcept
		{
			return it_a.ptr == it_b.ptr;
		}

		friend bool operator==(const const_iterator it, const pointer adr) noexcept
		{
			return it.ptr == adr;
		}

		friend bool operator!=(const const_iterator it_a, const const_iterator it_b) noexcept
		{
			return it_a.ptr != it_b.ptr;
		}

		friend bool operator!=(const const_iterator it, const pointer adr) noexcept
		{
			return it.ptr != adr;
		}

		friend const_iterator operator+(const const_iterator& it, const size_t offset) noexcept
		{
			return const_iterator(it.ptr + offset);
		}

		friend const_iterator operator+(const size_t offset, const const_iterator it) noexcept
		{
			return const_iterator(offset + it.ptr);
		}

		friend const_iterator operator-(const const_iterator it, const size_t offset) noexcept
		{
			return const_iterator(it.ptr - offset);
		}

		friend difference_type operator-(const const_iterator a, const const_iterator b) noexcept
		{
			return a.ptr - b.ptr;
		}

		friend auto operator<=>(const const_iterator a, const const_iterator b) noexcept
		{
			return a.ptr <=> b.ptr;
		}

	private:

		pointer ptr;
	};

	using value_type = T;
	using allocator_type = Allocator;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = std::allocator_traits<Allocator>::pointer;
	using const_pointer = std::allocator_traits<Allocator>::const_pointer;

	constexpr iterator begin() noexcept
	{
		return iterator(std::launder(reinterpret_cast<T*>(&_data[0])));
	}
	constexpr iterator end() noexcept
	{
		return iterator(std::launder(reinterpret_cast<T*>(&_data[_size])));
	}

	constexpr const_iterator cbegin() const noexcept
	{
		return const_iterator(std::launder(reinterpret_cast<T*>(&_data[0])));
	}

	constexpr const_iterator cend() const noexcept
	{
		return const_iterator(std::launder(reinterpret_cast<T*>(&_data[_size])));
	}

	constexpr static_vector() noexcept = default;

	template<size_t Other_Capacity>
	constexpr static_vector(const static_vector<T, Other_Capacity, Allocator>& other) noexcept
		requires (Other_Capacity <= Capacity)
	{
		if constexpr (std::is_trivially_copyable_v<T>)
		{
			std::copy_n(other.cbegin(), Other_Capacity, begin());
		}
		else
		{
			if (_size <= other._size)
			{
				std::copy_n(other.cbegin(), _size, begin());
				std::uninitialized_copy_n(other.cbegin() + _size, other._size - _size, begin() + _size);
				_size = other._size;
			}
			else
			{
				std::copy_n(other.cbegin(), other._size, begin());
				if constexpr (!std::is_trivially_destructible_v<T>)
				{
					std::destroy_n(begin() + other._size, _size - other._size);
				}
				_size = other._size;
			}
		}
	}

	constexpr ~static_vector() noexcept
	{
		if constexpr (!std::is_trivially_destructible_v<T>)
		{
			std::destroy_n(begin(), _size);
		}
	}

	constexpr std::size_t size() const noexcept
	{
		return _size;
	}

	constexpr void resize(std::size_t new_size)
	{
		if (new_size > Capacity)
			throw std::runtime_error("Can't resize beyond capacity!\n");

		if (new_size > _size)
		{
			const auto difference = new_size - _size;
			std::uninitialized_value_construct_n(begin() + difference, difference);
			_size = new_size;
		}
		else
		{
			const auto difference = _size - new_size;

			if constexpr (!std::is_trivially_destructible_v<T>)
			{
				std::destroy_n(begin() + difference, difference);
			}

			_size = new_size;
		}
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
};