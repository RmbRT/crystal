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
		using netlib::x::ConnectionListener::unlisten;

		/** Accepts a new machine into the slave pool.
			Blocks until a connection is pending.
		@return
			Whether a new machine was accepted. */
		bool accept();

		/** @return The number of slaves in the pool. */
		inline std::size_t slaves() const;

		/** Sends data to a slave.
		@param[in] slave:
			The slave's index.
		@param[in] data:
			The data to send.
		@param[in] size:
			The size of the data.
		@return
			True on success. */
		template<class T>
		inline bool send(
			std::size_t slave,
			T const& data);

		/** Receives data from a slave.
		@param[in] slave:
			The slave's index.
		@param[out] data:
			The data to receive.
		@param[in] size:
			The size of the data.
		@return
			Whether it succeeded. */
		template<class T>
		inline bool receive(
			std::size_t slave,
			T & data);

		/** Broadcasts data to all slaves.
		@param[in] data:
			The data to send to each slave.
		@param[in] count:
			The array size.
		@return
			True on success. */
		template<class T>
		bool broadcast(
			T const * data,
			std::size_t count);

		/** Receives data from all slaves.
		@param[out] data:
			The data to receive from the slaves.
			Must be an array of the size `slaves() * count`.
		@param[in] count:
			How many elements to gather from every slave.
		@return
			True on success. */
		template<class T>
		bool gather(
			T * data,
			std::size_t count);
	};
}

#include "SlavePool.inl"

#endif