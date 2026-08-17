#include "Shader.h"
#include "MiscUtility.h"
#include <d3dcompiler.h>
#include <cassert>
#include <dxcapi.h>
#pragma comment(lib,"dxcompiler.lib")

void Shader::Load(const std::wstring& filePath, const std::wstring& shaderModel) {
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	std::string mdShaderModel = ConvertString(shaderModel);

	HRESULT hr =D3DCompileFromFile(
		filePath.c_str(), 
		nullptr, 
		D3D_COMPILE_STANDARD_FILE_INCLUDE, 
		"main", mdShaderModel.c_str(), 
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 
		0, &shaderBlob, &errorBlob);
	if (FAILED(hr)) {
		if (errorBlob) {
			OutputDebugStringA(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
			errorBlob->Release();
		}
		assert(false);
	}
	// 生成したshaderBlobを返す
	blob_ = shaderBlob;
}

void Shader::LoadDxc(const std::wstring& filePath, const std::wstring& shaderModel) {
	static IDxcUtils* dxcUtils = nullptr;
	static IDxcCompiler3* dxcCompiler = nullptr;
	static IDxcIncludeHandler* includeHandler = nullptr;

	HRESULT hr;
	if (dxcUtils == nullptr) {
		hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
		assert(SUCCEEDED(hr));
	}
	if (dxcCompiler == nullptr) {
		hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
		assert(SUCCEEDED(hr));
	}
	if (includeHandler == nullptr) {
		hr = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
		assert(SUCCEEDED(hr));
	}
	IDxcBlobEncoding* shaderSource = nullptr;
	hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	assert(SUCCEEDED(hr));
	DxcBuffer shaderSourceBuffer{};
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;

	LPCWSTR arguments[] = {
	    filePath.c_str(),
		L"-E", L"main", 
		L"-T", shaderModel.c_str(), 
		L"-Zi", 
		L"-Qembed_debug", 
		L"-Od",
	    L"-Zpr", 
		L"-I", L"Resources/shaders",
	};
	IDxcResult* shaderResult = nullptr;
	hr = dxcCompiler->Compile(&shaderSourceBuffer, 
		arguments, 
		_countof(arguments), includeHandler, 
		IID_PPV_ARGS(&shaderResult)
	);
	assert(SUCCEEDED(hr));
	IDxcBlobUtf8* shaderError = nullptr;
	IDxcBlobWide* nameBlob = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError),&nameBlob);
	if (shaderError !=nullptr && shaderError->GetStringLength()!=0) {
		OutputDebugStringA(shaderError->GetStringPointer());
		assert(false);
	}
	IDxcBlob* shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), &nameBlob);
	assert(SUCCEEDED(hr));
	shaderSource->Release();
	shaderResult->Release();
	dxcBlob_ = shaderBlob;
}

ID3D10Blob* Shader::GetBlob() { 
	return blob_; }
IDxcBlob* Shader::GetDxcBlob() { 
	return dxcBlob_; 
}
Shader::Shader(){
}

//デストラクタ
Shader::~Shader() {
	if (blob_) {
		blob_->Release();
		blob_ = nullptr;
	}
	if (dxcBlob_) {
		dxcBlob_->Release();
		dxcBlob_ = nullptr;
	}
}
