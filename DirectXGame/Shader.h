#pragma once
#include <string>
#include <d3d12.h>
#include <d3dcompiler.h>
#include<dxcapi.h>

//Shaderクラス
class Shader {
public:

	//シェーダーファイルを読み込み、コンパイル済みデータを生成する
	void Load(const std::wstring& filePath, const std::wstring& shaderMaodel);
	void LoadDxc(const std::wstring& filePath, const std::wstring& shaderModel);

	//生成したコンパイル済みデータを取得する
	ID3D10Blob* GetBlob();
	IDxcBlob* GetDxcBlob();

	//コンストラクタ
	Shader();
	//デストラクタ
	~Shader();

	private:
	ID3D10Blob* blob_ = nullptr;
	    IDxcBlob* dxcBlob_ = nullptr;
};