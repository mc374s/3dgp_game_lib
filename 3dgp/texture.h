#ifndef _TEXTURE_H_
#define _TEXUTRE_H_

#include <d3d11.h>

namespace GLC {

	class Texture 
	{
	public:
		// Load a texture resource array
		static void Load(Texture textures[]);

	private:
		ID3D11ShaderResourceView* shaderResourceViewRef;

	public:
		char fileName[256];
		unsigned int width, height;

		Texture();
		~Texture();

		Texture(const char* fileName);

		// Return a Loaded ShaderResourceView*
		ID3D11ShaderResourceView* GetData();

		// Load a texture resource from @fileName
		void Load();
	};

}; // namespace GLC


#endif // !_TEXTURE_H_
