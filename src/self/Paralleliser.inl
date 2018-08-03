#include <atomic>

namespace crystal::self
{
	std::size_t Paralleliser::hardware_concurrency()
	{
		static std::size_t concurrency = std::thread::hardware_concurrency();
		if(!concurrency)
			throw std::runtime_error("Hardware concurrency not detectable");
		return concurrency;
	}

	template<class Callable, class ...Args>
	void Paralleliser::loop_chunks(
		std::size_t begin,
		std::size_t iterations,
		std::size_t chunk_size,
		Callable loop,
		Args const& ...args)
	{
		// Ignore empty tasks.
		if(!iterations)
			return;

		// The maximum supported concurrent job count.
		std::size_t max_supported_jobs = Paralleliser::hardware_concurrency();

		// Cap chunk size to iteration count.
		if(chunk_size > iterations)
			chunk_size = iterations;

		// The needed job count.
		std::size_t jobs = chunk_size
			? iterations / chunk_size
			: max_supported_jobs;

		// Do not allocate more jobs than iterations.
		if(max_supported_jobs > iterations)
			jobs = iterations;

		// The iterations per job.
		std::size_t job_size = iterations / jobs;
		// The number of left-over iterations.
		std::size_t distribute = iterations % jobs;

		// Create the synchronisation barrier.
		Barrier barrier{jobs, false};

		// So that the new job size doesn't have to be recalculated.
		std::size_t const inc_job_size = job_size + 1;
		// Create bigger jobs with left-over iterations.
		for(std::size_t i = distribute; i--; begin += inc_job_size)
			Paralleliser::execute(
				[&loop, begin, inc_job_size, &args...]() {
					loop(
						begin,
						inc_job_size,
						args...);
				},
				&barrier,
				false);

		// Create normal-sized jobs (except the last job).
		for(std::size_t i = jobs - distribute - 1; i--; begin += job_size)
			Paralleliser::execute(
				[&loop, begin, job_size, &args...]() {
					loop(
						begin,
						job_size,
						args...);
				},
				&barrier,
				false);

		// Execute the last job on the main thread.
		loop(
			begin,
			job_size,
			args...);

		// Notify the barrier (this is needed for synchronising() barriers).
		barrier.notify();

		// Wait for execution to finish.
		barrier.wait();
	}

	template<class Callable, class ...Args>
	void Paralleliser::loop(
		std::size_t begin,
		std::size_t iterations,
		std::size_t chunk_size,
		Callable iteration,
		Args const& ...args)
	{
		Paralleliser::loop_chunks(
			begin,
			iterations,
			chunk_size,
			[&iteration, &args...](std::size_t begin, std::size_t count)
			{
				for(std::size_t i = begin + count; i-->begin;)
					iteration(i, args...);
			},
			args...);
	}

	template<class Loop, class Reduce, class ...Args>
	loop_chunks_t<Loop, Args...> Paralleliser::reduce_loop_chunks(
		std::size_t begin,
		std::size_t iterations,
		std::size_t chunk_size,
		Loop loop,
		loop_chunks_t<Loop, Args...> accum_init,
		Reduce reduce,
		Args const&... args)
	{
		Paralleliser::loop_chunks(
			begin,
			iterations,
			chunk_size,
			[&loop, &reduce, &accum_init, &args...](
				std::size_t begin,
				std::size_t iterations)
			{
				reduce(accum_init, loop(begin, iterations, args...));
			});

		return std::move(accum_init);
	}

	template<class Loop, class Reduce, class ...Args>
	loop_t<Loop, Args...> Paralleliser::reduce_loop(
		std::size_t begin,
		std::size_t iterations,
		std::size_t chunk_size,
		Loop loop,
		loop_t<Loop, Args...> accum_init,
		Reduce reduce,
		Args const&... args)
	{
		Paralleliser::loop(
			begin,
			iterations,
			chunk_size,
			[&loop, &accum_init, &reduce, &args...](
				std::size_t i)
			{
				reduce(accum_init, loop(i, args...));
			});

		return std::move(accum_init);
	}

	template<class Loop, class Reduce, class ...Args>
	loop_chunks_t<Loop, Args...> Paralleliser::locked_reduce_loop_chunks(
			std::size_t begin,
			std::size_t iterations,
			std::size_t chunk_size,
			Loop loop,
			loop_chunks_t<Loop, Args...> accum_init,
			Reduce reduce,
			Args const&... args)
	{
		std::mutex mutex;
		return Paralleliser::reduce_loop_chunks(
			begin,
			iterations,
			chunk_size,
			loop,
			accum_init,
			[&mutex, &reduce](auto &a, auto b)
			{
				std::lock_guard<std::mutex> lock(mutex);
				reduce(a,b);
			},
			args...);
	}

	template<class Loop, class Reduce, class ...Args>
	loop_chunks_t<Loop, Args...> Paralleliser::atomic_reduce_loop_chunks(
		std::size_t begin,
		std::size_t iterations,
		std::size_t chunk_size,
		Loop loop,
		loop_chunks_t<Loop, Args...> accum_init,
		Reduce reduce,
		Args const&... args)
	{
		std::atomic<loop_chunks_t<Loop, Args...>> accum{accum_init};
		Paralleliser::loop_chunks(
			begin,
			iterations,
			chunk_size,
			[&loop, &reduce, &accum, &args...](
				std::size_t begin,
				std::size_t iterations)
			{
				reduce(accum, loop(begin, iterations, args...));
			});

		return accum.load(std::memory_order_relaxed);
	}

	template<class Loop, class Reduce, class ...Args>
	loop_t<Loop, Args...> Paralleliser::locked_reduce_loop(
			std::size_t begin,
			std::size_t iterations,
			std::size_t chunk_size,
			Loop loop,
			loop_t<Loop, Args...> accum_init,
			Reduce reduce,
			Args const&... args)
	{
		std::mutex mutex;
		Paralleliser::reduce_loop(
			begin,
			iterations,
			chunk_size,
			loop,
			accum_init,
			[&mutex, &reduce](auto &a, auto b)
			{
				std::lock_guard<std::mutex> lock(mutex);
				reduce(a,b);
			},
			args...);
	}

	template<class Loop, class Reduce, class ...Args>
	loop_t<Loop, Args...> Paralleliser::atomic_reduce_loop(
		std::size_t begin,
		std::size_t iterations,
		std::size_t chunk_size,
		Loop loop,
		loop_t<Loop, Args...> accum_init,
		Reduce reduce,
		Args const&... args)
	{
		std::atomic<loop_t<Loop, Args...>> accum{accum_init};
		Paralleliser::loop(
			begin,
			iterations,
			chunk_size,
			[&loop, &reduce, &accum, &args...](
				std::size_t i)
			{
				reduce(accum, loop(i, args...));
			});

		return accum.load(std::memory_order_relaxed);
	}
}