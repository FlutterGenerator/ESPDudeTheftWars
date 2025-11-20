package com.android.support;

import android.view.View;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.os.Handler;
import android.graphics.Color;
import android.graphics.PorterDuff;

public class ESPView extends View {

	private Paint strokePaint;
	private int FrameDelay = 33;

	public ESPView(Context context) {
		super(context);
		strokePaint = new Paint();
		strokePaint.setStyle(Paint.Style.STROKE);
		strokePaint.setAntiAlias(true);
		setFocusableInTouchMode(false);
		final Handler framehandler = new Handler();
		framehandler.postDelayed(
			new Runnable() {
				@Override
				public void run() {
					invalidate();
					framehandler.postDelayed(this, FrameDelay);
				}
			},
			FrameDelay);
	}

	@Override
	protected void onDraw(Canvas canvas) {
		super.onDraw(canvas);
		canvas.drawColor(Color.TRANSPARENT, PorterDuff.Mode.CLEAR);
		Menu.Draw(this, canvas);
	}

	public void DrawLine(
		Canvas cvs,
		int a,
		int r,
		int g,
		int b,
		float strokewidth,
		float fromX,
		float fromY,
		float toX,
		float toY) {
		strokePaint.setColor(Color.argb(a, r, g, b));
		strokePaint.setStrokeWidth(strokewidth);
		cvs.drawLine(fromX, fromY, toX, toY, strokePaint);
	}
}
