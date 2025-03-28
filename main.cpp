#include <windows.h>
#include <cstdio>
#include <cstdlib>

// stripped this from the shit sib mod, i havent looked at much of it yet
typedef struct _ACCENTPOLICY {
    int nAccentState;
    int nFlags;
    int nColor;
    int nAnimationId;
} ACCENTPOLICY;

typedef enum _WINDOWCOMPOSITIONATTRIB {
    WCA_ACCENT_POLICY = 19
} WINDOWCOMPOSITIONATTRIB;

typedef struct _WINDOWCOMPOSITIONATTRIBDATA {
    WINDOWCOMPOSITIONATTRIB Attrib;
    PVOID pvData;
    SIZE_T cbData;
} WINDOWCOMPOSITIONATTRIBDATA;

typedef BOOL(WINAPI* pfnSetWindowCompositionAttribute)(HWND, WINDOWCOMPOSITIONATTRIBDATA*);

void SetBlur(HWND hWnd, bool enable) {
    HMODULE hUser32 = GetModuleHandleA("user32.dll");
    if (!hUser32) {
        printf("Failed to load user32.dll\n");
        return;
    }

    auto SetWindowCompositionAttribute = (pfnSetWindowCompositionAttribute)
        GetProcAddress(hUser32, "SetWindowCompositionAttribute");
    if (!SetWindowCompositionAttribute) {
        printf("SetWindowCompositionAttribute not available\n");
        return;
    }

    ACCENTPOLICY policy = { enable ? 3 : 0, 2, 0, 0 };
    WINDOWCOMPOSITIONATTRIBDATA data = { WCA_ACCENT_POLICY, &policy, sizeof(policy) };
    SetWindowCompositionAttribute(hWnd, &data);

    printf("%s\n", enable ? "Blur enabled." : "Blur disabled.");
}

int main() {
    HWND lastHwnd = nullptr;
    char windowTitle[256];

    while (true) {
        HWND hWnd = GetForegroundWindow();
        if (hWnd && hWnd != lastHwnd) {
            lastHwnd = hWnd;
            GetWindowTextA(hWnd, windowTitle, sizeof(windowTitle));
            printf("Current window: %s\n", windowTitle);
            printf("Do you want to enable, disable blurbehind, or do nothing for the current window? (e/d/n): ");
            char choice;
            scanf(" %c", &choice);
            if (choice == 'e' || choice == 'E') {
                SetBlur(hWnd, true);
            } else if (choice == 'd' || choice == 'D') {
                SetBlur(hWnd, false);
            } else if (choice == 'n' || choice == 'N') {
                printf("No change applied.\n");
            }
        }
        Sleep(1000);
    }
    return 0;
}
