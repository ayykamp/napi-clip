#include "../cpp_modules/clip/clip.h"
#include <iostream>

#include <napi.h>
using namespace Napi;

Boolean clear(const CallbackInfo& args) {
	Napi::Env env = args.Env();

	return Boolean::New(env, clip::clear());
}

String get_text(const CallbackInfo& args) {
	Napi::Env env = args.Env();
	std::string value;
	clip::get_text(value);
  return String::New(env, value);
}

Boolean set_text(const CallbackInfo& args) {
	Napi::Env env = args.Env();

	if (args.Length() != 1) {
		Error::New(env, "Expected exactly one argument").ThrowAsJavaScriptException();
		return Boolean::New(env, false);
	}

	if (!args[0].IsString()) {
		Error::New(env, "Expected String as first argument").ThrowAsJavaScriptException();
		return Boolean::New(env, false);
	}
	
	std::string value = args[0].ToString();
	return Boolean::New(env, clip::set_text(value));
}

Boolean has_image(const CallbackInfo& args) {
	Napi::Env env = args.Env();

	bool has_image_value = clip::has(clip::image_format());
	
	return Boolean::New(env, has_image_value);
}

Boolean has_text(const CallbackInfo& args) {
	Napi::Env env = args.Env();

	bool has_text_value = clip::has(clip::text_format());
	
	return Boolean::New(env, has_text_value);
}

// https://github.com/dacap/clip/issues/28
/* Boolean is_empty(const CallbackInfo& args) {
	Napi::Env env = args.Env();
	bool has_empty_format = clip::has(clip::empty_format());
	
	return Boolean::New(env, has_empty_format);
} */

Object get_image(const CallbackInfo& args) {
	Napi::Env env = args.Env();

	if (!clip::has(clip::image_format())) {
		Error::New(env, "No image in clipboard.").ThrowAsJavaScriptException();
		return Object::New(env);
	}

	clip::image img;
	if (!clip::get_image(img)) {
		Error::New(env, "Error getting image from clipboard.").ThrowAsJavaScriptException();
		return Object::New(env);
	}

	clip::image_spec spec = img.spec();

	Object img_obj = Object::New(env);
	Object spec_obj = Object::New(env);

	// this may be imporved, not very DRY
	spec_obj.Set(String::New(env, "width"), Number::New(env, spec.width));
	spec_obj.Set(String::New(env, "height"), Number::New(env, spec.height));
	spec_obj.Set(String::New(env, "bitsPerPixel"), Number::New(env, spec.bits_per_pixel));
	spec_obj.Set(String::New(env, "bytesPerRow"), Number::New(env, spec.bytes_per_row));
	spec_obj.Set(String::New(env, "redMask"), Number::New(env, spec.red_mask));
	spec_obj.Set(String::New(env, "greenMask"), Number::New(env, spec.green_mask));
	spec_obj.Set(String::New(env, "blueMask"), Number::New(env, spec.blue_mask));
	spec_obj.Set(String::New(env, "alphaMask"), Number::New(env, spec.alpha_mask));
	spec_obj.Set(String::New(env, "redShift"), Number::New(env, spec.red_shift));
	spec_obj.Set(String::New(env, "greenShift"), Number::New(env, spec.green_shift));
	spec_obj.Set(String::New(env, "blueShift"), Number::New(env, spec.blue_shift));
	spec_obj.Set(String::New(env, "alphaShift"), Number::New(env, spec.alpha_shift));

	img_obj.Set(String::New(env, "spec"), spec_obj);
	/* char *data = img.data();
	const size_t length = spec.width * spec.height;

	std::cout << "buffer" << '\n';
	for (size_t i = 0; i < 4; i++)
	{
		std::cout << std::hex << static_cast<uint8_t>(data[i]) << '\n';
	}
	ArrayBuffer array_buf = ArrayBuffer::New(env, 4);
	std::cout << "ARrayBuf" << '\n';
	for (size_t i = 0; i < 4; i++)
	{
	  std::cout << "inside loop" << '\n';
		array_buf[i] = data[i];
	}
	Uint32Array xddd = Uint32Array::New(env, 1, array_buf, 0);
	std::cout << "ARrayBuf" << '\n';

	std::cout << "Uint32" << '\n';
	for (size_t i = 0; i < 4; i++)
	{
		std::cout << std::hex << xddd[i] << '\n';
	} */
	
	const size_t element_length = spec.width * spec.height;
	const size_t byte_length = element_length * (spec.bits_per_pixel / 8);

	char *data = img.data();
	char *data_copy = new char[byte_length];
	std::copy(data, data + byte_length, data_copy);
  
	ArrayBuffer array_buffer = ArrayBuffer::New(env, data_copy, byte_length,
		[](Env env, void* finalizeData) {
			delete[] static_cast<uint32_t*>(finalizeData);
		});
	Uint32Array img_array = Uint32Array::New(env, element_length, array_buffer, 0);

	img_obj.Set(String::New(env, "data"), img_array);

	return img_obj;
}

Boolean set_image(const CallbackInfo& args) {
	
	Napi::Env env = args.Env();

	if (args.Length() != 2) {
		Error::New(env, "Expected exactly two arguments").ThrowAsJavaScriptException();
		return Boolean::New(env, false);
	}

	if (!args[0].IsTypedArray()) {
		Error::New(env, "Expected TypedArray as first argument").ThrowAsJavaScriptException();
		return Boolean::New(env, false);
	}

	if (!args[1].IsObject()) {
		Error::New(env, "Expected Object as second argument").ThrowAsJavaScriptException();
		return Boolean::New(env, false);
	}

	Uint32Array img_array = args[0].As<Uint32Array>();
	uint32_t* img_data = img_array.Data();
	
	Object spec_obj = args[1].ToObject();
	clip::image_spec spec;

	// very bad code never hire this guy
	spec.width = spec_obj.Has("width") ?
		 spec_obj.Get("width").ToNumber().Uint32Value(): 0;
	spec.height = spec_obj.Has("height") ?
		 spec_obj.Get("height").ToNumber().Uint32Value(): 0;
	spec.bits_per_pixel = spec_obj.Has("bitsPerPixel") ?
		 spec_obj.Get("bitsPerPixel").ToNumber().Uint32Value(): 32;
	spec.bytes_per_row = spec_obj.Has("bytesPerRow") ?
		 spec_obj.Get("bytesPerRow").ToNumber().Uint32Value(): spec.width * 4;
	spec.red_mask = spec_obj.Has("redMask") ?
		 spec_obj.Get("redMask").ToNumber().Uint32Value()		: 0xff;
	spec.green_mask = spec_obj.Has("greenMask") ?
		 spec_obj.Get("greenMask").ToNumber().Uint32Value()	: 0xff00;
	spec.blue_mask = spec_obj.Has("blueMask") ?
		 spec_obj.Get("blueMask").ToNumber().Uint32Value() 	: 0xff0000;
	spec.alpha_mask = spec_obj.Has("alphaMask") ?
		 spec_obj.Get("alphaMask").ToNumber().Uint32Value() : 0xff000000;
	spec.red_shift = spec_obj.Has("redShift") ?
		 spec_obj.Get("redShift").ToNumber().Uint32Value() 	: 0;
	spec.green_shift = spec_obj.Has("greenShift") ?
		 spec_obj.Get("greenShift").ToNumber().Uint32Value(): 8;
	spec.blue_shift = spec_obj.Has("blueShift") ?
		 spec_obj.Get("blueShift").ToNumber().Uint32Value()	: 16;
	spec.alpha_shift = spec_obj.Has("alphaShift") ?
		 spec_obj.Get("alphaShift").ToNumber().Uint32Value(): 24;
	
	clip::image img(img_data, spec);
	return Boolean::New(env, clip::set_image(img));
}

 static Object Initialize(Env env, Object exports) {
	exports["getText"] = Function::New(env, get_text);
	exports["setText"] = Function::New(env, set_text);
	exports["hasText"] = Function::New(env, has_text);
	exports["hasImage"] = Function::New(env, has_image);
	exports["getImage"] = Function::New(env, get_image);
	exports["setImage"] = Function::New(env, set_image);
	exports["clear"] = Function::New(env, clear);
	// exports["isEmpty"] = Function::New(env, isEmpty);
	return exports;
}

NODE_API_MODULE(napi_clip, Initialize)
