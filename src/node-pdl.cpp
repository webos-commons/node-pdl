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
  NODE_SET_METHOD(target, "getHardwareID", node_pdl::GetHardwareID);
  NODE_SET_METHOD(target, "getUniqueID", node_pdl::GetUniqueID);
  NODE_SET_METHOD(target, "setFirewallPortStatus", node_pdl::SetFirewallPortStatus);
  NODE_SET_METHOD(target, "setKeyboardState", node_pdl::SetKeyboardState);
  NODE_SET_METHOD(target, "jsRegistrationComplete", node_pdl::JSRegistrationComplete);
  NODE_SET_METHOD(target, "callJS", node_pdl::CallJS);
  NODE_SET_METHOD(target, "registerPollingJSHandler", node_pdl::RegisterPollingJSHandler);
  NODE_SET_METHOD(target, "handleJSCalls", node_pdl::HandleJSCalls);
  
}




