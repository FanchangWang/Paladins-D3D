/*
* Paladins D3D Hack Source V1.2i by Nseven

How to compile:
- download and install "Microsoft Visual Studio Express 2015 for Windows DESKTOP" https://www.visualstudio.com/en-us/products/visual-studio-express-vs.aspx

- open paladinsd3d.vcxproj (not paladinsd3d.vcxproj.filters) with Visual Studio 2015 (Program Files (x86)\Microsoft Visual Studio 14.0\Common7\IDE\WDExpress.exe)
- select x86(32bit) 
- compile dll, press f7 or click the green triangle

x86 compiled dll will be in paladinsd3d\Release folder

If you share your dll with others, remove dependecy on vs runtime before compiling:
- click: project -> properties -> configuration properties -> C/C++ -> code generation -> runtime library: Multi-threaded (/MT)

Menu key:
- insert

Logging:
ALT + CTRL + L toggles logger
- press O to decrease values
- press P to increase, hold down P key until a texture changes
- press I to log values of changed textures
*/


#include "main.h" //less important stuff & helper funcs here

typedef HRESULT(APIENTRY *SetStreamSource)(IDirect3DDevice9*, UINT, IDirect3DVertexBuffer9*, UINT, UINT);
HRESULT APIENTRY SetStreamSource_hook(IDirect3DDevice9*, UINT, IDirect3DVertexBuffer9*, UINT, UINT);
SetStreamSource SetStreamSource_orig = 0;

typedef HRESULT(APIENTRY *SetVertexDeclaration)(IDirect3DDevice9*, IDirect3DVertexDeclaration9*);
HRESULT APIENTRY SetVertexDeclaration_hook(IDirect3DDevice9*, IDirect3DVertexDeclaration9*);
SetVertexDeclaration SetVertexDeclaration_orig = 0;

typedef HRESULT(APIENTRY *SetVertexShaderConstantF)(IDirect3DDevice9*, UINT, const float*, UINT);
HRESULT APIENTRY SetVertexShaderConstantF_hook(IDirect3DDevice9*, UINT, const float*, UINT);
SetVertexShaderConstantF SetVertexShaderConstantF_orig = 0;

typedef HRESULT(APIENTRY *DrawIndexedPrimitive)(IDirect3DDevice9*, D3DPRIMITIVETYPE, INT, UINT, UINT, UINT, UINT);
HRESULT APIENTRY DrawIndexedPrimitive_hook(IDirect3DDevice9*, D3DPRIMITIVETYPE, INT, UINT, UINT, UINT, UINT);
DrawIndexedPrimitive DrawIndexedPrimitive_orig = 0;

typedef HRESULT(APIENTRY* EndScene) (IDirect3DDevice9*);
HRESULT APIENTRY EndScene_hook(IDirect3DDevice9*);
EndScene EndScene_orig = 0;

typedef HRESULT(APIENTRY *Reset)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
HRESULT APIENTRY Reset_hook(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
Reset Reset_orig = 0;

typedef HRESULT(APIENTRY *CreateQuery)(IDirect3DDevice9*, D3DQUERYTYPE, IDirect3DQuery9**);
HRESULT APIENTRY CreateQuery_hook(IDirect3DDevice9*, D3DQUERYTYPE, IDirect3DQuery9**);
CreateQuery CreateQuery_orig = 0;

typedef HRESULT(APIENTRY *SetViewport)(IDirect3DDevice9*, CONST D3DVIEWPORT9*);
HRESULT APIENTRY SetViewport_hook(IDirect3DDevice9*, CONST D3DVIEWPORT9*);
SetViewport SetViewport_orig = 0;

//typedef HRESULT(APIENTRY *SetVertexShader)(IDirect3DDevice9*, IDirect3DVertexShader9*);
//HRESULT APIENTRY SetVertexShader_hook(IDirect3DDevice9*, IDirect3DVertexShader9*);
//SetVertexShader SetVertexShader_orig = 0;

typedef HRESULT(APIENTRY *SetPixelShader)(IDirect3DDevice9*, IDirect3DPixelShader9*);
HRESULT APIENTRY SetPixelShader_hook(IDirect3DDevice9*, IDirect3DPixelShader9*);
SetPixelShader SetPixelShader_orig = 0;

typedef HRESULT(APIENTRY *SetTexture)(IDirect3DDevice9*, DWORD, IDirect3DBaseTexture9*);
HRESULT APIENTRY SetTexture_hook(IDirect3DDevice9*, DWORD, IDirect3DBaseTexture9*);
SetTexture SetTexture_orig = 0;

//==========================================================================================================================

HRESULT APIENTRY SetStreamSource_hook(LPDIRECT3DDEVICE9 pDevice, UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT sStride)
{
	if (StreamNumber == 0)
	{
		Stride = sStride;

		//if (Stride == 12 && pStreamData)
		//{
			//pStreamData->GetDesc(&vdesc);
		//}
	}

	return SetStreamSource_orig(pDevice, StreamNumber, pStreamData, OffsetInBytes, sStride);
}

//=====================================================================================================================

HRESULT APIENTRY SetVertexShaderConstantF_hook(LPDIRECT3DDEVICE9 pDevice, UINT StartRegister, const float *pConstantData, UINT Vector4fCount)
{
	if (pConstantData != NULL)
	{
		//pConstantDataFloat = (float*)pConstantData;

		mStartRegister = StartRegister;
		mVector4fCount = Vector4fCount;
	}

	return SetVertexShaderConstantF_orig(pDevice, StartRegister, pConstantData, Vector4fCount);
}

//==========================================================================================================================

HRESULT APIENTRY SetVertexDeclaration_hook(LPDIRECT3DDEVICE9 pDevice, IDirect3DVertexDeclaration9* pDecl)
{
	if (pDecl != NULL)
	{
		HRESULT hr = pDecl->GetDeclaration(decl, &numElements);
		if (FAILED(hr))
		{
			//Log("GetDeclaration failed");
		}
	}

	return SetVertexDeclaration_orig(pDevice, pDecl);
}

//==========================================================================================================================

HRESULT APIENTRY DrawIndexedPrimitive_hook(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
	//models
	//Stride == 32 || Stride == 36 && decl->Type == 5 && numElements == 11 //models

	//outline shader
	//dWidth == 1024 && dHeight == 1024 && dFormat == 50 && Stride == 36 && decl->Type == 5 && numElements == 11 && vSize == 1216 && pSize == 164 && mStartRegister == 231 && mVector4fCount == 4 //outline shader

	//glow
	//dWidth == 1024 && dHeight == 1024 && dFormat == 50 && Stride == 36 && decl->Type == 5 && numElements == 11 && vSize == 1036 && pSize == 136 && mStartRegister == 235 && mVector4fCount == 1 //glow

	//hp bar
	//dwDataCRC == cb415efe && dWidth == 12 && dHeight == 8 && dFormat == 894720068 && Stride == 12 && NumVertices == 68 && primCount == 84 && decl->Type == 4 && numElements == 4 && vSize == 520 && pSize == 360 && mStartRegister == 6 && mVector4fCount == 12 //hp bar

	//both teams
	//dwDataCRC == 2b7da9f4 && dWidth == 1024 && dHeight == 1024 && dFormat == 50 && Stride == 12 && NumVertices == 66 && primCount == 84 && decl->Type == 4 && numElements == 4 && vSize == 520 && pSize == 360 && mStartRegister == 6 && mVector4fCount == 12

	//team blue (own team)
	//dwDataCRC == 0xad0d334b //blue hp bar

	//t
	//dwDataCRC == cb415efe && dWidth == 12 && dHeight == 8 && dFormat == 894720068 && Stride == 8 && NumVertices == 8 && primCount == 10 && decl->Type == 8 && numElements == 3 && vSize == 476 && pSize == 416 && mStartRegister == 8 && mVector4fCount == 2
	//dwDataCRC == d81bd7af && dWidth == 12 && dHeight == 8 && dFormat == 894720068 && Stride == 8 && NumVertices == 8 && primCount == 10 && decl->Type == 8 && numElements == 3 && vSize == 476 && pSize == 416 && mStartRegister == 8 && mVector4fCount == 2

	//unkn
	//dwDataCRC == 4ec9f327 && dWidth == 1024 && dHeight == 1024 && dFormat == 50 && Stride == 12 && NumVertices == 66 && primCount == 84 && decl->Type == 4 && numElements == 4 && vSize == 520 && pSize == 360 && mStartRegister == 6 && mVector4fCount == 12
	//dwDataCRC == 4ec9f327 && dWidth == 1024 && dHeight == 1024 && dFormat == 50 && Stride == 12 && NumVertices == 68 && primCount == 84 && decl->Type == 4 && numElements == 4 && vSize == 520 && pSize == 360 && mStartRegister == 6 && mVector4fCount == 12

	//wallhack
	if ((wallhack > 0) && (decl->Type == 5 && numElements == 11) || (Stride == 32 || Stride == 36)) //models
	{
		pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
		//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_NEVER);

		//chams
		if (wallhack == 3 && pSize != 164)
		{
			float sRed[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
			pDevice->SetPixelShaderConstantF(0, sRed, 1);//0, 4, 8
		}

		//glow
		if (wallhack == 2 && pSize == 136 && mStartRegister == 235) //hax shader (does not work for everyone)
		{
			pDevice->SetPixelShader(NULL);
		}

		DrawIndexedPrimitive_orig(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);

		if (wallhack == 3 && pSize != 164)
		{
			float sGreen[4] = { 0.0f, 0.5f, 0.0f, 0.0f };
			pDevice->SetPixelShaderConstantF(0, sGreen, 1);//0, 4, 8
		}

		pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	}

	//aimbot
	if(
		(aimbot == 1 && dwDataCRC == 0xcb415efe && dWidth == 12 && dHeight == 8 && /*dFormat == 894720068 && */Stride == 12 && NumVertices == 68 && primCount == 84)|| //orange far range
		(aimbot == 1 && dwDataCRC == 0xcb415efe && dWidth == 12 && dHeight == 8 && /*dFormat == 894720068 && */Stride == 12 && NumVertices == 66 && primCount == 84)|| //orange mid range
		(aimbot == 1 && dwDataCRC == 0xd81bd7af && dWidth == 12 && dHeight == 8 && /*dFormat == 894720068 && */Stride == 12 && NumVertices == 66 && primCount == 84) //red near range
		//(aimbot == 1 && dwDataCRC == 0x337194bd && dWidth == 1024 && dHeight == 1024 && dFormat == 50 && Stride == 12 && NumVertices == 66 && primCount == 84)//red/orange near/mid range 5+kills (no)
		//(aimbot == 1 && dwDataCRC != 0x2b7da9f4 && dWidth == 1024 && dHeight == 1024 && dFormat == 50 && Stride == 12 && primCount == 84)//orange mid range 5+kills (no)
		//(aimbot == 1 && dwDataCRC == 0xd4461a08 && dWidth == 1024 && dHeight == 1024 && dFormat == 50 && Stride == 12 && primCount == 84)//orange mid range 5+kills (no)
		)
		AddHPBarAim(pDevice, 1);

	//aimbot2
	if ((aimbot == 2 && Stride == 8 && NumVertices == 8 && primCount == 10) && (dwDataCRC == 0xd81bd7af || dwDataCRC == 0xcb415efe)) //team red/orange, enemy team
		AddTBarAim(pDevice, 1);

	//compatibility
	if (aimbot == 3 && Stride == 8 && NumVertices == 8 && primCount == 10)//all teams
		AddTBarAim(pDevice, 1);
	

	//crosshair
	//dwDataCRC == f14eb4d4 && dWidth == 256 && dHeight == 256 && Stride == 12 && NumVertices == 96 && primCount == 120 && decl->Type == 4 && numElements == 4 && vSize == 520 && pSize == 360 && mStartRegister == 6 && mVector4fCount == 36 && vdesc.Size == 1747616

	//remove, used for testing
	//if(NumVertices != 96 && Stride == 12 && primCount == 84 && decl->Type == 4 && numElements == 4 && vSize == 520 && pSize == 360 && mStartRegister == 6 && mVector4fCount == 12)
		//return D3D_OK; 
	
	/*
	//small bruteforce logger
	if (logger)
	{
		//log hp bar crc with f10
		if ((NumVertices != 96 && dwDataCRC != 0xad0d334b && Stride == 12 && primCount == 84 && decl->Type == 4 && numElements == 4 && vSize == 520 && pSize == 360 && mStartRegister == 6 && mVector4fCount == 12) && (GetAsyncKeyState(VK_F10) & 1))//hp
		//if(dwDataCRC != 0x337194bd && dWidth == 1024 && dHeight == 1024 && dFormat == 50 && Stride == 36 && decl->Type == 5 && numElements == 11 && vSize == 1216 && pSize == 164 && mStartRegister == 231 && mVector4fCount == 4) //outline shader
		//if ((NumVertices != 96 && Stride == 8 && NumVertices == 8 && primCount == 10) && (GetAsyncKeyState(VK_F10) & 1))//t
		Log("dwDataCRC == %x && dWidth == %d && dHeight == %d && dFormat == %d && Stride == %d && NumVertices == %d && primCount == %d && decl->Type == %d && numElements == %d && vSize == %d && pSize == %d && mStartRegister == %d && mVector4fCount == %d", dwDataCRC, dWidth, dHeight, dFormat, Stride, NumVertices, primCount, decl->Type, numElements, vSize, pSize, mStartRegister, mVector4fCount);

		//hold down P key until a texture changes, press I to log values of those textures
		if (GetAsyncKeyState('O') & 1) //-
			countnum--;
		if (GetAsyncKeyState('P') & 1) //+
			countnum++;
		if ((GetAsyncKeyState(VK_MENU)) && (GetAsyncKeyState('9') & 1)) //reset, set to -1
			countnum = -1;
		if (countnum == pSize)
			if ((Stride > NULL) && (GetAsyncKeyState('I') & 1)) //press I to log to log.txt
				Log("dwDataCRC == %x && dWidth == %d && dHeight == %d && dFormat == %d && Stride == %d && NumVertices == %d && primCount == %d && decl->Type == %d && numElements == %d && vSize == %d && pSize == %d && mStartRegister == %d && mVector4fCount == %d", dwDataCRC, dWidth, dHeight, dFormat, Stride, NumVertices, primCount, decl->Type, numElements, vSize, pSize, mStartRegister, mVector4fCount);
		if (countnum == pSize)
		{
			pDevice->SetPixelShader(NULL);
			return D3D_OK; //delete texture
		}
	}
	*/

	return DrawIndexedPrimitive_orig(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

//==========================================================================================================================

bool DoInit = true;
HRESULT APIENTRY EndScene_hook(IDirect3DDevice9* pDevice)
{
	if (pDevice == nullptr) return EndScene_orig(pDevice);

	//sprite
	//PreClear(pDevice);

	if (DoInit)
	{
		//generate shader
		//GenerateShader(pDevice, &shadRed, 1.0f, 0.0f, 0.0f, 1.0f, true); //true = 0depth shader (wallhacked)
		//GenerateShader(pDevice, &shadGreen, 0.0f, 1.0f, 0.0f, 1.0f, true);

		//Shift|RMouse|LMouse|Ctrl|Alt|Space|X|C
		if (aimkey == 0) Daimkey = 0;
		if (aimkey == 1) Daimkey = VK_SHIFT;
		if (aimkey == 2) Daimkey = VK_RBUTTON;
		if (aimkey == 3) Daimkey = VK_LBUTTON;
		if (aimkey == 4) Daimkey = VK_CONTROL;
		if (aimkey == 5) Daimkey = VK_MENU;
		if (aimkey == 6) Daimkey = VK_SPACE;
		if (aimkey == 7) Daimkey = 0x58; //X
		if (aimkey == 8) Daimkey = 0x43; //C

		LoadSettings();

		//wallhack = Load("Wallhack", "Wallhack", wallhack, GetDirectoryFile("palasettings.ini"));

		DoInit = false;
	}

	if (pFont == NULL)
	{
		HRESULT hr = D3DXCreateFont(pDevice, 13, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &pFont);

		if (FAILED(hr)) {
			//Log("D3DXCreateFont failed");
		}
	}

	if (pFont)
	{
		BuildMenu(pDevice);
	}

	//aimbot 1 part 2
	if (aimbot == 1 && AimHPBarInfo.size() != NULL && GetAsyncKeyState(Daimkey))
	//if (aimbot == 1 && AimHPBarInfo.size() != NULL)
	{
		UINT BestTarget = -1;
		DOUBLE fClosestPos = 99999;

		for (unsigned int i = 0; i < AimHPBarInfo.size(); i++)
		{
			//test w2s
			//if (logger)
			//DrawString(pFont, (int)AimHPBarInfo[i].vOutX, (int)AimHPBarInfo[i].vOutY, D3DCOLOR_ARGB(255, 255, 255, 255), "O");
			//DrawString(pFont, (int)AimHPBarInfo[i].vOutX, (int)AimHPBarInfo[i].vOutY, Green, "%x", dwDataCRC);

			//aimfov
			float radiusx = (aimfov*10.0f) * (ScreenCenterX / 100); //(aimfov*5.0f)
			float radiusy = (aimfov*10.0f) * (ScreenCenterY / 100); //(aimfov*5.0f)

			if (aimfov == 0)
			{
				radiusx = 5.0f * (ScreenCenterX / 100);
				radiusy = 5.0f * (ScreenCenterY / 100);
			}

			//get crosshairdistance
			AimHPBarInfo[i].CrosshairDistance = GetDistance(AimHPBarInfo[i].vOutX, AimHPBarInfo[i].vOutY, ScreenCenterX, ScreenCenterY);

			//aim at team 1 or 2 (not needed)
			//if (aimbot == AimHPBarInfo[i].iTeam)

			//if in fov
			if (AimHPBarInfo[i].vOutX >= ScreenCenterX - radiusx && AimHPBarInfo[i].vOutX <= ScreenCenterX + radiusx && AimHPBarInfo[i].vOutY >= ScreenCenterY - radiusy && AimHPBarInfo[i].vOutY <= ScreenCenterY + radiusy)

				//get closest/nearest target to crosshair
				if (AimHPBarInfo[i].CrosshairDistance < fClosestPos)
				{
					fClosestPos = AimHPBarInfo[i].CrosshairDistance;
					BestTarget = i;
				}
		}


		//if nearest target to crosshair
		if (BestTarget != -1 && AimTBarInfo[BestTarget].vOutX > 1 && AimTBarInfo[BestTarget].vOutY > 1)
		{
			double DistX = AimHPBarInfo[BestTarget].vOutX - ScreenCenterX;
			double DistY = AimHPBarInfo[BestTarget].vOutY - ScreenCenterY;

			//smooth aim
			DistX /= (1 + aimsens);
			DistY /= (1 + aimsens);

			//aim
			if (GetAsyncKeyState(Daimkey) & 0x8000)
				mouse_event(MOUSEEVENTF_MOVE, (float)DistX, (float)DistY, 0, NULL);

			//autoshoot on
			if ((!GetAsyncKeyState(VK_LBUTTON) && (autoshoot == 1) && (GetAsyncKeyState(Daimkey) & 0x8000))) //
			{
				if (autoshoot == 1 && !IsPressed)
				{
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					IsPressed = true;
				}
			}
		}
	}
	AimHPBarInfo.clear();



	//aimbot 2 part 2
	if (aimbot >= 2 && AimTBarInfo.size() != NULL && GetAsyncKeyState(Daimkey))
	//if (aimbot > 0 && AimTBarInfo.size() != NULL)
	{
		UINT BestTarget = -1;
		DOUBLE fClosestPos = 99999;

		for (unsigned int i = 0; i < AimTBarInfo.size(); i++)
		{
			//test w2s
			//if (logger)
			//DrawString(pFont, (int)AimTBarInfo[i].vOutX, (int)AimTBarInfo[i].vOutY, D3DCOLOR_ARGB(255, 0, 255, 0), "o");
			//DrawString(pFont, (int)AimTBarInfo[i].vOutX, (int)AimTBarInfo[i].vOutY, Green, "%x", dwDataCRC);

			//aimfov
			float radiusx = (aimfov*20.0f) * (ScreenCenterX / 100); //(aimfov*5.0f)
			float radiusy = (aimfov*20.0f) * (ScreenCenterY / 100); //(aimfov*5.0f)

			if (aimfov == 0)
			{
				radiusx = 10.0f * (ScreenCenterX / 100);
				radiusy = 10.0f * (ScreenCenterY / 100);
			}

			//get crosshairdistance
			AimTBarInfo[i].CrosshairDistance = GetDistance(AimTBarInfo[i].vOutX, AimTBarInfo[i].vOutY, ScreenCenterX, ScreenCenterY);

			//aim at team 1 or 2 (not needed)
			//if (aimbot == AimTBarInfo[i].iTeam)

			//if in fov
			if (AimTBarInfo[i].vOutX >= ScreenCenterX - radiusx && AimTBarInfo[i].vOutX <= ScreenCenterX + radiusx && AimTBarInfo[i].vOutY >= ScreenCenterY - radiusy && AimTBarInfo[i].vOutY <= ScreenCenterY + radiusy)

				//get closest/nearest target to crosshair
				if (AimTBarInfo[i].CrosshairDistance < fClosestPos)
				{
					fClosestPos = AimTBarInfo[i].CrosshairDistance;
					BestTarget = i;
				}
		}


		//if nearest target to crosshair
		if (BestTarget != -1 && AimTBarInfo[BestTarget].vOutX > 1 && AimTBarInfo[BestTarget].vOutY > 1)
		{
			double DistX = AimTBarInfo[BestTarget].vOutX - ScreenCenterX;
			double DistY = AimTBarInfo[BestTarget].vOutY - ScreenCenterY;

			//smooth aim
			DistX /= (1 + aimsens*2);
			DistY /= (1 + aimsens*2);

			//aim
			if (GetAsyncKeyState(Daimkey) & 0x8000)
				mouse_event(MOUSEEVENTF_MOVE, (float)DistX, (float)DistY, 0, NULL);

			//autoshoot on
			if ((!GetAsyncKeyState(VK_LBUTTON) && (autoshoot == 1) && (GetAsyncKeyState(Daimkey) & 0x8000))) //
			{
				if (autoshoot == 1 && !IsPressed)
				{
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					IsPressed = true;
				}
			}
		}
	}
	AimTBarInfo.clear();




	//autoshoot off
	if (autoshoot == 1 && IsPressed)
	{
		if (timeGetTime() - frametime >= asdelay)
		{
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			IsPressed = false;
			frametime = timeGetTime();
		}
	}
	/*
	//draw logger
	if ((GetAsyncKeyState(VK_MENU)) && (GetAsyncKeyState(VK_CONTROL)) && (GetAsyncKeyState(0x4C) & 1)) //ALT + CTRL + L toggles logger
		logger = !logger;
	if (pFont && logger) //&& countnum >= 0)
	{
		char szString[255];
		sprintf_s(szString, "countnum = %d", countnum);
		DrawString(pFont, 220, 100, White, (char*)&szString[0]);
		DrawString(pFont, 220, 110, Yellow, "hold P to +");
		DrawString(pFont, 220, 120, Yellow, "hold O to -");
		DrawString(pFont, 220, 130, Green, "press I to log");
	}
	*/
	return EndScene_orig(pDevice);
}

//==========================================================================================================================

class CFakeQuery : public IDirect3DQuery9
{
public:
	HRESULT WINAPI QueryInterface(REFIID riid, void** ppvObj)
	{
		return D3D_OK;
	}

	ULONG WINAPI AddRef()
	{
		m_ref++;

		return m_ref;
	}

	ULONG WINAPI Release()
	{
		return 1;
	}

	HRESULT WINAPI GetDevice(IDirect3DDevice9 **ppDevice)
	{
		return D3D_OK;
	}

	D3DQUERYTYPE WINAPI GetType()
	{
		return m_type;
	}

	DWORD WINAPI GetDataSize()
	{
		return sizeof(DWORD);
	}

	HRESULT    WINAPI Issue(DWORD dwIssueFlags)
	{
		return D3D_OK;
	}

	HRESULT WINAPI GetData(void* pData, DWORD dwSize, DWORD dwGetDataFlags)
	{
		DWORD* pdwData = (DWORD*)pData;

		*pdwData = 500; //500 pixels visible
		return D3D_OK;
	}

	int                m_ref;
	D3DQUERYTYPE    m_type;
};

HRESULT APIENTRY CreateQuery_hook(IDirect3DDevice9* pDevice, D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery)
{
	//Anti-Occlusion v2 (used by wallhack to see models through walls at all distances, reduces fps, 0 to 1 or 1 to 0 requires vid_restart F11 or alt + enter)
	if(occlusion == 1 && Type == D3DQUERYTYPE_OCCLUSION)
	{
		*ppQuery = new CFakeQuery;

		((CFakeQuery*)*ppQuery)->m_type = Type;

		return D3D_OK;
	}

	return CreateQuery_orig(pDevice, Type, ppQuery);
}

//==========================================================================================================================

HRESULT APIENTRY SetViewport_hook(IDirect3DDevice9* pDevice, CONST D3DVIEWPORT9* pViewport)
{
	//get viewport/screensize
	Viewport = *pViewport;
	ScreenCenterX = (float)Viewport.Width / 2.0f;
	ScreenCenterY = (float)Viewport.Height / 2.0f;

	return SetViewport_orig(pDevice, pViewport);
}

//==========================================================================================================================

HRESULT APIENTRY Reset_hook(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS *pPresentationParameters)
{
	//DeleteRenderSurfaces();

	if (pFont)
		pFont->OnLostDevice();

	HRESULT ResetReturn = Reset_orig(pDevice, pPresentationParameters);
	
	if (SUCCEEDED(ResetReturn))
	{
		if (pFont)
			pFont->OnResetDevice();
	}

	return ResetReturn;
}

//==========================================================================================================================
/*
HRESULT APIENTRY SetVertexShader_hook(LPDIRECT3DDEVICE9 pDevice, IDirect3DVertexShader9 *veShader)
{
	if (veShader != NULL)
	{
		vShader = veShader;
		vShader->GetFunction(NULL, &vSize);
	}
	return SetVertexShader_orig(pDevice, veShader);
}
*/
//==========================================================================================================================

HRESULT APIENTRY SetPixelShader_hook(LPDIRECT3DDEVICE9 pDevice, IDirect3DPixelShader9 *piShader)
{
	if (piShader != NULL)
	{
		pShader = piShader;
		pShader->GetFunction(NULL, &pSize);
	}
	return SetPixelShader_orig(pDevice, piShader);
}

//==========================================================================================================================

HRESULT APIENTRY SetTexture_hook(IDirect3DDevice9* pDevice, DWORD Sampler, IDirect3DBaseTexture9 *pTexture)
{
	if (pDevice == nullptr) return SetTexture_orig(pDevice, Sampler, pTexture);

	if (aimbot == 1 && mStartRegister == 6 && mVector4fCount == 2 && Sampler == 0 && pTexture || aimbot == 2 && mStartRegister == 6 && mVector4fCount == 2 && Sampler == 0 && pTexture)//reduce fps loss
	{//1
		pCurrentTexture = static_cast<IDirect3DTexture9*>(pTexture);

		if (pCurrentTexture)//
		{
			D3DSURFACE_DESC surfaceDesc;

			//pCurrentTexture->GetLevelDesc(0, &surfaceDesc);
			if (FAILED(pCurrentTexture->GetLevelDesc(0, &surfaceDesc)))
			{
				//Log("surfaceDesc failed");
				goto out;
			}

			if (SUCCEEDED(pCurrentTexture->GetLevelDesc(0, &surfaceDesc)))//can crash after game shuts down
			if (surfaceDesc.Pool == D3DPOOL_MANAGED)
			{
				dWidth = surfaceDesc.Width;
				dHeight = surfaceDesc.Height;
				//dFormat = surfaceDesc.Format;

				//if (pCurrentTexture->GetType() == D3DRTYPE_TEXTURE)
				if (reinterpret_cast<IDirect3DTexture9 *>(pCurrentTexture)->GetType() == D3DRTYPE_TEXTURE)
				//if ((surfaceDesc.Width == 12 && surfaceDesc.Height == 8 && surfaceDesc.Format == 894720068 || surfaceDesc.Width == 1024 && surfaceDesc.Height == 1024 && surfaceDesc.Format == 50) && (pCurrentTexture->GetType() == D3DRTYPE_TEXTURE))//
				{
					D3DLOCKED_RECT pLockedRect;

					//if (pCurrentTexture->LockRect(0, &pLockedRect, NULL, D3DLOCK_READONLY | D3DLOCK_DONOTWAIT | D3DLOCK_NOSYSLOCK) == S_OK)
					//pCurrentTexture->LockRect(0, &pLockedRect, NULL, D3DLOCK_NOOVERWRITE | D3DLOCK_READONLY);
					pCurrentTexture->LockRect(0, &pLockedRect, NULL, D3DLOCK_NOOVERWRITE | D3DLOCK_NOSYSLOCK | D3DLOCK_DONOTWAIT | D3DLOCK_READONLY);

					if (&pLockedRect != NULL && pLockedRect.pBits != NULL && pLockedRect.Pitch != NULL)
					{
						// get crc from the algorithm
						dwDataCRC = QuickChecksum((DWORD*)pLockedRect.pBits, pLockedRect.Pitch);
						//pCurrentTexture->UnlockRect(0);
					}
					pCurrentTexture->UnlockRect(0);
				}
			}
		}
	}
	out:
	
	return SetTexture_orig(pDevice, Sampler, pTexture);
}

//==========================================================================================================================

DWORD WINAPI DXInit(__in  LPVOID lpParameter)
{
	HWND GameHWND = NULL;
	while (!GameHWND)
	{
		//Multi national version compatible // PS: China "枪火游侠 (32-bit, DX9)"
		GameHWND = FindWindowA("LaunchUnrealUWindowsClient", 0); //avoid inj i.s.o.g
		//GameHWND = FindWindowW(0, L"Chaos (32-bit, DX9)");
		Sleep(100);
	}
	CloseHandle(GameHWND);

	HMODULE hDLL = NULL;
	while (!hDLL)
	{
		hDLL = GetModuleHandleA("d3d9.dll");
		Sleep(100);
	}
	CloseHandle(hDLL);

	IDirect3D9* d3d = NULL;
	IDirect3DDevice9* d3ddev = NULL;

	HWND tmpWnd = CreateWindowA("BUTTON", "Temp Window", WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 300, 300, NULL, NULL, dllHandle, NULL);
	if(tmpWnd == NULL)
	{
		//Log("[DirectX] Failed to create temp window");
		return 0;
	}

	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if(d3d == NULL)
	{
		DestroyWindow(tmpWnd);
		//Log("[DirectX] Failed to create temp Direct3D interface");
		return 0;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp)); 
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = tmpWnd;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	HRESULT result = d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, tmpWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);
	if(result != D3D_OK)
	{
		d3d->Release();
		DestroyWindow(tmpWnd);
		//Log("[DirectX] Failed to create temp Direct3D device");
		return 0;
	}

	// We have the device, so walk the vtable to get the address of all the dx functions in d3d9.dll
	#if defined _M_X64
	DWORD64* dVtable = (DWORD64*)d3ddev;
	dVtable = (DWORD64*)dVtable[0];
	#elif defined _M_IX86
	DWORD* dVtable = (DWORD*)d3ddev;
	dVtable = (DWORD*)dVtable[0]; // == *d3ddev
	#endif
	//Log("[DirectX] dvtable: %x", dVtable);

	//for(int i = 0; i < 95; i++)
	//{
		//Log("[DirectX] vtable[%i]: %x, pointer at %x", i, dVtable[i], &dVtable[i]);
	//}

	// Set EndScene_orig to the original EndScene etc.
	EndScene_orig = (EndScene)dVtable[42];
	SetStreamSource_orig = (SetStreamSource)dVtable[100];
	//SetVertexDeclaration_orig = (SetVertexDeclaration)dVtable[87];
	SetVertexShaderConstantF_orig = (SetVertexShaderConstantF)dVtable[94];
	DrawIndexedPrimitive_orig = (DrawIndexedPrimitive)dVtable[82];
	Reset_orig = (Reset)dVtable[16];
	CreateQuery_orig = (CreateQuery)dVtable[118];
	SetViewport_orig = (SetViewport)dVtable[47];
	//SetVertexShader_orig = (SetVertexShader)dVtable[92];
	SetPixelShader_orig = (SetPixelShader)dVtable[107];
	SetTexture_orig = (SetTexture)dVtable[65];

	// Detour functions x86 & x64
	if (MH_Initialize() != MH_OK) { return 1; }
	if (MH_CreateHook((DWORD_PTR*)dVtable[42], &EndScene_hook, reinterpret_cast<void**>(&EndScene_orig)) != MH_OK) { return 1; }
	if (MH_EnableHook((DWORD_PTR*)dVtable[42]) != MH_OK) { return 1; }
	if (MH_CreateHook((DWORD_PTR*)dVtable[100], &SetStreamSource_hook, reinterpret_cast<void**>(&SetStreamSource_orig)) != MH_OK) { return 1; }
	if (MH_EnableHook((DWORD_PTR*)dVtable[100]) != MH_OK) { return 1; }
	if (MH_CreateHook((DWORD_PTR*)dVtable[87], &SetVertexDeclaration_hook, reinterpret_cast<void**>(&SetVertexDeclaration_orig)) != MH_OK) { return 1; }
	if (MH_EnableHook((DWORD_PTR*)dVtable[87]) != MH_OK) { return 1; }
	if (MH_CreateHook((DWORD_PTR*)dVtable[94], &SetVertexShaderConstantF_hook, reinterpret_cast<void**>(&SetVertexShaderConstantF_orig)) != MH_OK) { return 1; }
	if (MH_EnableHook((DWORD_PTR*)dVtable[94]) != MH_OK) { return 1; }
	if (MH_CreateHook((DWORD_PTR*)dVtable[82], &DrawIndexedPrimitive_hook, reinterpret_cast<void**>(&DrawIndexedPrimitive_orig)) != MH_OK) { return 1; }
	if (MH_EnableHook((DWORD_PTR*)dVtable[82]) != MH_OK) { return 1; }
	if (MH_CreateHook((DWORD_PTR*)dVtable[16], &Reset_hook, reinterpret_cast<void**>(&Reset_orig)) != MH_OK) { return 1; }
	if (MH_EnableHook((DWORD_PTR*)dVtable[16]) != MH_OK) { return 1; }
	if (MH_CreateHook((DWORD_PTR*)dVtable[118], &CreateQuery_hook, reinterpret_cast<void**>(&CreateQuery_orig)) != MH_OK) { return 1; }
	if (MH_EnableHook((DWORD_PTR*)dVtable[118]) != MH_OK) { return 1; }
	if (MH_CreateHook((DWORD_PTR*)dVtable[47], &SetViewport_hook, reinterpret_cast<void**>(&SetViewport_orig)) != MH_OK) { return 1; }
	if (MH_EnableHook((DWORD_PTR*)dVtable[47]) != MH_OK) { return 1; }
	//if (MH_CreateHook((DWORD_PTR*)dVtable[92], &SetVertexShader_hook, reinterpret_cast<void**>(&SetVertexShader_orig)) != MH_OK) { return 1; }
	//if (MH_EnableHook((DWORD_PTR*)dVtable[92]) != MH_OK) { return 1; }
	if (MH_CreateHook((DWORD_PTR*)dVtable[107], &SetPixelShader_hook, reinterpret_cast<void**>(&SetPixelShader_orig)) != MH_OK) { return 1; }
	if (MH_EnableHook((DWORD_PTR*)dVtable[107]) != MH_OK) { return 1; }
	if (MH_CreateHook((DWORD_PTR*)dVtable[65], &SetTexture_hook, reinterpret_cast<void**>(&SetTexture_orig)) != MH_OK) { return 1; }
	if (MH_EnableHook((DWORD_PTR*)dVtable[65]) != MH_OK) { return 1; }

	//Log("[Detours] EndScene detour attached\n");

	d3ddev->Release();
	d3d->Release();
	DestroyWindow(tmpWnd);
		
	return 1;
}

//==========================================================================================================================

BOOL WINAPI DllMain(HMODULE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH: // A process is loading the DLL.
		dllHandle = hinstDLL;
		DisableThreadLibraryCalls(hinstDLL); // disable unwanted thread notifications to reduce overhead
		GetModuleFileNameA(hinstDLL, dlldir, 512);
		for (int i = (int)strlen(dlldir); i > 0; i--)
		{
			if (dlldir[i] == '\\')
			{
				dlldir[i + 1] = 0;
				break;
			}
		}

		CreateThread(0, 0, DXInit, 0, 0, 0); //init our hooks
		break;

	case DLL_PROCESS_DETACH: // A process unloads the DLL.
		/*
		if (MH_Uninitialize() != MH_OK) { return 1; }
		if (MH_DisableHook((DWORD_PTR*)dVtable[42]) != MH_OK) { return 1; }
		*/
		break;
	}
	return TRUE;
}
