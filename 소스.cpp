#include<iostream>
#include<boost/asio.hpp>
using namespace std;

const char SERVER_IP[] = "127.0.0.1";
const unsigned short PORT_NUMBER = 31400;

int main()
{

	boost::asio::io_service io_service; // I/O 이벤트를 디스패치하는 객체
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), PORT_NUMBER); // 네트워크 주소 설정
	boost::asio::ip::tcp::acceptor acceptor(io_service, endpoint); // 클라이언트의 접속을 담당

	boost::asio::ip::tcp::socket socket(io_service); // 접속한 클라이언트의 
	acceptor.accept(socket);
	cout << "클라이언트 접속" << endl;
	
	for (;;)
	{
		array<char, 128> buf;
		buf.assign(0);
		boost::system::error_code error;
		size_t len = socket.read_some(boost::asio::buffer(buf), error);

		if (error)
		{
			if (error == boost::asio::error::eof)
			{
				cout << "클라이언트와 연결이 끊어졌습니다." << endl;
			}
			else
			{
				cout << "error No : " << error.value() << " error Message : " << error.message() << endl;
			}
			break;
		}


		cout << "클라이언트에서 받은 메시지 : " << &buf[0] << endl;
		char szMessage[128] = { 0, };
		sprintf_s(szMessage, 128 - 1, "Re:%s", &buf[0]);
		int nMsgLen = strnlen_s(szMessage, 128 - 1);

		boost::system::error_code ignored_error;
		socket.write_some(boost::asio::buffer(szMessage, nMsgLen), ignored_error);
		cout << "클라이언트에 보낸 메시지 : " << szMessage << endl;
	}

	getchar();
}
