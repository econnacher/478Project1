//This is the text file for a receiver



#include <iostream>


class receiver {

public:
	//Constructors
	enum mode { WAIT, RECEIVE, SIFS, ACK };
	receiver() {
		name = 'S';
	}

	receiver(char newName) {
		name = newName;
	}

	char name;
	bool validMessage = false;
	int nextStageTime = INT_MAX;
	int channel = 0;
	mode status = WAIT;

	const int RECEIVETime = 100;
	const int SIFSTime = 1;
	const int ACKTime = 2;

	int getNextEvent() {
		return nextStageTime;
	}

	int refresh(int currentTime) {
		int returnValue = 0;
		
		switch (status) {
		case WAIT:
			validMessage = false; //reset valid message status
			if (channel == 1) { //A packet has begun arriving
				nextStageTime = currentTime + RECEIVETime;
				status = RECEIVE;
				validMessage = true;
				std::cout << currentTime << ": Receiver " << name << " has begun receiving a packet" << std::endl;
			}
			break;

		case RECEIVE:
			if (currentTime == nextStageTime) {
				status = SIFS;
				nextStageTime = currentTime + SIFSTime;
				std::cout << currentTime << ": Receiver " << name << " has started SIFS" << std::endl;
				break;
			}

			if (channel != 1) {
				validMessage = false;
				std::cout << currentTime << ": Receiver " << name << " detected a missing/erroneous packet" <<std::endl;
			}
			break;

		case SIFS:
			if (currentTime == nextStageTime) {
				status = ACK;
				nextStageTime = currentTime + ACKTime;
				std::cout << currentTime << ": Receiver " << name << " has begun ACK period" << std::endl;

				if (validMessage) {
					returnValue = 1;
					std::cout << currentTime << ": Receiver " << name << " has processed the packet and will send an ACK." << std::endl;

				}

				else {
					std::cout << currentTime << ": Receiver " << name << " did not process the packet and will not send an ACK" << std::endl;
				}
			}
			break;

		case ACK:
			if (currentTime == nextStageTime) {
				status = WAIT;
				nextStageTime = INT_MAX;
				std::cout << currentTime << ": Receiver " << name << " has exited ACK period and will wait for next packet" << std::endl;
				
				if (validMessage) {
					returnValue = -1;//Free the channel
				}
			}
			break;
		}
		return returnValue;


	}




};