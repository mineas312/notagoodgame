#pragma once
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <vector>
#include <queue>
#include <stack>
#include <type_traits>
#include <utility>

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
		{
			return _underlyingContainer.size();
		}

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

	template <typename T>
	class queue
	{
	public:
		~queue()
		{
			invalidate();
		}

		bool tryPop(T& out)
		{
			std::lock_guard<std::mutex> lock{ m_mutex };
			if (m_queue.empty() || !m_valid) {
				return false;
			}
			out = std::move(m_queue.front());
			m_queue.pop();
			return true;
		}

		bool waitPop(T& out)
		{
			std::unique_lock<std::mutex> lock{ m_mutex };
			m_condition.wait(lock, [this]()
			{
				return !m_queue.empty() || !m_valid;
			});

			if (!m_valid) {
				return false;
			}
			out = std::move(m_queue.front());
			m_queue.pop();
			return true;
		}

		void push(T value)
		{
			std::lock_guard<std::mutex> lock{ m_mutex };
			m_queue.push(std::move(value));
			m_condition.notify_one();
		}

		bool empty() const
		{
			std::lock_guard<std::mutex> lock{ m_mutex };
			return m_queue.empty();
		}

		void clear()
		{
			std::lock_guard<std::mutex> lock{ m_mutex };
			while (!m_queue.empty()) {
				m_queue.pop();
			}
			m_condition.notify_all();
		}

		void invalidate()
		{
			std::lock_guard<std::mutex> lock{ m_mutex };
			m_valid = false;
			m_condition.notify_all();
		}

		bool isValid() const
		{
			std::lock_guard<std::mutex> lock{ m_mutex };
			return m_valid;
		}

	private:
		std::atomic_bool m_valid{ true };
		mutable std::mutex m_mutex;
		std::queue<T> m_queue;
		std::condition_variable m_condition;
	};
}
