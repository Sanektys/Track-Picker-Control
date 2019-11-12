//---------------------------------------------------------------------------
#ifndef TrackPickerControlH
#define TrackPickerControlH
//---------------------------------------------------------------------------
#include <FMX.Ani.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.Objects.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <System.Bluetooth.Components.hpp>
#include <System.Bluetooth.hpp>
#include <System.Classes.hpp>
#ifdef ANDROID
#include <Androidapi.Helpers.hpp>
#include <Androidapi.JNI.JavaTypes.hpp>
#include <Androidapi.JNI.Os.hpp>
#include <Androidapi.JNI.App.hpp>
#include <Androidapi.JNIBridge.hpp>
#include <Androidapi.JNI.GraphicsContentViewText.hpp>
#endif
//---------------------------------------------------------------------------
class TConceptForm : public TForm
{
__published:	// IDE-managed Components
	TGridPanelLayout *ControlPanel;
	TButton *ForwardButton;
	TImage *ImageForward;
	TButton *BackwardButton;
	TImage *ImageBackward;
	TButton *LeftButton;
	TImage *ImageLeft;
	TButton *RightButton;
	TImage *ImageRight;
	TImage *ImageFront;
	TGradientAnimation *GradientAnimation;
	TFloatAnimation *BluetoothConnectAnimation01;
	TTimer *VibroTimer;
	TGridPanelLayout *TestLayout;
	TButton *TestBluetooth;
	TLabel *TestBluetoothText;
	TLabel *NotConnectedText;
	TGridPanelLayout *MessageLayout;
	TColorAnimation *ColorAnimation1;
	TImage *MessageImage1;
	TImage *MessageImage2;
	TFloatAnimation *FloatAnimation1;
	TFloatAnimation *FloatAnimation2;
	TFloatAnimation *MessageLayoutHiden;
	TTimer *OpacityTimer;
	TLabel *KeyText;
	TLabel *StateMovementText;
	TGridPanelLayout *ConnectedLayout;
	TLabel *PlatformaText;
	TLabel *ConnectedText;
	TFloatAnimation *ConnectedAnimation1;
	TFloatAnimation *ConnectedAnimation2;
	TFloatAnimation *PlatformaAnimation1;
	TFloatAnimation *PlatformaAnimation2;
	TScaledLayout *ScaledLayout1;
	TScaledLayout *ScaledLayout2;
	TScaledLayout *ScaledLayout3;
	TGridPanelLayout *GetLayout1;
	TGridPanelLayout *GetLayout2;
	TImage *GetImage;
	TButton *GetButton;
	TImage *GetImage2;
	TFloatAnimation *GetAnimation1;
	TFloatAnimation *BluetoothConnectAnimation02;
	TFloatAnimation *GetAnimation3;
	TLayout *Layout1;
	TFloatAnimation *GetAnimation4;
	TButton *BluetoothButtonStart;
	TAniIndicator *BluetoothIndicator;
	TButton *BluetoothButtonStop;
	TBluetooth *BlueConnect;
	TScaledLayout *ScaledLayout4;
	TButton *RefreshButton;
	TLayout *Layout2;
	TTimer *ConnectionTimer;
	TImage *ImageRefresh;
	TTimer *LoadingTimer;
	void __fastcall TestBluetoothClick(TObject *Sender);
	void __fastcall VibroTimerTimer(TObject *Sender);
	void __fastcall ForwardButtonMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  float X, float Y);
	void __fastcall BackwardButtonMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  float X, float Y);
	void __fastcall LeftButtonMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  float X, float Y);
	void __fastcall RightButtonMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  float X, float Y);
	void __fastcall ButtonMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  float X, float Y);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
          TShiftState Shift);
	void __fastcall FormKeyUp(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
          TShiftState Shift);
	void __fastcall OpacityTimerTimer(TObject *Sender);
	void __fastcall BluetoothDiscoveryEnd(TObject * const Sender, TBluetoothDeviceList * const ADeviceList);
	void __fastcall RefreshButtonClick(TObject *Sender);
	void __fastcall GetButtonClick(TObject *Sender);
	void __fastcall ConnectionTimerTimer(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall LoadingTimerTimer(TObject *Sender);
private:	// User declarations
	void __fastcall SetConnection(TObject *Sender);
    void __fastcall EnabledControls(TObject *Sender);
    void __fastcall DisabledControls(TObject *Sender);
public:		// User declarations
	void __fastcall GetDevice();
	__fastcall TConceptForm(TComponent* Owner);
};

class TDiscoveryThread : public TThread
{
	friend TConceptForm;
private:
    void __fastcall DiscoverySuccess(void);
    void __fastcall DiscoveryUnsuccess(void);
    void __fastcall DiscoveryException(void);
    void __fastcall DiscoveryRepeat(void);
protected:
	void __fastcall Execute(void);
public:
    __fastcall TDiscoveryThread(bool ACreateSuspended) : TThread(ACreateSuspended){}
};
//---------------------------------------------------------------------------
extern PACKAGE TConceptForm *ConceptForm;
//---------------------------------------------------------------------------
#endif
