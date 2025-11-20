#include "AadilTypes.h"
#include <unordered_set>

using namespace std;

unordered_set<void *> playerslist;
bool EnableESP, ESPLine;

Vector2 convertToDeviceScreen(float posX, float posY, float devicewidth, float deviceheight, float gamewidth, float gameheight) {
	return Vector2(posX*devicewidth/gamewidth, deviceheight-(posY*deviceheight/gameheight));
}

void (*old_npcupdate)(void *instance);
void npcupdate(void *instance) {
	if (instance && EnableESP) {
		int health = *(int *)((uintptr_t) instance + 0xC);
		bool isAlive = *(bool *)((uintptr_t) instance + 0x24);
		if (health > 0 && isAlive) playerslist.insert(instance);
	}
	return old_npcupdate(instance);
}

void (*old_npcondisable)(void *instance);
void npcondisable(void *instance) {
	if (instance) {
		playerslist.clear();
	}
	return old_npcondisable(instance);
}

void (*old_npcondestroy)(void *instance);
void npcondestroy(void *instance) {
	if (instance) {
		playerslist.clear();
	}
	return old_npcondestroy(instance);
}

void *(*get_transform)(void *object);
Vector3 (*get_position)(void *transform);
void *(*get_maincamera)();
Vector3 (*get_worldtoscreenpoint)(void *camera, Vector3 worldpos);
int (*get_gamewidth)();
int (*get_gameheight)();
void *(*get_gameobjecttransform)(void *gameobject);
/*
void DrawESP(AadilDrawing esp, int width, int height) {
	int playerssize = playerslist.size();
	if (!EnableESP || playerssize < 1) return;
	if (playerssize > 30) {
		playerslist.clear();
		return;
	}
	void *camera = get_maincamera();
	if (!camera) return;
	int gamewidth = get_gamewidth();
	int gameheight = get_gameheight();
	Vector2 DrawFrom = Vector2(width/2, 0);
	for (void *player : playerslist) {
		if (player) {
			int health = *(int *)((uintptr_t) player + 0xC);
			bool isAlive = *(bool *)((uintptr_t) player + 0x24);
			if (health < 1 || !isAlive) continue;
			void *transform = get_transform(player);
			if (transform) {
				Vector3 worldpos = get_position(transform);
				Vector3 gamescreenpos = get_worldtoscreenpoint(camera, worldpos);
				if (gamescreenpos.z > 1) {
					Vector2 DrawTo = convertToDeviceScreen(gamescreenpos.x, gamescreenpos.y, width, height, gamewidth, gameheight);
					if (ESPLine) {
						esp.DrawLine(Color(255,0,0,255), 2.2f, DrawFrom, DrawTo);
					}
				}
			}
		}
	}
}*/

void DrawESP(AadilDrawing esp, int width, int height) {
	int playerssize = playerslist.size();
	if (!EnableESP || playerssize < 1) return;
	if (playerssize > 30) {
		playerslist.clear();
		return;
	}
	void *camera = get_maincamera();
	if (!camera) return;
	int gamewidth = get_gamewidth();
	int gameheight = get_gameheight();
	Vector2 DrawFrom = Vector2(width/2, 0);
	for (void *player : playerslist) {
		if (player) {
			int health = *(int *)((uintptr_t) player + 0xC);
			bool isAlive = *(bool *)((uintptr_t) player + 0x24);
			if (health < 1 || !isAlive) continue;
			void *gameobject = *(void **)((uintptr_t) player + 0x20);
			if (gameobject) {
				void *transform = get_gameobjecttransform(gameobject);
				if (transform) {
					Vector3 worldpos = get_position(transform);
					Vector3 gamescreenpos = get_worldtoscreenpoint(camera, worldpos);
					if (gamescreenpos.z > 1) {
						Vector2 DrawTo = convertToDeviceScreen(gamescreenpos.x, gamescreenpos.y, width, height, gamewidth, gameheight);
						if (ESPLine) {
							esp.DrawLine(Color(255,0,0,255), 2.2f, DrawFrom, DrawTo);
						}
					}
				}
			}
		}
	}
}
