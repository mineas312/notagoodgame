#pragma once
#include <mutex>
#include <atomic>
#include <type_traits>

#include <vector>
#include <queue>
#include <stack>

namespace ThreadSafe
{
	template <class T>
	class vector
	{
	public:
		// Not assignable
		vector<T>(const vector<T>&) = delete;
		vector<T>& operator=(vector<T> const&) = delete;

		vector<T>() noexcept : _underlyingContainer{ std::vector<T>(0) }
		{}

		vector<T>(const size_t _iSize) noexcept : _underlyingContainer{ std::vector<T>(_iSize) }
		{}

		~vector<T>() noexcept
		{}

		// Not thread-safe!
		void resize(const size_t size)
		{
			_underlyingContainer.resize(size);
		}

		// Not thread-safe!
		auto capacity()
		{
			return _underlyingContainer.capacity();
		}

		// Not thread-safe!
		auto size()
		{ return _underlyingContainer.size(); }

		// Blocks other operations
		T get(const size_t index)
		{
			std::lock_guard<std::mutex> lg(m_Operations);
			return _underlyingContainer.at(index);
		}

		// Blocks other operations
		void set(const size_t i, const T& item)
		{
			std::lock_guard<std::mutex> lg(m_Operations);
			_underlyingContainer.insert(i, item);
		}

		// Blocks other operations
		T& operator[] (const size_t i)
		{
			std::lock_guard<std::mutex> lg(m_Operations);
			return _underlyingContainer.at(i);
		}

		// Blocks other operations
		const T& operator[] (const size_t i) const 
		{
			std::lock_guard<std::mutex> lg(m_Operations);
			return _underlyingContainer.at(i);
		}

	private:
		std::vector<T> _underlyingContainer;

		std::mutex m_Operations;
	};

	template <class T>
	class queue
	{
	public:
		// Not assignable
		queue<T>(const queue<T>&) = delete;
		queue<T>& operator=(queue<T> const&) = delete;
	};
}
