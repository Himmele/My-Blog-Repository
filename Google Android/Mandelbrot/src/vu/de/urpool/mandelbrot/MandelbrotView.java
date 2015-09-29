package vu.de.urpool.mandelbrot;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.os.Handler;
import android.os.Message;
import android.util.AttributeSet;
import android.view.View;

public class MandelbrotView extends View {
	private static final int DRAW_MANDELBROT_IMAGE = 1;
	
	private Thread mThread;
	private Bitmap mBitmap;
	private int mMaxIterations;
	
	public MandelbrotView(Context context) {
		this(context, null);
	}
	
	public MandelbrotView(Context context, AttributeSet attrs) {
		this(context, attrs, 0);
	}
	
	public MandelbrotView(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		mMaxIterations = 20;
	}

	@Override
	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
		super.onMeasure(widthMeasureSpec, heightMeasureSpec);
	}
	
	@Override
	protected void onLayout(boolean changed, int left, int top, int right, int bottom) {
		super.onLayout(changed, left, top, right, bottom);		
	}
	
	@Override
	protected void onDraw(Canvas canvas) {
		if (mBitmap != null) {
			canvas.drawBitmap(mBitmap, 0, 0, null);
		}
	}
	
	@Override
	protected void onSizeChanged(int w, int h, int oldw, int oldh) {
		if (w != 0 && h != 0) {
			mThread = new Thread(new Runnable() {
				@Override
				public void run() {
					Bitmap bitmap = Bitmap.createBitmap(getWidth(), getHeight(), Bitmap.Config.ARGB_8888);				
					drawMandelbrot(bitmap, getWidth(), getHeight());
				}
			});
			mThread.start();
		}
	}
	
	@Override
	protected void onDetachedFromWindow() {
		if (mThread != null) {
			mThread.interrupt();
			try {
				mThread.join();
			} catch (InterruptedException e) {
			}
		}
	}
	
	private void drawMandelbrot(Bitmap bitmap, int width, int height) {
		Canvas canvas = new Canvas(bitmap);
		Paint paint = new Paint();
		
		int x;
		int y;
		double cx = -2;
		double cy = -2;
		double dx = ((double)(-cx - cx)) / height;
		double dy = ((double)(-cy - cy)) / height;
				
		y = 0;
		while (y < height) {
			x = 0;
			cx = -2;
			while (x < width) {
				if (mThread.isInterrupted()) {
					return;
				}
				int iterations = calcMandelbrotIterations(cx, cy);
				if (iterations >= mMaxIterations) {
					paint.setColor(Color.BLACK);
				} else {
					int color = Color.argb(255,
							255 - (255 / mMaxIterations) * iterations,
							255 - (255 / mMaxIterations) * iterations,
							255);
					paint.setColor(color);
				}
				canvas.drawPoint(x, y, paint);
				x++;
				cx += dx;
				
				Message msg = mHandler.obtainMessage();
		        msg.arg1 = DRAW_MANDELBROT_IMAGE;
		        msg.obj = bitmap;
		        mHandler.sendMessage(msg);
			}
			y++;
			cy += dy;
		}
	}
	
	// http://de.wikipedia.org/wiki/Mandelbrot-Menge
	private int calcMandelbrotIterations(double cx, double cy) {
		int i = 0;
		
		double x = 0;
		double y = 0;
		double squaredSum = 0;
		
		while ((squaredSum <= 4.0) && (i < mMaxIterations)) {
			double xt = x * x - y * y + cx;
			double yt = 2 * x * y + cy;
			x = xt;
			y = yt;
			squaredSum = x * x + y * y;
			i++;
	    }
		
		return i;
	}
	
	private Handler mHandler = new Handler() {
		@Override
	    public void handleMessage(Message msg) {
			int event = msg.arg1;
			switch(event) {
			case DRAW_MANDELBROT_IMAGE:
				 mBitmap = (Bitmap) msg.obj;
				 invalidate();
				 break;
			default:
				super.handleMessage(msg);
			}
		}
	};
}