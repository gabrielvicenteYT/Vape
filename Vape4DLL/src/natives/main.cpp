/**
 * The MIT License (MIT)
 *
 * Copyright (C) 2022 Decencies
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "main.h"

PCLIENT_CTX g_client_ctx;

void Exit(JNIEnv *env, jclass caller, jboolean s)
{
    // clean_and_disable_hooks();
    // socket_send_packet(*(app_ctx + 16), 606);
    // socket_send_packet(*(app_ctx + 16), s);
    // app_ctx->running = false;
}

void DCallback(JNIEnv *env, jclass caller)
{
}

/*-------------------------------------------------------
    Tells the socket server (launcher) that we have sent
    all of the contents needed.
-------------------------------------------------------*/
void FinishSending(JNIEnv *env, jclass caller)
{
}

/*-------------------------------------------------------
    Gets the class with the specified name.

    Note:
    We don't know what the difference is between this and GVC.
-------------------------------------------------------*/
jclass GetClass(JNIEnv *env, jclass caller, jstring name)
{
    const char *className = env->GetStringUTFChars(name, NULL);

    jclass cls = 0;

    env->ReleaseStringUTFChars(name, className);

    return cls;
}

typedef struct TEXTURE
{
    char *name;
    jsize size;
    const jbyte *data;
} * PTEXTURE;

/*-------------------------------------------------------
    Gets the texture bytes of the specified resource

    Note:
    The texture names are XOR'd with the key derived
    from GetKey.
-------------------------------------------------------*/
jbyteArray GetTexture(JNIEnv *env, jclass caller, jstring name)
{
    const char *chars = env->GetStringUTFChars(name, NULL);

    PTEXTURE texture{};

    jbyteArray data = env->NewByteArray(texture->size);
    env->SetByteArrayRegion(data, 0, texture->size, texture->data);
    env->ReleaseStringUTFChars(name, chars);
    return data;
}

/*-------------------------------------------------------
    Gets the XOR key for texture names.
-------------------------------------------------------*/
jint GetKey(JNIEnv *env, jclass caller)
{
    return 0;
}

jstring GetSettings(JNIEnv *env, jclass caller)
{
    return 0;
}

void SaveSettings(JNIEnv *env, jclass caller, jstring string)
{
}

void n_MessageBox(JNIEnv *env, jclass caller, jint code)
{
    MessageBox(NULL, L"Error occured, error code: " + code, L"Error", MB_OK | MB_ICONERROR);
}

jclass GetClassJava(JNIEnv *env, jclass caller, jstring name)
{
    const char *className = env->GetStringUTFChars(name, NULL);
    jclass cls = 0;
    env->ReleaseStringUTFChars(name, className);
    return cls;
}

jstring GetClassSignature(JNIEnv *env, jclass caller, jclass cls)
{
    JavaVM *vm;
    jvmtiEnv *jvmti_env;
    env->GetJavaVM(&vm);
    vm->GetEnv(reinterpret_cast<void **>(&jvmti_env), JVMTI_VERSION_1_1);

    char *classSignature;
    jvmti_env->GetClassSignature(cls, &classSignature, NULL);

    jstring result = NULL;

    if (classSignature)
    {
        result = env->NewStringUTF(classSignature);
        jvmti_env->Deallocate((unsigned char *)classSignature);
    }
    return result;
}

jstring CopyString(JNIEnv *env, jclass caller, jint index)
{
    return 0;
}

void Reload(JNIEnv *env, jclass caller)
{
}

/*-------------------------------------------------------
    Gets the Forge RenderHandler for FreeCam.
-------------------------------------------------------*/
jobject GetRenderHandler(JNIEnv *env, jclass caller)
{
    return 0;
}

/*-------------------------------------------------------
    Copies the specified string to the clipboard.
-------------------------------------------------------*/
void ClipboardCopy(JNIEnv *env, jclass caller, jstring string)
{
    jint length = env->GetStringLength(string);
    char *buffer = new char[length];
    env->GetStringUTFRegion(string, 0, length, buffer);

    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, length + 1);
    char *gl = reinterpret_cast<char *>(GlobalLock(hMem));
    memcpy(gl, buffer, length + 1);
    GlobalUnlock(hMem);
    OpenClipboard(NULL);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, hMem);
    CloseClipboard();
    GlobalFree(hMem);
    free(buffer);
}

/*-------------------------------------------------------
    Prints the specified message to the console.
-------------------------------------------------------*/
void Print(JNIEnv *env, jclass caller, jstring string)
{
    const char *chars = env->GetStringUTFChars(string, NULL);
    printf("%s\n", chars);
    return env->ReleaseStringUTFChars(string, chars);
}

/*-------------------------------------------------------
    Gets the state of the specified virtual keycode.
-------------------------------------------------------*/
jshort n_GetKeyState(JNIEnv *env, jclass caller, jint virtualKey)
{
    if (FindWindowA("LWJGL", NULL) == GetForegroundWindow())
        return GetKeyState(virtualKey);
    return 0;
}

/*-------------------------------------------------------
     Gets the instances of a Class within the JVM instance.

     Note:
     Probably used for debugging on the client side.
 -------------------------------------------------------*/
jobjectArray GetClassObjects(JNIEnv *env, jclass caller, jclass cls)
{
    JavaVM *vm;
    jvmtiEnv *jvmti_env;
    env->GetJavaVM(&vm);
    vm->GetEnv(reinterpret_cast<void **>(&jvmti_env), JVMTI_VERSION_1_1);

    jvmtiCapabilities capabilities{};

    capabilities.can_tag_objects = 1;

    jint count;
    jobject *object_result;

    if (jvmti_env->AddCapabilities(&capabilities) == JVMTI_ERROR_NONE)
    {
        auto iterator = [](jlong, jlong, jlong *tag_ptr, void *) {
            *tag_ptr = 6969;
            return JVMTI_ITERATION_CONTINUE;
        };

        if (jvmti_env->IterateOverInstancesOfClass(cls, JVMTI_HEAP_OBJECT_EITHER, iterator, NULL) == JVMTI_ERROR_NONE)
            jvmti_env->GetObjectsWithTags(1, new jlong[]{6969}, &count, &object_result, NULL);
    }

    jobjectArray arr = env->NewObjectArray(count, cls, NULL);

    if (object_result)
    {
        for (jsize i = 0; i < count; i++)
        {
            env->SetObjectArrayElement(arr, i, object_result[i]);
            jvmti_env->SetTag(object_result[i], NULL);
        }

        jvmti_env->Deallocate((unsigned char *)object_result);
    }

    return arr;
}

/*-------------------------------------------------------
    Transforms (x,y,z) with Project.

    Note:
    Only really used for the Arrows module.
-------------------------------------------------------*/
jdoubleArray Translate(JNIEnv *env, jclass caller, jdouble x, jdouble y, jdouble z)
{
    GLdouble modelview_matrix[16];
    GLdouble projection_matrix[16];
    GLint viewport_matrix[4];

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview_matrix);
    glGetDoublev(GL_PROJECTION_MATRIX, projection_matrix);
    glGetIntegerv(GL_VIEWPORT, viewport_matrix);

    jdouble winX, winY, winZ;

    gluProject(x, y, z, modelview_matrix, projection_matrix, viewport_matrix, &winX, &winY, &winZ);

    jdoubleArray ret = env->NewDoubleArray(3);

    env->SetDoubleArrayRegion(ret, 0, 1, &winX);
    env->SetDoubleArrayRegion(ret, 1, 1, &winY);
    env->SetDoubleArrayRegion(ret, 2, 1, &winZ);

    return ret;
}

/*-------------------------------------------------------
    Gets the class bytes of the specified class.

    Note:
    Deez nuts.
-------------------------------------------------------*/
jbyteArray GetClassBytes(JNIEnv *env, jclass caller, jclass cls)
{
    jvmtiEnv *jvmti = g_client_ctx->jvm->jvmti;
    if (!jvmti)
        g_client_ctx->jvm->vm->GetEnv(reinterpret_cast<void **>(&jvmti), JVMTI_VERSION_1_2);
    char *class_name;
    jvmti->GetClassSignature(cls, &class_name, NULL);

    // is this really required?
    memmove(target_class_name, class_name, strlen(class_name));

    capturing_class_bytes = true;

    EnableClassFileLoadHook();

    jvmtiError error = jvmti->RetransformClasses(1, new jclass[]{cls});

    DisableClassFileLoadHook();

    if (error)
        return NULL;

    jbyteArray arr;

    if (true)
    {
        arr = env->NewByteArray(class_file_hook_buffer_size);
        env->SetByteArrayRegion(arr, 0, class_file_hook_buffer_size, reinterpret_cast<jbyte *>(class_file_hook_buffer));

        memset(class_file_hook_buffer, NULL, class_file_hook_buffer_size);
        free(class_file_hook_buffer);
    }
    else
    {
        arr = env->NewByteArray(v28);
        env->SetByteArrayRegion(arr, 0, v28, v27);
    }

    return arr;
}

/*-------------------------------------------------------
    Sets the class bytes of the specified class with
    the specified bytecode.

    Note:
    Deez nuts.
-------------------------------------------------------*/
jint SetClassBytes(JNIEnv *env, jclass caller, jclass cls, jbyteArray bytes)
{
}

/*-------------------------------------------------------
    Gets all of the fields within the specified class
    as a string array.

    Note:
    We don't know the purpose of this function.
-------------------------------------------------------*/
jobjectArray GetClassFields(JNIEnv *env, jclass caller, jclass cls)
{
    JavaVM *vm;
    jvmtiEnv *jvmti_env;
    env->GetJavaVM(&vm);
    vm->GetEnv(reinterpret_cast<void **>(&jvmti_env), JVMTI_VERSION_1_1);

    jint count;
    jfieldID *fields;

    jvmti_env->GetClassFields(cls, &count, &fields);

    jclass clsString = env->FindClass("java/lang/String");

    jobjectArray arr = env->NewObjectArray(count, clsString, NULL);

    for (int i = 0; i < count; i++)
    {
        char *name;
        jvmti_env->GetFieldName(cls, fields[i], &name, NULL, NULL);
        env->SetObjectArrayElement(arr, i, env->NewStringUTF(name));
        jvmti_env->Deallocate((unsigned char *)name);
    }

    jvmti_env->Deallocate((unsigned char *)fields);

    return arr;
}

/*-------------------------------------------------------
    Gets all of the methods within the specified class
    as a string array.

    Note:
    We don't know the purpose of this function.
-------------------------------------------------------*/
jobjectArray GetClassMethods(JNIEnv *env, jclass caller, jclass cls)
{
    JavaVM *vm;
    jvmtiEnv *jvmti_env;
    env->GetJavaVM(&vm);
    vm->GetEnv(reinterpret_cast<void **>(&jvmti_env), JVMTI_VERSION_1_1);

    jint count;
    jmethodID *methods;

    jvmti_env->GetClassMethods(cls, &count, &methods);

    jclass clsString = env->FindClass("java/lang/String");

    jobjectArray arr = env->NewObjectArray(count, clsString, NULL);

    for (int i = 0; i < count; i++)
    {
        char *name;
        jvmti_env->GetMethodName(methods[i], &name, NULL, NULL);
        env->SetObjectArrayElement(arr, i, env->NewStringUTF(name));
        jvmti_env->Deallocate((unsigned char *)name);
    }

    jvmti_env->Deallocate((unsigned char *)methods);

    return arr;
}

void DrawString0(const char *string, float *color, double x, double y)
{
    // texture_font_load_glyphs
}

jint DrawString(JNIEnv *env, jclass caller, jint id, jstring string, jdouble x, jdouble y, jint color)
{
    const char *cString = env->GetStringUTFChars(string, NULL);

    float fStack0000000000000030 = (color >> 16 & 0xff) / 255;
    float fStack0000000000000034 = (color >> 8 & 0xff) / 255;
    float fStack0000000000000038 = (color & 0xff) / 255;
    float fStack000000000000003c = (color >> 24 & 0xff) / 255;

    bool blendEnabled = glIsEnabled(GL_BLEND);
    bool tex2dEnabled = glIsEnabled(GL_TEXTURE_2D);

    if (!blendEnabled)
    {
        glEnable(GL_BLEND);
    }
    if (!tex2dEnabled)
    {
        glEnable(GL_TEXTURE_2D);
    }

    DrawString0(cString, new float[]{0}, x, y);

    if (!tex2dEnabled)
    {
        glDisable(GL_TEXTURE_2D);
    }
    if (!blendEnabled)
    {
        glDisable(GL_BLEND);
    }

    env->ReleaseStringUTFChars(string, cString);

    return 0;
}

jdouble GetStringWidth(JNIEnv *env, jclass caller, jint id, jstring string)
{
    return 0;
}

jdouble GetStringHeight(JNIEnv *env, jclass caller, jint id, jstring string)
{
    return 0;
}

void SendMouseDown(JNIEnv *env, jclass caller, jint wParam, jint msg)
{
    HWND hWnd = GetForegroundWindow();
    POINT point;
    if (GetCursorPos(&point))
        ScreenToClient(hWnd, &point);
    SendMessageW(hWnd, msg, wParam, MAKELPARAM(point.x, point.y));
}

jstring GetProfile(JNIEnv *env, jclass caller, jstring id)
{
    const char *profileId = env->GetStringUTFChars(id, NULL);

    // socket_send_packet(socket_ctx, 605);
    // socket_send(socket_ctx, profileId);
    // socket_send_packet(socket_ctx, 200);
    // char *contents = socket_read(socket_ctx);

    jstring profile = env->NewStringUTF("e30");

    env->ReleaseStringUTFChars(id, profileId);

    return profile;
}

// DLL -> Launcher RPC -> WebSocket
void SetUsername(JNIEnv *env, jclass caller, jstring string)
{
    // std::cout << "SetUsername: " << GetString(env, string)  << std::endl;
}

/*-------------------------------------------------------
    Returns True if the Minecraft version used is vanilla.

    Note:
    For Lunar / BLC, this returns True.
-------------------------------------------------------*/
jboolean IsVanilla(JNIEnv *env, jclass caller)
{
    return app_ctx->isVanilla;
}

/*-------------------------------------------------------
    Gets a vanilla minecraft class. (version class?)
-------------------------------------------------------*/
jclass GetVanillaClass(JNIEnv *env, jclass caller, jstring name)
{
    const char *className = env->GetStringUTFChars(name, NULL);
    jclass cls = NULL;
    env->ReleaseStringUTFChars(name, className);
    return cls;
}

/*-------------------------------------------------------
    Gets the minor version of Forge.
-------------------------------------------------------*/
jint GetMinorVersion(JNIEnv *env, jclass caller)
{
    return app_ctx->forgeVersion;
}

/*-------------------------------------------------------
    Tells the socket to recieve contents
-------------------------------------------------------*/
void RecieveSocketContents(JNIEnv *env, jclass caller)
{
}

int MakeFontInternal()
{
}

jint MakeFont(JNIEnv *env, jclass caller, jint id, jint size, jstring name)
{
    // font_manager_t *font_manager = font_manager_new(512, 512, LCD_FILTERING_OFF);

    // glClearColor(1.0, 1.0, 1.0, 1.0);
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glGenTextures(1, &font_manager->atlas->id);
    // glBindTexture(GL_TEXTURE_2D, font_manager->atlas->id);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, font_manager->atlas->width, font_manager->atlas->height, 0, GL_RGBA,
    // GL_UNSIGNED_BYTE, font_manager->atlas->data);

    // return font_manager->atlas->id;
}

/*-------------------------------------------------------
    Update Discord
-------------------------------------------------------*/
void UpdateDiscord(JNIEnv *env, jclass caller, jstring title, jstring subtitle)
{
}

/*-------------------------------------------------------
    DEPRECATED
-------------------------------------------------------*/
jint DrawStringV2(JNIEnv *env, jclass caller, jint id, jstring string, jdouble x, jdouble y, jint color, jfloat f)
{
    return 0;
}

/*-------------------------------------------------------
    DEPRECATED
-------------------------------------------------------*/
jdouble GetStringWidthV2(JNIEnv *env, jclass caller, jint id, jstring string)
{
    return 0;
}

/*-------------------------------------------------------
    DEPRECATED
-------------------------------------------------------*/
jdouble GetStringHeightV2(JNIEnv *env, jclass caller, jint id, jstring string)
{
    return 0;
}

/*-------------------------------------------------------
    DEPRECATED
-------------------------------------------------------*/
jint MakeFontV2(JNIEnv *env, jclass caller, jint id, jint size, jstring string)
{
    return 0;
}

// v4.07 ADDITIONS

/*-------------------------------------------------------
    Maps the virtual keycode specified with the type.

    Since: v4.07
-------------------------------------------------------*/
jint n_MapVirtualKey(JNIEnv *env, jclass caller, jint code, jint mapType)
{
    return MapVirtualKeyA(code, mapType);
}

/*-------------------------------------------------------
    Gets the key name of the virtual keycode specified.

    Since: v4.07
-------------------------------------------------------*/
jstring GeyKeyName(JNIEnv *env, jclass caller, jlong virtualKey)
{
    char name[1024];
    GetKeyNameTextA(virtualKey, name, 1024);
    return env->NewStringUTF(name);
}

/*-------------------------------------------------------
    Sets up rendering stages.

    Note:
    This function was virtualized in the new DLL.
    Full functionality is unknown.

    Since: v4.07
-------------------------------------------------------*/

GLfloat projectionMatrix;

void RenderState(JNIEnv *env, jclass caller, jint state, jdouble displayWidth, jdouble displayHeight)
{
    if (state)
        glGetFloatv(GL_PROJECTION_MATRIX, &projectionMatrix);

    // list initializer in virtualized section
    // glCallList(list);

    // glGenLists()

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glLoadMatrixf(&projectionMatrix);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPopMatrix();
}

void RegisterMainNatives(JNIEnv *env, jclass cls)
{
    JNINativeMethod natives[] = {
        DEFINE_NATIVE_METHOD("exit", "(Z)V", Exit),
        DEFINE_NATIVE_METHOD("dc", "()V", DCallback),
        DEFINE_NATIVE_METHOD("fs", "()V", FinishSending),
        DEFINE_NATIVE_METHOD("gc", "(Ljava/lang/String;)Ljava/lang/Class;", GetClass),
        DEFINE_NATIVE_METHOD("gt", "(Ljava/lang/String;)[B", GetTexture),
        DEFINE_NATIVE_METHOD("gk", "()I", GetKey),
        DEFINE_NATIVE_METHOD("gs", "()Ljava/lang/String;", GetSettings),
        DEFINE_NATIVE_METHOD("ss", "(Ljava/lang/String;)V", SaveSettings),
        DEFINE_NATIVE_METHOD("mb", "(I)V", n_MessageBox),
        DEFINE_NATIVE_METHOD("gcj", "(Ljava/lang/String;)Ljava/lang/Class;", GetClassJava),
        DEFINE_NATIVE_METHOD("gcs", "(Ljava/lang/Class;)Ljava/lang/String;", GetClassSignature),
        DEFINE_NATIVE_METHOD("cs", "(I)Ljava/lang/String;", CopyString),
        DEFINE_NATIVE_METHOD("rl", "()V", Reload),
        DEFINE_NATIVE_METHOD("grh", "()Ljava/lang/Object;", GetRenderHandler),
        DEFINE_NATIVE_METHOD("cpy", "(Ljava/lang/String;)V", ClipboardCopy),
        DEFINE_NATIVE_METHOD("p", "(Ljava/lang/String;)V", Print),
        DEFINE_NATIVE_METHOD("gks", "(I)S", n_GetKeyState),
        DEFINE_NATIVE_METHOD("gco", "(Ljava/lang/Class;)[Ljava/lang/Object;", GetClassObjects),
        DEFINE_NATIVE_METHOD("trn", "(DDD)[D", Translate),
        DEFINE_NATIVE_METHOD("gcb", "(Ljava/lang/Class;)[B", GetClassBytes),
        DEFINE_NATIVE_METHOD("scb", "(Ljava/lang/Class;[B)I", SetClassBytes),
        DEFINE_NATIVE_METHOD("gcf", "(Ljava/lang/Class;)[Ljava/lang/String;", GetClassFields),
        DEFINE_NATIVE_METHOD("gcm", "(Ljava/lang/Class;)[Ljava/lang/String;", GetClassMethods),
        DEFINE_NATIVE_METHOD("ds", "(ILjava/lang/String;DDI)I", DrawString),
        DEFINE_NATIVE_METHOD("gsw", "(ILjava/lang/String;)D", GetStringWidth),
        DEFINE_NATIVE_METHOD("gsh", "(ILjava/lang/String;)D", GetStringHeight),
        DEFINE_NATIVE_METHOD("smd", "(II)V", SendMouseDown),
        DEFINE_NATIVE_METHOD("gp", "(Ljava/lang/String;)Ljava/lang/String;", GetProfile),
        DEFINE_NATIVE_METHOD("su", "(Ljava/lang/String;)V", SetUsername),
        DEFINE_NATIVE_METHOD("iv", "()Z", IsVanilla),
        DEFINE_NATIVE_METHOD("gvc", "(Ljava/lang/String;)Ljava/lang/Class;", GetVanillaClass),
        DEFINE_NATIVE_METHOD("gmv", "()I", GetMinorVersion),
        DEFINE_NATIVE_METHOD("rsc", "()V", RecieveSocketContents),
        DEFINE_NATIVE_METHOD("mf", "(IILjava/lang/String;)I", MakeFont),
        DEFINE_NATIVE_METHOD("updc", "(Ljava/lang/String;Ljava/lang/String;)V", UpdateDiscord),
        DEFINE_NATIVE_METHOD("dsv2", "(ILjava/lang/String;DDIF)I", DrawStringV2),
        DEFINE_NATIVE_METHOD("gswv2", "(ILjava/lang/String;)D", GetStringWidthV2),
        DEFINE_NATIVE_METHOD("gshv2", "(ILjava/lang/String;)D", GetStringHeightV2),
        DEFINE_NATIVE_METHOD("mfv2", "(IILjava/lang/String;)I", MakeFontV2),
        DEFINE_NATIVE_METHOD("mvk", "(II)I", n_MapVirtualKey),
        DEFINE_NATIVE_METHOD("gkn", "(J)Ljava/lang/String;", GeyKeyName),
        DEFINE_NATIVE_METHOD("rs", "(IDD)V", RenderState),
    };

    env->RegisterNatives(cls, natives, sizeof(natives) / sizeof(JNINativeMethod));
}