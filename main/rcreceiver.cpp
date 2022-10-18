//rts/cts receiver

//This is the text file for a receiver



#include <iostream>


class rcreceiver {

public:
	//Constructors
	enum mode { WAIT, RTS, CTS, RECEIVE, SIFS, ACK };
	rcreceiver() {
		name = 'S';
	}

	rcreceiver(char newName) {
		name = newName;
	}

	char name;
	bool validMessage = false;
	int nextStageTime = INT_MAX;
	int channel = 0;
	mode status = WAIT;
	mode prevStatus = WAIT;

	const int RECEIVETime = 100;
	const int RTSTime = 2;
	const int CTSTime = 2;
	const int SIFSTime = 1;
	const int ACKTime = 2;

	int getNextEvent() {
		return nextStageTime;
	}

	int refreshR(int currentTime, int senderStatus) {
		int returnValue = 0;

		switch (status) {
		case WAIT:
			validMessage = true; //reset valid message status
			if (channel >= 1 && (senderStatus == 4)) { //RTS has begun arriving, greater than 1 because it must also accept packets that will collide, the collision will be detected later
				nextStageTime = currentTime + RTSTime;
				status = RTS;

				if (channel == 2) {
					validMessage = false;
				}

				std::cout << currentTime << ": Receiver " << name << " has begun receiving RTS" << std::endl;
			}
			break;

		case RTS:
			if (channel == 2) {
				validMessage = false;
			}
			if (channel >= 1 && nextStageTime == currentTime) {
				prevStatus = RTS;
				status = SIFS;
				nextStageTime = currentTime + SIFSTime;
				std::cout << currentTime << ": Receiver " << name << " has gone to SIFS from RTS" << std::endl;
			}
			break;

		case CTS:
			if (channel == 2) {
				validMessage = false;
			}
			if (channel >= 1 && nextStageTime == currentTime) {
				prevStatus = CTS;
				status = SIFS;
				nextStageTime = currentTime + SIFSTime;
				std::cout << currentTime << ": Receiver " << name << " has gone to SIFS from CTS" << std::endl;
				returnValue = -1;
			}
			break;

		case RECEIVE:
			prevStatus = RECEIVE;
			if (channel != 1) {
				validMessage = false;
				std::cout << currentTime << ": Receiver " << name << " detected a missing/erroneous packet" << std::endl;
			}
			if (currentTime == nextStageTime) {
				status = SIFS;
				nextStageTime = currentTime + SIFSTime;
				std::cout << currentTime << ": Receiver " << name << " has started SIFS" << std::endl;
				break;
			}
			break;

		case SIFS:
			if (currentTime == nextStageTime) {
				if (prevStatus == RTS) {
					nextStageTime = currentTime + CTSTime;
					status = CTS;
					std::cout << currentTime << ": Receiver " << name << " has left SIFS and gone to CTS" << std::endl;
					returnValue = 1;
				}
				if (prevStatus == CTS) {
					nextStageTime = currentTime + RECEIVETime;
					status = RECEIVE;
					std::cout << currentTime << ": Receiver " << name << " has left SIFS and gone to RECEIVE" << std::endl;
				}
				if (prevStatus == RECEIVE) {
					status = ACK;
					nextStageTime = currentTime + ACKTime;
					std::cout << currentTime << ": Receiver " << name << " has left SIFS and begun ACK period" << std::endl;
					returnValue = 1;
					if (validMessage) {
						std::cout << currentTime << ": Receiver " << name << " has processed the packet and will send an ACK." << std::endl;
					}
					else {
						std::cout << currentTime << ": Receiver " << name << " did not process the packet and will not send an ACK" << std::endl;
					}
				}
			}
			break;

		case ACK:
			if (currentTime == nextStageTime) {
				status = WAIT;
				nextStageTime = INT_MAX;
				std::cout << currentTime << ": Receiver " << name << " has exited ACK period and will wait for next packet" << std::endl;
				returnValue = -1;
			}
			break;
		}
		return returnValue;
	}
};