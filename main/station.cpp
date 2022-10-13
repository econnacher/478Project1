//for classes

enum mode {WAIT, DIFS, BACKOFF, SENDING, SIFS, ACK};

class station {
public:
	int windowSize = 4;
	const windowMin = 4;
	const windowMax = 1024;

	int successCount = 0;
	int collisionCount = 0;

	mode status = WAIT;
};