#include "CrossSockets.h"
#include "displayout.h"
#include "CS_COM.h"
#include "CS_Server.h"
#include "CS_Client.h"

 //#define DISABLE_AUTH 0 /*If you want to disable the SSC */


int main() {

	
	CS_Server ser(80, false, true);
	CS_Server SSS(567);





	while (true) {
		Sleep(1000);

		static int i = 0;
		i++;

		std::string HoldingStr = "";

		for (auto i : AllLog) {
			HoldingStr += "<p style=\"color:white\">" + i + "</p>\n";
		}

		std::string NewHtml = R"(<html>

<meta http-equiv="refresh" content="2" />
<body style="background-color:black;">
<h1 style="color:lightblue">CrossSockets v0.1</h1>
<p>Time in seconds : )" + std::to_string(i) + R"( </p>
)" + HoldingStr + R"(
</body>
</html>)";


		ser.HostHtml(NewHtml);

		
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