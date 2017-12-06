#ifndef __crystal_util_boundedqueue_hpp_defined
#define __crystal_util_boundedqueue_hpp_defined

#include <queue>

namespace crystal::util
{
	template<class T>
	/** A queue with an upper size boundary. */
	class BoundedQueue
	{
		/** The queue. */
		std::queue<T> m_queue;
		/** The size limit. */
		std::size_t m_limit;
	public:
		/** Constructs a bounded queue.
		@param[in] size:
			The queue's size limit. */
		explicit BoundedQueue(
			std::size_t limit);

		/** @return Whether the queue is full. */
		inline bool full() const;
		/** @return Whether the queue is empty. */
		inline bool empty() const;

		inline std::size_t limit() const;

		/** Reserves space in the queue.
		@param[in] count:
			The element count to reserve.
		@throws std::range_error
			If `count > limit()`. */
		inline void reserve(
			std::size_t count) const;

		template<class ...Args>
		/** Places an element at the end of the queue.
		@throws std::overflow_error
			If the queue is full. */
		inline void emplace(
			Args&& ...constructor_args);

		/** Retrieves and removes the first element in the queue.
		@throws std::underflow_error
			If the queue is empty.
		@return
			The first element of the queue. */
		inline T pop();
	};
}

#include "BoundedQueue.inl"

#endif