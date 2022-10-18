// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//hi i changed again
#include "sender.cpp"
#include "receiver.cpp"
#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <algorithm>
#include <random>
#include <math.h>
#include <fstream>
#include "rcsender.cpp"
#include "rcreceiver.cpp"
//#include <chrono>

void manualTestOneStation();

void manualTestOneReceiverOneStation();

void manualTestTwoRnS();

void testGenerateArrivalProcess(int lambda);

void testStationGeneration();

void testTwoSendersOneReceiver();

void scenario1(std::string fileName);

void scenario2(std::string fileName);

void scenario3(std::string fileName);

void scenario4(std::string fileName);

int main()
{
    srand(time(NULL));
    std::cout << "Hello World!\n";

    scenario1("CSMA_Same_Domain.csv");
    scenario2("CSMA_Hidden_Terminal.csv");
    scenario3("CSMA_VC_Same_Domain.csv");
    scenario4("CSMA_VC_Hidden_Terminal.csv");
    //manualTestOneStation();

    //manualTestOneReceiverOneStation();

    //manualTestTwoRnS();

    //testGenerateArrivalProcess(100);

    //testStationsWithArrivals();

    //testStationGeneration();

    //testTwoSendersOneReceiver();

    //scenario1("CSMA_same_collision.csv");
    //scenario2("CSMA_hidden_terminal.csv");

}

void testGenerateArrivalProcess(int lambda) {
    std::default_random_engine generator; //generates random values
    std::uniform_real_distribution<double> distribution(0.0, 1.0); //Uniform distribution
    generator.seed(time(NULL));

    std::vector<double> uniformArrivals = std::vector<double>();

    for (int i = 0; i < (lambda * 11); i++) {
        double number = distribution(generator);
        uniformArrivals.push_back(number);
    }

    std::vector<double> packetArrivalsSeconds = std::vector<double>();

    //Turn this into an exponential distribution with average value lambda
    for (int i = 0; i < uniformArrivals.size(); i++) {
        double number = (-log(1 - uniformArrivals[i])) / lambda;
        packetArrivalsSeconds.push_back(number);
    }

    std::vector<int> packetArrivalsSlots = std::vector<int>();


    //Make them all aadditions of each other
    for (int i = 1; i < packetArrivalsSeconds.size(); i++) {
        packetArrivalsSeconds[i] = packetArrivalsSeconds[i] + packetArrivalsSeconds[i - 1];
    }

    //Turn this into an exponential distribution with average value lambda
    for (int i = 0; i < packetArrivalsSeconds.size(); i++) {
        int number = ceil(packetArrivalsSeconds[i] / 0.000010);
        packetArrivalsSlots.push_back(number);
    }

    std::cout << "Packet arrivals: " << std::endl;

    //Print them all
    for (int i = 0; i < packetArrivalsSlots.size(); i++) {
        std::cout << "Packet " << i << ": " << packetArrivalsSlots[i] << std::endl;
    }

    std::cout << std::endl << std::endl;



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
        cPackets.push_back((i * 300) + 1);
    }

    A.packetArrivals = aPackets;
    C.packetArrivals = cPackets;

    int currentTime = 0;;
    int channelSum = 0;

    while ((!A.packetArrivals.empty() || !C.packetArrivals.empty()) || ((A.packetQueue > 0) || (C.packetQueue > 0))) {
        //first compare wheter A or C have the first packet arrival time
        currentTime = std::min(A.getNextEvent(), C.getNextEvent());
        currentTime = std::min(currentTime, B.getNextEvent());
        currentTime = std::min(currentTime, D.getNextEvent());

        if ((A.nextStageTime < C.nextStageTime)) {
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
            }
        }
        else if (A.nextStageTime > C.nextStageTime){
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
            }
        }
        else {// Special scenario due to timing issues
            std::cout << "HERE" << std::endl;
            if (A.status > C.status) {
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
                }
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
                }
            }
        }
    }

    std::cout << std::endl << std::endl;
    std::cout << "A Successes: " << A.successCount << std::endl;
    std::cout << "A Collisions: " << A.collisionCount << std::endl;
    std::cout << "C Successes: " << C.successCount << std::endl;
    std::cout << "C Collisions: " << C.collisionCount << std::endl;
}


void testStationGeneration() {
    sender A = sender('A', 100);
    sender C = sender('C', 100);
}

void testTwoSendersOneReceiver() {
    std::cout << "Beginning manual test.\n";

    rcsender A = rcsender('A');
    rcsender C = rcsender('C');
    rcreceiver B = rcreceiver('B');
    //receiver D = receiver('D');

    std::vector<int> aPackets = std::vector<int>();
    std::vector<int> cPackets = std::vector<int>();

    //Populate test packet arrival times
    for (int i = 0; i < 10; i++) {
        aPackets.push_back(i * 300);
    }
    for (int i = 0; i < 10; i++) {
        cPackets.push_back((i * 300) + 1);
    }

    //aPackets.push_back(0);
    //aPackets.push_back(150);
    //cPackets.push_back(101);

    A.packetArrivals = aPackets;
    C.packetArrivals = cPackets;

    int currentTime;
    int channelSum = 0;

    while ((!A.packetArrivals.empty() || !C.packetArrivals.empty()) || ((A.packetQueue > 0) || (C.packetQueue > 0))) {
        //first compare wheter A or C have the first packet arrival time
        currentTime = std::min(A.getNextEvent(), C.getNextEvent());
        currentTime = std::min(currentTime, B.getNextEvent());
        //currentTime = std::min(currentTime, D.getNextEvent());

        if ((A.nextStageTime < C.nextStageTime)) {
            if (A.nextStageTime < B.nextStageTime) {

                //We refresh the scenario that happens first, then update each channel each time we refresh a station
                channelSum = A.refreshS(currentTime, B.validMessage);
                A.channel += channelSum;
                B.channel += channelSum;
                //C.channel += channelSum;
                //D.channel += channelSum;

                channelSum = B.refreshR(currentTime, static_cast<int>(A.status));
                A.channel += channelSum;
                B.channel += channelSum;
                C.channel += channelSum;
                //D.channel += channelSum;

                channelSum = C.refreshS(currentTime, B.validMessage);
                C.channel += channelSum;
                //D.channel += channelSum;
                //A.channel += channelSum;
                B.channel += channelSum;

                /*channelSum = D.refreshR(currentTime, static_cast<int>(C.status));
                C.channel += channelSum;
                D.channel += channelSum;
                A.channel += channelSum;
                B.channel += channelSum;*/
                std::cout << "\t Channel values: " << A.name << ": " << A.channel << ", " << B.name << ": " << B.channel << ", " << C.name << ": " << C.channel << std::endl;
            }

            else {
                //We refresh the scenario that happens first, then update each channel each time we refresh a station
                channelSum = B.refreshR(currentTime, static_cast<int>(A.status));
                B.channel += channelSum;
                A.channel += channelSum;
                C.channel += channelSum;
                //D.channel += channelSum;

                channelSum = A.refreshS(currentTime, B.validMessage);
                B.channel += channelSum;
                A.channel += channelSum;
                //C.channel += channelSum;
                //D.channel += channelSum;

                channelSum = C.refreshS(currentTime, B.validMessage);
                C.channel += channelSum;
                //D.channel += channelSum;
                //A.channel += channelSum;
                B.channel += channelSum;

                /*channelSum = D.refreshR(currentTime, static_cast<int>(C.status));
                C.channel += channelSum;
                D.channel += channelSum;
                A.channel += channelSum;
                B.channel += channelSum;*/

                std::cout << "\t Channel values: " << A.name << ": " << A.channel << ", " << B.name << ": " << B.channel << ", " << C.name << ": " << C.channel << std::endl;
            };
        }
        else {
            if (C.nextStageTime < B.nextStageTime) {

                //We refresh the scenario that happens first, then update each channel each time we refresh a station
                channelSum = C.refreshS(currentTime, B.validMessage);
                C.channel += channelSum;
                //D.channel += channelSum;
                //A.channel += channelSum;
                B.channel += channelSum;

                channelSum = B.refreshR(currentTime, static_cast<int>(C.status));
                C.channel += channelSum;
                //D.channel += channelSum;
                B.channel += channelSum;
                A.channel += channelSum;


                channelSum = A.refreshS(currentTime, B.validMessage);
                A.channel += channelSum;
                B.channel += channelSum;
                //C.channel += channelSum;
                //D.channel += channelSum;

               /* channelSum = D.refreshR(currentTime, static_cast<int>(A.status));
                A.channel += channelSum;
                B.channel += channelSum;
                C.channel += channelSum;
                D.channel += channelSum;*/

                std::cout << "\t Channel values: " << A.name << ": " << A.channel << ", " << B.name << ": " << B.channel << ", " << C.name << ": " << C.channel << std::endl;
            }

            else {
                //We refresh the scenario that happens first, then update each channel each time we refresh a station
                channelSum = B.refreshR(currentTime, static_cast<int>(C.status));
                //D.channel += channelSum;
                C.channel += channelSum;
                A.channel += channelSum;
                B.channel += channelSum;

                channelSum = C.refreshS(currentTime, B.validMessage);
                //D.channel += channelSum;
                C.channel += channelSum;
                //A.channel += channelSum;
                B.channel += channelSum;

                channelSum = A.refreshS(currentTime, B.validMessage);
                A.channel += channelSum;
                B.channel += channelSum;
                //C.channel += channelSum;
                //D.channel += channelSum;

                /*channelSum = D.refreshR(currentTime, static_cast<int>(A.status));
                A.channel += channelSum;
                B.channel += channelSum;
                C.channel += channelSum;
                D.channel += channelSum;*/

                std::cout << "\t Channel values: " << A.name << ": " << A.channel << ", " << B.name << ": " << B.channel << ", " << C.name << ": " << C.channel << std::endl;
            };
        };
    }

    std::cout << std::endl << std::endl;
    std::cout << "A Successes: " << A.successCount << std::endl;
    std::cout << "A Collisions: " << A.collisionCount << std::endl;
    std::cout << "C Successes: " << C.successCount << std::endl;
    std::cout << "C Collisions: " << C.collisionCount << std::endl;
}

void scenario1(std::string fileName) {
    int lambdaVals[6] = { 100, 200, 300, 400, 700, 1000 };

    int cutoffTime = 1000000;

    std::ofstream myFile(fileName);

    myFile << "Lambda, ";
    myFile << "Asuccess, ";
    myFile << "Csuccess, ";
    myFile << "Acollision, ";
    myFile << "Ccollision, ";
    myFile << "FI\n";

    for (int i = 0; i < 6; i++) {
        int lambda = lambdaVals[i];

        std::cout << "Beginning manual test WITH LAMBDA = " << lambda << "\n";

        sender A = sender('A', lambda);
        sender C = sender('C', lambda);
        receiver B = receiver('B');
        receiver D = receiver('D');

        std::vector<int> aPackets = std::vector<int>();
        std::vector<int> cPackets = std::vector<int>();


        int currentTime = 0;
        int channelSum = 0;

        while (currentTime < cutoffTime) {
            //first compare wheter A or C have the first packet arrival time
            currentTime = std::min(A.getNextEvent(), C.getNextEvent());
            currentTime = std::min(currentTime, B.getNextEvent());
            currentTime = std::min(currentTime, D.getNextEvent());

            if ((A.nextStageTime < C.nextStageTime)) {
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
                }
            }
            else if (A.nextStageTime > C.nextStageTime) {
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
                }
            }
            else {// Special scenario due to timing issues
                //std::cout << "HERE" << std::endl;
                if (A.status > C.status) {
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
                    }
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
                    }
                }
            }
        };

        std::cout << std::endl << std::endl;
        std::cout << "A Successes: " << A.successCount << std::endl;
        std::cout << "A Collisions: " << A.collisionCount << std::endl;
        std::cout << "C Successes: " << C.successCount << std::endl;
        std::cout << "C Collisions: " << C.collisionCount << std::endl;


        myFile << lambda << ", ";
        myFile << A.successCount << ", ";
        myFile << C.successCount << ", ";
        myFile << A.collisionCount << ", ";
        myFile << C.collisionCount << ", ";
        myFile << (double((A.successCount + A.collisionCount)) / (C.successCount + C.collisionCount)) << "\n";


    }

    myFile.close();

}

void scenario2(std::string fileName) {
    int lambdaVals[6] = { 100, 200, 300, 400, 700, 1000 };

    int cutoffTime = 1000000;

    std::ofstream myFile(fileName);

    myFile << "Lambda, ";
    myFile << "Asuccess, ";
    myFile << "Csuccess, ";
    myFile << "Acollision, ";
    myFile << "Ccollision, ";
    myFile << "FI\n";

    for (int i = 0; i < 6; i++) {
        int lambda = lambdaVals[i];

        std::cout << "Beginning manual test WITH LAMBDA = " << lambda << "\n";

        sender A = sender('A', lambda);
        sender C = sender('C', lambda);
        receiver B = receiver('B');
        //receiver D = receiver('D');

        //std::vector<int> aPackets = std::vector<int>();
        //std::vector<int> cPackets = std::vector<int>();

        //Populate test packet arrival times
        /*for (int i = 0; i < 10; i++) {
            aPackets.push_back(i * 300);
        }
        for (int i = 0; i < 10; i++) {
            cPackets.push_back((i * 300) + 1);
        }*/


        //A.packetArrivals = aPackets;
        //C.packetArrivals = cPackets;

        int currentTime = 0;
        int channelSum = 0;

        while (currentTime < cutoffTime) {
            //first compare wheter A or C have the first packet arrival time
            currentTime = std::min(A.getNextEvent(), C.getNextEvent());
            currentTime = std::min(currentTime, B.getNextEvent());
            //currentTime = std::min(currentTime, D.getNextEvent());

            if ((A.nextStageTime < C.nextStageTime)) {
                if (A.nextStageTime < B.nextStageTime) {

                    //We refresh the scenario that happens first, then update each channel each time we refresh a station
                    channelSum = A.refreshS(currentTime, B.validMessage);
                    A.channel += channelSum;
                    B.channel += channelSum;
                    //C.channel += channelSum;
                    //D.channel += channelSum;

                    channelSum = B.refreshR(currentTime, static_cast<int>(A.status));
                    A.channel += channelSum;
                    B.channel += channelSum;
                    C.channel += channelSum;
                    //D.channel += channelSum;

                    channelSum = C.refreshS(currentTime, B.validMessage);
                    C.channel += channelSum;
                    //D.channel += channelSum;
                    //A.channel += channelSum;
                    B.channel += channelSum;

                    /*channelSum = D.refreshR(currentTime, static_cast<int>(C.status));
                    C.channel += channelSum;
                    D.channel += channelSum;
                    A.channel += channelSum;
                    B.channel += channelSum;*/
                    std::cout << "\t Channel values: " << A.name << ": " << A.channel << ", " << B.name << ": " << B.channel << ", " << C.name << ": " << C.channel << std::endl;
                }

                else {
                    //We refresh the scenario that happens first, then update each channel each time we refresh a station
                    channelSum = B.refreshR(currentTime, static_cast<int>(A.status));
                    B.channel += channelSum;
                    A.channel += channelSum;
                    C.channel += channelSum;
                    //D.channel += channelSum;

                    channelSum = A.refreshS(currentTime, B.validMessage);
                    B.channel += channelSum;
                    A.channel += channelSum;
                    //C.channel += channelSum;
                    //D.channel += channelSum;

                    channelSum = C.refreshS(currentTime, B.validMessage);
                    C.channel += channelSum;
                    //D.channel += channelSum;
                    //A.channel += channelSum;
                    B.channel += channelSum;

                    /*channelSum = D.refreshR(currentTime, static_cast<int>(C.status));
                    C.channel += channelSum;
                    D.channel += channelSum;
                    A.channel += channelSum;
                    B.channel += channelSum;*/

                    std::cout << "\t Channel values: " << A.name << ": " << A.channel << ", " << B.name << ": " << B.channel << ", " << C.name << ": " << C.channel << std::endl;
                };
            }
            else {
                if (C.nextStageTime < B.nextStageTime) {

                    //We refresh the scenario that happens first, then update each channel each time we refresh a station
                    channelSum = C.refreshS(currentTime, B.validMessage);
                    C.channel += channelSum;
                    //D.channel += channelSum;
                    //A.channel += channelSum;
                    B.channel += channelSum;

                    channelSum = B.refreshR(currentTime, static_cast<int>(C.status));
                    C.channel += channelSum;
                    //D.channel += channelSum;
                    B.channel += channelSum;
                    A.channel += channelSum;


                    channelSum = A.refreshS(currentTime, B.validMessage);
                    A.channel += channelSum;
                    B.channel += channelSum;
                    //C.channel += channelSum;
                    //D.channel += channelSum;

                   /* channelSum = D.refreshR(currentTime, static_cast<int>(A.status));
                    A.channel += channelSum;
                    B.channel += channelSum;
                    C.channel += channelSum;
                    D.channel += channelSum;*/

                    std::cout << "\t Channel values: " << A.name << ": " << A.channel << ", " << B.name << ": " << B.channel << ", " << C.name << ": " << C.channel << std::endl;
                }

                else {
                    //We refresh the scenario that happens first, then update each channel each time we refresh a station
                    channelSum = B.refreshR(currentTime, static_cast<int>(C.status));
                    //D.channel += channelSum;
                    C.channel += channelSum;
                    A.channel += channelSum;
                    B.channel += channelSum;

                    channelSum = C.refreshS(currentTime, B.validMessage);
                    //D.channel += channelSum;
                    C.channel += channelSum;
                    //A.channel += channelSum;
                    B.channel += channelSum;

                    channelSum = A.refreshS(currentTime, B.validMessage);
                    A.channel += channelSum;
                    B.channel += channelSum;
                    //C.channel += channelSum;
                    //D.channel += channelSum;

                    /*channelSum = D.refreshR(currentTime, static_cast<int>(A.status));
                    A.channel += channelSum;
                    B.channel += channelSum;
                    C.channel += channelSum;
                    D.channel += channelSum;*/

                    std::cout << "\t Channel values: " << A.name << ": " << A.channel << ", " << B.name << ": " << B.channel << ", " << C.name << ": " << C.channel << std::endl;
                };
            };
        }

        std::cout << std::endl << std::endl;
        std::cout << "A Successes: " << A.successCount << std::endl;
        std::cout << "A Collisions: " << A.collisionCount << std::endl;
        std::cout << "C Successes: " << C.successCount << std::endl;
        std::cout << "C Collisions: " << C.collisionCount << std::endl;

        myFile << lambda << ", ";
        myFile << A.successCount << ", ";
        myFile << C.successCount << ", ";
        myFile << A.collisionCount << ", ";
        myFile << C.collisionCount << ", ";
        myFile << (double((A.successCount + A.collisionCount)) / (C.successCount + C.collisionCount)) << "\n";
    }

    myFile.close();


}

void scenario3(std::string fileName) {
    int lambdaVals[6] = { 100, 200, 300, 400, 700, 1000 };

    int cutoffTime = 1000000;

    std::ofstream myFile(fileName);

    myFile << "Lambda, ";
    myFile << "Asuccess, ";
    myFile << "Csuccess, ";
    myFile << "Acollision, ";
    myFile << "Ccollision, ";
    myFile << "FI\n";

    for (int i = 0; i < 6; i++) {
        int lambda = lambdaVals[i];

        std::cout << "Beginning manual test WITH LAMBDA = " << lambda << "\n";

        rcsender A = rcsender('A', lambda);
        rcsender C = rcsender('C', lambda);
        rcreceiver B = rcreceiver('B');
        rcreceiver D = rcreceiver('D');

        std::vector<int> aPackets = std::vector<int>();
        std::vector<int> cPackets = std::vector<int>();


        int currentTime = 0;
        int channelSum = 0;

        while (currentTime < cutoffTime) {
            //first compare wheter A or C have the first packet arrival time
            currentTime = std::min(A.getNextEvent(), C.getNextEvent());
            currentTime = std::min(currentTime, B.getNextEvent());
            currentTime = std::min(currentTime, D.getNextEvent());

            if ((A.nextStageTime < C.nextStageTime)) {
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
                }
            }
            else if (A.nextStageTime > C.nextStageTime) {
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
                }
            }
            else {// Special scenario due to timing issues
                //std::cout << "HERE" << std::endl;
                if (A.status > C.status) {
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
                    }
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
                    }
                }
            }
        };

        std::cout << std::endl << std::endl;
        std::cout << "A Successes: " << A.successCount << std::endl;
        std::cout << "A Collisions: " << A.collisionCount << std::endl;
        std::cout << "C Successes: " << C.successCount << std::endl;
        std::cout << "C Collisions: " << C.collisionCount << std::endl;


        myFile << lambda << ", ";
        myFile << A.successCount << ", ";
        myFile << C.successCount << ", ";
        myFile << A.collisionCount << ", ";
        myFile << C.collisionCount << ", ";
        myFile << (double((A.successCount + A.collisionCount)) / (C.successCount + C.collisionCount)) << "\n";


    }

    myFile.close();

}

void scenario4(std::string fileName) {
    int lambdaVals[6] = { 100, 200, 300, 400, 700, 1000 };

    int cutoffTime = 1000000;

    std::ofstream myFile(fileName);

    myFile << "Lambda, ";
    myFile << "Asuccess, ";
    myFile << "Csuccess, ";
    myFile << "Acollision, ";
    myFile << "Ccollision, ";
    myFile << "FI\n";

    for (int i = 0; i < 6; i++) {
        int lambda = lambdaVals[i];

        std::cout << "Beginning manual test WITH LAMBDA = " << lambda << "\n";

        rcsender A = rcsender('A', lambda);
        rcsender C = rcsender('C', lambda);
        rcreceiver B = rcreceiver('B');
        //receiver D = receiver('D');

        //std::vector<int> aPackets = std::vector<int>();
        //std::vector<int> cPackets = std::vector<int>();

        //Populate test packet arrival times
        /*for (int i = 0; i < 10; i++) {
            aPackets.push_back(i * 300);
        }
        for (int i = 0; i < 10; i++) {
            cPackets.push_back((i * 300) + 1);
        }*/


        //A.packetArrivals = aPackets;
        //C.packetArrivals = cPackets;

        int currentTime = 0;
        int channelSum = 0;

        while (currentTime < cutoffTime) {
            //first compare wheter A or C have the first packet arrival time
            currentTime = std::min(A.getNextEvent(), C.getNextEvent());
            currentTime = std::min(currentTime, B.getNextEvent());
            //currentTime = std::min(currentTime, D.getNextEvent());

            if ((A.nextStageTime < C.nextStageTime)) {
                if (A.nextStageTime < B.nextStageTime) {

                    //We refresh the scenario that happens first, then update each channel each time we refresh a station
                    channelSum = A.refreshS(currentTime, B.validMessage);
                    A.channel += channelSum;
                    B.channel += channelSum;
                    //C.channel += channelSum;
                    //D.channel += channelSum;

                    channelSum = B.refreshR(currentTime, static_cast<int>(A.status));
                    A.channel += channelSum;
                    B.channel += channelSum;
                    C.channel += channelSum;
                    //D.channel += channelSum;

                    channelSum = C.refreshS(currentTime, B.validMessage);
                    C.channel += channelSum;
                    //D.channel += channelSum;
                    //A.channel += channelSum;
                    B.channel += channelSum;

                    /*channelSum = D.refreshR(currentTime, static_cast<int>(C.status));
                    C.channel += channelSum;
                    D.channel += channelSum;
                    A.channel += channelSum;
                    B.channel += channelSum;*/
                    std::cout << "\t Channel values: " << A.name << ": " << A.channel << ", " << B.name << ": " << B.channel << ", " << C.name << ": " << C.channel << std::endl;
                }

                else {
                    //We refresh the scenario that happens first, then update each channel each time we refresh a station
                    channelSum = B.refreshR(currentTime, static_cast<int>(A.status));
                    B.channel += channelSum;
                    A.channel += channelSum;
                    C.channel += channelSum;
                    //D.channel += channelSum;

                    channelSum = A.refreshS(currentTime, B.validMessage);
                    B.channel += channelSum;
                    A.channel += channelSum;
                    //C.channel += channelSum;
                    //D.channel += channelSum;

                    channelSum = C.refreshS(currentTime, B.validMessage);
                    C.channel += channelSum;
                    //D.channel += channelSum;
                    //A.channel += channelSum;
                    B.channel += channelSum;

                    /*channelSum = D.refreshR(currentTime, static_cast<int>(C.status));
                    C.channel += channelSum;
                    D.channel += channelSum;
                    A.channel += channelSum;
                    B.channel += channelSum;*/

                    std::cout << "\t Channel values: " << A.name << ": " << A.channel << ", " << B.name << ": " << B.channel << ", " << C.name << ": " << C.channel << std::endl;
                };
            }
            else {
                if (C.nextStageTime < B.nextStageTime) {

                    //We refresh the scenario that happens first, then update each channel each time we refresh a station
                    channelSum = C.refreshS(currentTime, B.validMessage);
                    C.channel += channelSum;
                    //D.channel += channelSum;
                    //A.channel += channelSum;
                    B.channel += channelSum;

                    channelSum = B.refreshR(currentTime, static_cast<int>(C.status));
                    C.channel += channelSum;
                    //D.channel += channelSum;
                    B.channel += channelSum;
                    A.channel += channelSum;


                    channelSum = A.refreshS(currentTime, B.validMessage);
                    A.channel += channelSum;
                    B.channel += channelSum;
                    //C.channel += channelSum;
                    //D.channel += channelSum;

                   /* channelSum = D.refreshR(currentTime, static_cast<int>(A.status));
                    A.channel += channelSum;
                    B.channel += channelSum;
                    C.channel += channelSum;
                    D.channel += channelSum;*/

                    std::cout << "\t Channel values: " << A.name << ": " << A.channel << ", " << B.name << ": " << B.channel << ", " << C.name << ": " << C.channel << std::endl;
                }

                else {
                    //We refresh the scenario that happens first, then update each channel each time we refresh a station
                    channelSum = B.refreshR(currentTime, static_cast<int>(C.status));
                    //D.channel += channelSum;
                    C.channel += channelSum;
                    A.channel += channelSum;
                    B.channel += channelSum;

                    channelSum = C.refreshS(currentTime, B.validMessage);
                    //D.channel += channelSum;
                    C.channel += channelSum;
                    //A.channel += channelSum;
                    B.channel += channelSum;

                    channelSum = A.refreshS(currentTime, B.validMessage);
                    A.channel += channelSum;
                    B.channel += channelSum;
                    //C.channel += channelSum;
                    //D.channel += channelSum;

                    /*channelSum = D.refreshR(currentTime, static_cast<int>(A.status));
                    A.channel += channelSum;
                    B.channel += channelSum;
                    C.channel += channelSum;
                    D.channel += channelSum;*/

                    std::cout << "\t Channel values: " << A.name << ": " << A.channel << ", " << B.name << ": " << B.channel << ", " << C.name << ": " << C.channel << std::endl;
                };
            };
        }

        std::cout << std::endl << std::endl;
        std::cout << "A Successes: " << A.successCount << std::endl;
        std::cout << "A Collisions: " << A.collisionCount << std::endl;
        std::cout << "C Successes: " << C.successCount << std::endl;
        std::cout << "C Collisions: " << C.collisionCount << std::endl;

        myFile << lambda << ", ";
        myFile << A.successCount << ", ";
        myFile << C.successCount << ", ";
        myFile << A.collisionCount << ", ";
        myFile << C.collisionCount << ", ";
        myFile << (double((A.successCount + A.collisionCount)) / (C.successCount + C.collisionCount)) << "\n";
    }

    myFile.close();


}

