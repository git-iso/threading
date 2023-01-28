// Tristan Arana Charlebois ta18lj
//6680672

#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <signal.h>
#include <pthread.h>
#include <cmath>

volatile bool continuing; // still calculating
volatile int occupied; // threads still running
volatile double bestmin = 9999999; // current best answer
volatile double xmin; // current best x
volatile double ymin; // current best y
pthread_mutex_t lock; // thread lock

using namespace std;

void interrupted(int sig) { // Handle ctrl-c interrupt, print best answer, stop calculating

	cout<<"\nCurrent global best = "<<bestmin<< ", x = " <<xmin<< ", y = "<<ymin<<endl;
	continuing = false;

}

double egg(double x, double y){ // calculate function value

    if (x > 512 || x < -512 || y > 512 || y < -512){ // outside domain

        return 999999;


    }

    double part1 = (y + 47) * -1;
    double part2 = sin(sqrt(abs((x/2) + y + 47)));
    double part3 = x * sin(sqrt(abs(x-y-47)));
    double answer = part1 * part2 - part3;

    return answer;

}

void* busywork(void* blank){ // thread

    double currentpos = 99999999999; // thread position
    double currentx; // x position
    double currenty; // y position
    double randmodx1, randmodx2, randmodx3, randmodx4; // x value of moves
    double randmody1, randmody2, randmody3, randmody4; // y value of movdes

	while (continuing){ // until ctrl c
        
        //calculate random move in domain
        double move1 = egg(randmodx1 = -512 + ((double) rand() / RAND_MAX) * (512 + 512), randmody1 = -512 + ((double) rand() / RAND_MAX) * (512 + 512));
        double move2 = egg(randmodx2 = -512 + ((double) rand() / RAND_MAX) * (512 + 512), randmody2 = -512 + ((double) rand() / RAND_MAX) * (512 + 512));
        double move3 = egg(randmodx3 = -512 + ((double) rand() / RAND_MAX) * (512 + 512), randmody3 = -512 + ((double) rand() / RAND_MAX) * (512 + 512));
        double move4 = egg(randmodx4 = -512 + ((double) rand() / RAND_MAX) * (512 + 512), randmody4 = -512 + ((double) rand() / RAND_MAX) * (512 + 512));

        if (move1 < currentpos){ //find lowest of all moves

            currentpos = move1;
            currentx = randmodx1;
            currenty = randmody1;

        }

        if (move2 < currentpos){

            currentpos = move2;
            currentx = randmodx2;
            currenty = randmody2;

        }

        if (move3 < currentpos){

            currentpos = move3;
            currentx = randmodx3;
            currenty = randmody3;

        }

        if (move4 < currentpos){

            currentpos = move4;
            currentx = randmodx4;
            currenty = randmody4;

        }
        
        if (currentpos < bestmin){ // if lowest move is the current best move

            bestmin = currentpos; // update and print best answer
            xmin = currentx;
            ymin = currenty;
            cout<<"New lowest minimum:"<<bestmin;
            cout<<" | New lowest x value:"<<xmin;
            cout<<" | New lowest y value:"<<ymin<<endl;

        }

	}

	pthread_mutex_lock(&lock); // reserve lock
	occupied--;
	cout<<"Exiting thread."<<endl;
	pthread_mutex_unlock(&lock); // release lock

}

int main(){

	srand(time(NULL)); // RNG

	int select; // menu selection
    int climbers; // number of threads


	while (true){ // loop until quit

		continuing=true; // will calculate

		cout<<"1. Find minimum of egg function.\n0. Quit"<<endl;
		
        cin>>select;

		if (select == 0)break; // quit
		
		if (select == 1){ // calculate

			cout<<"Enter number of climbers."<<endl;
			cin>>climbers;

            if (climbers > 8){

                cout<<"Too many climbers, enter number less than 8."<<endl;

            } else {

            pthread_t ct[climbers]; // threads

			for (int i=0;i<climbers;i++) { // lock and create threads

				pthread_mutex_lock(&lock);//reserve lock
				pthread_create(&ct[i], NULL, &busywork, NULL);
				occupied++;
				pthread_mutex_unlock(&lock);//release lock

			}

			if (signal(SIGINT,interrupted)) { // ctrl c

			}
            }
		} else {

            cout<<"Invalid input, try again."<<endl;

        } 

		while (occupied>0){ // exiting calculation

			sleep(1);

		}

		cout<<"Returned to menu."<<endl;

	}

}