
#include <stdio.h>
#include <SerialStream.h>
#include <iostream>
#include <thread>
#include <chrono>
using namespace LibSerial;
using namespace std;

static SerialStream port;

void
setup()
{
	port.Open("/dev/ttyUSB0");
	port.SetBaudRate(LibSerial::BaudRate::BAUD_9600);
	port.SetCharacterSize(LibSerial::CharacterSize::CHAR_SIZE_8);
	port.SetParity(LibSerial::Parity::PARITY_NONE);
	//port.SetStopBits(LibSerial::StopBits::ONE);
}

void
read()
{
	char temp[100];
	int ii = 0;

	for (; ii < 96; ++ii) {
		port.read(temp + ii, 1);
		if (temp[ii] == '\n') {
			break;
		}
	}

	temp[ii+1] = 0;
	cout << temp << endl;
}

int
main(int argc, char* argv[])
{
	setup();
	while (true) {
		port.write("Go", 2);
		read();
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
}
