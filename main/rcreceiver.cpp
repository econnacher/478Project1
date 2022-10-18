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
			if (channel != 1) {
				validMessage = false;
				std::cout << currentTime << ": Receiver " << name << " detected a missing/erroneous RTS" << std::endl;
			}
			if (currentTime == nextStageTime) {
				prevStatus = RTS;
				status = SIFS;
				nextStageTime = currentTime + SIFSTime;
				std::cout << currentTime << ": Receiver " << name << " has gone to SIFS from RTS" << std::endl;
			}
			break;

		case CTS:
			//prevStatus = CTS;
			if (currentTime == nextStageTime) {
				prevStatus = CTS;
				if (validMessage) {
					//returnValue = -1;//Free the channel
					status = SIFS;
					nextStageTime = currentTime + SIFSTime;
					std::cout << currentTime << ": Receiver " << name << " has exited CTS period and will go through SIFS and then wait for the packet" << std::endl;
				}

				else {
					status = WAIT;
					nextStageTime = INT_MAX;
					std::cout << currentTime << ": Receiver " << name << " has exited CTS period and will wait for next RTS" << std::endl;

				}
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
				std::cout << currentTime << ": Receiver " << name << " has started SIFS from RECEIVING" << std::endl;
				break;
			}
			break;

		case SIFS:
			if (currentTime == nextStageTime) {
				if (prevStatus == RTS) {
					nextStageTime = currentTime + CTSTime;
					status = CTS;
					std::cout << currentTime << ": Receiver " << name << " has left SIFS and gone to CTS" << std::endl;
					if (validMessage) {
						returnValue = 1;
						std::cout << currentTime << ": Receiver " << name << " has processed the RTS and will send a CTS." << std::endl;

					}

					else {
						std::cout << currentTime << ": Receiver " << name << " did not process the RTS and will not send a CTS" << std::endl;
					}
				}
				else if (prevStatus == CTS ) {
					nextStageTime = currentTime + RECEIVETime;
					status = RECEIVE;
					validMessage = true;
					std::cout << currentTime << ": Receiver " << name << " has left SIFS and gone to RECEIVE" << std::endl;
					returnValue = -1;
				}
				else if (prevStatus == RECEIVE) {
					status = ACK;
					nextStageTime = currentTime + ACKTime;
					std::cout << currentTime << ": Receiver " << name << " has left SIFS and begun ACK period" << std::endl;
					
					if (validMessage) {
						returnValue = 1;
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
				if (validMessage) {
					returnValue = -1;//Free the channel
				}
			}
			break;
		}
		return returnValue;
	}
};