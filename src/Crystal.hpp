#ifndef __crystal_crystal_hpp_defined
#define __crystal_crystal_hpp_defined

namespace crystal
{
	/** Represents an algorithm that is parallelisable across multiple machines. */
	class Crystal
	{
	public:
		Crystal();

		virtual void operator()(Machine & machine);
	};
}

#endif