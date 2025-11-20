#include <list>
#include <vector>
#include <cstring>
#include <pthread.h>
#include <thread>
#include <cstring>
#include <string>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <dlfcn.h>
#include "Includes/Logger.h"
#include "Includes/obfuscate.h"
#include "Includes/Utils.hpp"
#include "Menu/Menu.hpp"
#include "Menu/Jni.hpp"
#include "Includes/Macros.h"
#include "Menu/Setup.h"

struct MemPatches {
    // let's assume we have patches for these functions for whatever game
    // boolean get_canShoot() function
    MemoryPatch noDeath;
    // etc...
} gPatches;

// Do not change or translate the first text unless you know what you are doing
// Assigning feature numbers is optional. Without it, it will automatically count for you, starting from 0
// Assigned feature numbers can be like any numbers 1,3,200,10... instead in order 0,1,2,3,4,5...
// ButtonLink, Category, RichTextView and RichWebView is not counted. They can't have feature number assigned
// Toggle, ButtonOnOff and Checkbox can be switched on by default, if you add True_. Example: CheckBox_True_The Check Box
// To learn HTML, go to this page: https://www.w3schools.com/

jobjectArray GetFeatureList(JNIEnv *env, jobject context) {
    jobjectArray ret;

    const char *features[] = {
            OBFUSCATE("1_Toggle_Enable ESP"),
            OBFUSCATE("2_Toggle_ESP Line"),
    };

    int Total_Feature = (sizeof features / sizeof features[0]);
    ret = (jobjectArray)
            env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")),
                                env->NewStringUTF(""));

    for (int i = 0; i < Total_Feature; i++)
        env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));

    return (ret);
}

bool btnPressed = false;

void Changes(JNIEnv *env, jclass clazz, jobject obj, jint featNum, jstring featName, jint value, jlong Lvalue, jboolean boolean, jstring text) {

    switch (featNum) {
        case 1:
            EnableESP = boolean;
            break;
        case 2:
            ESPLine = boolean;
            break;
    }
}

//CharacterPlayer
/*
void (*StartInvcibility)(void *instance, float duration);

void (*old_Update)(void *instance);

void Update(void *instance) {
    if (instance != nullptr) {
        if (btnPressed) {
            StartInvcibility(instance, 30);
            btnPressed = false;
        }
    }
    return old_Update(instance);
}

void (*old_AddScore)(void *instance, int score);
void AddScore(void *instance, int score) {
    return old_AddScore(instance, score * scoreMul);
}

void (*old_AddCoins)(void *instance, int count);
void AddCoins(void *instance, int count) {
    return old_AddCoins(instance, count * coinsMul);
}*/

//Target lib here
#define targetLibName OBFUSCATE("libil2cpp.so")

ElfScanner g_il2cppELF;

// we will run our hacks in a new thread so our while loop doesn't block process main thread
void *hack_thread(void *) {
    LOGI(OBFUSCATE("pthread created"));

    //Check if target lib is loaded
    /*do {
        sleep(1);
    } while (!isLibraryLoaded(targetLibName));*/

    do {
        sleep(1);
        // getElfBaseMap can also find lib base even if it was loaded from zipped base.apk
        g_il2cppELF = ElfScanner::createWithPath(targetLibName);
    } while (!g_il2cppELF.isValid());

    LOGI(OBFUSCATE("%s has been loaded"), (const char *) targetLibName);

#if defined(__aarch64__)
    uintptr_t il2cppBase = g_il2cppELF.base();
/*
    //Il2Cpp: Use RVA offset
    StartInvcibility = (void (*)(void *, float)) getAbsoluteAddress(targetLibName, str2Offset(
            OBFUSCATE("0x107A3BC")));

    HOOK(targetLibName, str2Offset(OBFUSCATE("0x107A2E0")), AddScore, old_AddScore);
    HOOK(targetLibName, str2Offset(OBFUSCATE("0x107A2FC")), AddCoins, old_AddCoins);
    HOOK(targetLibName, str2Offset(OBFUSCATE("0x1078C44")), Update, old_Update);

    gPatches.noDeath = MemoryPatch::createWithHex(il2cppBase + str2Offset(OBFUSCATE("0x1079728")), "C0 03 5F D6");
*/
    //HOOK(targetLibName, str2Offset(OBFUSCATE("0x1079728")), Kill, old_Kill);

    //PATCH(targetLibName, str2Offset("0x10709AC"), "E05F40B2 C0035FD6");
    //HOOK(OBFUSCATE("libFileB.so"), str2Offset(OBFUSCATE("0x123456")), FunctionExample, old_FunctionExample);
    //HOOK("libFileB.so", 4646464, FunctionExample, old_FunctionExample);
    //HOOK_NO_ORIG("libFileC.so", str2Offset("0x123456"), FunctionExample);
    //HOOKSYM("libFileB.so", "__SymbolNameExample", FunctionExample, old_FunctionExample);
    //HOOKSYM_NO_ORIG("libFileB.so", "__SymbolNameExample", FunctionExample);

#elif defined(__arm__)
    //Put your code here if you want the code to be compiled for armv7 only
	 // public class Component : Object public Transform get_transform() { }
	get_transform = (void *(*)(void *)) getAbsoluteAddress(targetLibName, str2Offset(OBFUSCATE("0x1C63060")));
	// public class Transform : Component, IEnumerable public Vector3 get_position() { }
	get_position = (Vector3 (*)(void *)) getAbsoluteAddress(targetLibName, str2Offset(OBFUSCATE("0x1C6BDBC")));
	// public sealed class Camera : Behaviour public static Camera get_main() { }
	get_maincamera = (void *(*)()) getAbsoluteAddress(targetLibName, str2Offset(OBFUSCATE("0x1C5DFDC")));
	// public sealed class Camera : Behaviour public Vector3 WorldToScreenPoint(Vector3 position) { }
	get_worldtoscreenpoint = (Vector3 (*)(void *, Vector3)) getAbsoluteAddress(targetLibName, str2Offset(OBFUSCATE("0x1C5D694")));
	// public sealed class Screen public static int get_width() { }
	get_gamewidth = (int (*)()) getAbsoluteAddress(targetLibName, str2Offset(OBFUSCATE("0x1C5833C")));
	// public sealed class Screen public static int get_height() { }
	get_gameheight = (int (*)()) getAbsoluteAddress(targetLibName, str2Offset(OBFUSCATE("0x1C58384")));
	// public sealed class GameObject : Object public Transform get_transform() { }
	get_gameobjecttransform = (void *(*)(void *)) getAbsoluteAddress(targetLibName, str2Offset(OBFUSCATE("0x1C66324")));
	// public class ManAI : MonoBehaviour private void Update() { }
	HOOK(targetLibName, str2Offset(OBFUSCATE("0x72CD98")), npcupdate, old_npcupdate);
	// public class ManAI : MonoBehaviour private void OnDisable() { }
	HOOK(targetLibName, str2Offset(OBFUSCATE("0x72B820")), npcondisable, old_npcondisable);
	// public class ManAI : MonoBehaviour private void OnDestroy() { }
	HOOK(targetLibName, str2Offset(OBFUSCATE("0x72B908")), npcondestroy, old_npcondestroy);
	
#endif

    LOGI(OBFUSCATE("Done"));
    return nullptr;
}

__attribute__((constructor))
void lib_main() {
    // Create a new thread so it does not block the main thread, means the game would not freeze
    pthread_t ptid;
    pthread_create(&ptid, NULL, hack_thread, NULL);
}
