#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include "SetLimits.h"

void SetLimits ()
{
	// Beginning of resource limit code
	int getrlimit(int resource, struct rlimit *rlim);
	int getrusage(int who, struct rusage *usage);
	int setrlimit(int resource, const struct rlimit *rlim);
	struct rlimit __trl1;
	const int __trl1r = getrlimit(RLIMIT_CPU, & __trl1);
	struct rlimit __trl2 = {20, __trl1.rlim_max};
	const int __trl2r = setrlimit(RLIMIT_CPU, & __trl2);
	struct rlimit __trl3;
	const int __trl3r = getrlimit(RLIMIT_FSIZE, & __trl3);
	struct rlimit __trl4 = {10000000, __trl3.rlim_max};
	const int __trl4r = setrlimit(RLIMIT_FSIZE, & __trl4);
	// End of time limit code
}
