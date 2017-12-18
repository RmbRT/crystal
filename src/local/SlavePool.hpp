#ifndef __crystal_local_slavepool_hpp_defined
#define __crystal_local_slavepool_hpp_defined

#include "Machine.hpp"

#include <netlib/x/ConnectionListener.hpp>
#include <vector>

namespace crystal::local
{
	/** A local area network slave pool.
		Supports basic broadcast / gather functionality. */
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
		@return
			Whether a new machine was accepted. */
		bool accept();

		/** @return The number of slaves in the pool. */
		inline std::size_t slaves() const;

		/** Retrieves a slave's byte order.
		@param[in] slave:
			The slave's index.
		@return
			The slave's byte order. */
		inline util::ByteOrder const& byte_order(
			std::size_t slave) const;

		/** Sends data to a slave.
		@param[in] slave:
			The slave's index.
		@param[in] data:
			The data to send.
		@param[in] size:
			The size of the data.
		@return
			True on success. */
		inline bool send(
			std::size_t slave,
			void const * data,
			std::size_t size);

		/** Receives data from a slave.
		@param[in] slave:
			The slave's index.
		@param[out] data:
			The data to receive.
		@param[in] size:
			The size of the data.
		@return
			Whether it succeeded. */
		inline bool receive(
			std::size_t slave,
			void * data,
			std::size_t size);

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