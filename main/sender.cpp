#include <vector>
#include <iostream>
#include <time.h> //Used for generating random numbers
#include <stdlib.h>     /* srand, rand */
#include <random>
#include <math.h>

//This file contains class definitions for sender stations (such as A)

/*
* FIRST BAREBONES IMPLEMENTATION OF A SENDER STATION
* Essentially, the way this mechanism works is that the station
* keeps track of what state it currently is, as well as when does the next state (called stages in the code) occurs.
* 
* So the way the mechanism works is, the station keeps track of its current state in the 'status' variable.
* Whenever a packet arrives or status changes, it changes states and updates when will the pstation to something next
* In 'main', main will look at when the next event is for all packets, and basically move forward in time to that event
* 
* For example, let's say we are in the 'wait' period, and a packet arrives to the station
* The station will start DIFS and set status = DIFS
* Then, it will update its nextStageTime timer to what the current Time is + DIFS time
* at that point, it is meant to end DIFS and move on to the next stage.
* 
* So whenever it refreshes, it checks "hey, is the current time the time I go on to the next stage? (currentTime == nextStageTime)
* If it is, it knows its current state has ended, and moves on to the next stage
* 
* The way main figures when the next event for a station is, is with the getNextEvent method. (check out main). 
* We basically choose our next refresh to happen either when we change states, or when a packet arrives
* (we keep them as separate variables for reasons I can discuss in-person).
* 
* Most of the logic happens in the refresh method. For now, I have it return an int which refers to the channel usage
* If the function returns 0, then it has not added any packets to the channel.
* If it returns 1, it has tarted occupying the channel
* If it returns -1, it has stopped occupying a channel
* 
*/



class sender {
	//Maybe we should make all of the attributes private? And keep methods as public (it is good practice in Object Oriented Design)
	//Except the constructor and methods, those should be public
public:
	enum mode { WAIT, DIFS, NAV, BACKOFF, SENDING, SIFS, ACK }; //We use this to keep track of the current station status
	//CONSTRUCTORS
	sender() {
		//srand(time(NULL));
		name = 'R';
		generateBackoff();
	}

	sender(char newName) {
		name = newName;
		//srand(time(NULL));
		generateBackoff();
	}

	sender(char newName, int lambda) {
		name = newName;
		//srand(time(NULL));
		generateBackoff();
		generatePacketArrivals(lambda);
	}

	sender(int lambda) {
		generateBackoff();
		generatePacketArrivals(lambda);
	}

	//ATTRIBUTES
	
	char name;
	
	int windowSize = 4; //The window for the possible backoff values
	int backoffValue = 0; //The current backoff value
	const int windowMin = 4; //Minimum backoff window size
	const int windowMax = 1024; //Maximum backoff window size

	const int DIFSTime = 4;
	const int SENDINGTime = 100;
	const int SIFSTime = 1;
	const int ACKTime = 2;

	int channel = 0; //Channel usage, a 1 means someone is transmitting, while a 2 means its receiving 2 signals (mimicking the superposition aspect)
	int nextStageTime = INT_MAX; //Keeps track of when the next transmission stage for this channel is meant to happen (event-driven timeline)

	std::vector<int> packetArrivals = std::vector<int>(); //Keeps track of the packet arrival times (basically an array)
	int packetQueue = 0; //How many packets are waiting to be sent

	int successCount = 0; //Keep track of total successes (or ACKS received)
	int collisionCount = 0; //Keep track of total collisions (or failures)

	mode status = WAIT; //Initial status
	bool validMessage;


	//METHODS

	//When a poisson-generated packet arrives to the sender station
	void packetArrived () {
		packetQueue += 1; //Add one to the queue
		packetArrivals.erase(packetArrivals.begin()); //Deletes the packet from the list of packets to arrive
	}

	void generateBackoff() {
		backoffValue = rand() % windowSize;
		//backoffValue = 2;
	}

	int getNextEvent() {
		if (packetArrivals.size() != 0) {
			if (nextStageTime < packetArrivals[0]) {
				return nextStageTime;
			}

			else {
				return packetArrivals[0];
			}
		}
		else {
			return nextStageTime;
		}
	}

	void addToChannel(int value) {
		channel += value;
	}

	//Refresh whenever something happens
	int refreshS(int currentTime, bool receiverValid) {
		int returnValue = 0; //Holds the return value (whether it has started sending a packet, nothing changed, or a packet has been removed from channel


		//Make sure to add any packets that are supposed to have arrived already
		//FIXME: THIS SYSTEM IS FLAWED, WILL LOOK INTO FIXING IT LATER
		while (!packetArrivals.empty() && packetArrivals[0] <= currentTime) { //FIXME: That while loop may cause an error
			packetArrived();
			std::cout << currentTime << ": A packet has arrived to " << name; //Printing for debugging
			std::cout << ". Outstanding packets for " << name << ": " << packetQueue << std::endl; //Printing for debugging
		}

		//Switch statement, basically depending on the scenario we are, we can determine what the next event will be
		switch (status) {
		case WAIT:
			//nextStageTime = INT_MAX;
			if (packetQueue > 0) { //If a packet has arrived/we have packets in the queue
				status = DIFS;
				nextStageTime = currentTime + DIFSTime;
				std::cout << currentTime << ": " << name << " has started DIFS for a packet" << std::endl;
				//check for channel > 0, if it is go directly to NAV
				if (channel > 0) {
					status = NAV;
					nextStageTime = currentTime + SENDINGTime + SIFSTime + ACKTime; //Have an upper limit just in case
					std::cout << currentTime << ": " << name << " has entered NAV" << std::endl;
				}
			}
			break;

		case DIFS:
			//check for channel > 0, if it is finsih difs and pause backoff go to NAV
			if (channel > 0) {
				status = NAV;
				nextStageTime = currentTime + SENDINGTime + SIFSTime + ACKTime; //Have an upper limit just in case
				std::cout << currentTime << ": " << name << " has entered NAV" << std::endl;
			}
			else if (currentTime == nextStageTime) { //DIFS is finished, move to backoff period
				status = BACKOFF;
				nextStageTime = currentTime + backoffValue;
				std::cout << currentTime << ": " << name << " has started BACKOFF for a packet ( " << backoffValue << " slots)" << std::endl;
			}
			break;

		case NAV:
			if (channel == 0 || currentTime == nextStageTime) {
				status = DIFS;
				nextStageTime = currentTime + DIFSTime;
				std::cout << currentTime << ": " << name << " has entered DIFS for a packet after leaving NAV" << std::endl;
			}
			else if (channel == 1) {
				std::cout << currentTime << ": " << name << " will stay in NAV" << std::endl;
			}
			else {
				std::cout << currentTime << ": " << name << "ERROR: Colision occured but shouldn't be caught here" << std::endl;
			}
			break;

		case BACKOFF:
			backoffValue = nextStageTime - currentTime; //Keep refreshing the current backoff counter
			if (currentTime == nextStageTime) { //Backoff reached 0, send packet
				status = SENDING;
				nextStageTime = currentTime + SENDINGTime;
				std::cout << currentTime << ": " << name << " has started SENDING a packet" << std::endl;
				returnValue = 1;
				//channel++; //temp so we can test
			}

			else if (channel > 0) { //Detected a busy channel FIXME
				status = NAV; //go to NAV
				nextStageTime = currentTime + SENDINGTime + SIFSTime + ACKTime; //Have an upper limit just in case
				//Need to find a way to determine when the other station is expected to finish (solved w nav?)
				std::cout << currentTime << ": " << name << " has detected a busy channel. Will wait till next round (backoff is " << backoffValue << " slots)" << std::endl;
			}
			else {
				std::cout << currentTime << ": " << name << " is on backoff value " << backoffValue << std::endl;
			}
			break;

		case SENDING:
			if (currentTime == nextStageTime) { //If packet has been sent move to SIFS
				status = SIFS;
				nextStageTime = currentTime + SIFSTime;
				std::cout << currentTime << ": " << name << " has sent a packet and started SIFS" << std::endl;
			}
			break;

		case SIFS:
			if (currentTime == nextStageTime) { //If SIFS has ended
				status = ACK;
				validMessage = true; //Start off by assuming a message is valid
				nextStageTime = currentTime + ACKTime;
				returnValue = -1; //After SIFS, channel is free (but the receiver will start occupying it later)
				std::cout << currentTime << ": " << name << " has started to receive an ACK" << std::endl;
				/*if (channel == 1) { //ACK received (temporary solution)
					successCount += 1;
					windowSize = windowMin;
					packetQueue--;
					std::cout << currentTime << ": " << name << " has started to received an ACK and successfully delivered a packet" << std::endl;
					std::cout << "\tNew total successes: " << successCount << std::endl; //Printing for debugging
				}
				else { //ACK not received
					collisionCount += 1;
					if (windowSize < windowMax) {
						windowSize *= 2;
					}
					std::cout << currentTime << ": " << name << " did not receive an ACK and failed to deliver a packet" << std::endl;
					std::cout << "\tNew total collisions: " << collisionCount << std::endl; //Printing for debugging
				}
				generateBackoff();
				returnValue = -1; //After SIFS, channel is free (but the receiver will start occupying it later)*/
			}

			//Ensure message is still valid DURING
			break;

		case ACK:
			if (receiverValid == false) { //Channel stopped sending OR two messages are arriving // LOOK: could change this to be an input from the function where we send in receivers value and use that to update this, that would be more accurate logic wise but i think this accomplishes the same thing
				validMessage = false;
				std::cout << currentTime << ": Station " << name << " has detected an erroneous or missing ACK message" << std::endl;
			}
			if (currentTime == nextStageTime) { //If ACK period has ended, 
				if (validMessage) { //ACK received (temporary solution)
					successCount += 1;
					windowSize = windowMin;
					packetQueue--;
					
					std::cout << currentTime << ": " << name << " has received an ACK and successfully delivered a packet" << std::endl;
					std::cout << "\t" << name << " total successes : " << successCount << std::endl; //Printing for debugging
				}
				else { //ACK not received
					collisionCount += 1;
					if (windowSize < windowMax) {
						windowSize *= 2;
					}
					std::cout << currentTime << ": " << name << " did not receive an ACK and failed to deliver a packet" << std::endl;
					std::cout << "\t" << name << " total collisions : " << collisionCount << std::endl; //Printing for debugging
				}
				generateBackoff();
				
				//returnValue = -1;
				//channel = 0; //temp to see how it works

				status = WAIT; //ACK round is finished and can move on to WAIT state
				if (packetQueue > 0) {
					nextStageTime = currentTime; //If there are packets in the queue, restart the process
				}
				else {
					nextStageTime = INT_MAX; //Set nextStageTime as infinity time since there are no packets arrived yet
				}
				break;
			}

			

			std::cout << currentTime << ": " << name << " has finished a round with " << packetQueue << " packets in queue." << std::endl;
		}

		return returnValue;
	}

	void generatePacketArrivals(int lambda) {
		std::default_random_engine generator; //generates random values
		std::uniform_real_distribution<double> distribution(0.0, 1.0); //Uniform distribution
		generator.seed(rand());

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


		//Make them all aadditions of each other
		for (int i = 1; i < packetArrivalsSeconds.size(); i++) {
			packetArrivalsSeconds[i] = packetArrivalsSeconds[i] + packetArrivalsSeconds[i - 1];
		}

		//Turn this into an exponential distribution with average value lambda
		for (int i = 0; i < packetArrivalsSeconds.size(); i++) {
			int number = ceil(packetArrivalsSeconds[i] / 0.000010);
			packetArrivals.push_back(number);
		}

		/*
		std::cout << "Packet arrivals: " << std::endl;

		//Print them all
		for (int i = 0; i < packetArrivals.size(); i++) {
			std::cout << "Packet " << i << ": " << packetArrivals[i] << std::endl;
		}

		std::cout << std::endl << std::endl;
		*/
	}
	

};