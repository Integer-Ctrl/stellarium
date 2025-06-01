/**
 * @file ScmDraw.hpp
 * @author vgerlach, lgrumbach
 * @brief Draws between stars objects and free coordinate points.
 * @version 0.1
 * @date 2025-05-17
 */

#ifndef SCMDRAW_H
#define SCMDRAW_H

#include "StelCore.hpp"
#include "StelObjectMgr.hpp"
#include <vector>
#include <QString>
#include "enumBitops.hpp"
#include <optional>
#include <tuple>
#include "StelObjectType.hpp"
#include <variant>
#include "ScmTypes.hpp"

namespace scm
{

class ScmDraw
{
private:
	//! @brief The search radius to attach to a point on a existing line.
	uint32_t maxSnapRadiusInPixels = 25;

	//! @brief Indicates that the startPoint has been set.
	Drawing drawState;

	//! @brief Indicates if a line start or end will snap to the nearest star.
	bool snapToStar;

	//! @brief The current pending point.
	std::tuple<CoordinateLine, StarLine> currentLine;

	//! @brief The fixed points.
	Lines drawnLines;

public:
	//! @brief The frame that is used for calculation and is drawn on.
	static const StelCore::FrameType drawFrame = StelCore::FrameJ2000;

	ScmDraw();

	/**
	 * @brief Draws the line between the start and the current end point.
	 *
	 * @param core The core used for drawing the line.
	 */
	void drawLine(StelCore *core);

	//! Handle mouse clicks. Please note that most of the interactions will be done through the GUI module.
	//! @return set the event as accepted if it was intercepted
	void handleMouseClicks(QMouseEvent *);

	//! Handle mouse moves. Please note that most of the interactions will be done through the GUI module.
	//! @return true if the event was intercepted
	bool handleMouseMoves(int x, int y, Qt::MouseButtons b);

	//! Handle key events. Please note that most of the interactions will be done through the GUI module.
	//! @param e the Key event
	//! @return set the event as accepted if it was intercepted
	void handleKeys(QKeyEvent *e);

	/**
	 * @brief Finds the nearest star point to the given position.
	 *
	 * @param x The x viewport coordinate of the mouse.
	 * @param y The y viewport coordinate of the mouse.
	 * @param prj The projector to use for the calculation.
	 * @return std::optional<StarPoint> The found star point if available.
	 */
	std::optional<StarPoint> findNearestPoint(int x, int y, StelProjectorP prj);

	//! Undo the last drawn line.
	void undoLastLine();

	/**
	 * @brief Get the drawn stick figures as stars if available.
	 *
	 * @return std::vector<StarLine> The optional filled vector of stars matching the coordinates.
	 */
	std::vector<StarLine> getStars();

	/**
	 * @brief Get the drawn stick figures as coordinates.
	 *
	 * @return std::vector<CoordinateLine> The drawn coordinates.
	 */
	std::vector<CoordinateLine> getCoordinates();
};

}  // namespace scm

// Opt In for Drawing to use bitops & and |
template <> struct generic_enum_bitops::allow_bitops<scm::Drawing>
{
	static constexpr bool value = true;
};

#endif	// SCMDRAW_H