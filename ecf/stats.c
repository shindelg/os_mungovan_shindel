extern int parse_stats(char **argv);
extern void print_stats();

int enableUT;
int enableST;
int enablePF;
int enableVCS;
int enableICS;
int enableALL;
// evaluate all the inputs of stat command

// if there is no arg after stat, return "no command or something"

// if there is one arg, check for a single or multiple characters

// if there are multiple args, do them sequentially

// for each arg, make sure it has the dash before it
int enable_stats(char **argv){
	int i = 1;
	while(argv[i] != NULL){
		

		if (!strcmp(argv[i], "u")){
			enableUT = 1;
		}
		if (!strcmp(argv[i], "s")){
 			enableST = 1;
		}
		if (!strcmp(argv[i], "p")){
			enablePF = 1;
		}
		if (!strcmp(argv[i], "v")){
			enableVCS = 1;
		}
		if (!strcmp(argv[i], "i")){
			enableICS = 1;			
		}
		// Do all of the stats
		if (!strcmp(argv[i], "a")){
			enableALL = 1;
		}
		// List enabled stats
		if (!strcmp(argv[i], "l")){
			
		}
		// clear all stats
		if (!strcmp(argv[i], "c")){
			enableUT = 0;
			enableST = 0;
			enablePF = 0;
			enableVCS = 0;
			enableICS = 0;
			enableALL = 0;
		}
		i++;	
	}
	return(0);
}

void print_stats(){
	struct rusage usage;

	if (enableUT == 1){
		getrusage (RUSAGE_SELF, &usage);
			printf ("CPU time: %ld.%06ld sec user\n", usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
	}

	if (enableST == 1){
			getrusage (RUSAGE_SELF, &usage);
		printf("CPU time: %ld.%06ld sec system\n", usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
	}
	if (enablePF == 1){
		getrusage (RUSAGE_SELF, &usage);
		printf("Hard page faults: %ld\n", usage.ru_majflt);
	}

	if (enableVCS == 1){
		getrusage (RUSAGE_SELF, &usage);
		printf("Voluntary context switches: %ld\n", usage.ru_nvcsw);
	}

	if (enableICS == 1){
		getrusage (RUSAGE_SELF, &usage);
		printf("Involuntary context switches: %ld\n", usage.ru_nivcsw);					
	}
	if (enableALL == 1){
		getrusage (RUSAGE_SELF, &usage);	
		printf ("CPU time: %ld.%06ld sec user\n", usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);	
		printf("CPU time: %ld.%06ld sec system\n", usage.ru_stime.tv_sec, usage.ru_stime.tv_usec); 		
		printf("Hard page faults: %ld\n", usage.ru_majflt);				
		printf("Voluntary context switches: %ld\n", usage.ru_nvcsw);
		printf("Involuntary context switches: %ld\n", usage.ru_nivcsw);
	}
	if (enableUT == 0 && enableST == 0 && enablePF == 0 && enableVCS == 0 && enableICS == 0 && enableALL == 0){
		printf("%s\n", "No stats are enabled.");
	}
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


