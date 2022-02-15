#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "processInfo.h"

int main(int argc, char **argv)
{
	struct processInfo pi;
	int pid=get_max_pid();
	get_proc_info(pid,&pi);
    exit();
}
