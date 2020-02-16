#include <iostream>
#include <windows.h>
#include <atlstr.h>

using namespace std;

void getPixels();


bool WriteComPort(CString PortSpecifier, CString data)
{
	DCB dcb;
	DWORD byteswritten;

	HANDLE hPort = CreateFile(
		PortSpecifier, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL
	);

	if (!GetCommState(hPort, &dcb)) return false;

	dcb.BaudRate = CBR_9600;          //9600 Baud
	dcb.ByteSize = 8;                 //8 data bits
	dcb.Parity = NOPARITY;            //no parity
	dcb.StopBits = ONESTOPBIT;        //1 stop

	if (!SetCommState(hPort, &dcb))
		return false;

	bool retVal = WriteFile(hPort, data, 1, &byteswritten, NULL);
	CloseHandle(hPort); //Closes the handle
	return retVal;
}


int topLED = 0;
int rightLED = 0;
int leftLED = 0;

int temp3 = 0;

HDC hdc = GetDC(NULL);//NULL returns the current screen as the entire screen (problem for multi display)


int main()
{
	//Temp Area, replaced by GUI program

	cout << "How many LEDS are on top?: ";
	cin >> topLED;
	cout << endl << endl;

	cout << "How many LEDS are on the right?: ";
	cin >> rightLED;
	cout << endl << endl;

	cout << "How many LEDS are on the left?: ";
	cin >> leftLED;
	cout << endl << endl;


	//End Temp Area


	try
	{
		int n = 0;
		while (n < 15)
		{
			WriteComPort("COM6", "" + n);
			n++;
		}

		if (WriteComPort("COM6", "123"))
			cout << "good";
	}
	catch (exception e)
	{
		cout << "Com Port not connected, Is it plugged in?\n";
	}
	system("pause");
}


/*
	determine how many pixels are going to be on the top, left, right (should be done by gui but not sure how)

	math the size of each LED

	Select the "quality" of each LED
	   ^
	   |
	do X samples per pixel
*/

//2560 x 1440 monitor resolution


void getPixels()
{

}