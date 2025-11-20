#include <jni.h>

struct Vector2 {
	float x,y;
	Vector2(float X, float Y) : x(X), y(Y) {}
};

struct Vector3 {
	float x,y,z;
	Vector3(float X, float Y, float Z) : x(X), y(Y), z(Z) {}
};

struct Color {
	float r,g,b,a;
	Color(float R, float G, float B, float A) : r(R), g(G), b(B), a(A) {}
};

class AadilDrawing {
	private:
		JNIEnv *jnienv;
		jobject espview;
		jobject canvas;
		
	public:
		AadilDrawing() {
			jnienv = nullptr;
			espview = nullptr;
			canvas = nullptr;
		}
		
		AadilDrawing(JNIEnv *env, jobject esp, jobject cvs) {
			this->jnienv = env;
			this->espview = esp;
			this->canvas = cvs;
		}
		
		bool isValid() const {
			return (jnienv && espview && canvas);
		}
		
		int getWidth() const {
			if (isValid()) {
				jclass cvs = jnienv->GetObjectClass(canvas);
				jmethodID width = jnienv->GetMethodID(cvs, "getWidth", "()I");
				return jnienv->CallIntMethod(canvas, width);
			}
		}
		
		int getHeight() const {
			if (isValid()) {
				jclass cvs = jnienv->GetObjectClass(canvas);
				jmethodID height = jnienv->GetMethodID(cvs, "getHeight", "()I");
				return jnienv->CallIntMethod(canvas, height);
			}
		}
		
		void DrawLine(Color color, float thickness, Vector2 from, Vector2 to) {
			if (isValid()) {
				jclass cvs = jnienv->GetObjectClass(espview);
				jmethodID drawline = jnienv->GetMethodID(cvs, "DrawLine", "(Landroid/graphics/Canvas;IIIIFFFFF)V");
				jnienv->CallVoidMethod(espview, drawline, canvas, (int)color.a, (int)color.r, (int)color.g, (int)color.b,
				thickness, from.x, from.y, to.x, to.y);
			}
		}
		
};
