#include <iostream>
#include <windows.h>
#include <atlstr.h>

using namespace std;

void setLEDS();


//DEFINES FOR MONITOR DIMENSIONS
#define MONITOR_WIDTHpx 2560
#define MONITOR_HEIGHTpx 1440
//End Monitor Dimesions


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

	system("pause");
}

/*Assumed there is a LED in both top right and top left corners
This function gets all the samples of the pixels and outputs the values to the screen
future use of this function will be to calculate the pixel value and immediatly send through the COM port*/
void setLEDS()
{
	COLORREF color = GetPixel(hdc, 0, 0);//0,0 is the coords at the top left of the main monitor (-1920 for left) (+ 2560 for 2nd monitor)

	int topOffset = MONITOR_WIDTHpx / topLED; //Hardcode for Personal monitor Lookup universal solution
	int leftOffset = MONITOR_HEIGHTpx / leftLED; //Hardcode for Personal monitor Lookup universal solution
	int rightOffset = MONITOR_HEIGHTpx / rightLED; //Hardcode for Personal monitor Lookup universal solution


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
		for (int c = 0; c < Quality / 2; c++) //used for the columns
		{
			for (int r = 0; r < 2; r++)//used for top and bottom row of the samples
			{
				if (r == 0)color = GetPixel(hdc, ((c * 100) / (Quality / 2)) + n * topOffset, (.15*topOffset));//Might not use topOffset becuase thats the width of pixel
				if (r == 1)color = GetPixel(hdc, ((c * 100) / (Quality / 2)) + n * topOffset, (.75*topOffset));

				//Error checking
				if (r == 0)cout << "Getting the pixel at the coords: " << ((c * 100) / (Quality / 2)) + n * topOffset << " " << (.15*topOffset) << endl;
				else cout << "Getting the pixel at the coords: " << ((c * 100) / (Quality / 2)) + n * topOffset << " " << (.75*topOffset) << endl;
				//End error checking

				rT += GetRValue(color);
				gT += GetGValue(color);
				bT += GetBValue(color);
			}
		}
		//End getting samples

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
		//Getting X amount of samples per pixel;
		for (int c = 0; c < Quality / 2; c++) //used for the columns
		{
			for (int r = 0; r < 2; r++)//used for top and bottom row of the samples
			{
				if (r == 0)color = GetPixel(hdc, (.15*leftOffset), ((c * 100) / (Quality / 2)) + n * leftOffset);//Might not use leftOffset becuase thats the width of pixel
				if (r == 1)color = GetPixel(hdc, (.75*leftOffset), ((c * 100) / (Quality / 2)) + n * leftOffset);

				//Error checking
				if (r == 0)cout << "LEFTLED: Getting the pixel at the coords: " << (.15*leftOffset) << " " << (((c * 100) / (Quality / 2)) + n * leftOffset) << endl;
				else cout << "LEFTLED: Getting the pixel at the coords: " << (.75*leftOffset) << " " << (((c * 100) / (Quality / 2)) + n * leftOffset) << endl;
				//End error checking

				rT += GetRValue(color);
				gT += GetGValue(color);
				bT += GetBValue(color);
			}
		}
		//End getting samples

		r = rT / Quality;
		g = gT / Quality;
		b = bT / Quality;

		cout << "Pixel Color: " << r << " " << g << " " << b << endl;

		rT = 0;
		gT = 0;
		bT = 0;
	}

	for (int n = 0; n < rightLED; n++)//setting Right LEDS
	{

	}
}