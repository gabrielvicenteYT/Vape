#include "MinHook.h"

#include "export.h"

#include "java.h"

#include "natives/forge.h"
#include "natives/main.h"
#include "natives/reflection.h"

#include "rpc/rpcsocket.h"

#include <vector>

// TODO
std::vector<jobject> GetLoadedClasses(PCLIENT_CTX client_ctx)
{
    PJVM_CTX jvm = client_ctx->jvm;

    jint class_count;
    jclass *classes;

    jvm->jvmti->GetLoadedClasses(&class_count, &classes);

    for (jint index = 0; index < class_count; index++)
    {

        jclass cls = classes[class_count];

        jobject global = jvm->jni->NewGlobalRef(cls);

        char *signature;
        jvm->jvmti->GetClassSignature(cls, &signature, NULL);

        jvm->jvmti->Deallocate((unsigned char *)signature);
    }

    jvm->jvmti->Deallocate((unsigned char *)classes);
}

// todo initialize.
jobject g_renderHandler;

jmethodID midOnNotification;
JNIEnv *detourJNIEnv;
jclass clsGUI;

BOOL finished;

HWND window;

typedef BOOL(WINAPI *_SwapBuffers)(HDC);
_SwapBuffers fpSwapBuffers;

VOID DetourSwapBuffers(HDC hdc)
{
    HWND hwnd = WindowFromDC(hdc);
    if (hwnd != window)
    {
        printf("hooked\n");
        window = hwnd;
        WNDPROC lpPrevWndFunc = (WNDPROC)SetWindowLongPtr(
            GetForegroundWindow(), GWLP_WNDPROC,
            (LONG_PTR) & [lpPrevWndFunc](HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT {
                long v8 = 0x1DB7; // WM_.... | WM_..... | ....
                long v9 = 0x37;   // WM_.... | WM_..... | ....

                if (msg - 512 > 0xC || !_bittest(&v8, msg - 512))
                {
                    if (msg - 256 > 0x5)
                        return CallWindowProc(lpPrevWndFunc, hWnd, msg, wParam, lParam);
                    if (!_bittest(&v9, msg - 256))
                        return CallWindowProc(lpPrevWndFunc, hWnd, msg, wParam, lParam);
                }

                if (!detourJNIEnv)
                {
                    jint result = g_jvmCtx->vm->GetEnv((void **)(&detourJNIEnv), JNI_VERSION_1_8);
                    if (result == JNI_EDETACHED)
                    {
                        jint attachResult = g_jvmCtx->vm->AttachCurrentThread((void **)(&detourJNIEnv), NULL);
                        if (attachResult == JNI_OK)
                        {
                            printf("attached\n");
                        }
                        else
                        {
                            printf("failed to attach\n");
                        }
                    }
                    else
                    {
                        printf("already attached\n");
                    }
                }

                if (midOnNotification &&
                    detourJNIEnv->CallStaticBooleanMethod(clsGUI, midOnNotification, msg, wParam, lParam))
                {
                    return 0;
                }

                return CallWindowProc(lpPrevWndFunc, hWnd, msg, wParam, lParam);
            });
    }
    fpSwapBuffers(hdc);
}

BOOL Disable()
{
    MH_Uninitialize();
}

VOID Unitialize()
{
    MH_Initialize();

    MH_RemoveHook(SwapBuffers);

    MH_CreateHookApi(L"GDI32.dll", "SwapBuffers", DetourSwapBuffers, reinterpret_cast<void **>(&fpSwapBuffers));

    if (!Disable())
    {
    }
}

// PacketID
// Recieve Count
// For each, read int (size of resource)
// read size bytes
// put to vector
VOID GetTexturesAndStringsFromSocket()
{
    // SendPacketId(v13, v7, 609u);
    // SendPacketId(v13, v14, 2u);
    // SendPacketId(v13, v15, 200u);
}

// Expose the Initialize function to be called from the start thread.
VOID Initialize(int rpcPort)
{
    MainInitialization(rpcPort);
    while (!finished)
        Sleep(100);
}

BOOL OtherVersionStuff(PCLIENT_CTX client_ctx)
{
}

BOOL SendMinecraftVersion(PCLIENT_CTX client_ctx, PJVM_CTX jvm_ctx, RPCSocket::PRPCSocketContext socket_ctx)
{
}

BOOL has_jvmti_access = FALSE;

// 0x0000000180001DC0
DWORD WINAPI SendGoodbyeToSocketAndDetatch(LPVOID /*lpThreadParameter*/)
{

    return 0;
}

DWORD WINAPI GarbageCollectAndCallGoodbye(LPVOID /*lpThreadParameter*/)
{
    PJVM_CTX ctx = GetJVMContext();

    if (has_jvmti_access)
    {
        ctx->jvmti->ForceGarbageCollection();
    }

    jclass clsSystem = ctx->jni->FindClass("java/lang/System");
    if (clsSystem)
    {
        jmethodID midGC = ctx->jni->GetStaticMethodID(clsSystem, "gc", "()V");
        ctx->jni->CallStaticVoidMethod(clsSystem, midGC);
    }

    if (ctx->vm)
    {
        ctx->vm->DetachCurrentThread();
    }

    CreateThread(NULL, 0, SendGoodbyeToSocketAndDetatch, NULL, NULL, NULL);
    ExitThread(EXIT_SUCCESS);
}

BOOL MainInitialization(INT rpcPort)
{
    PJVM_CTX jvm = GetJVMContext();

    // RPCSocket::SendStatus(socket, 8);

    // jvm->jvmti->GetLoadedClasses();

    // RPCSocket::SendStatus(socket, 9);

    while (jvm->jni->ExceptionCheck())
    {
        jvm->jni->ExceptionClear();
    }

    has_jvmti_access = jvm->jvmti != NULL;

    if (jvm->vm && jvm->jni)
    {
        if (jvm->jvmti)
        {
            jvm->jvmti->DisposeEnvironment();
        }

        jvm->vm->DetachCurrentThread();
        jvm->vm = NULL;
        jvm->jni = NULL;
    }

    free(jvm);

    Sleep(1000);

    HANDLE tHandle = CreateThread(NULL, 0, GarbageCollectAndCallGoodbye, NULL, NULL, NULL);
    BOOL result = CloseHandle(tHandle);

    finished = 1;

    return result;
}

VOID UnregisterNativesAndWindowCallbacks(PCLIENT_CTX client_ctx)
{
    // g_jvmCtx->jni->UnregisterNatives(client_ctx->clsMain);
    // g_jvmCtx->jni->UnregisterNatives(client_ctx->clsReflection);

    // TODO: Delete all JVMTI loadedClasses globals.
    // TODO: Research secondary global deletion loop. (0x00000001800101D9)

    PFORGE_CTX forge_ctx = client_ctx->forge;
    if (forge_ctx)
    {
        UnregisterForge(forge_ctx);

        g_jvmCtx->jni->DeleteGlobalRef(g_renderHandler);
    }

    SetWindowLongPtr(window, GWLP_WNDPROC, NULL);
}