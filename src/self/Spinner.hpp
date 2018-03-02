#ifndef __crystal_self_spinner_hpp_defined
#define __crystal_self_spinner_hpp_defined

#include <cstddef>

namespace crystal::self
{
	/** Manages yielding and sleeping in spin-wait loops. */
	class Spinner
	{
		/** The current fail count. */
		std::size_t m_fails;
		/** The fails needed before the spinner yields. */
		std::size_t m_yield_count;
		/** The fails needed before the spinner sleeps. */
		std::size_t m_sleep_count;
		/** The spinner's sleep duration, in microseconds. */
		std::size_t m_sleep_us;
	public:
		/** Creates a spin-wait loop manager.
		@param[in] yield_count:
			The amount of retries before the loop starts yielding.
		@param[in] sleep_count:
			The amount of retries before the loop starts sleeping.
		@param[in] sleep_us:
			The time duration, in microseconds, to sleep. */
		Spinner(
			std::size_t yield_count,
			std::size_t sleep_count,
			std::size_t sleep_us);

		/** Handles a spin attempt.
			If the spin failed, yields or sleeps.
		@param[in] success:
			Whether the spin succeeded.
		@return
			Whether the spin succeeded. */
		bool spin(
			bool success);

		/** The fails needed before the spinner yields. */
		inline std::size_t yield_count() const noexcept;
		/** The fails needed before the spinner sleeps. */
		inline std::size_t sleep_count() const noexcept;
		/** The spinner's sleep duration, in microseconds. */
		inline std::size_t sleep_us() const noexcept;
	};
}

#endif