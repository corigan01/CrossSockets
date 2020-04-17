#include "CrossSockets.h"
#include "displayout.h"
#include "CS_COM.h"
#include "CS_Server.h"

 //#define DISABLE_AUTH 0 /*If you want to disable the SSC */


int main() {

	
	CS_Server ser(80, false, true);
	CS_Server SSS(567);





	while (true) {
		Sleep(1000);

		static int i = 0;

		i++;

		std::string NewHtml = R"(<html>
<body>
<h1>Hello, world!</h1>
<p>Time : )" + std::to_string(i) + R"( </p>
</body>
</html>)";


		ser.HostHtml(NewHtml);

		
		//ser.SendToAll(Html);
	}
}