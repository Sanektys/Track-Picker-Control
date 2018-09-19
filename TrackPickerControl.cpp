//---------------------------------------------------------------------------
#include <fmx.h>
#pragma hdrstop

#include "TrackPickerControl.h"
#include "TrackPicker controlPCH1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
#pragma resource ("*.LgXhdpiPh.fmx", _PLAT_ANDROID)
#pragma resource ("*.SmXhdpiPh.fmx", _PLAT_ANDROID)
#pragma resource ("*.NmXhdpiPh.fmx", _PLAT_ANDROID)

TConceptForm *ConceptForm;
TBluetoothManager *BluetoothManager;
TBluetoothAdapter *Adapter;
TBluetoothDeviceList *DeviceList;
TBluetoothSocket *Socket;

bool bluetoothConnected = false;
double androidVersion;
const int DISCOVERTIME = 5000;
const String ServiceGUI = _D("{00001101-0000-1000-8000-00805F9B34FB}");
TBytes command;
//---------------------------------------------------------------------------

__fastcall TConceptForm::TConceptForm(TComponent* Owner)
	: TForm(Owner)
{
	#ifdef NDEBUG
	TestLayout->Visible = false;
	#endif
	#ifdef MSWINDOWS
	ConceptForm->BorderStyle = TFmxFormBorderStyle::Single;
	#endif
	#ifdef ANDROID
	androidVersion = TOSVersion::Major + (0.1 * TOSVersion::Minor);
	KeyText->Text = androidVersion;
	#endif
	ImageFront->Opacity = 0;
	GetLayout1->Opacity = 0;
	GetLayout2->Opacity = 0;
}
//---------------------------------------------------------------------------

void __fastcall TConceptForm::TestBluetoothClick(TObject *Sender)
{
	if (bluetoothConnected == true)
	{
		#ifdef _DEBUG
		bluetoothConnected = false;
		#endif
		TestBluetooth->Text = L"подключено";
		//Анимация включения блютуза (верхнее изображение).
		if (BluetoothConnectAnimation02->Enabled) BluetoothConnectAnimation02->Enabled = false;
        if (BluetoothConnectAnimation01->Enabled) BluetoothConnectAnimation01->Enabled = false;
		BluetoothConnectAnimation01->Enabled = true;
		ImageFront->Opacity = 0.85;
		//Обработка поля сообщения о неподключении.
		if (MessageLayoutHiden) MessageLayoutHiden->Inverse = false;
		MessageLayoutHiden->Enabled = false;
		MessageLayoutHiden->Enabled = true;
		//Обработка клавиш управления платформой.
		ImageForward->Enabled = true;
		ImageBackward->Enabled = true;
		ImageLeft->Enabled = true;
		ImageRight->Enabled = true;
		ForwardButton->Visible = true;
		BackwardButton->Visible = true;
		LeftButton->Visible = true;
		RightButton->Visible = true;
		//Анимация подключения (верхняя половина).
        if (PlatformaAnimation1->Enabled)
		{
			PlatformaAnimation1->Enabled = false;
			PlatformaAnimation1->Inverse = false;
		}
		if (PlatformaAnimation2->Enabled)
		{
			PlatformaAnimation2->Enabled = false;
			PlatformaAnimation2->Inverse = false;
		}
		PlatformaAnimation1->Enabled = true;
		PlatformaAnimation2->Enabled = true;
        if (ConnectedAnimation1->Enabled)
		{
			ConnectedAnimation1->Enabled = false;
			ConnectedAnimation1->Inverse = false;
		}
		if (ConnectedAnimation2->Enabled)
		{
			ConnectedAnimation2->Enabled = false;
			ConnectedAnimation2->Inverse = false;
		}
		ConnectedAnimation1->Enabled = true;
		ConnectedAnimation2->Enabled = true;
		//Анимация подключения (нижняя половина).
		if (GetAnimation1->Enabled) GetAnimation1->Enabled = false;
		GetAnimation1->Enabled = true;
		if (GetAnimation4->Enabled) GetAnimation4->Enabled = false;
		if (GetAnimation3->Enabled) GetAnimation3->Enabled = false;
		if (GetImage2->Position->X != 0)
		{
			GetLayout2->Opacity = 0,75;
			GetImage2->Position->X = 0;
        }
		GetAnimation3->Enabled = true;
	}
	else
	{
        #ifdef _DEBUG
		bluetoothConnected = true;
		#endif
		TestBluetooth->Text = L"нет соединения";
		//Анимация включения блютуза (верхнее изображение).
		if (BluetoothConnectAnimation01->Enabled) BluetoothConnectAnimation01->Enabled - false;
		if (BluetoothConnectAnimation02->Enabled) BluetoothConnectAnimation02->Enabled = false;
		BluetoothConnectAnimation02->Enabled = true;
		//Обработка поля сообщения о неподключении.
		MessageLayoutHiden->Enabled = false;
		MessageLayoutHiden->Inverse = true;
		MessageLayoutHiden->Enabled = true;
		//Обработка клавиш управления платформой.
		ImageForward->Enabled = false;
		ImageBackward->Enabled = false;
		ImageLeft->Enabled = false;
		ImageRight->Enabled = false;
		ForwardButton->Visible = false;
		BackwardButton->Visible = false;
		LeftButton->Visible = false;
		RightButton->Visible = false;
		//Анимация подключения (верхняя половина).
		if (PlatformaAnimation1->Enabled)
		{
			PlatformaAnimation1->Enabled = false;
			PlatformaAnimation1->Inverse = true;
		}
		if (PlatformaAnimation2->Enabled)
		{
			PlatformaAnimation2->Enabled = false;
			PlatformaAnimation2->Inverse = true;
		}
		PlatformaAnimation1->Enabled = true;
		PlatformaAnimation2->Enabled = true;
		if (ConnectedAnimation1->Enabled)
		{
			ConnectedAnimation1->Enabled = false;
			ConnectedAnimation1->Inverse = true;
		}
		if (ConnectedAnimation2->Enabled)
		{
			ConnectedAnimation2->Enabled = false;
			ConnectedAnimation2->Inverse = true;
		}
		ConnectedAnimation1->Enabled = true;
		ConnectedAnimation2->Enabled = true;
		//Анимация подключения (нижняя половина).
        if (GetAnimation1->Enabled) GetAnimation1->Enabled = false;
		GetLayout1->Opacity = 0;
		if (GetAnimation4->Enabled) GetAnimation4->Enabled = false;
		GetAnimation4->Enabled = true;
		if (GetAnimation3->Enabled) GetAnimation3->Enabled = false;
	}
}
//---------------------------------------------------------------------------


void __fastcall TConceptForm::VibroTimerTimer(TObject *Sender)
{
	#ifdef ANDROID
	_di_JVibrator Vibrator = TJVibrator::Wrap(((_di_ILocalObject)
	SharedActivityContext()->getSystemService(TJContext::JavaClass->VIBRATOR_SERVICE))->GetObjectID());
	if (Vibrator->hasVibrator()) Vibrator->vibrate(80);
	#endif
}
//---------------------------------------------------------------------------


void __fastcall TConceptForm::ForwardButtonMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, float X, float Y)
{
	VibroTimer->Interval = 200;
	VibroTimer->Enabled = true;
	OpacityTimer->Enabled = false;
	ImageForward->Opacity = 1;
	StateMovementText->Text = "GOFORWARD";
	command = TEncoding::UTF8->GetBytes("forward");
	Socket->SendData(command);
}
//---------------------------------------------------------------------------
void __fastcall TConceptForm::BackwardButtonMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, float X, float Y)
{
	VibroTimer->Interval = 300;
	VibroTimer->Enabled = true;
	OpacityTimer->Enabled = false;
	ImageBackward->Opacity = 1;
	StateMovementText->Text = "GOBACKWARD";
	command = TEncoding::UTF8->GetBytes("backward");
	Socket->SendData(command);
}
//---------------------------------------------------------------------------
void __fastcall TConceptForm::LeftButtonMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, float X, float Y)
{
	VibroTimer->Interval = 500;
	VibroTimer->Enabled = true;
	OpacityTimer->Enabled = false;
	ImageLeft->Opacity = 1;
	StateMovementText->Text = "GOLEFT";
	command = TEncoding::UTF8->GetBytes("left");
	Socket->SendData(command);
}
//---------------------------------------------------------------------------
void __fastcall TConceptForm::RightButtonMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, float X, float Y)
{
	VibroTimer->Interval = 500;
	VibroTimer->Enabled = true;
	OpacityTimer->Enabled = false;
	ImageRight->Opacity = 1;
	StateMovementText->Text = "GORIGHT";
	command = TEncoding::UTF8->GetBytes("right");
	Socket->SendData(command);
}
//---------------------------------------------------------------------------
void __fastcall TConceptForm::ButtonMouseUp(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, float X, float Y)
{
	VibroTimer->Enabled = false;
	OpacityTimer->Enabled = true;
	ImageForward->Opacity = 0.4;
	ImageBackward->Opacity = 0.4;
	ImageLeft->Opacity = 0.4;
	ImageRight->Opacity = 0.4;
	StateMovementText->Text = "STOP";
	command = TEncoding::UTF8->GetBytes("stop");
	Socket->SendData(command);
}
//---------------------------------------------------------------------------

void __fastcall TConceptForm::FormKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
          TShiftState Shift)
{
	#ifdef MSWINDOWS
	if (bluetoothConnected)
	{
		if (Key == VK_UP)
		{
			ForwardButton->OnMouseDown(this, TMouseButton::mbLeft, TShiftState(NULL), 0, 0);
			KeyText->Text = "Key UP";
		}
		if (Key == VK_DOWN)
		{
			BackwardButton->OnMouseDown(this, TMouseButton::mbLeft, TShiftState(NULL), 0, 0);
			KeyText->Text = "Key DOWN";
		}
		if (Key == VK_LEFT)
		{
			LeftButton->OnMouseDown(this, TMouseButton::mbLeft, TShiftState(NULL), 0, 0);
			KeyText->Text = "Key LEFT";
		}
		if (Key == VK_RIGHT)
		{
			RightButton->OnMouseDown(this, TMouseButton::mbLeft, TShiftState(NULL), 0, 0);
            KeyText->Text = "Key RIGHT";
		}
	}
    #endif
}
//---------------------------------------------------------------------------

void __fastcall TConceptForm::FormKeyUp(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
          TShiftState Shift)
{
	if (bluetoothConnected)
	{
		ButtonMouseUp(this, TMouseButton::mbLeft, TShiftState(NULL), 0, 0);
		KeyText->Text = "non key";
	}
}
//---------------------------------------------------------------------------

void __fastcall TConceptForm::OpacityTimerTimer(TObject *Sender)
{
    #ifdef ANDROID
	ForwardButton->Opacity = 0;
	BackwardButton->Opacity = 0;
	LeftButton->Opacity = 0;
	RightButton->Opacity = 0;
	GetButton->Opacity = 0;
	#endif
    if (Socket != NULL)
	if (Socket->Connected && !bluetoothConnected)
	{
		bluetoothConnected = true;
		TestBluetoothClick(this);
	}
	else if (!Socket->Connected && bluetoothConnected)
	{
		bluetoothConnected = false;
        RefreshButton->Visible = true;
		TestBluetoothClick(this);
	}

}
//---------------------------------------------------------------------------

void __fastcall TConceptForm::BluetoothDiscoveryEnd(TObject * const Sender, TBluetoothDeviceList * const ADeviceList)
{
	#ifdef MSWINDOWS
	for (int i = 0; i < ADeviceList->Count; i++)
	{
		if (ADeviceList->Items[i]->DeviceName == "H-C-2010-06-01")
		{
			TestBluetoothText->Text = ADeviceList->Items[i]->DeviceName;
			Adapter->Pair(ADeviceList->Items[i]);
			break;
		}
	}
	for (int i = 0; i < BluetoothManager->GetPairedDevices()->Count; i++)
	{
		if (BluetoothManager->GetPairedDevices()->Items[i]->DeviceName == "H-C-2010-06-01")
		{
			try
			{
				Socket = BluetoothManager->GetPairedDevices()->Items[i]->CreateClientSocket(StringToGUID(ServiceGUI), false);
				Socket->Connect();
                BluetoothIndicator->Visible = false;
				RefreshButton->Visible = false;
				return;
			}
			catch (Exception &Except)
			{
				MessageBox(NULL, L"Отсутствует связь с платформой", L"Нет соединения", MB_OK | MB_ICONERROR);
				BluetoothIndicator->Visible = false;
				RefreshButton->Visible = true;
				return;
			}

		}
	}
	if (bluetoothConnected == false) MessageBox(NULL, L"Платформа не сопряжена или не найдена", L"Нет соединения", MB_ICONWARNING | MB_OK); 
	RefreshButton->Visible = true;
	BluetoothIndicator->Visible = false;
	#endif
}
//---------------------------------------------------------------------------

void __fastcall TConceptForm::FormShow(TObject *Sender)
{
	RefreshButton->Visible = true;
	#ifdef MSWINDOWS
	switch (MessageBox(NULL, L"Будет произведена попытка автоматического подключения к платформе", L"Проверьте Bluetooth", MB_OKCANCEL | MB_ICONINFORMATION))
	{
		case IDOK:
		BluetoothManager = TBluetoothManager::Current;
		Adapter = BluetoothManager->CurrentAdapter;
		BluetoothManager->OnDiscoveryEnd = ConceptForm->BluetoothDiscoveryEnd;
		BluetoothManager->StartDiscovery(DISCOVERTIME);
		ConceptForm->BluetoothIndicator->Visible = true;
		break;
		case IDCANCEL: 
		RefreshButton->Visible = true;
		break;
	}
	#endif
	#ifdef ANDROID
	struct OnCloseDialog : public TCppInterfacedObject<TInputCloseDialogProc>
	{
		void __fastcall Invoke(const System::Uitypes::TModalResult AResult)
		{
			switch (AResult)
			{
				case mrOk:
				ConceptForm->BluetoothIndicator->Visible = true;
				Sleep(1000);
				ConceptForm->GetDevice();
				break;
				case mrCancel: 
				ConceptForm->RefreshButton->Visible = true;
				break;
			}
		}
	};
	_di_TInputCloseDialogProc close = new OnCloseDialog();
	MessageDlg(L"Перед тем, как начать работу с платформой, вручную подключитесь к ней.\nИдентификатор: \"H-C-2010-06-01\"\nПароль: 1234\nЗатем нажмите \"OK\"", TMsgDlgType::mtInformation,
	TMsgDlgButtons() << TMsgDlgBtn::mbCancel << TMsgDlgBtn::mbOK, 0, close);
	#endif
}
//---------------------------------------------------------------------------


void __fastcall TConceptForm::GetDevice()
{
	for (int i = 0; i < ConceptForm->BlueConnect->PairedDevices()->Count; i++)
	{
		if (ConceptForm->BlueConnect->PairedDevices()->Items[i]->DeviceName == "H-C-2010-06-01")
		{
			try
			{
				Socket = ConceptForm->BlueConnect->PairedDevices()->Items[i]->CreateClientSocket(StringToGUID(ServiceGUI), false);
				Socket->Connect();
				ConceptForm->BluetoothIndicator->Visible = false;
				ConceptForm->RefreshButton->Visible = false;
				return;
			}
			catch (Exception &Except)
			{
				ShowMessage(L"Отсутствует связь с платформой");
				ConceptForm->BluetoothIndicator->Visible = false;
				ConceptForm->RefreshButton->Visible = true;
				return;
            }
		}
	}
	if (bluetoothConnected == false) ShowMessage(L"Платформа не сопряжена или не найдена");
	ConceptForm->RefreshButton->Visible = true;
	ConceptForm->BluetoothIndicator->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TConceptForm::RefreshButtonClick(TObject *Sender)
{
	FormShow(this);	
}
//---------------------------------------------------------------------------

void __fastcall TConceptForm::GetButtonClick(TObject *Sender)
{
	command = TEncoding::UTF8->GetBytes("load");
	Socket->SendData(command);	
}
//---------------------------------------------------------------------------
