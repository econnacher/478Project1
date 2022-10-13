//for classes yay

enum mode {WAIT, DIFS, BACKOFF, SENDING, SIFS, ACK};

class station {
public:
	int windowSize = 4;
	const int windowMin = 4;
	const int windowMax = 1024;

	int successCount = 0;
	int collisionCount = 0;

	mode status = WAIT;
};