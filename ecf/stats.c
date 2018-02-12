extern int parse_stats(char **argv);

// evaluate all the inputs of stat command
int parse_stats(char **argv){
	int i = 1;
	while(argv[i] != NULL){
		// either if-else or switch statement for all commands
		
		if (!strcmp(argv[i], "u")){
			struct rusage usage;
 			getrusage (RUSAGE_SELF, &usage);
 			printf ("CPU time: %ld.%06ld sec user\n", usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
		}

		if (!strcmp(argv[i], "s")){
			struct rusage usage;
 			getrusage (RUSAGE_SELF, &usage);
			printf("CPU time: %ld.%06ld sec system\n", usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
		}

		i++;	
	}
	return(0);
	// if there is no arg after stat, return "no command or something"

	// if there is one arg, check for a single or multiple characters

	// if there are multiple args, do them sequentially

	// for each arg, make sure it has the dash before it
}


/* stats to be enabled...
-u   the cpu time spent in user mode
-s   the cpu time spent in system/kernel mode
-p   the hard page faults
-v   the voluntary context switches
-i   the involuntary context switches
-a   all of the above
-l   a list of stats that are enabled
-c   to clear all the enabled stats (in this case your shell runs external commands like the default shell)
- 	 It should be possible to enter multiple flags after one dash, e.g. -us instead of -u -s, but a series of single flags should also work.
- 	 When multiple statistics are returned, they should always be printed in the same order.
*/


