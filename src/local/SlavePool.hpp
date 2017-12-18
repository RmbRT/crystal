#ifndef __crystal_local_slavepool_hpp_defined
#define __crystal_local_slavepool_hpp_defined

#include "Machine.hpp"

#include <netlib/x/ConnectionListener.hpp>
#include <vector>

namespace crystal::local
{
	/** A local area network slave pool. */
	class SlavePool : netlib::x::ConnectionListener
	{
		/** The slaves. */
		std::vector<Machine> m_slaves;
	public:
		using netlib::x::ConnectionListener::listening;
		using netlib::x::ConnectionListener::listen;
		using netlib::x::ConnectionListener::pending;

		/** Accepts a new machine into the slave pool.
			Blocks until a connection is pending.
		@throws std::runtime_error on failure. */
		Machine accept();

		/** @return The number of slaves in the pool. */
		inline std::size_t slaves() const;

		/** Broadcasts data to all slaves.
		@param[in] data:
			The data to send to each slave.
		@param[in] size:
			The size of the data.
		@return
			True on success. */
		bool broadcast(
			void const * data,
			std::size_t size);

		/** Receives data from all slaves.
		@param[out] data:
			The data to receive from the slaves.
			Must be of the size `slaves() * size`.
		@param[in] size:
			The size of the data to receive from each slave.
		@return
			True on success. */
		bool gather(
			void * data,
			std::size_t size);
	};
}

#include "SlavePool.inl"

#endif