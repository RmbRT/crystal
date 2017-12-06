#include <stdexcept>

namespace crystal::util
{
	template<class T>
	BoundedQueue<T>::BoundedQueue(
		std::size_t limit):
		m_queue(),
		m_limit(limit)
	{
	}

	template<class T>
	bool BoundedQueue<T>::full() const
	{
		return m_queue.size() == m_limit;
	}

	template<class T>
	bool BoundedQueue<T>::empty() const
	{
		return m_queue.empty();
	}

	template<class T>
	std::size_t BoundedQueue<T>::limit() const
	{
		return m_limit;
	}

	template<class T>
	void BoundedQueue<T>::reserve(
		std::size_t count) const
	{
		if(count > m_limit)
			throw std::out_of_range("reserved capacity must be <= limit().");

		m_queue.reserve(count);
	}

	template<class T>
	template<class ...Args>
	void BoundedQueue<T>::emplace(
		Args&& ...constructor_args)
	{
		if(full())
			throw std::overflow_error("BoundedQueue must not be full.");
		m_queue.emplace(
			std::forward<Args>(constructor_args)...);
	}

	template<class T>
	T BoundedQueue<T>::pop()
	{
		if(empty())
			throw std::underflow_error("BoundedQueue must not be empty.");

		T result = std::move(m_queue.front());
		m_queue.pop();
		return std::move(result);
	}
}