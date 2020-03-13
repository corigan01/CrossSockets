#include "CPSocket.h"

 //#define DISABLE_AUTH 0 /*If you want to disable the SSC */


int main() {
	CPSocket socket("127.0.0.1", 56040, 56000);

	//socket.StartClient();
	socket.StartServer();

	while (true) {

	}
	
	std::cout << "Sure" << std::endl;

	socket.~CPSocket();

}