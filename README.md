# Crystal

> *A parallelisation and distribution library written using the STL only.*

## Contents

* Contents
* License
* Features
	* Local parallelisation
		* Thread Pool
			* Guarantees
		* Barriers
		* Parallel loops
			* Advanced parallel loops
			* Optimisation settings
			* Functions
* Planned features
	* Self-optimisation for loops
	* Distributed execution
		* Master-slave execution
		* Decentralised execution

## License

The code is released under the GNU Affero General Public License, version 3 or newer.

## Features

* Local parallelisation
	* Contains a thread pool that reuses threads.
	* Contains barriers for synchronisation.
	* Contains an easy-to-use loop parallelisation interface with high levels of control over optimisation options.

### Local parallelisation

#### Thread pool

In Crystal, the thread pool uses as many threads as execution units are available to the program (as detected via `std::thread::hardware_concurrency()`). The thread pool uses a queue for new jobs and if the queue is full, new jobs are handled by the calling thread, except if it is insisted upon that the job is executed asynchronously, in which case the calling thread waits for a worker thread to become available.

##### Guarantees

* The tasks added with forced asynchronicity will never be executed by the main thread.
* If a worker thread is available, the requested job is guaranteed to be executed asynchronously.

#### Barriers

The class `util::Barrier` represents a simple synchronisation barrier, which can be used for waiting for tasks to execute. It accepts the number of events that have to happen before letting anything through as a constructor argument.

#### Parallel loops

Parallel loops work by having the main thread post the iteration chunks as jobs to the thread pool. This implies that the main thread is utilised for job execution as well, if there is no available worker thread. The loops are blocking the caller thread until all iterations are finished.

Below is an example of the control flow of a parallel loop (`.` is idle / waiting, `-` is execution, `+` is fork, `|` is join / barrier, `<` is the start of a job and `>` is the end of a job):

	              before loop    parallel loop       after loop
	main thread   ------------+-<-------------> . |------------
	worker thread . . . . . . .<----------------->. . . . . . .
	worker thread . . . . . . . <-------------->. . . . . . . .
	worker thread . . . . . . .<----------------> . . . . . . .

##### Advanced parallel loops

If one wants to prepare computations for the next loop while the current loop is running, one can do that by using barriers and forced asynchronous execution.

```cpp

	util::Barrier next_loop{1}; // barrier waiting for 1 event.

	Paralleliser::execute(
		prepare_next_loop, // the function to execute.
		&next_loop, // notify the barrier after execution is done.
		true); // force asynchronous execution.

	Paralleliser::loop(
		0, 100, // 100 iterations, starting at 0.
		1, // chunk size.
		[](std::size_t i) { first_loop(data[i]); }); // iteration function.

	next_loop.wait(); // wait for preparations of the next loop to finish.

	// run the next loop.
	Paralleliser::loop(
		0, 100,
		0, // dynamic chunk size.
		[](std::size_t i) { do_something(prepared_data[i]); });
```

Thread diagram (aligned thread durations for readability):

	                      prepare next     first loop        next loop
	worker thread . . <-----------------><------------>. . <----------->
	main thread   ---+----+<-------------------------->| |+<----------->|--
	worker thread . . . . .<-------------------------->. . <----------->
	worker thread . . . . .<-------------------------->. . <----------->
	                                 first loop              next loop

Alternative diagram (to show the effect of the `next_loop` barrier):

	                             prepare next                  next loop
	worker thread . . <----------------------------------->. <----------->
	main thread   ---+----+<---------------->| . . . . . . |+<----------->|--
	worker thread . . . . .<---------------->. . . . . . . . <----------->
	worker thread . . . . .<---------------->. . . . . . . . <----------->
	                           first loop                      next loop

In this example, the preparation of the second loop is started asynchronously, and then the first loop is executed. In case the second loop is prepared before the first loop finishes, the used worker thread becomes available for the first loop as well. After the first loop is finished, the barrier variable `next_loop` is used for waiting until the second loop is prepared (in case it wasn't prepared yet). Then, the second loop is executed.

##### Optimisation settings

When creating a parallelised loop, one can set the chunk size (iteration count of the jobs passed to the thread pool), to create the optimal distribution of workload. The default behaviour (when passing `0`) is to divide the iteration count by the number of worker threads and have every thread execute one chunk of iterations. This has the minimum overhead of job creation and should be used for loops that have constant-length iterations. For non-constant iterations, one should choose a parameter that is small enough to eliminate the waiting time caused by longer iterations but big enough to minimise the overhead created by job initialisation. If a chunk size greater than the iteration count is passed, the whole loop is executed on one worker thread (or the caller thread, depending on whether any worker thread is available). Since the chunk size can be set separately for every parallel loop, one can optimise the program by finding the optimal parameter for every single loop.

##### Functions

One can use different sets of functions to create parallel loops:

* `*loop()`: Creates a parallel loop, where the passed function is a single iteration.
* `*loop_chunk()` Creates a parallel loop, where the passed function must execute multiple adjacent iterations.
* `*reduce_*`: Creates a parallel loop where the iterations return a value that is then reduced by an additional reduction function.
* `locked_reduce*`: The reduction function does not need to be thread safe: An exclusive lock on the accumulator variable is obtained before the reduction function is called.
* `atomic_reduce*`: The accumulator variable is an `std::atomic<T>`, where `T` is the result type of the iteration function.

The function `execute` can be used to create a job manually.

## Planned features

### Self-optimisation for loops

Every loop should have its chunk size mutated over time to optimise itself. The loop should have an execution time tracking function and a mutating function that represents the equation of the chunk size depending on the iteration count. This would ensure that after running for long enough time on a machine, the program can adapt to utilise the machine's configuration as efficiently as possible. The loop's parameters should be stored in a file at the end of the program execution and loaded from the file at the start of the program.

### Distributed execution

The programs should be able to run on multiple machines and use them efficiently. For that, cross-machine communication and synchronisation is required.

#### Master-slave execution

Crystal should support using other machines as slaves in a hierarchical distribution of workloads.

#### Decentralised execution

Crystal should also support decentralised execution functionality for scalability and error-resistance.