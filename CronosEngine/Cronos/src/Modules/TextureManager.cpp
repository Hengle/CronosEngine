#include "Providers/cnpch.h"

#include "TextureManager.h"
#include "Application.h"

#include "DevIL/include/ilut.h"
//#include <codecvt>

namespace Cronos {

	TextureManager::TextureManager(Application* app, bool start_enabled) : Module(app, "Module TextureManager", start_enabled)
	{
	}

	TextureManager::~TextureManager()
	{
	}

	bool TextureManager::OnInit()
	{
		ilInit();
		iluInit();
		ilutInit();
		ilEnable(IL_CONV_PAL);
		ilutEnable(ILUT_OPENGL_CONV);
		ilutRenderer(ILUT_OPENGL);

		glEnable(GL_TEXTURE_2D);

		return true;
	}

	bool TextureManager::OnCleanUp()
	{
		ilShutDown();
		return true;
	}

	Texture* TextureManager::CreateTexture(const char* path)
	{
		Texture* ret = new Texture(path);
		return ret;
	}

	uint TextureManager::CreateTextureAndData(const char * path,ImVec2& resolution)
	{
		uint TempTex;
		ILuint TempImage;
		ilGenImages(1, &TempImage);
		ilBindImage(TempImage);

		if (ilLoadImage((const ILstring)path)) {

			TempTex = ilutGLBindTexImage();
			iluFlipImage();

			int height, width, level, bpp, format;

			height = ilGetInteger(IL_IMAGE_HEIGHT);
			width = ilGetInteger(IL_IMAGE_WIDTH);
			bpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
			format = ilGetInteger(IL_IMAGE_FORMAT);

			resolution.x = width;
			resolution.y = height;

			ILubyte *Data = ilGetData();
			if (Data) {
				glEnable(GL_TEXTURE_2D);
				//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				glGenTextures(1, &TempTex);
				glBindTexture(GL_TEXTURE_2D, TempTex);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, Data);
				glGenerateMipmap(GL_TEXTURE_2D);
				//Erase textures
				glBindTexture(GL_TEXTURE_2D, 0);
				ilBindImage(0);
				ilDeleteImage(TempImage);
			}
			else {
				std::cout << "failed to load the data of " << path << std::endl;
			}

		}
		else {
			ilDeleteImage(TempImage);
			return 0;
		}

		return TempTex;
	}
}