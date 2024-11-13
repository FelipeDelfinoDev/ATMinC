#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float saldo = 0;
int valorSaque = 0;
HWND hSaldo, hResultado, hSaqueInput, hDepositoInput, hSaqueBtn, hDepositoBtn;
HWND hSaqueLabel, hDepositoLabel;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void atualizarSaldoDisplay() {
    char buffer[50];
    sprintf(buffer, "R$ %.2f", saldo);
    SetWindowText(hSaldo, buffer);  //mostra saldo para usuario
}

void realizarSaque() {
    float valorSaque2;
    char buffer[50];
    
    GetWindowText(hSaqueInput, buffer, sizeof(buffer));
    valorSaque2 = atof(buffer);
    
    if (valorSaque2 < 0) {
        MessageBox(NULL, "Quantia invalida", "Erro", MB_ICONERROR);
        return;  //exibe mensagem caso usuario digite um valor abaixo de 0
    }
    
    valorSaque = (int)valorSaque2;
    if (valorSaque > saldo) {
        MessageBox(NULL, "Saldo insuficiente", "Erro", MB_ICONERROR);
        return; //exibe mensagem se o valor de saque for maior que o saldo 
    }
    
    saldo -= valorSaque;
    atualizarSaldoDisplay();  
    
    //exibe o saque
    sprintf(buffer, "Voce retirou R$ %.2f", (float)valorSaque);
    SetWindowText(hResultado, buffer);
    
   //distribuição das notas
    char notas[500];
    int n100 = valorSaque / 100;
    valorSaque -= n100 * 100; 
    int n50 = valorSaque / 50; 
    valorSaque -= n50 * 50; 
    int n20 = valorSaque / 20;
    valorSaque -= n20 * 20;
    int n10 = valorSaque / 10;
    valorSaque -= n10 * 10;
    int n5 = valorSaque / 5;
    valorSaque -= n5 * 5;
    int n2 = valorSaque / 2;
    valorSaque -= n2 * 2;
    int n1 = valorSaque;
    
   
    sprintf(notas, 
            "\nNotas de R$100: %d\r\n"
            "Notas de R$50: %d\r\n"
            "Notas de R$20: %d\r\n"
            "Notas de R$10: %d\r\n"
            "Notas de R$5: %d\r\n"
            "Notas de R$2: %d\r\n"
            "Notas de R$1: %d\r\n", 
            n100, n50, n20, n10, n5, n2, n1);
    
    SetWindowText(hResultado, notas);  
    
    
    SetWindowText(hSaqueInput, "");
}

void realizarDeposito() {
    float valorDeposito;
    char buffer[50];
    
    GetWindowText(hDepositoInput, buffer, sizeof(buffer));
    valorDeposito = atof(buffer);
    
    if (valorDeposito < 0) {
        MessageBox(NULL, "Quantia invalida", "Erro", MB_ICONERROR);
        return;
    }
    
    saldo += valorDeposito;
    atualizarSaldoDisplay();  
    

    sprintf(buffer, "Voce depositou R$ %.2f", valorDeposito);
    SetWindowText(hResultado, buffer);
    
    
    SetWindowText(hDepositoInput, "");
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wcex;
    HWND hWnd;
    MSG msg;
    
    //janela
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "CaixaEletronico";
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    
    if (!RegisterClassEx(&wcex)) {
        return 1;
    }
    
    hWnd = CreateWindow("CaixaEletronico", "Caixa Eletronico", WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, CW_USEDEFAULT, 450, 450, NULL, NULL, hInstance, NULL);
    
    if (!hWnd) {
        return 1;
    }
    
    
    CreateWindow("STATIC", "Saldo:", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 10, 100, 20, hWnd, NULL, hInstance, NULL);
    hSaldo = CreateWindow("STATIC", "R$ 0.00", WS_VISIBLE | WS_CHILD | SS_LEFT, 100, 10, 150, 20, hWnd, NULL, hInstance, NULL);
    
    hSaqueLabel = CreateWindow("STATIC", "Valor do Saque:", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 40, 120, 20, hWnd, NULL, hInstance, NULL);
    hSaqueInput = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 130, 40, 100, 20, hWnd, NULL, hInstance, NULL);
    hSaqueBtn = CreateWindow("BUTTON", "Saque", WS_VISIBLE | WS_CHILD, 10, 70, 100, 30, hWnd, (HMENU)1, hInstance, NULL);
    
    hDepositoLabel = CreateWindow("STATIC", "Valor do Deposito:", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 110, 120, 20, hWnd, NULL, hInstance, NULL);
    hDepositoInput = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 130, 110, 100, 20, hWnd, NULL, hInstance, NULL);
    hDepositoBtn = CreateWindow("BUTTON", "Deposito", WS_VISIBLE | WS_CHILD, 10, 140, 100, 30, hWnd, (HMENU)2, hInstance, NULL);
    
    // resultado
    hResultado = CreateWindow("EDIT", "Resultado:", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL, 
                              10, 180, 420, 200, hWnd, NULL, hInstance, NULL);
    
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case 1:  // Botao para sacar
                    realizarSaque();
                    break;
                case 2:  // botao para depositar
                    realizarDeposito();
                    break;
            }
            break;
        
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    
    return 0;
}
