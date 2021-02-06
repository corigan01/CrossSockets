#include "CrossSockets.h"
#include "displayout.h"
#include "CS_COM.h"
#include "CS_Server.h"
#include "CS_Client.h"

 //#define DISABLE_AUTH 0 /*If you want to disable the SSC */


int main() {

	
	CS_Server ser(80, false, true);
	//CS_Server SSS(567);





	while (true) {
		Sleep(100);

		


		ser.HostHtml("<html><body>TEST</body></html>");

		
		//ser.SendToAll(Html);
	}
}
/*

int main() {
	CS_Client cl(56012, "127.0.0.1");
	


	while (true) { 
		sleep_for(1s);
		
		std::string str = "VER::";
		

		cl.Send(str);
	


		
	}




}*/