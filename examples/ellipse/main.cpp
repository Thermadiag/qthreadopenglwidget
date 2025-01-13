#include <QThreadOpenGLWidget.h>
#include <QTimer>
#include <QApplication>
#include <QSurfaceFormat>
#include <QDateTime>
 
#include <cmath>
 
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
 		float c = std::cos(theta);//precalculate the sine and cosine
 		float s = std::sin(theta);
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
 
 
 
 
int main(int argc, char *argv[])
{
    QSurfaceFormat format;
    format.setSamples(4);
    format.setSwapInterval(0);
    QSurfaceFormat::setDefaultFormat(format);

    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    MyOpenGLWidget window;
    window.show();

    return app.exec();
}
