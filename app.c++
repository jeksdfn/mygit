#include <Windows.h>
#include <iostream>
#include <vector>
#include "imgui/imgui.h"
#include <d3d11.h>

// DirectX variables
HWND window = NULL;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

// Global variables
bool showGui = true;

// Function to initialize ImGui
void InitImGui(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context) {
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(device, context);
}

// Win32 Window Procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) return true;
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

// Main function (GUI-based application)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Register window class
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, "ImGui Example", NULL };
    RegisterClassEx(&wc);
    window = CreateWindow(wc.lpszClassName, "ImGui DirectX Example", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 720, NULL, NULL, wc.hInstance, NULL);

    // Initialize DirectX
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 1;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = window;
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;

    IDXGISwapChain* pSwapChain;
    D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, 0, D3D11_SDK_VERSION, &sd, &pSwapChain, &pDevice, NULL, &pContext);

    ID3D11Texture2D* pBackBuffer;
    pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
    pBackBuffer->Release();

    ShowWindow(window, SW_SHOWDEFAULT);
    UpdateWindow(window);

    // Initialize ImGui
    InitImGui(window, pDevice, pContext);

    // Main loop
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }

        // Start ImGui Frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // Render GUI
        ImGui::Begin("Example GUI");
        ImGui::Text("Hello, world!");
        ImGui::End();

        // Render ImGui
        ImGui::Render();
        pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    pContext->ClearState();
    pDevice->Release();
    pContext->Release();
    pSwapChain->Release();
    mainRenderTargetView->Release();

    UnregisterClass(wc.lpszClassName, wc.hInstance);
    return 0;
}
