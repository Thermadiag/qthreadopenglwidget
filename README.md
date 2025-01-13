
Purpose
-------

*qthreadopenglwidget* is a small library providing opengl related features for Qt Widgets.

The main class `QThreadOpenGLWidget` is a `QWidget` providing opengl rendering much like `QOpenGLWidget`, but using a dedicated rendering thread to offload the GUI thread.

Indeed, using a dedicated rendering thread with `QOpenGLWidget`, while probably possible (according to the documentation), is a real pain. I wasn't able to produce a reliable working class with it, which is why I rolled my own class.
`QThreadOpenGLWidget` provides a similar API to `QOpenGLWidget` with the virtual members `initializeGL()`, `resizeGL()` and `paintGL()`. It is possible to override its `paintEvent()` to perform QPainter based drawing.
`QThreadOpenGLWidget` uses a the following approach:
-	Using a QPainter on a QThreadOpenGLWidget will use its own custom <LINK>QPaintEngine.
-	The paint engine serializes drawing commands in a structure similar to <LINK>QPicture (see `QPaintRecord` class).
-	Drawing commands are periodically sent to a rendering thread.
-	The rendering thread apply the drawing commands in an internal QWindow using <LINK>QOpenGLPaintDevice.
	If the opengl thread is slower than the time spent in recording drawing commands, it will
	discard older commands and only paint the last ones.
	
This greatly reduces the time spent in QWidget::paintEvent(), allow higher frame rates and increase the GUI responsiveness.

`QThreadOpenGLWidget` can be used as the viewport of a <LINK>QGraphicsView, as the class was primary made for this. Indeed, from my experience, using a `QOpenGLWidget` as viewport does not provide much benefits: the CPU load is similar and 
the painting takes the same amount of time as with the raster paint engine (at least most of the times).
The library provides additional classes to help working with large scenes:
-	`QOpenGLItem`: by inheriting this class, a <LINK>QGraphicsItem can use a caching mechanism based on `QPaintRecord`. This caching mechanism is only enabled when using a `QThreadOpenGLWidget` viewport,
	and reduces the time spent in QGraphicsItem::paint() method. See the class documentation for more details.
-	`QOpenGLGraphicsItem`: convenient QGraphicsItem inheriting `QOpenGLItem`.
-	`QOpenGLGraphicsObject`: convenient QGraphicsObject inheriting `QOpenGLItem`.
-	`QOpenGLGraphicsWidget`: convenient QGraphicsWidget inheriting `QOpenGLItem`.
	

Usage
-----

The following example shows how to use `QThreadOpenGLWidget` to display several static or dynamic ellipses using all possible ways:

```cpp
 // Example of QThreadOpenGLWidget that displays static and dynamic ellipses
 // by combining all possible types of drawing mechanisms:
 //  - Overload of QThreadOpenGLWidget::paintGL()
 //  - Calling drawFunction() from within paintEvent()
 //  - Using a regular QPainter from within paintEvent()
 //
 class MyOpenGLWidget : public QThreadOpenGLWidget
 {
 	static void DrawOpenGLEllipse(const QRectF& r, int num_segments)
 	{
 		// Draw an ellipse using old school opengl, just for the use case

 		float cx = r.center().x();
 		float cy = r.center().y();
 		float rx = r.width() / 2;
 		float ry = r.height() / 2;

 		float theta = 2 * 3.1415926 / float(num_segments);
 		float c = std::cosf(theta);//precalculate the sine and cosine
 		float s = std::sinf(theta);
 		float t;
 		float x = 1;//we start at angle = 0
 		float y = 0;
 		glEnable(GL_MULTISAMPLE);
 		glEnable(GL_COLOR_MATERIAL);
 		glColor3f(0, 1, 0);
 		glBegin(GL_LINE_LOOP);
 		for (int ii = 0; ii < num_segments; ii++)
 		{
 			//apply radius and offset
 			glVertex2f(x * rx + cx, y * ry + cy);//output vertex

 			//apply the rotation matrix
 			t = x;
 			x = c * x - s * y;
 			y = s * t + c * y;
 		}
 		glEnd();
 	}

 public:
 	QTimer timer;

 	MyOpenGLWidget()
 		:QThreadOpenGLWidget()
 	{
 		timer.setSingleShot(false);
 		timer.setInterval(10);
 		connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
 		timer.start();
 	}

 	virtual void paintGL()
 	{
 		// One way to display an ellipse: override paintGL()
 		DrawOpenGLEllipse(QRectF(0, 0, 1, 1), 100);
 	}

 	virtual void paintEvent(QPaintEvent* evt)
 	{
 		qint64 time = QDateTime::currentMSecsSinceEpoch();
 		int size = time % 500;

 		// Another (faster) way to display an ellipse:
 		// send a drawing function to the rendering thread
 		// that uses QPainter calls
 		drawFunction([size](QPainter* p)
 			{
 				p->setPen(Qt::red);
 				p->setRenderHint(QPainter::Antialiasing);
 				p->drawEllipse(QRectF(10, 10, size, size));
 			});

 		// Another (faster) way to display an ellipse:
 		// send a drawing function using raw opengl calls
 		drawFunction([size](QPainter* p)
 			{
 				p->beginNativePainting();
 				MyOpenGLWidget::DrawOpenGLEllipse(QRectF(10 / 500., 10 / 500., size / 500., size / 500.), 500);
 				p->endNativePainting();

 			});

 		// Yet another way to draw an ellipse:
 		// the regular QWidget way
 		QPainter p(this);
 		p.setPen(Qt::blue);
 		p.setRenderHint(QPainter::Antialiasing);
 		p.drawEllipse(QRectF(10, 10, 50, 50));
 	}

 };
```


Build
-----

The *qthreadopenglwidget* library requires compilation using cmake, but you can just copy/paste the source files in your project.


Using Seq library with CMake
----------------------------

This [cmake example](tests/test_cmake/CMakeLists.txt) shows how to use the *seq* library within a cmake project. It creates 3 targets using the shared seq library, the static one, and the header only mode.



qthreadopenglwidget library and this page Copyright (c) 2025, Victor Moncada
