#include "simplelog.h"

void output_log()
{
	D_TRACE("trace");
	D_DEBUG("debug");
	D_INFO("info");
	D_NOTICE("notice");
	D_WARN("warn");
	D_ERROR("error");
	D_CRIT("crit");
}

#ifdef USE_VLL
int __g_loglevel;	// for simple log
#endif

int main()
{
	printf("==> output all level\n");
	simplelog_setlevel(LOGLEVEL_TRACE);
	output_log();

	printf("==> output none\n");
	simplelog_setlevel(LOGLEVEL_NONE);
	output_log();

	printf("==> output level <= warn\n");
	simplelog_setlevel(LOGLEVEL_WARN);
	output_log();

	return 0;
}
