#include "Machine.hpp"
#include <cmath>
#include <random>

#include <iostream>
#include "local/Paralleliser.hpp"

namespace crystal
{
	Machine::Machine(
		Location location,
		util::latency_t communication,
		util::latency_t computation_seq,
		util::latency_t computation_par):
		m_location(location),
		m_communication(communication),
		m_computation_seq(computation_seq),
		m_computation_par(computation_par)
	{
	}

	util::latency_t Machine::estimate(
		double workload_seq,
		double workload_par,
		double data) const
	{
		return std::fma(m_communication, data,
			std::fma(m_computation_seq, workload_seq,
				m_computation_par * workload_par));
	}

	static inline std::uint32_t benchmark_seq(
		std::mt19937 && mt,
		std::size_t n)
	{
	}


	std::uint32_t Machine::benchmark_seq(
		std::uint32_t seed,
		std::size_t n)
	{
		std::mt19937 mt{seed};

		std::uint32_t sum = 0;
		std::uint32_t min = mt.max();
		std::uint32_t max = mt.min();

		for(std::size_t i = 0; i < n; i++)
		{
			std::uint32_t const v = mt();
			if(v > max)
				max = v;
			if(v < min)
				min = v;
			sum += v;
		}

		return sum - max + min;
	}

	std::uint32_t Machine::benchmark_par(
		std::uint32_t seed,
		std::size_t n,
		std::size_t count)
	{
		std::uint32_t const base = std::mt19937(seed)();

		return local::Paralleliser::atomic_reduce_loop(
			0, count,
			0,
			[base, n](std::size_t i)->std::uint32_t {
				return Machine::benchmark_seq(base + i, n);
			},
			0, local::sum);
	}
}