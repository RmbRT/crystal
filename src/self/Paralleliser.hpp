#ifndef __crystal_self_paralleliser_hpp_defined
#define __crystal_self_paralleliser_hpp_defined

#include <thread>
#include <type_traits>
#include <condition_variable>

#include "Barrier.hpp"
#include "ExecutionUnit.hpp"
#include "Job.hpp"

namespace crystal::self
{
	template<class Fn, class ...Args>
	/** Return type of a chunk loop function. */
	using loop_chunks_t = std::invoke_result_t<Fn, std::size_t, std::size_t, Args const&...>;
	template<class Fn, class ...Args>
	/** Return type of a loop function. */
	using loop_t = std::invoke_result_t<Fn, std::size_t, Args const&...>;

	auto const sum = [](auto &acc, auto val) { acc += val; };
	auto const product = [](auto &acc, auto val) { acc *= val; };
	auto const conjunction = [](auto &acc, auto val) { if(!val) acc = false; };
	auto const disjunction = [](auto &acc, auto val) { if(val) acc = true; };

	/** Thread pool. */
	class Paralleliser
	{
		/** To efficiently check for available jobs. */
		std::atomic_size_t m_available_jobs;
		/** How many workers are currently active. */
		std::atomic_size_t m_active_workers;
		/** The job list. */
		lock::ThreadSafe<util::BoundedQueue<Job>> m_jobs;
		/** The execution units. */
		lock::ThreadSafe<std::vector<ExecutionUnit>> m_execution;

		/** The global instance. */
		static Paralleliser s_instance;
	public:
		Paralleliser();

		// Must not be moved.
		Paralleliser(Paralleliser&&) = delete;

		template<class Callable, class...Args>
		/** Parallelises a loop of known iteration count.
			Blocks until the loop was completely executed.
		@param[in] begin:
			The loop's starting iteration.
		@param[in] iterations:
			The loop's iteration count.
		@param[in] chunk_size:
			The size of chunks. If `0`, then the chunk size is as big as possible. If bigger than `iterations`, it is set to `iterations`.
		@param[in] loop:
			The iteration function. Must be thread safe. It must have the following signature: `void (std::size_t begin, std::size_t count, Args const&...args)` and it should loop over `iterations` elements, starting at `begin`.
		@param[in] args:
			The additional arguments to pass to `loop`. */
		static void loop_chunks(
			std::size_t begin,
			std::size_t iterations,
			std::size_t chunk_size,
			Callable loop,
			Args const& ...args);

		template<class Callable, class ...Args>
		/** Parallelises a loop of known iteration count.
			Blocks until the loop was completely executed.
		@param[in] iterations:
			The loop's starting iteration.
		@param[in] iterations:
			The iteration count.
		@param[in] chunk_size:
			The size of chunks. If `0`, then the chunk size is as big as possible. If bigger than `iterations`, it is set to `iterations`.
		@param[in] iteration:
			The iteration function. Must be thread safe. It must have the following signature: `void (std::size_t i, Args const&...args)` and should represent an iteration over a single element.
		@param[in] args:
			The additional arguments to pass to `loop`. */
		static void loop(
			std::size_t begin,
			std::size_t iterations,
			std::size_t chunk_size,
			Callable iteration,
			Args const& ...args);

		template<class Loop, class Reduce, class ...Args>
		/** Parallelises a loop of known iteration count and reduces the result.
			Blocks until the loop was completely executed.
		@param[in] begin:
			The loop's starting point.
		@param[in] iterations:
			The loop's iteration count.
		@param[in] chunk_size:
			The size of chunks. If `0`, then the chunk size is as big as possible. If bigger than `iterations`, it is set to `iterations`.
		@param[in] loop:
			The iteration function. Must be thread safe. It must have the following signature: `T (std::size_t begin, std::size_t count, Args const&...args)`, where `T` is the accumulator type.
		@param[in] accum_init:
			The initial accumulator value.
		@param[in] reduce:
			The reduction operation. Must be thread safe. It must have the following signature: `void (T &, T const&)`, where `T` is the accumulator type.
		@param[in] args:
			The additional arguments to pass to `loop`. */
		static loop_chunks_t<Loop, Args...> reduce_loop_chunks(
			std::size_t begin,
			std::size_t iterations,
			std::size_t chunk_size,
			Loop loop,
			loop_chunks_t<Loop, Args...> accum_init,
			Reduce reduce,
			Args const&... args);

		template<class Loop, class Reduce, class ...Args>
		/** Parallelises a loop of known iteration count and reduces the result.
			Blocks until the loop was completely executed.
		@param[in] begin:
			The loop's starting point.
		@param[in] iterations:
			The loop's iteration count.
		@param[in] chunk_size:
			The size of chunks. If `0`, then the chunk size is as big as possible. If bigger than `iterations`, it is set to `iterations`.
		@param[in] loop:
			The iteration function. Must be thread safe. It must have the following signature: `T (std::size_t i, Args const&...args)`, where `T` is the accumulator type.
		@param[in] accum_init:
			The initial accumulator value.
		@param[in] reduce:
			The reduction operation. Must be thread safe. It must have the following signature: `void (T &, T const&)`, where `T` is the accumulator type.
		@param[in] args:
			The additional arguments to pass to `loop`. */
		static loop_t<Loop, Args...> reduce_loop(
			std::size_t begin,
			std::size_t iterations,
			std::size_t chunk_size,
			Loop loop,
			loop_t<Loop, Args...> accum_init,
			Reduce reduce,
			Args const&... args);

		template<class Loop, class Reduce, class ...Args>
		/** Parallelises a loop of known iteration count and reduces the result via mutex locking.
			Blocks until the loop was completely executed.
		@param[in] begin:
			The loop's starting point.
		@param[in] iterations:
			The loop's iteration count.
		@param[in] chunk_size:
			The size of chunks. If `0`, then the chunk size is as big as possible. If bigger than `iterations`, it is set to `iterations`.
		@param[in] loop:
			The iteration function. Must be thread safe. It must have the following signature: `T (std::size_t begin, std::size_t count, Args const&...args)`, where `T` is the accumulator type.
		@param[in] accum_init:
			The initial accumulator value.
		@param[in] reduce:
			The reduction operation. Needs not to be thread safe. It must have the following signature: `void (T &, T const&)`, where `T` is the accumulator type.
		@param[in] args:
			The additional arguments to pass to `loop`. */
		static loop_chunks_t<Loop, Args...> locked_reduce_loop_chunks(
			std::size_t begin,
			std::size_t iterations,
			std::size_t chunk_size,
			Loop loop,
			loop_chunks_t<Loop, Args...> accum_init,
			Reduce reduce,
			Args const&... args);

		template<class Loop, class Reduce, class ...Args>
		/** Parallelises a loop of known iteration count and reduces the result using an atomic accumulator.
			Blocks until the loop was completely executed.
		@param[in] begin:
			The loop's starting point.
		@param[in] iterations:
			The loop's iteration count.
		@param[in] chunk_size:
			The size of chunks. If `0`, then the chunk size is as big as possible. If bigger than `iterations`, it is set to `iterations`.
		@param[in] loop:
			The iteration function. Must be thread safe. It must have the following signature: `T (std::size_t begin, std::size_t count, Args const&...args)`, where `T` is the accumulator type.
		@param[in] accum_init:
			The initial accumulator value.
		@param[in] reduce:
			The reduction operation. Needs not to be thread safe. It must have the following signature: `void (T &, T const&)`, where `T` is the accumulator type.
		@param[in] args:
			The additional arguments to pass to `loop`. */
		static loop_chunks_t<Loop, Args...> atomic_reduce_loop_chunks(
			std::size_t begin,
			std::size_t iterations,
			std::size_t chunk_size,
			Loop loop,
			loop_chunks_t<Loop, Args...> accum_init,
			Reduce reduce,
			Args const&... args);

		template<class Loop, class Reduce, class ...Args>
		/** Parallelises a loop of known iteration count and reduces the result using a mutex lock.
			Blocks until the loop was completely executed.
		@param[in] begin:
			The loop's starting point.
		@param[in] iterations:
			The loop's iteration count.
		@param[in] chunk_size:
			The size of chunks. If `0`, then the chunk size is as big as possible. If bigger than `iterations`, it is set to `iterations`.
		@param[in] loop:
			The iteration function. Must be thread safe. It must have the following signature: `T (std::size_t i, Args const&...args)`, where `T` is the accumulator type.
		@param[in] accum_init:
			The initial accumulator value.
		@param[in] reduce:
			The reduction operation. Needs not to be thread safe. It must have the following signature: `void (T &, T const&)`, where `T` is the accumulator type.
		@param[in] args:
			The additional arguments to pass to `loop`. */
		static loop_t<Loop, Args...> locked_reduce_loop(
			std::size_t begin,
			std::size_t iterations,
			std::size_t chunk_size,
			Loop loop,
			loop_t<Loop, Args...> accum_init,
			Reduce reduce,
			Args const&... args);

		template<class Loop, class Reduce, class ...Args>
		/** Parallelises a loop of known iteration count and reduces the result using an atomic accumulator.
			Blocks until the loop was completely executed.
		@param[in] begin:
			The loop's starting point.
		@param[in] iterations:
			The loop's iteration count.
		@param[in] chunk_size:
			The size of chunks. If `0`, then the chunk size is as big as possible. If bigger than `iterations`, it is set to `iterations`.
		@param[in] loop:
			The iteration function. Must be thread safe. It must have the following signature: `T (std::size_t i, Args const&...args)`, where `T` is the accumulator type.
		@param[in] accum_init:
			The initial accumulator value.
		@param[in] reduce:
			The reduction operation. Needs not to be thread safe. It must have the following signature: `void (T &, T const&)`, where `T` is the accumulator type.
		@param[in] args:
			The additional arguments to pass to `loop`. */
		static loop_t<Loop, Args...> atomic_reduce_loop(
			std::size_t begin,
			std::size_t iterations,
			std::size_t chunk_size,
			Loop loop,
			loop_t<Loop, Args...> accum_init,
			Reduce reduce,
			Args const&... args);

		/** Asynchronously executes the given job, if possible.
			This might be executed on the calling thread, if all worker threads are busy.
		@param[in] task:
			The job to execute.
		@param[in] barrier:
			The barrier to notify when the task finishes.
		@param[in] force_async:
			Whether to block the calling thread until a worker is ready. */
		static void execute(
			std::function<void (void)> task,
			Barrier * barrier = nullptr,
			bool force_async = false);

		static inline std::size_t hardware_concurrency();

		/** Stops all workers.
		@param[in] wait:
			Whether to wait until all workers finished shutting down. */
		static void stop_workers(
			bool wait);
	};
}

#include "Paralleliser.inl"

#endif