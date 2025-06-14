#include "ScmDraw.hpp"
#include "StelActionMgr.hpp"
#include "StelModule.hpp"
#include "StelMovementMgr.hpp"
#include "StelProjector.hpp"
#include <QApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWidget>

void scm::ScmDraw::setSearchMode(bool active)
{
	// search mode deactivates before the star is set by the search
	if (inSearchMode == true && active == false)
	{
		selectedStarIsSearched = true;

		// HACK an Ctrl + Release is not triggered if Ctrl + F is trigger it manually
		QKeyEvent release = QKeyEvent(QEvent::KeyRelease, Qt::Key_Control, Qt::NoModifier);
		QWidget *mainView = qApp->activeWindow();

		if (mainView)
		{
			QApplication::sendEvent(mainView, &release);
		}
		else
		{
			qDebug() << "Failed to release ctrl key";
		}
	}

	inSearchMode = active;
}

void scm::ScmDraw::appendDrawPoint(Vec3d point, QString starId, double raJ2000, double decJ2000)
{
	if (hasFlag(drawState, (Drawing::hasStart | Drawing::hasFloatingEnd)))
	{
		currentLine.endCoordinate = point;
		currentLine.endId         = starId;
		currentLine.endRAJ2000    = raJ2000;
		currentLine.endDecJ2000   = decJ2000;
		currentLine.endIdNumber	  = ConstellationLine::getStarIdNumber(starId);	
		drawState          		  = Drawing::hasEnd;

		drawnLines.push_back(currentLine);
		currentLine.startCoordinate = point;
		currentLine.startId     	= starId;
		currentLine.startRAJ2000  	= raJ2000;
		currentLine.startDecJ2000 	= decJ2000;
		currentLine.startIdNumber = ConstellationLine::getStarIdNumber(starId);
		drawState             		= drawState | Drawing::hasStart;
	}
	else
	{
		currentLine.startCoordinate = point;
		currentLine.startId     	= starId;
		currentLine.startRAJ2000  	= raJ2000;
		currentLine.startDecJ2000 	= decJ2000;
		currentLine.startIdNumber	= ConstellationLine::getStarIdNumber(starId);
		drawState             		= Drawing::hasStart;
	}
}

void scm::ScmDraw::setMoveToAnotherStart()
{
	if (selectedStarIsSearched == true)
	{
		if (activeTool == DrawTools::Pen)
		{
			StelApp &app   = StelApp::getInstance();
			StelCore *core = app.getCore();

			StelObjectMgr &objectMgr = app.getStelObjectMgr();

			if (objectMgr.getWasSelected())
			{
				StelObjectP stelObj = objectMgr.getLastSelectedObject();
				Vec3d stelPos       = stelObj->getJ2000EquatorialPos(core);
				double raJ2000      = stelObj->getInfoMap(core).value("raJ2000").toDouble();
				double decJ2000     = stelObj->getInfoMap(core).value("decJ2000").toDouble();
				appendDrawPoint(stelPos, stelObj->getID(), raJ2000, decJ2000);
			}
		}

		selectedStarIsSearched = false;
	}
}

const Vec2d scm::ScmDraw::defaultLastEraserPos(std::nan("1"), std::nan("1"));

bool scm::ScmDraw::segmentIntersect(Vec2d startA, Vec2d directionA, Vec2d startB, Vec2d directionB)
{
	if (std::abs(directionA.dot(directionB)) < std::numeric_limits<double>::epsilon()) // check with near zero value
	{
		// No intersection if lines are parallel
		return false;
	}

	// Also see: https://www.sunshine2k.de/coding/javascript/lineintersection2d/LineIntersect2D.html
	// endA = startA + s * directionA with s=1
	double s = perpDot(directionB, startB - startA) / perpDot(directionB, directionA);
	// endB = startB + t * directionB with t=1
	double t = perpDot(directionA, startA - startB) / perpDot(directionA, directionB);

	return 0 <= s && s <= 1 && 0 <= t && t <= 1;
}

scm::ScmDraw::ScmDraw()
	: drawState(Drawing::None)
	, snapToStar(true)
{
	currentLine.startCoordinate.set(0, 0, 0);
	currentLine.endCoordinate.set(0, 0, 0);
	lastEraserPos.set(std::nan("1"), std::nan("1"));

	StelApp &app   = StelApp::getInstance();
	StelCore *core = app.getCore();
	maxSnapRadiusInPixels *= core->getCurrentStelProjectorParams().devicePixelsPerPixel;

	StelActionMgr *actionMgr = app.getStelActionManager();
	auto action              = actionMgr->findAction(id_search_window);
	connect(action, &StelAction::toggled, this, &ScmDraw::setSearchMode);

	StelMovementMgr *mvmMgr = core->getMovementMgr();
	connect(mvmMgr, &StelMovementMgr::flagTrackingChanged, this, &ScmDraw::setMoveToAnotherStart);
}

void scm::ScmDraw::drawLine(StelCore *core)
{
	StelPainter painter(core->getProjection(drawFrame));
	painter.setBlending(true);
	painter.setLineSmooth(true);
	Vec3f color = {1.f, 0.5f, 0.5f};
	bool alpha  = 1.0f;
	painter.setColor(color, alpha);

	for (ConstellationLine p : drawnLines)
	{
		painter.drawGreatCircleArc(p.startCoordinate, 
								   p.endCoordinate);
	}

	if (hasFlag(drawState, Drawing::hasFloatingEnd))
	{
		color = {1.f, 0.7f, 0.7f};
		painter.setColor(color, 0.5f);
		painter.drawGreatCircleArc(currentLine.startCoordinate,
		                           currentLine.endCoordinate);
	}
}

void scm::ScmDraw::handleMouseClicks(class QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		// do not interfere with navigation and draw or erase anything
		isNavigating |= (event->type() == QEvent::MouseButtonPress);
		isNavigating &= (event->type() != QEvent::MouseButtonRelease);

		return;
	}

	if (isNavigating)
	{
		return;
	}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	qreal x = event->position().x(), y = event->position().y();
#else
	qreal x = event->x(), y = event->y();
#endif

	if (activeTool == DrawTools::Pen)
	{
		// Draw line
		if (event->button() == Qt::RightButton && event->type() == QEvent::MouseButtonPress)
		{
			StelApp &app       = StelApp::getInstance();
			StelCore *core     = app.getCore();
			StelProjectorP prj = core->getProjection(drawFrame);
			Vec3d point;
			double raJ2000 = scm::ConstellationLine::DEFAULT_RA_DEC;
			double decJ2000 = scm::ConstellationLine::DEFAULT_RA_DEC;
			QString starId;
			prj->unProject(x, y, point);

			// We want to combine any near start point to an existing point so that we don't create
			// duplicates.
			std::optional<StarPoint> nearest = findNearestPoint(x, y, prj);
			if (nearest.has_value())
			{
				point    = nearest.value().coordinate;
				starId   = nearest.value().starId;
				raJ2000  = nearest.value().raJ2000;
				decJ2000 = nearest.value().decJ2000;
			}
			else if (snapToStar)
			{
				if (hasFlag(drawState, Drawing::hasEndExistingPoint))
				{
					point    = currentLine.endCoordinate;
					starId   = currentLine.endId;
					raJ2000  = currentLine.endRAJ2000;
					decJ2000 = currentLine.endDecJ2000;
				}
				else
				{
					StelObjectMgr &objectMgr = app.getStelObjectMgr();

					objectMgr.findAndSelect(core, x, y);
					if (objectMgr.getWasSelected())
					{
						StelObjectP stelObj = objectMgr.getLastSelectedObject();
						Vec3d stelPos       = stelObj->getJ2000EquatorialPos(core);
						point               = stelPos;
						starId              = stelObj->getID();
						raJ2000			 	= stelObj->getInfoMap(core).value("raJ2000").toDouble();
						decJ2000			= stelObj->getInfoMap(core).value("decJ2000").toDouble();
					}
				}
			}

			appendDrawPoint(point, starId, raJ2000, decJ2000);

			event->accept();
			return;
		}

		// Reset line drawing
		if (event->button() == Qt::RightButton && event->type() == QEvent::MouseButtonDblClick &&
		    hasFlag(drawState, Drawing::hasEnd))
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
	else if (activeTool == DrawTools::Eraser)
	{
		if (event->button() == Qt::RightButton && event->type() == QEvent::MouseButtonPress)
		{
			Vec2d currentPos(x, y);
			lastEraserPos = currentPos;
		}
		else if (event->button() == Qt::RightButton && event->type() == QEvent::MouseButtonRelease)
		{
			// Reset
			lastEraserPos = defaultLastEraserPos;
		}
	}
}

bool scm::ScmDraw::handleMouseMoves(int x, int y, Qt::MouseButtons b)
{
	StelApp &app   = StelApp::getInstance();
	StelCore *core = app.getCore();

	if (activeTool == DrawTools::Pen)
	{
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
					Vec3d stelPos       = stelObj->getJ2000EquatorialPos(core);
					currentLine.endCoordinate = stelPos;
				}
				else
				{
					currentLine.endCoordinate = position;
				}
			}
			else
			{
				currentLine.endCoordinate = position;
			}

			drawState = Drawing::hasFloatingEnd;
		}
	}
	else if (activeTool == DrawTools::Eraser)
	{
		if (b.testFlag(Qt::MouseButton::RightButton))
		{
			Vec2d currentPos(x, y);

			if (lastEraserPos != defaultLastEraserPos && lastEraserPos != currentPos)
			{
				StelApp &app        = StelApp::getInstance();
				StelCore *core      = app.getCore();
				StelProjectorP prj  = core->getProjection(drawFrame);
				auto mouseDirection = lastEraserPos - currentPos;

				std::vector<int> erasedIndices;

				for (auto line = drawnLines.begin(); line != drawnLines.end();
				     ++line)
				{
					Vec3d lineEnd, lineStart;
					prj->project(line->startCoordinate, lineStart);
					prj->project(line->endCoordinate, lineEnd);
					Vec2d lineStart2d(lineStart.v[0], lineStart.v[1]);
					Vec2d lineEnd2d(lineEnd.v[0], lineEnd.v[1]);
					auto lineDirection = lineEnd2d - lineStart2d;

					bool intersect = segmentIntersect(currentPos, mouseDirection, lineStart2d,
					                                  lineDirection);
					if (intersect)
					{
						erasedIndices.push_back(
							std::distance(drawnLines.begin(), line));
					}
				}

				for (auto index : erasedIndices)
				{
					drawnLines[index] = drawnLines.back();
					drawnLines.pop_back();
				}
			}

			lastEraserPos = currentPos;
		}
	}

	// We always return false as we still want to navigate in Stellarium with left mouse button
	return false;
}

void scm::ScmDraw::handleKeys(QKeyEvent *e)
{
	if (activeTool == DrawTools::Pen)
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
}

void scm::ScmDraw::undoLastLine()
{
	if (!drawnLines.empty())
	{
		currentLine = drawnLines.back();
		drawnLines.pop_back();
		drawState = Drawing::hasFloatingEnd;
	}
	else
	{
		drawState = Drawing::None;
	}
}

std::vector<scm::ConstellationLine> scm::ScmDraw::getConstellationLines()
{
	return drawnLines;
}

void scm::ScmDraw::setTool(scm::DrawTools tool)
{
	activeTool    = tool;
	lastEraserPos = defaultLastEraserPos;
	drawState     = Drawing::None;
}

std::optional<scm::StarPoint> scm::ScmDraw::findNearestPoint(int x, int y, StelProjectorP prj)
{
	if (drawnLines.empty())
	{
		return {};
	}

	auto min = drawnLines.begin();
	Vec3d position(x, y, 0);
	Vec3d minPosition;
	prj->project(min->startCoordinate, minPosition);
	double minDistance = (minPosition - position).dot(minPosition - position);
	bool isStartPoint  = true;

	for (auto line = drawnLines.begin(); line != drawnLines.end(); ++line)
	{
		Vec3d iPosition;
		if (prj->project(line->startCoordinate, iPosition))
		{
			double distance = (iPosition - position).dot(iPosition - position);
			if (distance < minDistance)
			{
				min          = line;
				minPosition  = iPosition;
				minDistance  = distance;
				isStartPoint = true;
			}
		}

		if (prj->project(line->endCoordinate, iPosition))
		{
			double distance = (iPosition - position).dot(iPosition - position);
			if (distance < minDistance)
			{
				min          = line;
				minPosition  = iPosition;
				minDistance  = distance;
				isStartPoint = false;
			}
		}
	}

	if (minDistance < maxSnapRadiusInPixels * maxSnapRadiusInPixels)
	{
		if (isStartPoint)
		{
			StarPoint point = {min->startCoordinate,
			                   min->startId,
							   min->startRAJ2000,
							   min->startDecJ2000};
			return point;
		}
		else
		{
			StarPoint point = {min->endCoordinate,
			                   min->endId,
							   min->endRAJ2000,
							   min->endDecJ2000};
			return point;
		}
	}

	return {};
}

void scm::ScmDraw::resetDrawing()
{
	drawnLines.clear();
	drawState = Drawing::None;
	currentLine.startCoordinate.set(0, 0, 0);
	currentLine.endCoordinate.set(0, 0, 0);
	currentLine.startId.clear();
	currentLine.endId.clear();
	currentLine.startIdNumber = "-1";
	currentLine.endIdNumber = "-1";
	currentLine.startRAJ2000 = scm::ConstellationLine::DEFAULT_RA_DEC;
	currentLine.startDecJ2000 = scm::ConstellationLine::DEFAULT_RA_DEC;
	currentLine.endRAJ2000 = scm::ConstellationLine::DEFAULT_RA_DEC;
	currentLine.endDecJ2000 = scm::ConstellationLine::DEFAULT_RA_DEC;
}
