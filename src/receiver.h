#include "dcomm.h"
#include "frame.h"

class receiver{
	public:
	private:
	const int bufsize = 1024;
	frame buffer[bufsize];
	int windowposition;
	const int windowsize=WINDOW_SIZE;
	int buffertail; //menunjukkan posisi "ekor" buffer: yang belum dikonsumsi. apabila buffertail==windowposition maka buffer belum dapat digunakan
}
