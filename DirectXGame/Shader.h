#pragma once
#include <string>
#include <d3d12.h>

//Shaderクラス
class Shader {
public:

	//シェーダーファイルを読み込み、コンパイル済みデータを生成する
	void Load(const std::wstring& filePath, const std::string& shaderMaodel);
	
	//生成したコンパイル済みデータを取得する
	ID3D10Blob* GetBlob();

	//コンストラクタ
	Shader();
	//デストラクタ
	~Shader();

	private:
	ID3D10Blob* blob_ = nullptr;
};