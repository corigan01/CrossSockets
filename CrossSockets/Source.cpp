#include "CPSocket.h"

 //#define DISABLE_AUTH 0 /*If you want to disable the SSC */


int main() {
	CPSocket socket("127.0.0.1", 56040, 56040);

	socket.StartClient();

	Sleep(1000);
	socket.SendString("Hello, I am CrossSocket");
	socket.SendString("Hello, I am CrossSocket");
	socket.SendString("Hello, I am CrossSocket");


}