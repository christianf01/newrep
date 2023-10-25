/* The following code is added/modified by Christian Flores cdf200003, Yash Dalvi ypd170000
** This code is used to gather data for a bar graph to demonstrate
** how the lottery scheduler distributes ticks to processes.
*/
#include "types.h"
#include "stat.h"
#include "user.h"
#include "pstat.h"

// Generic function to spin for a very long time, similar to the ones used in the testcases given
void spin(){
    int i = 0, j = 0, k = 0;
    for(i = 0; i < 10000000; ++i){
        for(j = 0; j < 1000000000000; ++j){
            k = j % 10;
            k = k + 1;
        }
    }
}

int
main(int argc, char *argv[])
{
    struct pstat st;
    // Declare variable to hold the pid of each process
    int pidA;
    int pidB;
    int pidC;

    // Fork each process and set the tickets in a 3:2:1 ratio then spin for a long time
    pidA = fork();
    if(pidA == 0)
    {
        settickets(30);
        spin();
        exit();
    }
    pidB = fork();
    if(pidB == 0)
    {
        settickets(20);
        spin();
        exit();
    }
    pidC = fork();
    if(pidC == 0)
    {
        settickets(10);
        spin();
        exit();
    }
    // Make the parent sleep for a long time while the children spin
    sleep(10000);
    // Get the process info
    getpinfo(&st);
	int i;
    // Print the process info for each process
	for(i = 0; i < NPROC; i++) {
		if (st.inuse[i]) {
            if(st.pid[i] == pidA)
                printf(1, "Pid A: Tickets: %d Ticks: %d\n", st.tickets[i], st.ticks[i]);
            else if(st.pid[i] == pidB)
                printf(1, "Pid B: Tickets: %d Ticks: %d\n", st.tickets[i], st.ticks[i]);
            else if(st.pid[i] == pidC)
                printf(1, "Pid C: Tickets: %d Ticks: %d\n", st.tickets[i], st.ticks[i]);
		}
	}
    // Kill each process and wait
    kill(pidA);
    kill(pidB);
    kill(pidC);
    while (wait() > 0);
    exit();
}
/* End of code added/modified */
