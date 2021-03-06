#include "stdfax.h"
#include "TouchPadDlg.h"
#include   <io.h>
#include   <fcntl.h>
#include   <io.h>
#include   <fcntl.h>
#include <fstream>
ISynAPI *pAPI = NULL;
ISynDevice *pDev = NULL;
long Xmin, Xmax, Ymin, Ymax;
class TouchPad : public CWinApp,public _ISynDeviceEvents {
public:
	TouchPadDlg *dlg;
	int Xsize, Ysize;
	double Size, Xoffset, Yoffset;
	int width, height;
	int left, top;
	TouchPad() {
	}
	virtual BOOL InitInstance();
	HRESULT STDMETHODCALLTYPE OnSynDevicePacket(long lSequence);
	DECLARE_MESSAGE_MAP()
};
BEGIN_MESSAGE_MAP(TouchPad, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()
TouchPad theApp;
BOOL TouchPad::InitInstance() {
//	OpenConsole();
	AfxEnableControlContainer();
	HRESULT hRes = CoInitialize(0);
	if (hRes && hRes != S_FALSE)
		return false;
	long lHandle = -1;
	if (CoCreateInstance(_uuidof(SynAPI), 0,
		CLSCTX_INPROC_SERVER, _uuidof(ISynAPI), (void **)&pAPI) ||
		pAPI->Initialize() ||
		pAPI->FindDevice(SE_ConnectionAny, SE_DeviceTouchPad, &lHandle) ||
		pAPI->CreateDevice(lHandle, &pDev))
	{
		return -1;
	}
	pDev->Acquire(0);
	dlg = new TouchPadDlg();
	pDev->GetProperty(SP_XLoSensor, &Xmin);
	pDev->GetProperty(SP_XHiSensor, &Xmax);
	pDev->GetProperty(SP_YLoSensor, &Ymin);
	pDev->GetProperty(SP_YHiSensor, &Ymax);
	std::ifstream config("config.ini");
	config >> Xsize >> Ysize >> Size >> Xoffset >> Yoffset;
	width = (Xmax - Xmin) * Size;
	height = width * Ysize / Xsize;
	int xmid = Xmin * Xoffset + Xmax * (1 - Xoffset);
	int ymid = Ymin * Yoffset + Ymax * (1 - Yoffset);
	left = xmid - width / 2;
	top = ymid - height / 2;
	pDev->SetSynchronousNotification(this);
	dlg->DoModal();
	pDev->Unacquire();
	CoUninitialize();
	return 0;
}
HRESULT STDMETHODCALLTYPE TouchPad::OnSynDevicePacket(long lSequence) {
	SynPacket mp;
	pDev->LoadPacket(mp);
	static int x = 0, y = 0;
	static bool leftDown;
	if (mp.ZRaw() > 10) {
		x = (mp.XRaw() - left) * Xsize / width;
		y = Ysize - (mp.YRaw() - top) * Ysize / height;
	}
	SetCursorPos(x, y);
	long k = mp.ButtonState();
	bool ll = (k & SF_ButtonLeft) == SF_ButtonLeft;
	if (ll) {
		if (!leftDown) {
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			leftDown = true;
		}
	}
	else {
		if (leftDown) {
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			leftDown = false;
		}
	}
	return 0;
}

