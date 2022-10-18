//rts/cts sender

#include <vector>
#include <iostream>
#include <time.h> //Used for generating random numbers
#include <stdlib.h>     /* srand, rand */
#include <random>
#include <math.h>

/*
* Same as sender file but will have RTS and CTS functionality
*
*/


class rcsender {
	//Maybe we should make all of the attributes private? And keep methods as public (it is good practice in Object Oriented Design)
	//Except the constructor and methods, those should be public
public:
	enum mode { WAIT, DIFS, NAV, BACKOFF, RTS, CTS, SENDING, SIFS, ACK }; //We use this to keep track of the current station status
	//CONSTRUCTORS
	rcsender() {
		//srand(time(NULL));
		name = 'R';
		generateBackoff();
	}

	rcsender(char newName) {
		name = newName;
		//srand(time(NULL));
		generateBackoff();
	}

	rcsender(char newName, int lambda) {
		name = newName;
		//srand(time(NULL));
		generateBackoff();
		generatePacketArrivals(lambda);
	}

	rcsender(int lambda) {
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
	const int RTSTime = 2;
	const int CTSTime = 2;
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

	mode prevStatus = WAIT; // since sifs will be entered multiple times and needs to know where to go next

	//METHODS

	//When a poisson-generated packet arrives to the sender station
	void packetArrived() {
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
					nextStageTime = currentTime  + RTSTime + CTSTime + SENDINGTime + 3 * SIFSTime + ACKTime; //Have an upper limit just in case
					std::cout << currentTime << ": " << name << " has entered NAV" << std::endl;
				}
			}
			break;

		case DIFS:
			//check for channel > 0, if it is finsih difs and pause backoff go to NAV
			if (channel > 0) {
				status = NAV;
				nextStageTime = currentTime + RTSTime + CTSTime + SENDINGTime + 3 * SIFSTime + ACKTime; //Have an upper limit just in case
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
			if (currentTime == nextStageTime) { //Backoff reached 0, enter RTS
				status = RTS;
				nextStageTime = currentTime + RTSTime;
				std::cout << currentTime << ": " << name << " has started RTS" << std::endl;
				returnValue = 1;
			}

			else if (channel > 0) { //Detected a busy channel FIXME
				status = NAV; //go to NAV
				//TODO: this is currently where it ends up if other channel enters RTS, which is what we want, but nextStageTime is off, so we need to be percise w channel lowering, although im not sure how that will work when they are not all in the same domain
				nextStageTime = currentTime + RTSTime + CTSTime + SENDINGTime + 3*SIFSTime + ACKTime; //Have an upper limit just in case //if broken change nextStageTime back to currentTime
				//Need to find a way to determine when the other station is expected to finish (solved w nav?)
				std::cout << currentTime << ": " << name << " has detected a busy channel. Will enter NAV to wait till next round (backoff is " << backoffValue << " slots)" << std::endl;
			}
			else {
				std::cout << currentTime << ": " << name << " is on backoff value " << backoffValue << std::endl;
			}
			break;

		case RTS:
			prevStatus = status;
			//send rts to rsreceiver then go to sifs
			if (currentTime == nextStageTime) {
				status = SIFS;
				nextStageTime = currentTime + SIFSTime;
				//maybe flag that it is sending rts
				std::cout << currentTime << ": " << name << " has entered SIFS from RTS" << std::endl;
			}
			break;

		case CTS:
			prevStatus = status;
			//receive cts from rsreceiver then go to sifs

			if (receiverValid == false) { //Channel stopped sending OR two messages are arriving // LOOK: could change this to be an input from the function where we send in receivers value and use that to update this, that would be more accurate logic wise but i think this accomplishes the same thing
				validMessage = false;
				std::cout << currentTime << ": Station " << name << " has detected an erroneous or missing CTS message" << std::endl;
			}

			if (currentTime == nextStageTime) { //If ACK period has ended, 
				if (validMessage) { //ACK received (temporary solution)
					status = SIFS;
					nextStageTime = currentTime + SIFSTime;
					//toggle valid to send packet

					std::cout << currentTime << ": " << name << " has successfully received a CTS and entered SIFS from CTS" << std::endl;
				}
				else { //CTS not received
					collisionCount += 1;
					if (windowSize < windowMax) {
						windowSize *= 2;
					}
					std::cout << currentTime << ": " << name << " did not receive a CTS and failed to start sending a packet" << std::endl;
					std::cout << "\t" << name << " total collisions : " << collisionCount << std::endl; //Printing for debugging
					status = WAIT; //CTS round is finished and can move on to WAIT state
					generateBackoff();
					if (packetQueue > 0) {
						nextStageTime = currentTime; //If there are packets in the queue, restart the process
					}
					else {
						nextStageTime = INT_MAX; //Set nextStageTime as infinity time since there are no packets arrived yet
					}
				}
			}

			break;

		case SENDING:
			if (currentTime == nextStageTime) { //If packet has been sent move to SIFS
				prevStatus = status;
				status = SIFS;
				nextStageTime = currentTime + SIFSTime;
				std::cout << currentTime << ": " << name << " has sent a packet and started SIFS" << std::endl;
				//returnValue = -1;
			}
			break;

		case SIFS:
			if (currentTime == nextStageTime) { //If SIFS has ended
				if (prevStatus == RTS) {
					status = CTS;
					validMessage = true;
					nextStageTime = currentTime + CTSTime;
					std::cout << currentTime << ": " << name << " has started to receive CTS" << std::endl;
					returnValue = -1;
				}
				if (prevStatus == CTS) {
					status = SENDING;
					nextStageTime = currentTime + SENDINGTime;
					std::cout << currentTime << ": " << name << " has started to send packet" << std::endl;
					returnValue = 1;
				}
				if (prevStatus == SENDING) {
					status = ACK;
					validMessage = true; //Start off by assuming a message is valid
					nextStageTime = currentTime + ACKTime;
					returnValue = -1; //need to toggle in receiver
					std::cout << currentTime << ": " << name << " has started to receive an ACK" << std::endl;
				}
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


		//Make them all aadditions of each other
		for (int i = 1; i < packetArrivalsSeconds.size(); i++) {
			packetArrivalsSeconds[i] = packetArrivalsSeconds[i] + packetArrivalsSeconds[i - 1];
		}

		//Turn this into an exponential distribution with average value lambda
		for (int i = 0; i < packetArrivalsSeconds.size(); i++) {
			int number = ceil(packetArrivalsSeconds[i] / 0.000010);
			packetArrivals.push_back(number);
		}
	}
};