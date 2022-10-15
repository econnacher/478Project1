// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//hi i changed again
#include "sender.cpp"
#include "receiver.cpp"
#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <algorithm>

void manualTestOneStation();

void manualTestOneReceiverOneStation();

int main()
{
    srand(time(NULL));
    std::cout << "Hello World!\n";
    //manualTestOneStation();

    manualTestOneReceiverOneStation();

}

//FUNCTION TO BE DELETED LATER
//This test basically tests a sender station with pre-determined packet arrival times
//As for whether a packet transmission is successful, is a 66% chance  in this made-up scneario.
void manualTestOneStation() {
    /*
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
    */
}

void manualTestOneReceiverOneStation() {
    std::cout << "Beginning first manual test.\n";

    sender A = sender('A');
    receiver B = receiver('B');

    std::vector<int> packets = std::vector<int>();

    //Populate test packet arrival times
    for (int i = 0; i < 10; i++) {
        packets.push_back(i * 300);
    }

    A.packetArrivals = packets;

    int currentTime;

    while (!A.packetArrivals.empty()) {
        int channelSum = 0;

        
        currentTime = std::min(A.getNextEvent(), B.getNextEvent());

        if (A.nextStageTime < B.nextStageTime) {

            //We refresh the scenario that happens first, then update each channel each time we refresh a station
            channelSum = A.refresh(currentTime);
            A.channel += channelSum;
            B.channel += channelSum;

            channelSum = B.refresh(currentTime);
            A.channel += channelSum;
            B.channel += channelSum;
            std::cout << "\t Channel values: " << A.name << ": " << A.channel << ", " << B.name << ": " << B.channel << std::endl;
        }

        else {
            //We refresh the scenario that happens first, then update each channel each time we refresh a station
            channelSum = B.refresh(currentTime);
            B.channel += channelSum;
            A.channel += channelSum;
       
            channelSum = A.refresh(currentTime);
            B.channel += channelSum;
            A.channel += channelSum;

            std::cout << "\t Channel values: " << A.name << ": " << A.channel << ", " << B.name << ": " << B.channel << std::endl;
        };

    }

    std::cout << std::endl << std::endl;
    std::cout << "Successes: " << A.successCount << std::endl;
    std::cout << "Collisions: " << A.collisionCount << std::endl;
}

