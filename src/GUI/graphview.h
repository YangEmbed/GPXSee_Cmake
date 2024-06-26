#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include <QGraphicsView>
#include <QList>
#include "data/graph.h"
#include "palette.h"
#include "units.h"
#include "infoitem.h"


class AxisItem;
class AxisLabelItem;
class SliderItem;
class SliderInfoItem;
class GraphItem;
class PathItem;
class GridItem;
class QGraphicsSimpleTextItem;
class GraphicsScene;
class QGestureEvent;
class QPinchGesture;

class GraphView : public QGraphicsView
{
	Q_OBJECT

public:
	GraphView(QWidget *parent = 0);
	~GraphView();

	bool isEmpty() const {return _graphs.isEmpty();}
	const QList<KV<QString, QString> > &info() const {return _info->info();}
	void clear();

	void plot(QPainter *painter, const QRectF &target, qreal scale);

	void setPalette(const Palette &palette);
	void setGraphWidth(int width);
	void showGrid(bool show);
	void showSliderInfo(bool show);
	void useOpenGL(bool use);
	void useAntiAliasing(bool use);

	void setSliderPosition(qreal pos);
	void setSliderColor(const QColor &color);

	qreal sliderPosition() const {return _sliderPos;}

signals:
	void sliderPositionChanged(qreal);

protected:
	void addGraph(GraphItem *graph);
	void removeGraph(GraphItem *graph);
	void setGraphType(GraphType type);
	void setUnits(Units units);

	void resizeEvent(QResizeEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void wheelEvent(QWheelEvent *e);
	void changeEvent(QEvent *e);
	void paintEvent(QPaintEvent *e);
	bool event(QEvent *event);

	const QString &yLabel() const {return _yLabel;}
	const QString &yUnits() const {return _yUnits;}
	qreal yScale() const {return _yScale;}
	qreal yOffset() const {return _yOffset;}
	void setYLabel(const QString &label);
	void setYUnits(const QString &units);
	void setYScale(qreal scale) {_yScale = scale;}
	void setYOffset(qreal offset) {_yOffset = offset;}

	void setSliderPrecision(int precision) {_precision = precision;}
	void setMinYRange(qreal range) {_minYRange = range;}

	QRectF bounds() const;
	void redraw();
	void addInfo(const QString &key, const QString &value);
	void clearInfo();

	GraphType _graphType;
	Units _units;
	Palette _palette;
	int _width;

private slots:
	void emitSliderPositionChanged(const QPointF &pos);
	void newSliderPosition(const QPointF &pos);

private:
	void redraw(const QSizeF &size);
	void setXUnits();
	void updateSliderPosition();
	void updateSliderInfo();
	void removeItem(QGraphicsItem *item);
	void addItem(QGraphicsItem *item);
	bool singleGraph() const;
	bool gestureEvent(QGestureEvent *event);
	void pinchGesture(QPinchGesture *gesture);

	GraphicsScene *_scene;

	AxisItem *_xAxis, *_yAxis;
	AxisLabelItem *_xAxisLabel, *_yAxisLabel;
	SliderItem *_slider;
	SliderInfoItem *_sliderInfo;
	InfoItem *_info;
	GridItem *_grid;
	QGraphicsSimpleTextItem *_message;
	QList<GraphItem*> _graphs;

	QRectF _bounds;
	qreal _sliderPos;

	qreal _xScale, _yScale;
	qreal _yOffset;
	QString _xUnits, _yUnits;
	QString _xLabel, _yLabel;
	int _precision;
	qreal _minYRange;

	qreal _zoom;

	int _angleDelta;
	int _dragStart;
};

#endif // GRAPHVIEW_H
