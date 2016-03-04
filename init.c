#include "def.h"
#include "common.h"
#include "init.h"
#include <string.h>
#include <unistd.h>
#include <signal.h>

void init()
{
	get_hostname();
	get_username();
	get_homedir();
	setup();
}

