#ifndef _TEMPLATE_H_
#define _TEMPLATE_H_

template <typename T>
class Singleton
{
public:

	static T* GetInstance() {
		static T instance;
		return &instance;
	};

};

template <typename D3D_TYPE>
struct D3D11_RESOURCES {
	D3D_TYPE pData;
	char pFileName[256];
	int fileRefNum = 0;
	D3D11_RESOURCES() :pData(NULL), fileRefNum(0) {};
	void Release() {
		fileRefNum--;
		if (fileRefNum <= 0) {
			SAFE_RELEASE(pData);
			ZeroMemory(pFileName, sizeof(pFileName));
			fileRefNum = 0;
		}
	};
};


#endif // !_TEMPLATE_H_
