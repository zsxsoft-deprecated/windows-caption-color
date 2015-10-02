#include <node.h>
#include <v8.h>
#ifdef _WIN32
#include <windows.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#endif
using namespace v8;

bool IsWindowsVista() {
	DWORD version = GetVersion();
	DWORD major = (DWORD) (LOBYTE(LOWORD(version)));
	DWORD minor = (DWORD) (HIBYTE(LOWORD(version)));
	return (major >= 6);
}

char* analyzeColor(DWORD pcrColorization) {
	unsigned long h = pcrColorization;
	char buf[9];
	sprintf(buf, "%lx", h);
	return (char*)buf;
}

void dwmColor(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);
	DWORD pcrColorization = 0;
	DWORD regColorizationColor = 0;
	DWORD regColorizationColorBalance = 0;
	DWORD size = sizeof(DWORD);
	BOOL pfOpaqueBlend = TRUE;
	Local<Object> obj = Object::New(isolate);
	#ifdef _WIN32
	if (!IsWindowsVista()) {
		args.GetReturnValue().Set(false);
		return;
	}
	HRESULT hr = DwmGetColorizationColor(&pcrColorization, &pfOpaqueBlend);
	if (hr != S_OK) {
		args.GetReturnValue().Set(false);
		return;
	}
	obj->Set(String::NewFromUtf8(isolate, "color"), v8::String::NewFromUtf8(isolate, analyzeColor(pcrColorization)));
	obj->Set(String::NewFromUtf8(isolate, "opaque"), v8::Boolean::New(isolate, pfOpaqueBlend));
	args.GetReturnValue().Set(obj);
	return;
	#endif
	args.GetReturnValue().Set(false);
}

void Init(Handle<Object> exports) {
	Isolate* isolate = Isolate::GetCurrent();
	exports->Set(String::NewFromUtf8(isolate, "dwmColor"),
		FunctionTemplate::New(isolate, dwmColor)->GetFunction());
}

NODE_MODULE(color, Init)
