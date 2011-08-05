#include <v8.h>
#include <node.h>
#include <PDL.h>


using namespace v8;
using namespace node;

namespace node_pdl {

  static Persistent<Object> functionMap;

  static Handle<Value> ThrowPDLError(const char* name) {
    return ThrowException(Exception::Error(String::Concat(
      String::Concat(
        String::New(name),
        String::New(": ")
      ),
      String::New(PDL_GetError())
    )));
  }

  static Handle<Value> Init(const Arguments& args) {
    HandleScope scope;

    if (!(args.Length() == 1 && args[0]->IsNumber())) {
      return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected Init(Number)")));
    }

    int flags = (args[0]->Int32Value());
    PDL_Err err = PDL_Init(flags);
    if (err != PDL_NOERROR) return ThrowPDLError(__func__);
    return Undefined();
  }

  static Handle<Value> Quit(const Arguments& args) {
    HandleScope scope;

    if (!(args.Length() == 0)) {
      return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected Quit()")));
    }

    PDL_Quit();
    return Undefined();
  }

  static Handle<Value> BannerMessagesEnable(const Arguments& args) {
    HandleScope scope;

    if (!(args.Length() == 1 && args[0]->IsBoolean())) {
      return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected BannerMessagesEnable(Boolean)")));
    }

    bool enable = args[0]->BooleanValue();

    PDL_Err err = PDL_BannerMessagesEnable((PDL_bool)enable);
    if (err != PDL_NOERROR) return ThrowPDLError(__func__);

    return Undefined();
  }

  static Handle<Value> CustomPauseUiEnable(const Arguments& args) {
    HandleScope scope;

    if (!(args.Length() == 1 && args[0]->IsBoolean())) {
      return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected CustomPauseUiEnable(Boolean)")));
    }

    bool enable = args[0]->BooleanValue();

    PDL_Err err = PDL_CustomPauseUiEnable((PDL_bool)enable);
    if (err != PDL_NOERROR) return ThrowPDLError(__func__);

    return Undefined();
  }

  static Handle<Value> EnableCompass(const Arguments& args) {
    HandleScope scope;

    if (!(args.Length() == 1 && args[0]->IsBoolean())) {
      return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected EnableCompass(Boolean)")));
    }

    bool activate = args[0]->BooleanValue();

    PDL_Err err = PDL_EnableCompass((PDL_bool)activate);
    if (err != PDL_NOERROR) return ThrowPDLError(__func__);

    return Undefined();
  }

  static Handle<Value> EnableLocationTracking(const Arguments& args) {
    HandleScope scope;

    if (!(args.Length() == 1 && args[0]->IsBoolean())) {
      return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected EnableLocationTracking(Boolean)")));
    }

    bool activate = args[0]->BooleanValue();

    PDL_Err err = PDL_EnableLocationTracking((PDL_bool)activate);
    if (err != PDL_NOERROR) return ThrowPDLError(__func__);

    return Undefined();
  }

  static Handle<Value> GesturesEnable(const Arguments& args) {
    HandleScope scope;

    if (!(args.Length() == 1 && args[0]->IsBoolean())) {
      return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected GesturesEnable(Boolean)")));
    }

    bool enable = args[0]->BooleanValue();

    PDL_Err err = PDL_GesturesEnable((PDL_bool)enable);
    if (err != PDL_NOERROR) return ThrowPDLError(__func__);

    return Undefined();
  }

  static Handle<Value> GetDataFilePath(const Arguments& args) {
    HandleScope scope;

    if (!(args.Length() == 1 && args[0]->IsString())) {
      return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected GetDataFilePath(String)")));
    }

    String::Utf8Value dataFileName(args[0]);
    int bufferLen = 256;
    char buffer[bufferLen];

    PDL_Err err = PDL_GetDataFilePath(*dataFileName, buffer, bufferLen);
    if (err != PDL_NOERROR) return ThrowPDLError(__func__);
    return String::New((char*)buffer);
  }

  static Handle<Value> GetDeviceName(const Arguments& args) {
    HandleScope scope;

    if (!(args.Length() == 0)) {
      return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected GetDeviceName()")));
    }

    char buffer[64];
    PDL_Err err = PDL_GetDeviceName(buffer, 64);
    if (err != PDL_NOERROR) return ThrowPDLError(__func__);
    return String::New((char*)buffer);
  }

  static Handle<Value> GetLanguage(const Arguments& args) {
    HandleScope scope;

    if (!(args.Length() == 0)) {
      return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected GetLanguage()")));
    }

    char buffer[64];
    PDL_Err err = PDL_GetLanguage(buffer, 64);
    if (err != PDL_NOERROR) return ThrowPDLError(__func__);
    return String::New((char*)buffer);
  }

  static Handle<Value> GetPDKVersion(const Arguments& args) {
    HandleScope scope;

    if (!(args.Length() == 0)) {
      return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected GetPDKVersion()")));
    }

    return Number::New(PDL_GetPDKVersion());
  }


  static Handle<Value> GetRegionCountryCode(const Arguments& args) {
    HandleScope scope;

    if (!(args.Length() == 0)) {
      return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected GetRegionCountryCode()")));
    }

    char buffer[64];
    PDL_Err err = PDL_GetRegionCountryCode(buffer, 64);
    if (err != PDL_NOERROR) return ThrowPDLError(__func__);
    return String::New((char*)buffer);
  }

  static Handle<Value> GetScreenMetrics(const Arguments& args) {
    HandleScope scope;

    if (!(args.Length() == 0)) {
      return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected GetScreenMetrics()")));
    }

    PDL_ScreenMetrics outMetrics;
    PDL_Err err = PDL_GetScreenMetrics(&outMetrics);
    if (err != PDL_NOERROR) return ThrowPDLError(__func__);

    Local<Object> metrics = Object::New();
    metrics->Set(String::New("horizontalPixels"), Number::New(outMetrics.horizontalPixels));
    metrics->Set(String::New("verticalPixels"), Number::New(outMetrics.verticalPixels));
    metrics->Set(String::New("horizontalDPI"), Number::New(outMetrics.horizontalDPI));
    metrics->Set(String::New("verticalDPI"), Number::New(outMetrics.verticalDPI));
    metrics->Set(String::New("aspectRatio"), Number::New(outMetrics.aspectRatio));

    return scope.Close(metrics);
  }


  static Handle<Value> IsPlugin(const Arguments& args) {
    HandleScope scope;

    if (!(args.Length() == 0)) {
      return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected IsPlugin()")));
    }

    return Boolean::New(PDL_IsPlugin());
  }

  static Handle<Value> LaunchBrowser(const Arguments& args) {
    HandleScope scope;

    if (!(args.Length() == 1 && args[0]->IsString())) {
      return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected LaunchBrowser(String)")));
    }

    String::Utf8Value url(args[0]);

    PDL_Err err = PDL_LaunchBrowser(*url);
    if (err != PDL_NOERROR) return ThrowPDLError(__func__);
    return Undefined();
  }

  static Handle<Value> LaunchEmail(const Arguments& args) {
    HandleScope scope;

    if (!(args.Length() == 2 && args[0]->IsString() && args[1]->IsString())) {
      return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected LaunchEmail(String, String)")));
    }

    String::Utf8Value subject(args[0]);
    String::Utf8Value body(args[1]);

    PDL_Err err = PDL_LaunchEmail(*subject, *body);
    if (err != PDL_NOERROR) return ThrowPDLError(__func__);
    return Undefined();
  }

  static Handle<Value> LaunchEmailTo(const Arguments& args) {
    HandleScope scope;

    if (!(args.Length() == 3 && args[0]->IsString() && args[1]->IsString() && args[2]->IsArray())) {
      return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected LaunchEmailTo(String, String, Array)")));
    }

    String::Utf8Value subject(args[0]);
    String::Utf8Value body(args[1]);

    Array* arr = Array::Cast(*args[2]);
    int length = arr->Length();
    const char* recipients[length];
    for (int i = 0; i < length; i++) {
      String::Utf8Value recipient(arr->Get(i));
      recipients[i] = strdupa(*recipient);
    }

    PDL_Err err = PDL_LaunchEmailTo(*subject, *body, length, recipients);
    if (err != PDL_NOERROR) return ThrowPDLError(__func__);
    return Undefined();
  }

  static Handle<Value> Minimize(const Arguments& args) {
    HandleScope scope;

    if (!(args.Length() == 0)) {
      return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected Minimize()")));
    }

    PDL_Err err = PDL_Minimize();
    if (err != PDL_NOERROR) return ThrowPDLError(__func__);
    return Undefined();
  }

  static Handle<Value> NotifyMusicPlaying(const Arguments& args) {
    HandleScope scope;

    if (!(args.Length() == 1 && args[0]->IsBoolean())) {
      return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected NotifyMusicPlaying(Boolean)")));
    }

    bool musicPlaying = args[0]->BooleanValue();
    PDL_Err err = PDL_NotifyMusicPlaying((PDL_bool)musicPlaying);
    if (err != PDL_NOERROR) return ThrowPDLError(__func__);
    return Undefined();
  }

  static Handle<Value> ScreenTimeoutEnable(const Arguments& args) {
    HandleScope scope;

    if (!(args.Length() == 1 && args[0]->IsBoolean())) {
      return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected ScreenTimeoutEnable(Boolean)")));
    }

    bool enable = args[0]->BooleanValue();
    PDL_Err err = PDL_ScreenTimeoutEnable((PDL_bool)enable);
    if (err != PDL_NOERROR) return ThrowPDLError(__func__);
    return Undefined();
  }

  static Handle<Value> SetOrientation(const Arguments& args) {
    HandleScope scope;

    if (!(args.Length() == 1 && args[0]->IsNumber())) {
      return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected SetOrientation(Number)")));
    }

    int orientationNum = args[0]->Int32Value();

    PDL_Orientation orientation;
    switch (orientationNum) {
      case 0: orientation = PDL_ORIENTATION_0; break;
      case 90: orientation = PDL_ORIENTATION_90; break;
      case 180: orientation = PDL_ORIENTATION_180; break;
      case 270: orientation = PDL_ORIENTATION_270; break;
      default: return ThrowException(Exception::TypeError(String::New("Orientation must be 0, 90, 180, or 270")));
    }

    PDL_Err err = PDL_SetOrientation(orientation);
    if (err != PDL_NOERROR) return ThrowPDLError(__func__);
    return Undefined();
  }

  static Handle<Value> Vibrate(const Arguments& args) {
    HandleScope scope;

    if (!(args.Length() == 2 && args[0]->IsNumber() && args[1]->IsNumber())) {
      return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected Vibrate(Number, Number)")));
    }

    int periodMS = args[0]->Int32Value();
    int durationMS = args[1]->Int32Value();

    PDL_Err err = PDL_Vibrate(periodMS, durationMS);
    if (err != PDL_NOERROR) return ThrowPDLError(__func__);
    return Undefined();
  }

  static Handle<Value> GetHardwareID(const Arguments& args) {
    HandleScope scope;

    if (!(args.Length() == 0)) {
      return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected GetHardwareID()")));
    }

    return Number::New(PDL_GetHardwareID());
  }

  static Handle<Value> GetUniqueID(const Arguments& args) {
    HandleScope scope;

    if (!(args.Length() == 0)) {
      return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected GetUniqueID()")));
    }

    char buffer[64];
    PDL_Err err = PDL_GetUniqueID(buffer, 64);
    if (err != PDL_NOERROR) return ThrowPDLError(__func__);

    return String::New((char*) buffer);
  }

  static Handle<Value> SetFirewallPortStatus(const Arguments& args) {
    HandleScope scope;

    if (!(args.Length() == 2 && args[0]->IsNumber() && args[1]->IsBoolean())) {
      return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected SetFirewallPortStatus(Number, Boolean)")));
    }

    int port = args[0]->Int32Value();
    bool open = args[1]->BooleanValue();

    PDL_Err err = PDL_SetFirewallPortStatus(port, (PDL_bool)open);
    if (err != PDL_NOERROR) return ThrowPDLError(__func__);

    return Undefined();
  }

  static Handle<Value> SetKeyboardState(const Arguments& args) {
    HandleScope scope;

    if (!(args.Length() == 1 && args[0]->IsBoolean())) {
      return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected SetKeyboardState(Boolean)")));
    }

    bool visible = args[0]->BooleanValue();
    PDL_Err err = PDL_SetKeyboardState((PDL_bool)visible);
    if (err != PDL_NOERROR) return ThrowPDLError(__func__);
    return Undefined();
  }

  static Handle<Value> JSRegistrationComplete(const Arguments& args) {
    HandleScope scope;

    if (!(args.Length() == 0)) {
      return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected JSRegistrationComplete()")));
    }

    PDL_Err err = PDL_JSRegistrationComplete();
    if (err != PDL_NOERROR) return ThrowPDLError(__func__);
    return Undefined();
  }

  static Handle<Value> CallJS(const Arguments& args) {
    HandleScope scope;

    if (!(args.Length() >= 1 && args[0]->IsString())) {
      return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected CallJS(String, ...)")));
    }

    String::Utf8Value name(args[0]);

    // Copy the rest of the args as an array of strings
    int length = args.Length() - 1;
    const char* newArgs[length];
    for (int i = 0; i < length; i++) {
      String::Utf8Value arg(args[i + 1]);
      newArgs[i] = strdupa(*arg);
    }

    PDL_Err err = PDL_CallJS(*name, newArgs, length);

    if (err != PDL_NOERROR) return ThrowPDLError(__func__);
    return Undefined();
  }


  PDL_bool JSRouter(PDL_JSParameters *params) {
    const char* name = PDL_GetJSFunctionName(params);
    const int numParams = PDL_GetNumJSParams(params);

    Handle<Value> argv[numParams];
    for (int i = 0; i < numParams; i++) {
      argv[i] = String::New(PDL_GetJSParamString(params, i));
    }

    Handle<Function>::Cast(functionMap->Get(String::New(name)))->Call(functionMap, numParams, argv);
    printf("ME!\n");
    return PDL_TRUE;
  }

  static Handle<Value> RegisterPollingJSHandler(const Arguments& args) {
    HandleScope scope;

    if (!(args.Length() == 2 && args[0]->IsString() && args[1]->IsFunction())) {
      return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected RegisterPollingJSHandler(String, Function)")));
    }

    String::Utf8Value name(args[0]);
    functionMap->Set(args[0], args[1]);
    PDL_Err err = PDL_RegisterPollingJSHandler(*name, JSRouter);
    if (err != PDL_NOERROR) return ThrowPDLError(__func__);

    return Undefined();
  }

  static Handle<Value> HandleJSCalls(const Arguments& args) {
    HandleScope scope;

    if (!(args.Length() == 0)) {
      return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected HandleJSCalls()")));
    }

    int num = PDL_HandleJSCalls();
    if (num < 0) return ThrowPDLError(__func__);

    return Number::New(num);
  }

}


extern "C" void
init(Handle<Object> target)
{
  HandleScope scope;

  node_pdl::functionMap = Persistent<Object>::New(Object::New());

  // Hardware Types
  target->Set(String::New("HARDWARE_UNKNOWN"), Number::New(HARDWARE_UNKNOWN));
  target->Set(String::New("HARDWARE_PRE"), Number::New(HARDWARE_PRE));
  target->Set(String::New("HARDWARE_PRE_PLUS"), Number::New(HARDWARE_PRE_PLUS));
  target->Set(String::New("HARDWARE_PIXI"), Number::New(HARDWARE_PIXI));
  target->Set(String::New("HARDWARE_VEER"), Number::New(HARDWARE_VEER));
  target->Set(String::New("HARDWARE_PRE_2"), Number::New(HARDWARE_PRE_2));
  target->Set(String::New("HARDWARE_PRE_3"), Number::New(HARDWARE_PRE_3));
  target->Set(String::New("HARDWARE_TOUCHPAD"), Number::New(HARDWARE_TOUCHPAD));

  // Custom User Events
  target->Set(String::New("GPS_UPDATE"), Number::New(PDL_GPS_UPDATE));
  target->Set(String::New("GPS_FAILURE"), Number::New(PDL_GPS_FAILURE));
  target->Set(String::New("COMPASS"), Number::New(PDL_COMPASS));

  // Bound functions
  NODE_SET_METHOD(target, "init", node_pdl::Init);
  NODE_SET_METHOD(target, "quit", node_pdl::Quit);
  NODE_SET_METHOD(target, "bannerMessagesEnable", node_pdl::BannerMessagesEnable);
  NODE_SET_METHOD(target, "customPauseUIEnable", node_pdl::CustomPauseUiEnable);
  NODE_SET_METHOD(target, "enableCompass", node_pdl::EnableCompass);
  NODE_SET_METHOD(target, "enableLocationTracking", node_pdl::EnableLocationTracking);
  NODE_SET_METHOD(target, "gesturesEnable", node_pdl::GesturesEnable);
  NODE_SET_METHOD(target, "getDataFilePath", node_pdl::GetDataFilePath);
  NODE_SET_METHOD(target, "getDeviceName", node_pdl::GetDeviceName);
  NODE_SET_METHOD(target, "getLanguage", node_pdl::GetLanguage);
  NODE_SET_METHOD(target, "getPDKVersion", node_pdl::GetPDKVersion);
  NODE_SET_METHOD(target, "getRegionCountryCode", node_pdl::GetRegionCountryCode);
  NODE_SET_METHOD(target, "getScreenMetrics", node_pdl::GetScreenMetrics);
  NODE_SET_METHOD(target, "isPlugin", node_pdl::IsPlugin);
  NODE_SET_METHOD(target, "launchBrowser", node_pdl::LaunchBrowser);
  NODE_SET_METHOD(target, "launchEmail", node_pdl::LaunchEmail);
  NODE_SET_METHOD(target, "launchEmailTo", node_pdl::LaunchEmailTo);
  NODE_SET_METHOD(target, "minimize", node_pdl::Minimize);
  NODE_SET_METHOD(target, "notifyMusicPlaying", node_pdl::NotifyMusicPlaying);
  NODE_SET_METHOD(target, "screenTimeoutEnable", node_pdl::ScreenTimeoutEnable);
  NODE_SET_METHOD(target, "setOrientation", node_pdl::SetOrientation);
  NODE_SET_METHOD(target, "vibrate", node_pdl::Vibrate);

  NODE_SET_METHOD(target, "getHardwareID", node_pdl::GetHardwareID);
  NODE_SET_METHOD(target, "getUniqueID", node_pdl::GetUniqueID);
  NODE_SET_METHOD(target, "setFirewallPortStatus", node_pdl::SetFirewallPortStatus);
  NODE_SET_METHOD(target, "setKeyboardState", node_pdl::SetKeyboardState);
  NODE_SET_METHOD(target, "jsRegistrationComplete", node_pdl::JSRegistrationComplete);
  NODE_SET_METHOD(target, "callJS", node_pdl::CallJS);
  NODE_SET_METHOD(target, "registerPollingJSHandler", node_pdl::RegisterPollingJSHandler);
  NODE_SET_METHOD(target, "handleJSCalls", node_pdl::HandleJSCalls);

}




