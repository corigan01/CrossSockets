#include "CPSocket.h"

 //#define DISABLE_AUTH 0 /*If you want to disable the SSC */


int main() {
	CPSocket socket("127.0.0.1", 56040, 56040);

	//socket.StartClient();
	socket.StartServer();

	while (true) {
		std::string Input = "";
		std::cin >> Input;

		if (Input == "STOP") break;
	}

	socket.~CPSocket();

}