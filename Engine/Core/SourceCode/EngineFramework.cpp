
#include "stdafx.h"

#include "EngineFramework.h"

#include <Core/IWindow.h>
#include "Window/WindowsGUI.h"

#include <Core/Config/Config.h>
#include <Core/Render/DxRenderer.h>

#include <cassert>


#include <Core/CompressBox.h>

namespace Engine
{
	static IEngineFramework *sgpEngineFramework = nullptr;

//	
//	CEngineFramework::CEngineFramework() : m_pd3dDevice(NULL), m_pd3dDeviceContext(NULL), m_pDXGISwapChain(NULL),
//		m_nwndclientheight(FRAME_BUFFER_HEIGHT), m_nwndclientwidth(FRAME_BUFFER_WIDTH), m_pScene(NULL), m_pPlayer(NULL)
//		, m_pd3dDepthStencilBuffer(NULL), m_pd3dDepthStencilView(NULL)
//	{
//		_tcscpy_s(m_pszBuffer, _T("LabProject ("));
//		m_pPlayerShader = NULL;
//
//
//
//
//		m_ppCamera = NULL;
//		m_nCameras = 0;
//		// m_ppd3dRenderTargetViews = NULL;
//		m_nRenderTargetViews = 0;
//
//	}
//
//
//	CEngineFramework::~CEngineFramework()
//	{
//	}
//
//
//	//응용프로그램 실행시 호출됨에 유의
//	bool CEngineFramework::OnCreate(HINSTANCE hInstance, HWND hMainWnd)
//	{
//		m_hInstance = hInstance;
//		m_hwnd = hMainWnd;
//
//
//		//Direct3D 디바이스, 디바이스 컨텍스트, 스왑체인등을 생성하는 함수 호출
//		if (!CreateDirect3DDisplay()) return false;
//
//		//렌더링할 객체(게임월드객체)를 생성
//		BuildObject();
//
//		return true;
//	}
//
//	//응용프로그램이 종료될 때 호출됨에 유의 
//	void CEngineFramework::OnDestroy()
//	{
//		//게임 객체를 소멸 
//		ReleaseObjects();
//
//		//Direct3D와 관련된 객체를 소멸한다. 
//		if (m_pd3dDeviceContext) m_pd3dDeviceContext->ClearState();
//		if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();
//		if (m_pd3dDepthStencilBuffer) m_pd3dDepthStencilBuffer->Release();
//		if (m_pd3dDepthStencilView) m_pd3dDepthStencilView->Release();
//		if (m_pDXGISwapChain) m_pDXGISwapChain->Release();
//		if (m_pd3dDeviceContext) m_pd3dDeviceContext->Release();
//		if (m_pd3dDevice) m_pd3dDevice->Release();
//	}
//
//	//디바이스, 스왑 체인, 디바이스 컨텍스트, 디바이스와 관련된 뷰를 생성하는 함수
//	bool CEngineFramework::CreateRenderTargetDepthStencilView()
//	{
//		HRESULT hResult = S_OK;
//
//
//		//스왑 체인의 후면버퍼에 대한 렌더 타겟뷰를 생성
//		ID3D11Texture2D *pd3dBackBuffer;
//
//		if (FAILED(hResult = 
//			m_pDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pd3dBackBuffer)))
//			return false;
//		if (FAILED(hResult = m_pd3dDevice->CreateRenderTargetView(pd3dBackBuffer, NULL, &m_pd3dRenderTargetView)))
//			return false;
//
//		if (pd3dBackBuffer)
//			pd3dBackBuffer->Release();
//
//
//		//렌더 타겟과 같은 크기의 깊이 버퍼 생성
//		D3D11_TEXTURE2D_DESC d3dDepthStencilBufferDesc;
//		ZeroMemory(&d3dDepthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
//		d3dDepthStencilBufferDesc.Width = m_nwndclientwidth;
//		d3dDepthStencilBufferDesc.Height = m_nwndclientheight;
//		d3dDepthStencilBufferDesc.MipLevels = 1;
//		d3dDepthStencilBufferDesc.ArraySize = 1;
//		d3dDepthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
//		d3dDepthStencilBufferDesc.SampleDesc.Count = 1;
//		d3dDepthStencilBufferDesc.SampleDesc.Quality = 0;
//		d3dDepthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
//		d3dDepthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
//		d3dDepthStencilBufferDesc.CPUAccessFlags = 0;
//		d3dDepthStencilBufferDesc.MiscFlags = 0;
//		if (FAILED(hResult = m_pd3dDevice->CreateTexture2D(&d3dDepthStencilBufferDesc, NULL, &m_pd3dDepthStencilBuffer))) return(false);
//
//		D3D11_DEPTH_STENCIL_VIEW_DESC d3dViewDesc;
//		ZeroMemory(&d3dViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
//		d3dViewDesc.Format = d3dDepthStencilBufferDesc.Format;
//		d3dViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
//		d3dViewDesc.Texture2D.MipSlice = 0;
//		if (FAILED(hResult = m_pd3dDevice->CreateDepthStencilView(m_pd3dDepthStencilBuffer, &d3dViewDesc, &m_pd3dDepthStencilView))) return(false);
//
//		m_pd3dDeviceContext->OMSetRenderTargets(1, &m_pd3dRenderTargetView, m_pd3dDepthStencilView);
//		return true;
//	}
//
//	bool CEngineFramework::CreateDirect3DDisplay()
//	{
//		RECT rcClient;
//		::GetClientRect(m_hwnd, &rcClient);
//
//		m_nwndclientheight = rcClient.bottom - rcClient.top;
//		m_nwndclientwidth = rcClient.right - rcClient.left;
//
//		UINT dwCreateDeviceFlags = 0;
//#ifdef _DEBUG
//		//디버그 모드시 d3d 디버그 모드 활성화
//		dwCreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
//#endif
//
//		//디바이스를 생성하기 위하여 시도할 드라이버 유형 리스트
//		D3D_DRIVER_TYPE d3dDriverTypes[] = { D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_REFERENCE };
//		UINT nDriverTypes = _countof(d3dDriverTypes);
//
//		//디바이스를 생성하기 위하여 시도할 특성 레벨 순서
//		D3D_FEATURE_LEVEL d3dFeatureLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0 };
//		UINT nFeatureLevels = _countof(d3dFeatureLevels);
//
//		//생성할 스왑 체인을 서술하는 구조체
//
//		DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
//		::ZeroMemory(&dxgiSwapChainDesc, sizeof(dxgiSwapChainDesc));
//		dxgiSwapChainDesc.BufferCount = 1;
//		dxgiSwapChainDesc.BufferDesc.Width = m_nwndclientwidth;
//		dxgiSwapChainDesc.BufferDesc.Height = m_nwndclientheight;
//		dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//		dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
//		dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
//		dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//		dxgiSwapChainDesc.OutputWindow = m_hwnd;
//		dxgiSwapChainDesc.SampleDesc.Count = 1;
//		dxgiSwapChainDesc.SampleDesc.Quality = 0;
//		dxgiSwapChainDesc.Windowed = TRUE;
//
//		D3D_DRIVER_TYPE nd3dDriverType = D3D_DRIVER_TYPE_NULL;
//		D3D_FEATURE_LEVEL nd3dFeatureLevel = D3D_FEATURE_LEVEL_11_0; //##?? GPU가 지원하는지 확인하는 부분이 없는거 같다
//
//		HRESULT hResult = S_OK;
//
//		//디바이스의 드라이버 유형과 특성 레벨을 지원하는 디바이스와 스왑 체인을 생성 
//		//고수준의 디바이스 생성을 시도하고 실패하면 다음 수준의 디바이스를 생성
//		for (UINT i = 0; i < nDriverTypes; i++)
//		{
//			nd3dDriverType = d3dDriverTypes[i];
//			if (SUCCEEDED(hResult = D3D11CreateDeviceAndSwapChain(NULL, nd3dDriverType, NULL, dwCreateDeviceFlags, d3dFeatureLevels,
//				nFeatureLevels, D3D11_SDK_VERSION, &dxgiSwapChainDesc, &m_pDXGISwapChain,
//				&m_pd3dDevice, &nd3dFeatureLevel, &m_pd3dDeviceContext))) break;
//		}
//		if (!m_pDXGISwapChain || !m_pd3dDevice || !m_pd3dDeviceContext) return false;
//
//		if (!CreateRenderTargetDepthStencilView()) return false;
//
//
//		return true;
//	}
//
//
//	//랜더링할 메쉬, 객체를 생성하고 소멸하는 함수
//	void CEngineFramework::BuildObject()
//	{
//		CShader::CreateShaderVariables(m_pd3dDevice);
//		CIlluminatedShader::CreateShaderVariables(m_pd3dDevice);
//
//		m_pScene = new CScene();
//		m_pScene->BuildObjects(m_pd3dDevice);
//
//
//		ID3D11SamplerState *pd3dSamplerState = NULL;
//		D3D11_SAMPLER_DESC d3dSamplerDesc;
//		ZeroMemory(&d3dSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
//		d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
//		d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
//		d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
//		d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
//		d3dSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
//		d3dSamplerDesc.MinLOD = 0;
//		d3dSamplerDesc.MaxLOD = 0;
//		m_pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &pd3dSamplerState);
//
//		ID3D11ShaderResourceView *pd3dsrvTexture = NULL;
//		CTexture *pAireplaneTexture = new CTexture(1, 1, 0, 0);
//		D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, _T("../HomeworkAssets/ww2_USA.png"), NULL, NULL, &pd3dsrvTexture, NULL);
//		pAireplaneTexture->SetTexture(0, pd3dsrvTexture);
//		pAireplaneTexture->SetSampler(0, pd3dSamplerState);
//		pd3dsrvTexture->Release();
//		pd3dSamplerState->Release();
//
//		m_pPlayerShader = new CPlayerShader();
//		m_pPlayerShader->CreateShader(m_pd3dDevice);
//		m_pPlayerShader->BuildObjects(m_pd3dDevice);
//		m_pPlayer = m_pPlayerShader->GetPlayer();
//		m_pPlayer->SetTexture(pAireplaneTexture);
//
//		/*지형의 xz-평면의 가운데에 플레이어가 위치하도록 한다. 플레이어의 y-좌표가 지형의 높이 보다 크고 중력이 작용하도록 플레이어를 설정하였으므로 플레이어는 점차적으로 하강하게 된다.*/
//		CHeightMapTerrain *pTerrain = m_pScene->GetTerrain();
//		m_pPlayer->SetPosition(D3DXVECTOR3(pTerrain->GetWidth()*0.5f, pTerrain->GetPeakHeight() + 1000.0f, pTerrain->GetLength()*0.5f));
//		//플레이어의 위치가 변경될 때 지형의 정보에 따라 플레이어의 위치를 변경할 수 있도록 설정한다.
//		m_pPlayer->SetPlayerUpdatedContext(pTerrain);
//		//카메라의 위치가 변경될 때 지형의 정보에 따라 카메라의 위치를 변경할 수 있도록 설정한다.
//		m_pPlayer->SetCameraUpdatedContext(pTerrain);
//
//
//		//필요한 카메라들을 얻거나 생성한다.
//		m_nCameras = CAMERAS;
//		m_ppCamera = new CCamera*[m_nCameras];
//
//		m_ppCamera[CAMERA_PLAYER_FRONT] = m_pPlayer->GetCamera();
//		m_ppCamera[CAMERA_PLAYER_FRONT]->GenerateViewMatrix();
//
//		m_ppCamera[CAMERA_PLAYER_BACK] = ((CTerrainPlayer*)m_pPlayer)->GetBackCamera();
//		m_ppCamera[CAMERA_PLAYER_BACK]->GenerateViewMatrix();
//
//		m_ppCamera[CAMERA_MISSILE] = new CThirdPersonCamera(NULL);
//		m_ppCamera[CAMERA_MINIMAP] = new CThirdPersonCamera(NULL);
//
//		//카메라별 뷰포트 생성
//		m_ppCamera[CAMERA_PLAYER_FRONT]->SetViewport(m_pd3dDeviceContext, 0, 0, FRAME_BUFFER_WIDTH / 2, FRAME_BUFFER_HEIGHT / 2, 0.0f, 1.0f);
//		m_ppCamera[CAMERA_PLAYER_BACK]->SetViewport(m_pd3dDeviceContext, 320, 0, FRAME_BUFFER_WIDTH / 2, FRAME_BUFFER_HEIGHT / 2, 0.0f, 1.0f);
//		m_ppCamera[CAMERA_MISSILE]->SetViewport(m_pd3dDeviceContext, 0, 240, FRAME_BUFFER_WIDTH / 2, FRAME_BUFFER_HEIGHT / 2, 0.0f, 1.0f);
//		m_ppCamera[CAMERA_MINIMAP]->SetViewport(m_pd3dDeviceContext, 320, 240, FRAME_BUFFER_WIDTH / 2, FRAME_BUFFER_HEIGHT / 2, 0.0f, 1.0f);
//
//		m_ppCamera[CAMERA_PLAYER_FRONT]->GenerateOrthoMatrix();
//		m_ppCamera[CAMERA_PLAYER_BACK]->GenerateOrthoMatrix();
//
//		//카메라들의 초기 값들을 설정한다.
//		m_ppCamera[CAMERA_MISSILE]->SetPosition(D3DXVECTOR3(500, 500, 500));
//		m_ppCamera[CAMERA_MISSILE]->SetLookAtPosition(D3DXVECTOR3(0, 0, 1));
//		m_ppCamera[CAMERA_MISSILE]->GenerateViewMatrix();
//		m_ppCamera[CAMERA_MISSILE]->GenerateProjectionMatrix(1.01f, 50000.0f, ASPECT_RATIO, 60.0f);
//		m_ppCamera[CAMERA_MISSILE]->GenerateOrthoMatrix();
//		m_ppCamera[CAMERA_MISSILE]->CreateShaderVariables(m_pd3dDevice);
//		m_ppCamera[CAMERA_MISSILE]->CalculateFrustumPlanes();
//
//		D3DXVECTOR3 scale = pTerrain->GetScale();
//		float x = (pTerrain->GetWidth() / scale.x);
//		float z = (pTerrain->GetLength() / scale.z) - 100;
//
//		m_ppCamera[CAMERA_MINIMAP]->SetPosition(D3DXVECTOR3(x, 2100, z));
//		m_ppCamera[CAMERA_MINIMAP]->SetLookAtPosition(D3DXVECTOR3(x, 0, z + 1));
//		m_ppCamera[CAMERA_MINIMAP]->GenerateViewMatrix();
//		m_ppCamera[CAMERA_MINIMAP]->GenerateProjectionMatrix(1.01f, 50000.0f, ASPECT_RATIO, 60.0f);
//		m_ppCamera[CAMERA_MINIMAP]->GenerateOrthoMatrix();
//		m_ppCamera[CAMERA_MINIMAP]->CreateShaderVariables(m_pd3dDevice);
//		m_ppCamera[CAMERA_MINIMAP]->CalculateFrustumPlanes();
//		m_pScene->SetCamera(m_ppCamera, m_nCameras);
//	}
//	void CEngineFramework::ReleaseObjects()
//	{
//		//CShader 클래스의 정적(static) 멤버 변수로 선언된 상수 버퍼를 반환한다.
//		CShader::ReleaseShaderVariables();
//		CIlluminatedShader::ReleaseShaderVariables();
//
//		if (m_pScene)m_pScene->ReleaseObjects();
//		if (m_pScene)delete m_pScene;
//
//		if (m_pPlayerShader) m_pPlayerShader->ReleaseObjects();
//		if (m_pPlayerShader) delete m_pPlayerShader;
//
//	}
//
//
//	void CEngineFramework::ProcessInput()
//	{
//		bool bProcessedByScene = false;
//		if (m_pScene) bProcessedByScene = m_pScene->ProcessInput();
//		if (!bProcessedByScene)
//		{
//			static UCHAR pKeyBuffer[256];
//			DWORD dwDirection = 0;
//			/*키보드의 상태 정보를 반환한다. 화살표 키(‘→’, ‘←’, ‘↑’, ‘↓’)를 누르면 플레이어를 오른쪽/왼쪽(로컬 x-축), 앞/뒤(로컬 z-축)로 이동한다. ‘Page Up’과 ‘Page Down’ 키를 누르면 플레이어를 위/아래(로컬 y-축)로 이동한다.*/
//			if (GetKeyboardState(pKeyBuffer))
//			{
//				if (pKeyBuffer[VK_UP] & 0xF0) dwDirection |= DIR_FORWARD;
//				if (pKeyBuffer[VK_DOWN] & 0xF0) dwDirection |= DIR_BACKWARD;
//				if (pKeyBuffer[VK_LEFT] & 0xF0) dwDirection |= DIR_LEFT;
//				if (pKeyBuffer[VK_RIGHT] & 0xF0) dwDirection |= DIR_RIGHT;
//				if (pKeyBuffer[VK_PRIOR] & 0xF0) dwDirection |= DIR_UP;
//				if (pKeyBuffer[VK_NEXT] & 0xF0) dwDirection |= DIR_DOWN;
//			}
//			float cxDelta = 0.0f, cyDelta = 0.0f;
//			POINT ptCursorPos;
//			/*마우스를 캡쳐했으면 마우스가 얼마만큼 이동하였는 가를 계산한다. 마우스 왼쪽 또는 오른쪽 버튼이 눌러질 때의 메시지(WM_LBUTTONDOWN, WM_RBUTTONDOWN)를 처리할 때 마우스를 캡쳐하였다. 그러므로 마우스가 캡쳐된 것은 마우스 버튼이 눌려진 상태를 의미한다. 마우스를 좌우 또는 상하로 움직이면 플레이어를 x-축 또는 y-축으로 회전한다.*/
//			if (GetCapture() == m_hwnd)
//			{
//				//마우스 커서를 화면에서 없앤다(보이지 않게 한다).
//				SetCursor(NULL);
//				//현재 마우스 커서의 위치를 가져온다.
//				GetCursorPos(&ptCursorPos);
//				//마우스 버튼이 눌린 채로 이전 위치에서 현재 마우스 커서의 위치까지 움직인 양을 구한다.
//				cxDelta = (float)(ptCursorPos.x - m_ptOldCursorPos.x) / 3.0f;
//				cyDelta = (float)(ptCursorPos.y - m_ptOldCursorPos.y) / 3.0f;
//				SetCursorPos(m_ptOldCursorPos.x, m_ptOldCursorPos.y);
//			}
//			//플레이어를 이동하거나(dwDirection) 회전한다(cxDelta 또는 cyDelta).
//			if ((dwDirection != 0) || (cxDelta != 0.0f) || (cyDelta != 0.0f))
//			{
//				if (cxDelta || cyDelta)
//				{
//					/*cxDelta는 y-축의 회전을 나타내고 cyDelta는 x-축의 회전을 나타낸다. 오른쪽 마우스 버튼이 눌려진 경우 cxDelta는 z-축의 회전을 나타낸다.*/
//					if (pKeyBuffer[VK_RBUTTON] & 0xF0)
//						m_pPlayer->Rotate(cyDelta, 0.0f, -cxDelta);
//					else
//						m_pPlayer->Rotate(cyDelta, cxDelta, 0.0f);
//				}
//				/*플레이어를 dwDirection 방향으로 이동한다(실제로는 속도 벡터를 변경한다). 이동 거리는 시간에 비례하도록 한다. 플레이어의 이동 속력은 (50/초)로 가정한다. 만약 플레이어의 이동 속력이 있다면 그 값을 사용한다.*/
//				if (dwDirection)
//					m_pPlayer->Move(dwDirection, 50.0f * m_GameTimer.GetTimeElapsed(), true);
//
//			}
//		}
//		//플레이어를 실제로 이동하고 카메라를 갱신한다. 중력과 마찰력의 영향을 속도 벡터에 적용한다.
//		m_pPlayer->Update(m_GameTimer.GetTimeElapsed());
//	}
//	void CEngineFramework::AnimateObjects()
//	{
//		if (m_pScene) m_pScene->AnimateObjects(m_GameTimer.GetTimeElapsed());
//	}
//	void CEngineFramework::FrameAdvance()
//	{
//		m_GameTimer.Tick();
//
//		ProcessInput();
//
//		AnimateObjects();
//
//		float fClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
//		if (m_pd3dRenderTargetView)
//			m_pd3dDeviceContext->ClearRenderTargetView(m_pd3dRenderTargetView, fClearColor);
//
//		if (m_pd3dDepthStencilView)
//			m_pd3dDeviceContext->ClearDepthStencilView(m_pd3dDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
//
//		if (m_pScene)
//		{
//			for (int c = 0; c < m_nCameras - 1; c++)
//			{
//
//				m_pScene->Render(m_pd3dDeviceContext, m_ppCamera[c]);
//
//				if (c == CAMERA_PLAYER_BACK)
//				{
//					m_pd3dDeviceContext->ClearDepthStencilView(m_pd3dDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
//					m_pPlayerShader->Render(m_pd3dDeviceContext, m_ppCamera[CAMERA_PLAYER_BACK]);
//				}
//			}
//			m_pScene->RenderUI(m_pd3dDeviceContext, m_ppCamera[CAMERA_PLAYER], HUD);
//			m_pScene->RenderUI(m_pd3dDeviceContext, m_ppCamera[CAMERA_MINIMAP], MINIMAP);
//		}
//		
//		m_pDXGISwapChain->Present(0, 0);
//		m_GameTimer.GetFrameRate(m_pszBuffer + 12, 37);
//		::SetWindowText(m_hwnd, m_pszBuffer);
//	}
//
//	//윈도우 메시지(키보드, 마우스 입력)를 처리하는 함수
//	void CEngineFramework::OnProcessingMouseMessage(HWND hwnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
//	{
//		if (m_pScene)
//			m_pScene->OnProcessingMouseMessage(hwnd, nMessageID, wParam, lParam);
//
//		switch (nMessageID)
//		{
//		case WM_LBUTTONDOWN:
//		case WM_RBUTTONDOWN:
//			//마우스 캡쳐를 하고 현재 마우스 위치를 가져온다.
//			SetCapture(hwnd);
//			GetCursorPos(&m_ptOldCursorPos);
//			break;
//		case WM_LBUTTONUP:
//		case WM_RBUTTONUP:
//			//마우스 캡쳐를 해제한다.
//			ReleaseCapture();
//			break;
//		case WM_MOUSEMOVE:
//			break;
//		default:
//			break;
//		}
//	}
//	void CEngineFramework::OnProcessingKeyboardMessage(HWND hwnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
//	{
//		if (m_pScene)
//			m_pScene->OnProcessingKeyboardMessage(hwnd, nMessageID, wParam, lParam);
//
//		switch (nMessageID)
//		{
//		case WM_KEYUP:
//			switch (wParam)
//			{
//				/*‘F1’ 키를 누르면 1인칭 카메라,
//				‘F2’ 키를 누르면 스페이스-쉽 카메라로 변경한다,
//				‘F3’ 키를 누르면 3인칭 카메라로 변경한다.*/
//			case VK_F1:
//			case VK_F2:
//			case VK_F3:
//				if (m_pPlayer)
//				{
//					m_pPlayer->ChangeCamera(m_pd3dDevice, (wParam - VK_F1 + 1), m_GameTimer.GetTimeElapsed());
//					m_ppCamera[CAMERA_PLAYER] = m_pPlayer->GetCamera();
//					//씬에 현재 카메라를 설정한다.
//					m_pScene->SetCamera(m_ppCamera, m_nCameras);
//				}
//				break;
//			case VK_ESCAPE:
//				::PostQuitMessage(0);
//				break;
//			default:
//				break;
//			}
//			break;
//		default:
//			break;
//		}
//	}
//	LRESULT CALLBACK CEngineFramework::OnProcessingWindowMessage(HWND hwnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
//	{
//		switch (nMessageID)
//		{
//			//윈도우의 크기가 변경될 때(현재는 lt+Enter 전체 화면 모드와 윈도우 모드로 전환될 때)
//			//스왑 체인의 후면버퍼 크기를 조정하고 후면버퍼에 대한 렌더 타겟 뷰를 다시 생성
//		case WM_SIZE:
//		{
//			m_nwndclientwidth = LOWORD(lParam);
//			m_nwndclientheight = HIWORD(lParam);
//
//			m_pd3dDeviceContext->OMSetRenderTargets(0, NULL, NULL);
//
//			if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();
//			if (m_pd3dDepthStencilBuffer) m_pd3dDepthStencilBuffer->Release();
//			if (m_pd3dDepthStencilView) m_pd3dDepthStencilView->Release();
//
//			m_pDXGISwapChain->ResizeBuffers(2, m_nwndclientwidth, m_nwndclientheight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
//
//			CreateRenderTargetDepthStencilView();
//
//			CCamera *pCamera = m_pPlayer->GetCamera();
//
//			if (pCamera)
//				pCamera->SetViewport(m_pd3dDeviceContext, 0, 0, m_nwndclientwidth, m_nwndclientheight, 0.0f, 1.0f);
//
//			break;
//		}
//		case WM_LBUTTONDOWN:
//		case WM_RBUTTONDOWN:
//		case WM_LBUTTONUP:
//		case WM_RBUTTONUP:
//		case WM_MOUSEMOVE:
//			OnProcessingMouseMessage(hwnd, nMessageID, wParam, lParam);
//			break;
//		case WM_KEYDOWN:
//		case WM_KEYUP:
//			OnProcessingKeyboardMessage(hwnd, nMessageID, wParam, lParam);
//			break;
//		}
//		return(0);
//	}

	CEngineFramework::CEngineFramework(HINSTANCE hInstace) 
		: m_hInstance(hInstace)
		, m_initializedCoreSystem(false)
		, m_initializedEngineSystem(false)
		, m_isReady(false)
		, m_isRunning(false)
	{
		//등록
		sgpEngineFramework = this;
	}

	CEngineFramework::~CEngineFramework()
	{

	}

	bool CEngineFramework::InitializCore(const char* pCmd)
	{
		//명령어에 따른 처리 코어 시스템 초기화
		if (Core::CoreSystem::InitializeCore(pCmd) == false)
		{
			assert(false && TEXTL("코어 시스템 초기화 실패"));
			return false;
		}

		CoreLogInsert(TEXTL("[Core] 초기화 완료"));
		m_initializedCoreSystem = true;
		return true;
	}


	LRESULT CEngineFramework::_GameWindowHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		LRESULT res = message;
		assert(sgpEngineFramework != nullptr);

		if (message == WM_CLOSE)
		{
			sgpEngineFramework->Stop();
		}
		else
		{
			res = sgpEngineFramework->OnWindowMessageHandle(hWnd, message, wParam, lParam);
		}
		

		//처리가 안됬다면 기본 함수 호출로 처리
		if (res == message)
		{
			return DefWindowProcW(hWnd, message, wParam, lParam);
		}
		else
		{
			return 0;
		}
	}

	//비공개 함수
	LRESULT CEngineFramework::OnWindowMessageHandle(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		using namespace Core;
				
		const int mousePosX = GET_X_LPARAM(lParam);
		const int mousePosY = GET_Y_LPARAM(lParam);
		const int mouseWheel = (message == WM_MOUSEWHEEL) ? (GET_WHEEL_DELTA_WPARAM(wParam)) : (0);
		EMouseEvent mouseEvent = static_cast<EMouseEvent>(-1);


		//이벤트를 CoreEvent에 전달
		switch (message)
		{
		case WM_CLOSE:
			m_isRunning = false;
			break;

		case WM_CREATE:
			CoreEventDispatcher->OnSystemEvent(ESystemEvent::WindowCreated, wParam, lParam);
			break;

		case WM_MOVE:
			CoreEventDispatcher->OnSystemEvent(ESystemEvent::WindowMove, wParam, lParam);
			break;

		case WM_SIZE:
			CoreEventDispatcher->OnSystemEvent(ESystemEvent::WindowResize, wParam, lParam);
			break;

		case WM_WINDOWPOSCHANGED:
			CoreEventDispatcher->OnSystemEvent(ESystemEvent::WindowPosChange, wParam, lParam);
			break;

		case WM_ACTIVATE:
			CoreEventDispatcher->OnSystemEvent(ESystemEvent::WindowActivate, wParam, lParam);
			break;

		case WM_SETFOCUS:
		case WM_KILLFOCUS:
			CoreEventDispatcher->OnSystemEvent(ESystemEvent::WindowChangeFocus, wParam, lParam);
			break;

		case WM_INPUTLANGCHANGE:
			CoreEventDispatcher->OnSystemEvent(ESystemEvent::InputLanguageChange, wParam, lParam);
			break;

			/*
		case WM_SYSCOMMAND:
			if ((wParam & 0xFFF0) == SC_SCREENSAVE)
			{
				// Check if screen saver is allowed
				IConsole* const pConsole = gEnv->pConsole;

				const ICVar* const pVar = pConsole ? pConsole->GetCVar("sys_screensaver_allowed") : 0;

				return pVar && pVar->GetIVal() == 0;
			}
			return false;



			//마우스 이벤트

		case WM_MOUSEACTIVATE:

			*pResult = MA_ACTIVATEANDEAT;

			return true;



			// Hardware mouse counters

		case WM_ENTERSIZEMOVE:

			sbInSizingModalLoop = true;

			// Fall through intended

		case WM_ENTERMENULOOP:
		{
			IHardwareMouse* const pMouse = GetIHardwareMouse();
			if (pMouse)
			{
				pMouse->IncrementCounter();
			}
		}
		return true;

		case WM_CAPTURECHANGED:

			// If WM_CAPTURECHANGED is received after WM_ENTERSIZEMOVE (ie, moving/resizing begins).

			// but no matching WM_EXITSIZEMOVE is received (this can happen if the window is not actually moved).

			// we still need to decrement the hardware mouse counter that was incremented when WM_ENTERSIZEMOVE was seen.

			// So in this case, we effectively treat WM_CAPTURECHANGED as if it was the WM_EXITSIZEMOVE message.

			// This behavior has only been reproduced the window is deactivated during the modal loop (ie, breakpoint triggered and focus moves to VS).

		case WM_EXITSIZEMOVE:
			if (!sbInSizingModalLoop)
			{
			return false;
			}

			sbInSizingModalLoop = false;

			// Fall through intended

		case WM_EXITMENULOOP:
		{
			IHardwareMouse* const pMouse = GetIHardwareMouse();
			if (pMouse)
			{
				pMouse->DecrementCounter();
			}
		}

		return (uMsg != WM_CAPTURECHANGED);
		*/


		//마우스 이벤트전달을 위한 이벤트 세팅

		case WM_MOUSEMOVE:
			mouseEvent = EMouseEvent::MOVE;
			break;

		case WM_LBUTTONDOWN:
			mouseEvent = EMouseEvent::LBUTTONDOWN;
			break;

		case WM_LBUTTONUP:
			mouseEvent = EMouseEvent::LBUTTONUP;
			break;

		case WM_LBUTTONDBLCLK:
			mouseEvent = EMouseEvent::LBUTTONDOUBLECLICK;
			break;

		case WM_RBUTTONDOWN:
			mouseEvent = EMouseEvent::RBUTTONDOWN;
			break;

		case WM_RBUTTONUP:
			mouseEvent = EMouseEvent::RBUTTONUP;
			break;

		case WM_RBUTTONDBLCLK:
			mouseEvent = EMouseEvent::RBUTTONDOUBLECLICK;
			break;

		case WM_MBUTTONDOWN:
			mouseEvent = EMouseEvent::MBUTTONDOWN;
			break;

		case WM_MBUTTONUP:
			mouseEvent = EMouseEvent::MBUTTONUP;
			break;

		case WM_MBUTTONDBLCLK:
			mouseEvent = EMouseEvent::MBUTTONDOUBLECLICK;
			break;

		case WM_MOUSEWHEEL:
			mouseEvent = EMouseEvent::WHEEL;
			break;

		default:
			return message;
		}


		if (CoreMouse != nullptr)
		{
			CoreMouse->OnMouseEvent(mousePosX, mousePosY, mouseEvent, mouseWheel);
		}

		//미처리 메시지 전송
		return 0;
	}

	bool CEngineFramework::_CreateWindow()
	{
		using namespace std::placeholders;
		//설정값 획득
		Core::Render::CDx11RendererConfig config;
		config.LoadConfig(*CoreConfig);
		
		Core::GetCoreEnv()->pWindowScreen = std::make_unique<Window::CWindowsGUI>();
		Core::GetCoreEnv()->pWindowScreen->SetMouse(CoreMouse.get());

		if (config.IsLoaded() && config.IsHereWorkspace())
		{
			uint32 w, h;

			try
			{
				w = std::stoi(config.pWorkSpace->Attribute(TEXTL("Width")));
				h = std::stoi(config.pWorkSpace->Attribute(TEXTL("Height")));
			}
			catch (...)
			{
				w = 1024;
				h = 800;
			}

			Core::GetCoreEnv()->pWindowScreen->SetSize(w, h);
		}
		else
		{
			assert(false && "Failed Config Load");
		}


		//게임 윈도우 전용 핸들러 연결
		Core::GetCoreEnv()->pWindowScreen->SetMessageHandler(_GameWindowHandler);


		if (Core::GetCoreEnv()->pWindowScreen->Create(m_hInstance, TEXTL("GypGUI")) == false)
		{
			return false;
		}


		return true;
	}

	bool CEngineFramework::InitializSystem()
	{
		if (m_initializedCoreSystem == false)
		{
			assert(false && TEXTL("코어 시스템 미 초기화"));
			return false;
		}
		CoreLogInsert(TEXTL("[Engine] 초기화 시작"));


		//윈도우 생성
		if (_CreateWindow() == false)
		{
			return false;
		}
		CoreLogInsert(TEXTL("[Engine] Window 생성완료"));



		//모든 시스템이 전부 초기화 되었는지 확인하는 코드 작성

		//완료면 ready 따로 처리 할 수 있음
		m_isReady = true;
		m_initializedEngineSystem = true;




		CoreLogInsert(TEXTL("[Engine] 초기화 종료"));
		return true;
	}

	int CEngineFramework::ServiceLoop()
	{
		//gGameFrameWork.FrameAdvance();
		m_GameTimer.Tick(120.f);





		//코어 이벤트 업데이트
		CoreEventDispatcher->Update();


		/////////////
		//Render
		//////////////
		Core::GetCoreEnv()->pRenderer->Render(nullptr);
		Core::GetCoreEnv()->pRenderer->Present();

		//frame 계산
		std::stringstream frame;
		frame << std::setfill('0') << std::setw(3) << m_GameTimer.GetFrameRate() << TEXTL("FPS");
		Core::GetCoreEnv()->pWindowScreen->SetTitleBar(frame.str().c_str());

		return 0;
	}

	int CEngineFramework::Start()
	{
		if (m_initializedEngineSystem == false)
		{
			CoreLogInsert(TEXTL("[Engine] FatalError EngineSystem 초기화 되지 않음"));
			return -1;
		}

		CoreLogInsert(TEXTL("[Engine] Start"));

		//유효 상태 확인
		if (m_isReady == false)
		{
			return -1;
		}	

		//MainGUI on
		if (Core::GetCoreEnv()->pWindowScreen->Show() == false)
		{
			return -1;
		}

		m_isRunning = true;
		RECT rcClient;
		GetClientRect(Core::GetCoreEnv()->pWindowScreen->GetHWND(), &rcClient);
		std::stringstream s;
		s << TEXTL("작업 영역 크기 ");
		s << rcClient.right - rcClient.left << " " << rcClient.bottom - rcClient.top;
		CoreLogInsert(s.str().c_str());


		CoreLogInsert(TEXTL("[Engine] Enter Running Loop"));

		MSG msg;

		//Running 상태일 때만 동작
		while (m_isRunning)
		{

			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			//EngineFrameworkService Loop
			ServiceLoop();
		}
		CoreLogInsert(TEXTL("[Engine] Stop"));

		Release();
		return (int)msg.wParam;
	}

	void CEngineFramework::Stop()
	{
		m_isRunning = false;
	}

	void CEngineFramework::Release()
	{
		CoreLogInsert(TEXTL("[Engine] Release Start"));

		//자원해제 명령 전파
		CoreEventDispatcher->OnSystemEvent(Core::ESystemEvent::ReleaseSystem, 0, 0);

		//마지막에 Core
		Core::CoreSystem::ReleaseCore();

		//종료
		PostQuitMessage(0);
		CoreLogInsert(TEXTL("[Engine] Release End"));
	}

}

