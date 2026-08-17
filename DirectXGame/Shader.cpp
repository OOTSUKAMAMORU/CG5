#include "Shader.h"
#include <d3dcompiler.h>
#include <cassert>

void Shader::Load(const std::wstring& filePath, const std::string& shaderModel) {
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	
	HRESULT hr =D3DCompileFromFile(
		filePath.c_str(), 
		nullptr, 
		D3D_COMPILE_STANDARD_FILE_INCLUDE, 
		"main", shaderModel.c_str(), 
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

ID3D10Blob* Shader::GetBlob() { 
	return blob_;
}
Shader::Shader(){
}

//デストラクタ
Shader::~Shader() {
	if (blob_) {
		blob_->Release();
		blob_ = nullptr;
	}
}
