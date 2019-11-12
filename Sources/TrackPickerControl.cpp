//---------------------------------------------------------------------------
#include <fmx.h>
#pragma hdrstop

#include "TrackPickerControl.h"
#include "TrackPickerControlPanelPCH1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"

TConceptForm *ConceptForm;
TBluetoothManager *BluetoothManager;
TBluetoothAdapter *Adapter;
TBluetoothDeviceList *DeviceList;
TBluetoothSocket *Socket;
TDiscoveryThread *DiscoveryThread;

bool bluetoothConnected = false;
double androidVersion;
const int DISCOVERTIME = 5000;
const String ServiceGUI = _D("{00001101-0000-1000-8000-00805F9B34FB}");
TBytes command;
//---------------------------------------------------------------------------

__fastcall TConceptForm::TConceptForm(TComponent* Owner) : TForm(Owner)
{
	#ifdef NDEBUG
	TestLayout->Visible = false;
	#endif
	#ifdef MSWINDOWS
	ConceptForm->BorderStyle = TFmxFormBorderStyle::Single;
	#endif
	#ifdef ANDROID
	androidVersion = TOSVersion::Major + (0.1 * TOSVersion::Minor);
    #ifdef _DEBUG
	KeyText->Text = androidVersion;
	#endif
	#endif
	ImageFront->Opacity = 0;
	GetLayout1->Opacity = 0;
	GetLayout2->Opacity = 0;
	ImageRefresh->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TConceptForm::EnabledControls(TObject *Sender)
{
    ImageForward->Enabled = true;
    ImageBackward->Enabled = true;
    ImageLeft->Enabled = true;
    ImageRight->Enabled = true;
    ForwardButton->Visible = true;
    BackwardButton->Visible = true;
    LeftButton->Visible = true;
    RightButton->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TConceptForm::DisabledControls(TObject *Sender)
{
	ImageForward->Enabled = false;
	ImageBackward->Enabled = false;
	ImageLeft->Enabled = false;
	ImageRight->Enabled = false;
	ForwardButton->Visible = false;
	BackwardButton->Visible = false;
	LeftButton->Visible = false;
	RightButton->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TConceptForm::TestBluetoothClick(TObject *Sender)
{
	if (bluetoothConnected == true)
	{
		#ifdef _DEBUG
		bluetoothConnected = false;
        TestBluetooth->Text = L"подключено";
		#endif
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
        EnabledControls(this);
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
        TestBluetooth->Text = L"нет соединения";
		#endif
		//Анимация включения блютуза (верхнее изображение).
		if (BluetoothConnectAnimation01->Enabled) BluetoothConnectAnimation01->Enabled - false;
		if (BluetoothConnectAnimation02->Enabled) BluetoothConnectAnimation02->Enabled = false;
		BluetoothConnectAnimation02->Enabled = true;
		//Обработка поля сообщения о неподключении.
		MessageLayoutHiden->Enabled = false;
		MessageLayoutHiden->Inverse = true;
		MessageLayoutHiden->Enabled = true;
		//Обработка клавиш управления платформой.
        DisabledControls(this);
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
    #ifdef ANDROID
	VibroTimer->Interval = 200;
	VibroTimer->Enabled = true;
    #endif
	OpacityTimer->Enabled = false;
	ImageForward->Opacity = 1;
    #ifdef _DEBUG
	StateMovementText->Text = "GOFORWARD";
    #endif
    try

    {
        command = TEncoding::UTF8->GetBytes(1);
    	Socket->SendData(command);
    }
    catch (Exception &Except)
    {
        Socket->Close();
    }
}
//---------------------------------------------------------------------------
void __fastcall TConceptForm::BackwardButtonMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, float X, float Y)
{
    #ifdef ANDROID
	VibroTimer->Interval = 300;
	VibroTimer->Enabled = true;
    #endif
	OpacityTimer->Enabled = false;
	ImageBackward->Opacity = 1;
    #ifdef _DEBUG
	StateMovementText->Text = "GOBACKWARD";
    #endif
    try
    {
		command = TEncoding::UTF8->GetBytes(2);
    	Socket->SendData(command);
    }
    catch (Exception &Except)
    {
        Socket->Close();
    }
}
//---------------------------------------------------------------------------
void __fastcall TConceptForm::LeftButtonMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, float X, float Y)
{
    #ifdef ANDROID
	VibroTimer->Interval = 500;
	VibroTimer->Enabled = true;
    #endif
	OpacityTimer->Enabled = false;
	ImageLeft->Opacity = 1;
    #ifdef _DEBUG
	StateMovementText->Text = "GOLEFT";
    #endif
    try
    {
		command = TEncoding::UTF8->GetBytes(3);
    	Socket->SendData(command);
    }
    catch (Exception &Except)
    {
        Socket->Close();
    }
}
//---------------------------------------------------------------------------
void __fastcall TConceptForm::RightButtonMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, float X, float Y)
{
    #ifdef ANDROID
	VibroTimer->Interval = 500;
	VibroTimer->Enabled = true;
    #endif
	OpacityTimer->Enabled = false;
	ImageRight->Opacity = 1;
    #ifdef _DEBUG
	StateMovementText->Text = "GORIGHT";
    #endif
    try
    {
		command = TEncoding::UTF8->GetBytes(4);
    	Socket->SendData(command);
    }
    catch (Exception &Except)
    {
        Socket->Close();
    }
}
//---------------------------------------------------------------------------
void __fastcall TConceptForm::ButtonMouseUp(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, float X, float Y)
{
    #ifdef ANDROID
	VibroTimer->Enabled = false;
    #endif
	OpacityTimer->Enabled = true;
	ImageForward->Opacity = 0.4;
	ImageBackward->Opacity = 0.4;
	ImageLeft->Opacity = 0.4;
	ImageRight->Opacity = 0.4;
    #ifdef _DEBUG
	StateMovementText->Text = "STOP";
    #endif
    try
    {
        command = TEncoding::UTF8->GetBytes(0);
		Socket->SendData(command);
    }
    catch (Exception &Except)
    {
        Socket->Close();
    }
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
            #ifdef _DEBUG
			KeyText->Text = "Key UP";
			#endif
		}
		if (Key == VK_DOWN)
		{
			BackwardButton->OnMouseDown(this, TMouseButton::mbLeft, TShiftState(NULL), 0, 0);
            #ifdef _DEBUG
			KeyText->Text = "Key DOWN";
            #endif
		}
		if (Key == VK_LEFT)
		{
			LeftButton->OnMouseDown(this, TMouseButton::mbLeft, TShiftState(NULL), 0, 0);
            #ifdef _DEBUG
			KeyText->Text = "Key LEFT";
            #endif
		}
		if (Key == VK_RIGHT)
		{
			RightButton->OnMouseDown(this, TMouseButton::mbLeft, TShiftState(NULL), 0, 0);
            #ifdef _DEBUG
            KeyText->Text = "Key RIGHT";
            #endif
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
        #ifdef _DEBUG
		KeyText->Text = "non key";
        #endif
	}
}
//---------------------------------------------------------------------------

void __fastcall TConceptForm::OpacityTimerTimer(TObject *Sender)
{
    if (Socket != NULL && Socket->Connected)
    {
        try 
        {
        	Socket->Connected;
        }
        catch (Exception &Except) 
        {
        	Socket->Close();
        }
    }
    #ifdef ANDROID
	ForwardButton->Opacity = 0;
	BackwardButton->Opacity = 0;
	LeftButton->Opacity = 0;
	RightButton->Opacity = 0;
	GetButton->Opacity = 0;
    RefreshButton->Opacity = 0;
	#endif
	if (Socket != NULL)
	if (Socket->Connected && !bluetoothConnected)
	{
		bluetoothConnected = true;
		TestBluetoothClick(this);
        if (DiscoveryThread != NULL)
    	{
    		DiscoveryThread->Terminate();
        	DiscoveryThread->WaitFor();
        	delete DiscoveryThread;
        	DiscoveryThread = NULL;
    	}
	}
	else if (!Socket->Connected && bluetoothConnected)
	{
		bluetoothConnected = false;
        ImageRefresh->Visible = true;
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
        	#ifdef _DEBUG
			TestBluetoothText->Text = ADeviceList->Items[i]->DeviceName;
            #endif
            try
            {
				Adapter->Pair(ADeviceList->Items[i]);
            }
            catch (Exception &Except)
            {
            	MessageBox(NULL, L"Адаптер bluetooth выключен или отсутствует", L"Bluetooth отключён", MB_OK | MB_ICONERROR);
            	ImageRefresh->Visible = true;
                BluetoothIndicator->Visible = false;
                return;
            }
			break;
		}
	}
	GetDevice();
	#endif
}
//---------------------------------------------------------------------------

void __fastcall TConceptForm::FormCreate(TObject *Sender)
{
    ConnectionTimer->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TConceptForm::ConnectionTimerTimer(TObject *Sender)
{
    ConnectionTimer->Enabled = false;
    SetConnection(this);
}
//---------------------------------------------------------------------------

void __fastcall TDiscoveryThread::DiscoverySuccess(void)
{
    ConceptForm->BluetoothIndicator->Visible = false;
    ConceptForm->ImageRefresh->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TDiscoveryThread::DiscoveryUnsuccess(void)
{
    ShowMessage(L"Отсутствует связь с платформой");
    ConceptForm->BluetoothIndicator->Visible = false;
	ConceptForm->ImageRefresh->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TDiscoveryThread::DiscoveryException(void)
{
    ShowMessage(L"Сперва включите bluetooth");
    ConceptForm->BluetoothIndicator->Visible = false;
	ConceptForm->ImageRefresh->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TDiscoveryThread::DiscoveryRepeat(void)
{
    if (bluetoothConnected == false)
    {
        ConceptForm->BluetoothIndicator->Visible = false;
        struct OnCloseDialog : public TCppInterfacedObject<TInputCloseDialogProc>
		{
			void __fastcall Invoke(const System::Uitypes::TModalResult AResult)
			{
				switch (AResult)
				{
					case mrOk:
					ConceptForm->BluetoothIndicator->Visible = true;
					ConceptForm->GetDevice();
					break;
					case mrCancel:
					ConceptForm->ImageRefresh->Visible = true;
					break;
                	default:
                	ConceptForm->ImageRefresh->Visible = true;
                	break;
				}
			}
		};
		_di_TInputCloseDialogProc close = new OnCloseDialog();
		MessageDlg(L"Платформа не сопряжена\n\nПеред тем, как начать работу с платформой, вручную подключитесь к ней.\nИдентификатор: \"H-C-2010-06-01\"\nАдрес: 98:D3:32:30:7E:E9\nПароль: 1234\nЗатем нажмите \"OK\"", TMsgDlgType::mtInformation,
		TMsgDlgButtons() << TMsgDlgBtn::mbCancel << TMsgDlgBtn::mbOK, 0, close);
    }
}
//---------------------------------------------------------------------------

void __fastcall TDiscoveryThread::Execute(void)
{
    try
    {
		for (int i = 0; i < ConceptForm->BlueConnect->PairedDevices()->Count; i++)
		{
            Sleep(1000);
			if (ConceptForm->BlueConnect->PairedDevices()->Items[i]->DeviceName == "H-C-2010-06-01")
			{
				try
				{
					Socket = ConceptForm->BlueConnect->PairedDevices()->Items[i]->CreateClientSocket(StringToGUID(ServiceGUI), false);
					Socket->Connect();
                    TThread::Synchronize(DiscoverySuccess);
					return;
				}
			   	catch (Exception &Except)
				{
                    TThread::Synchronize(DiscoveryUnsuccess);
					return;
            	}
			}
		}
    }
    catch (Exception &Except)
    {
    	TThread::Synchronize(DiscoveryException);
        return;
    }
    TThread::Synchronize(DiscoveryRepeat);
}
//---------------------------------------------------------------------------

void __fastcall TConceptForm::SetConnection(TObject *Sender)
{
	#ifdef MSWINDOWS
	switch (MessageBox(NULL, L"Будет произведена попытка автоматического подключения к платформе", L"Проверьте Bluetooth", MB_OKCANCEL | MB_ICONINFORMATION))
	{
		case IDOK:
        try
        {
			BluetoothManager = TBluetoothManager::Current;
			Adapter = BluetoothManager->CurrentAdapter;
			BluetoothManager->OnDiscoveryEnd = BluetoothDiscoveryEnd;
			BluetoothManager->StartDiscovery(DISCOVERTIME);
			BluetoothIndicator->Visible = true;
        }
        catch (Exception &Except)
        {
            MessageBox(NULL, L"Адаптер bluetooth выключен или отсутствует", L"Bluetooth отключён", MB_OK | MB_ICONERROR);
            ImageRefresh->Visible = true;
        }
		break;
		case IDCANCEL:
		ImageRefresh->Visible = true;
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
				ConceptForm->GetDevice();
				break;
				case mrCancel:
				ConceptForm->ImageRefresh->Visible = true;
				break;
                default:
                ConceptForm->ImageRefresh->Visible = true;
                break;
			}
		}
	};
	_di_TInputCloseDialogProc close = new OnCloseDialog();
	MessageDlg(L"Будет произведена попытка подключения к платформе", TMsgDlgType::mtInformation,
	TMsgDlgButtons() << TMsgDlgBtn::mbCancel << TMsgDlgBtn::mbOK, 0, close);
	#endif
}
//---------------------------------------------------------------------------

void __fastcall TConceptForm::GetDevice()
{
    #ifdef MSWINDOWS
    try
    {
    	for (int i = 0; i < BluetoothManager->GetPairedDevices()->Count; i++)
		{
			if (BluetoothManager->GetPairedDevices()->Items[i]->DeviceName == "H-C-2010-06-01")
			{
				try
				{
					Socket = BluetoothManager->GetPairedDevices()->Items[i]->CreateClientSocket(StringToGUID(ServiceGUI), false);
					Socket->Connect();
                	BluetoothIndicator->Visible = false;
					ImageRefresh->Visible = false;
					return;
				}
				catch (Exception &Except)
				{
					MessageBox(NULL, L"Отсутствует связь с платформой", L"Нет соединения", MB_OK | MB_ICONERROR);
					BluetoothIndicator->Visible = false;
					ImageRefresh->Visible = true;
					return;
				}
			}
    	}
    }
    catch (Exception &Except)
    {
    	MessageBox(NULL, L"Адаптер bluetooth выключен или отсутствует", L"Bluetooth отключён", MB_OK | MB_ICONERROR);
        ImageRefresh->Visible = true;
		BluetoothIndicator->Visible = false;
        return;
    }
	if (bluetoothConnected == false) MessageBox(NULL, L"Платформа не сопряжена или не найдена", L"Нет соединения", MB_ICONWARNING | MB_OK);
	ImageRefresh->Visible = true;
	BluetoothIndicator->Visible = false;
    #endif
    #ifdef ANDROID
    if (DiscoveryThread == NULL)
    {
    	DiscoveryThread = new TDiscoveryThread(true);
    	DiscoveryThread->Start();
    }
    else
    {
        DiscoveryThread->Terminate();
        DiscoveryThread->WaitFor();
        delete DiscoveryThread;
        DiscoveryThread = NULL;
        DiscoveryThread = new TDiscoveryThread(true);
    	DiscoveryThread->Start();
    }
    #endif
}
//---------------------------------------------------------------------------

void __fastcall TConceptForm::RefreshButtonClick(TObject *Sender)
{
	#ifdef MSWINDOWS
    try
    {
    	BluetoothManager = TBluetoothManager::Current;
    	Adapter = BluetoothManager->CurrentAdapter;
        BluetoothManager->OnDiscoveryEnd = BluetoothDiscoveryEnd;
        BluetoothManager->StartDiscovery(DISCOVERTIME);
        BluetoothIndicator->Visible = true;
    }
    catch (Exception &Except)
    {
    	MessageBox(NULL, L"Адаптер bluetooth выключен или отсутствует", L"Bluetooth отключён", MB_OK | MB_ICONERROR);
    	ImageRefresh->Visible = true;
    }
	#endif
    #ifdef ANDROID                      
    ConceptForm->BluetoothIndicator->Visible = true;
    GetDevice();
	#endif
}
//---------------------------------------------------------------------------

void __fastcall TConceptForm::GetButtonClick(TObject *Sender)
{
    try
    {
        command = TEncoding::UTF8->GetBytes(10);
    	Socket->SendData(command);
        DisabledControls(this);
        LoadingTimer->Enabled = true;
    }
    catch (Exception &Except)
    {
        Socket->Close();
    }
}
//---------------------------------------------------------------------------

void __fastcall TConceptForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	if (Socket != NULL) Socket->Close();
}
//---------------------------------------------------------------------------

void __fastcall TConceptForm::LoadingTimerTimer(TObject *Sender)
{
    try
    {
        if (Socket->ReceiveData(2) == TEncoding::UTF8->GetBytes(100))
        {
            EnabledControls(this);
            LoadingTimer->Enabled = false;
        }
    }
    catch (Exception &Except)
    {
        Socket->Close();
    }
}
//---------------------------------------------------------------------------

