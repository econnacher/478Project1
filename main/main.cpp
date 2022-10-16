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

void manualTestTwoRnS();

int main()
{
    srand(time(NULL));
    std::cout << "Hello World!\n";
    //manualTestOneStation();

    //manualTestOneReceiverOneStation();
    manualTestTwoRnS();

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
/*
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
*/

void manualTestTwoRnS() {
    std::cout << "Beginning manual test.\n";

    sender A = sender('A');
    sender C = sender('C');
    receiver B = receiver('B');
    receiver D = receiver('D');

    std::vector<int> aPackets = std::vector<int>();
    std::vector<int> cPackets = std::vector<int>();

    //Populate test packet arrival times
    for (int i = 0; i < 10; i++) {
        aPackets.push_back(i * 300);
    }
    for (int i = 0; i < 10; i++) {
        cPackets.push_back((i * 300)+1);
    }

    A.packetArrivals = aPackets;
    C.packetArrivals = cPackets;

    int currentTime;
    int channelSum = 0;

    while (!A.packetArrivals.empty() || !C.packetArrivals.empty()) {
        //first compare wheter A or C have the first packet arrival time
        currentTime = std::min(A.getNextEvent(), C.getNextEvent());

        if ((A.getNextEvent() < C.getNextEvent())) {
            if (A.nextStageTime < B.nextStageTime) {

                //We refresh the scenario that happens first, then update each channel each time we refresh a station
                channelSum = A.refreshS(currentTime, B.validMessage);
                A.channel += channelSum;
                B.channel += channelSum;
                C.channel += channelSum;
                D.channel += channelSum;

                channelSum = B.refreshR(currentTime, static_cast<int>(A.status));
                A.channel += channelSum;
                B.channel += channelSum;
                C.channel += channelSum;
                D.channel += channelSum;

                channelSum = C.refreshS(currentTime, D.validMessage);
                C.channel += channelSum;
                D.channel += channelSum;
                A.channel += channelSum;
                B.channel += channelSum;

                channelSum = D.refreshR(currentTime, static_cast<int>(C.status));
                C.channel += channelSum;
                D.channel += channelSum;
                A.channel += channelSum;
                B.channel += channelSum;
                std::cout << "\t Channel values: " << A.name << ": " << A.channel << ", " << B.name << ": " << B.channel << ", " << C.name << ": " << C.channel << ", " << D.name << ": " << D.channel << std::endl;
            }

            else {
                //We refresh the scenario that happens first, then update each channel each time we refresh a station
                channelSum = B.refreshR(currentTime, static_cast<int>(A.status));
                B.channel += channelSum;
                A.channel += channelSum;
                C.channel += channelSum;
                D.channel += channelSum;

                channelSum = A.refreshS(currentTime, B.validMessage);
                B.channel += channelSum;
                A.channel += channelSum;
                C.channel += channelSum;
                D.channel += channelSum;

                channelSum = C.refreshS(currentTime, D.validMessage);
                C.channel += channelSum;
                D.channel += channelSum;
                A.channel += channelSum;
                B.channel += channelSum;

                channelSum = D.refreshR(currentTime, static_cast<int>(C.status));
                C.channel += channelSum;
                D.channel += channelSum;
                A.channel += channelSum;
                B.channel += channelSum;

                std::cout << "\t Channel values: " << A.name << ": " << A.channel << ", " << B.name << ": " << B.channel << ", " << C.name << ": " << C.channel << ", " << D.name << ": " << D.channel << std::endl;
            };
        }
        else {
            if (C.nextStageTime < D.nextStageTime) {

                //We refresh the scenario that happens first, then update each channel each time we refresh a station
                channelSum = C.refreshS(currentTime, D.validMessage);
                C.channel += channelSum;
                D.channel += channelSum;
                A.channel += channelSum;
                B.channel += channelSum;

                channelSum = D.refreshR(currentTime, static_cast<int>(C.status));
                C.channel += channelSum;
                D.channel += channelSum;
                A.channel += channelSum;
                B.channel += channelSum;

                channelSum = A.refreshS(currentTime, B.validMessage);
                A.channel += channelSum;
                B.channel += channelSum;
                C.channel += channelSum;
                D.channel += channelSum;

                channelSum = B.refreshR(currentTime, static_cast<int>(A.status));
                A.channel += channelSum;
                B.channel += channelSum;
                C.channel += channelSum;
                D.channel += channelSum;

                std::cout << "\t Channel values: " << A.name << ": " << A.channel << ", " << B.name << ": " << B.channel << ", " << C.name << ": " << C.channel << ", " << D.name << ": " << D.channel << std::endl;
            }

            else {
                //We refresh the scenario that happens first, then update each channel each time we refresh a station
                channelSum = D.refreshR(currentTime, static_cast<int>(C.status));
                D.channel += channelSum;
                C.channel += channelSum;
                A.channel += channelSum;
                B.channel += channelSum;

                channelSum = C.refreshS(currentTime, D.validMessage);
                D.channel += channelSum;
                C.channel += channelSum;
                A.channel += channelSum;
                B.channel += channelSum;

                channelSum = A.refreshS(currentTime, B.validMessage);
                A.channel += channelSum;
                B.channel += channelSum;
                C.channel += channelSum;
                D.channel += channelSum;

                channelSum = B.refreshR(currentTime, static_cast<int>(A.status));
                A.channel += channelSum;
                B.channel += channelSum;
                C.channel += channelSum;
                D.channel += channelSum;

                std::cout << "\t Channel values: " << A.name << ": " << A.channel << ", " << B.name << ": " << B.channel << ", " << C.name << ": " << C.channel << ", " << D.name << ": " << D.channel << std::endl;
            };
        };
    }

    std::cout << std::endl << std::endl;
    std::cout << "A Successes: " << A.successCount << std::endl;
    std::cout << "A Collisions: " << A.collisionCount << std::endl;
    std::cout << "C Successes: " << C.successCount << std::endl;        
    std::cout << "C Collisions: " << C.collisionCount << std::endl;
}

