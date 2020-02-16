#include <iostream>
#include <windows.h>
#include <atlstr.h>

using namespace std;

void setLEDS();

//Function to send data through COM Port to the Arduino connected VIA USB
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

//Temp Variables, should be set by GUI program later

int topLED = 0;
int rightLED = 0;
int leftLED = 0;
int Quality = 0;

// End Temp Variables

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

	cout << "How many Samples per Pixel?(More accurate color): ";
	cin >> Quality;


	//End Temp Area
	setLEDS();

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



//Assumed there is a LED in both top right and top left corners
void setLEDS()
{
	COLORREF color = GetPixel(hdc, 0, 0);//0,0 is the coords at the top left of the main monitor (-1920 for left) (+ 2560 for 2nd monitor)

	int topOffset = 2560 / topLED; //Hardcode for Personal monitor Lookup universal solution
	int leftOffset = 1440 / leftLED; //Hardcode for Personal monitor Lookup universal solution
	int rightOffset = 1440 / rightLED; //Hardcode for Personal monitor Lookup universal solution


	//Colors
	int r = 0;
	int g = 0;
	int b = 0;


	//color totals (for averaging pixels)
	int rT = 0;
	int gT = 0;
	int bT = 0;

	//End Colors

	cout << "Pixel Size is: " << topOffset << endl;
	for (int n = 0; n < topLED; n++)//setting top LEDS PER PIXEL BASIS
	{
		//Getting X amount of samples per pixel;
		for (int c = 0; c < Quality/2; c++) //used for the columns
		{
			for (int r = 0; r < 2; r++)//used for top and bottom row of the samples
			{
				if(r==0)color = GetPixel(hdc, ((c * 100) / (Quality / 2)) + n * topOffset, (.15*topOffset));//Might not use topOffset becuase thats the width of pixel
				if(r==1)color = GetPixel(hdc, ((c * 100) / (Quality / 2)) + n * topOffset, (.75*topOffset));

				if (r == 0)cout << "Getting the pixel at the coords: " << ((c*100) / (Quality / 2)) + n * topOffset << " " << (.15*topOffset) << endl;
				else cout << "Getting the pixel at the coords: " << ((c * 100) / (Quality / 2)) + n * topOffset << " " << (.75*topOffset) << endl;
				rT += GetRValue(color);
				gT += GetGValue(color);
				bT += GetBValue(color);
			}
		}
		//End getting samples
		//color = GetPixel(hdc, 15+n*topOffset, 20);
		r = rT / Quality;
		g = gT / Quality;
		b = bT / Quality;

		cout << "Pixel Color: " << r << " " << g << " " << b << endl;

		rT = 0;
		gT = 0;
		bT = 0;
	}

	for (int n = 0; n < leftLED; n++)//setting Left LEDS
	{

	}

	for (int n = 0; n < rightLED; n++)//setting Right LEDS
	{

	}
}