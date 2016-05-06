#include <node.h>
#include <v8.h>
#ifdef _WIN32
#include <windows.h>
#include <VersionHelpers.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
using namespace v8;


void dwmColor(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);
	DWORD pcrColorization = 0;
	DWORD regColorizationColor = 0;
	DWORD regColorizationColorBalance = 0;
	DWORD size = sizeof(DWORD);
	BOOL pfOpaqueBlend = TRUE;
	Local<Object> obj = Object::New(isolate);
	if (!IsWindowsVistaOrGreater()) {
		args.GetReturnValue().Set(false);
		return;
	}
	HRESULT hr = DwmGetColorizationColor(&pcrColorization, &pfOpaqueBlend);
	if (hr != S_OK) {
		args.GetReturnValue().Set(false);
		return;
	}
	obj->Set(String::NewFromUtf8(isolate, "color"), v8::Uint32::New(isolate, pcrColorization));
	obj->Set(String::NewFromUtf8(isolate, "opaque"), v8::Boolean::New(isolate, pfOpaqueBlend));
	args.GetReturnValue().Set(obj);
	return;
	args.GetReturnValue().Set(false);
}
#else
using namespace v8;
void dwmColor(const FunctionCallbackInfo<Value>& args) {
	args.GetReturnValue().Set(false);
}
#endif

void Init(Handle<Object> exports) {
	Isolate* isolate = Isolate::GetCurrent();
	exports->Set(String::NewFromUtf8(isolate, "dwmColor"),
		FunctionTemplate::New(isolate, dwmColor)->GetFunction());
}

NODE_MODULE(color, Init)