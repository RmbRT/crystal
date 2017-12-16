#ifndef __crystal_machine_hpp_defined
#define __crystal_machine_hpp_defined

#include "Location.hpp"
#include "util/Latency.hpp"

#include <cstddef>
#include <cinttypes>
#include <random>

namespace crystal
{
	/** A known machine. */
	class Machine
	{
		/** The machine's network location. */
		Location m_location;
	protected:
		/** The machine's communication latency. */
		util::latency_t m_communication;
		/** The machine's sequential computation latency. */
		util::latency_t m_computation_seq;
		/** The machine's parallel computation latency. */
		util::latency_t m_computation_par;
	public:
		/** Creates a machine at the given location. */
		Machine(
			Location location);
		/** Creates a machine at the given location and with the given latencies.
		@param[in] location:
			The machine's location.
		@param[in] communication:
			The machine's communication latency.
		@param[in] computation_seq:
			The machine's sequential computation latency.
		@param[in] computation_par:
			The machine's parallel computation latency. */
		Machine(
			Location location,
			util::latency_t communication,
			util::latency_t computation_seq,
			util::latency_t computation_par);

		/** Checks and updates the machine's latencies.
		@return
			Whether the check succeeded. */
		virtual bool check_latencies(
			std::uint32_t seed,
			std::size_t data,
			std::size_t seq,
			std::size_t par) = 0;

		/** The machine's location. */
		inline Location location() const;
		/** The machine's communication latency. */
		inline util::latency_t communication() const;
		/** The machine's sequential computation latency. */
		inline util::latency_t computation_seq() const;
		/** The machine's parallel computation latency. */
		inline util::latency_t computation_par() const;


		/** Estimates the machine's latency for executing a task with the given workload.
		@param[in] workload_seq:
			The sequential workload of the task.
		@param[in] workload_par:
			The parallel workload of the task.
		@param[in] data:
			The amount of data to be communicated for the task.
		@return
			The estimated time until the computation result is returned. */
		util::latency_t estimate(
			double workload_seq,
			double workload_par,
			double data) const;

		/** Benchmark algorithm for comparing sequential computation power.
			Time complexity: linear.
		@param[in] seed:
			The seed of the algorithm.
		@param[in] n:
			The benchmark size.
		@return
			The sum of the first `n` outputs of a mt19937 initialised with `seed`, minus the maximum value encountered plus the minimum value encountered. */
		static std::uint32_t benchmark_seq(
			std::uint32_t seed,
			std::size_t n);

		/** Benchmark algorithm for comparing parallel computation power.
			Performs the parallel benchmark `count` times. */
		static std::uint32_t benchmark_par(
			std::uint32_t seed,
			std::size_t n,
			std::size_t count);
	};
}

#include "Machine.inl"

#endif