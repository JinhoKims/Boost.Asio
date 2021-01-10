#include<iostream>
#include<boost/asio.hpp>
using namespace std;

const char SERVER_IP[] = "127.0.0.1";
const unsigned short PORT_NUMBER = 31400;

int main()
{
	// 동기형 방식을 사용하는 프로그램
	boost::asio::io_service io_service; // I/O 이벤트를 OS에서 디스패치하는 객체
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), PORT_NUMBER); // 네트워크 주소(포트) 설정
	boost::asio::ip::tcp::acceptor acceptor(io_service, endpoint); // 클라이언트의 접속을 담당

	boost::asio::ip::tcp::socket socket(io_service); // 접속한 클라이언트에 할당할 소켓(제어용)
	acceptor.accept(socket); // 클라이언트의 접속을 담당하는 acceptor에 socket을 넘겨줌, 동기형이므로 접속이 완료될 때까지 대기함
	cout << "클라이언트 접속" << endl;
	
	for (;;)
	{
		// 데이터 수신
		std::array<char, 128> buf; // 수신용 메시지 버퍼
		buf.assign(0);
		boost::system::error_code error; // 에러 클래스
		size_t len = socket.read_some(boost::asio::buffer(buf), error); // 클라이언트가 보낸 데이터를 받음, 동기형이므로 데이터를 다 받을때까지 기다림
		
		if (error) // 에러 처리
		{
			if (error == boost::asio::error::eof)
			{
				cout << "클라이언트와 연결이 끊어졌습니다." << endl; // 클라이언트가 접속 종료 시에도 처리
			}
			else
			{
				cout << "error No : " << error.value() << " error Message : " << error.message() << endl;
			}
			break;
		}

		cout << "클라이언트에서 받은 메시지 : " << &buf[0] << endl; // 받은 메시지 출력

		// 데이터 송신
		char szMessage[128] = { 0, }; // 발신용 데이터
		sprintf_s(szMessage, 128 - 1, "Re:%s", &buf[0]); // szMessage에 수신된 buf 값을 넣어줌
		int nMsgLen = strnlen_s(szMessage, 128 - 1); // 메시지 길이 계산

		boost::system::error_code ignored_error;
		socket.write_some(boost::asio::buffer(szMessage, nMsgLen), ignored_error); // 메시지 전송(데이터, 길이) 에러 코드 함께 전송
		cout << "클라이언트에 보낸 메시지 : " << szMessage << endl; // 보낸 메시지 출력
	}

}
