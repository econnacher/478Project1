// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//hi i changed again
#include "sender.cpp"
#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>

void manualTestOneStation();

int main()
{
    srand(time(NULL));
    std::cout << "Hello World!\n";
    manualTestOneStation();
}

//FUNCTION TO BE DELETED LATER
//This test basically tests a sender station with pre-determined packet arrival times
//As for whether a packet transmission is successful, is a 66% chance  in this made-up scneario.
void manualTestOneStation() {
    std::cout << "Beginning first manual test.\n";

    sender A = sender('A');

    std::vector<int> packets = std::vector<int>();

    //Populate test packet arrival times
    for (int i = 0; i < 10; i++) {
        packets.push_back(i * 300);
    }

    A.packetArrivals = packets;

    int currentTime;

    //While A still has packets to arrive
    while (!A.packetArrivals.empty()) {
        currentTime = A.getNextEvent();
        A.refresh(currentTime);
        if (A.status == DIFS || A.status == NAV || A.status == BACKOFF) {
            int temp = rand() % 3;
            if (temp < 1) {
                A.channel = 1;
                std::cout << "Random channel = 1" << std::endl;
            }
            else {
                A.channel = 0;
                std::cout << "Random channel = 0" << std::endl;
            }
        }
    }

    std::cout << std::endl << std::endl;
    std::cout << "Successes: " << A.successCount << std::endl;
    std::cout << "Collisions: " << A.collisionCount << std::endl;


}

