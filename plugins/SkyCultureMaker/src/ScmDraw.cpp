#include "ScmDraw.hpp"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QApplication>
#include <QDebug>
#include "StelModule.hpp"
#include "StelProjector.hpp"

scm::ScmDraw::ScmDraw()
	: drawState(Drawing::None)
	, snapToStar(true)
{
	currentLine.start.position.set(0, 0, 0);
	currentLine.end.position.set(0, 0, 0);

	StelCore *core = StelApp::getInstance().getCore();
	maxSnapRadiusInPixels *= core->getCurrentStelProjectorParams().devicePixelsPerPixel;
}

scm::ScmDraw::~ScmDraw()
{
}

void scm::ScmDraw::drawLine(StelCore *core)
{
	StelPainter painter(core->getProjection(drawFrame));
	painter.setBlending(true);
	painter.setLineSmooth(true);
	Vec3f color = {1.f, 0.5f, 0.5f};
	bool alpha = 1.0f;
	painter.setColor(color, alpha);

	for (StarLine p : drawnLines)
	{
		painter.drawGreatCircleArc(p.start.position, p.end.position);
	}

	if (hasFlag(drawState, Drawing::hasFloatingEnd))
	{
		color = {1.f, 0.7f, 0.7f};
		painter.setColor(color, 0.5f);
		painter.drawGreatCircleArc(currentLine.start.position, currentLine.end.position);
	}
}

void scm::ScmDraw::handleMouseClicks(class QMouseEvent *event)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	qreal x = event->position().x(), y = event->position().y();
#else
	qreal x = event->x(), y = event->y();
#endif

	// Draw line
	if (event->button() == Qt::RightButton && event->type() == QEvent::MouseButtonPress)
	{
		StelApp &app = StelApp::getInstance();
		StelCore *core = app.getCore();
		StelProjectorP prj = core->getProjection(drawFrame);
		Vec3d point;
		std::optional<QString> starID;
		prj->unProject(x, y, point);

		// We want to combine any near start point to an existing point so that we don't create
		// duplicates.
		std::optional<StarPoint> nearest = findNearestPoint(x, y, prj);
		if (nearest.has_value())
		{
			point = nearest.value().position;
			starID = nearest.value().starID;
		}
		else if (snapToStar)
		{
			if (hasFlag(drawState, Drawing::hasEndExistingPoint))
			{
				point = currentLine.end.position;
				starID = currentLine.end.starID;
			}
			else
			{
				StelObjectMgr &objectMgr = app.getStelObjectMgr();

				objectMgr.findAndSelect(core, x, y);
				if (objectMgr.getWasSelected())
				{
					StelObjectP stelObj = objectMgr.getLastSelectedObject();
					Vec3d stelPos = stelObj->getJ2000EquatorialPos(core);
					point = stelPos;
					starID = stelObj->getID();
				}
			}
		}

		if (hasFlag(drawState, (Drawing::hasStart | Drawing::hasFloatingEnd)))
		{
			currentLine.end.position = point;
			currentLine.end.starID = starID;
			drawState = Drawing::hasEnd;

			drawnLines.push_back(currentLine);
			currentLine.start.position = currentLine.end.position;
			currentLine.start.starID = currentLine.end.starID;
			drawState = Drawing::hasStart;
		}
		else
		{
			currentLine.start.position = point;
			currentLine.start.starID = starID;
			drawState = Drawing::hasStart;
		}

		event->accept();
		return;
	}

	// Reset line drawing
	// Also works as a Undo feature.
	if (event->button() == Qt::RightButton && event->type() == QEvent::MouseButtonDblClick)
	{
		if (!drawnLines.empty())
		{
			drawnLines.pop_back();
		}
		drawState = Drawing::None;
		event->accept();
		return;
	}
}

bool scm::ScmDraw::handleMouseMoves(int x, int y, Qt::MouseButtons b)
{
	StelApp &app = StelApp::getInstance();
	StelCore *core = app.getCore();

	if (snapToStar)
	{
		StelObjectMgr &objectMgr = app.getStelObjectMgr();
		objectMgr.findAndSelect(core, x, y);
	}

	if (hasFlag(drawState, (Drawing::hasStart | Drawing::hasFloatingEnd)))
	{
		StelProjectorP prj = core->getProjection(drawFrame);
		Vec3d position;
		prj->unProject(x, y, position);
		if (snapToStar)
		{
			StelObjectMgr &objectMgr = app.getStelObjectMgr();
			if (objectMgr.getWasSelected())
			{
				StelObjectP stelObj = objectMgr.getLastSelectedObject();
				Vec3d stelPos = stelObj->getJ2000EquatorialPos(core);
				currentLine.end.position = stelPos;
			}
			else
			{
				currentLine.end.position = position;
			}
		}
		else
		{
			currentLine.end.position = position;
		}

		drawState = Drawing::hasFloatingEnd;
	}

	// We always return false as we still want to navigate in Stellarium with left mouse button
	return false;
}

void scm::ScmDraw::handleKeys(QKeyEvent *e)
{
	if (e->key() == Qt::Key::Key_Control)
	{
		snapToStar = e->type() != QEvent::KeyPress;

		e->accept();
	}

	if (e->key() == Qt::Key::Key_Z && e->modifiers() == Qt::Modifier::CTRL)
	{
		undoLastLine();
		e->accept();
	}
}

void scm::ScmDraw::undoLastLine()
{
	if (!drawnLines.empty())
	{
		scm::ScmDraw::StarLine last = drawnLines.back();
		drawnLines.pop_back();
		currentLine.start = last.start;
		drawState = Drawing::hasFloatingEnd;
	}
	else
	{
		drawState = Drawing::None;
	}
}

std::optional<scm::ScmDraw::StarPoint> scm::ScmDraw::findNearestPoint(int x, int y, StelProjectorP prj)
{
	if (drawnLines.empty())
	{
		return {};
	}

	StarPoint min = drawnLines[0].start;
	Vec3d position(x, y, 0);
	Vec3d minPosition;
	prj->project(min.position, minPosition);
	double minDistance = (minPosition - position).dot(minPosition - position);

	for (auto line : drawnLines)
	{
		Vec3d iPosition;
		if (!prj->project(line.start.position, iPosition))
		{
			continue;
		}

		double distance = (iPosition - position).dot(iPosition - position);
		if (distance < minDistance)
		{
			min = line.start;
			minPosition = iPosition;
			minDistance = distance;
		}
	}

	if (minDistance < maxSnapRadiusInPixels * maxSnapRadiusInPixels)
	{
		return min;
	}

	return {};
}
