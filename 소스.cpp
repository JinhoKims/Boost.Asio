#include<iostream>
#include<boost/asio.hpp>
using namespace std;

const char SERVER_IP[] = "127.0.0.1";
const unsigned short PORT_NUMBER = 31400;

int main()
{
	// ������ ����� ����ϴ� ���α׷�
	boost::asio::io_service io_service; // I/O �̺�Ʈ�� OS���� ����ġ�ϴ� ��ü
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), PORT_NUMBER); // ��Ʈ��ũ �ּ�(��Ʈ) ����
	boost::asio::ip::tcp::acceptor acceptor(io_service, endpoint); // Ŭ���̾�Ʈ�� ������ ���

	boost::asio::ip::tcp::socket socket(io_service); // ������ Ŭ���̾�Ʈ�� �Ҵ��� ����(�����)
	acceptor.accept(socket); // Ŭ���̾�Ʈ�� ������ ����ϴ� acceptor�� socket�� �Ѱ���, �������̹Ƿ� ������ �Ϸ�� ������ �����
	cout << "Ŭ���̾�Ʈ ����" << endl;
	
	for (;;)
	{
		// ������ ����
		std::array<char, 128> buf; // ���ſ� �޽��� ����
		buf.assign(0);
		boost::system::error_code error; // ���� Ŭ����
		size_t len = socket.read_some(boost::asio::buffer(buf), error); // Ŭ���̾�Ʈ�� ���� �����͸� ����, �������̹Ƿ� �����͸� �� ���������� ��ٸ�
		
		if (error) // ���� ó��
		{
			if (error == boost::asio::error::eof)
			{
				cout << "Ŭ���̾�Ʈ�� ������ ���������ϴ�." << endl; // Ŭ���̾�Ʈ�� ���� ���� �ÿ��� ó��
			}
			else
			{
				cout << "error No : " << error.value() << " error Message : " << error.message() << endl;
			}
			break;
		}

		cout << "Ŭ���̾�Ʈ���� ���� �޽��� : " << &buf[0] << endl; // ���� �޽��� ���

		// ������ �۽�
		char szMessage[128] = { 0, }; // �߽ſ� ������
		sprintf_s(szMessage, 128 - 1, "Re:%s", &buf[0]); // szMessage�� ���ŵ� buf ���� �־���
		int nMsgLen = strnlen_s(szMessage, 128 - 1); // �޽��� ���� ���

		boost::system::error_code ignored_error;
		socket.write_some(boost::asio::buffer(szMessage, nMsgLen), ignored_error); // �޽��� ����(������, ����) ���� �ڵ� �Բ� ����
		cout << "Ŭ���̾�Ʈ�� ���� �޽��� : " << szMessage << endl; // ���� �޽��� ���
	}

}
