/**
 * @file ScmDraw.hpp
 * @author vgerlach
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

namespace scm
{

class ScmDraw
{
public:
	struct StarPoint
	{
		/// @brief The position of the point in coordinates.
		Vec3d position;

		/// @brief The star id of the the point if available.
		std::optional<QString> starID;
	};

	struct StarLine
	{
		/// @brief The start point of the line.
		StarPoint start;

		/// @brief The end point of the line.
		StarPoint end;
	};

	/// @brief The possibles states during the drawing.
	enum class Drawing
	{
		/// @brief No line is available.
		None = 1,
		/// @brief The line as a starting point.
		hasStart = 2,
		/// @brief The line has a not placed end that is attached to the cursor.
		hasFloatingEnd = 4,
		/// @brief The line is complete i.e. has start and end point.
		hasEnd = 8,
		/// @brief The end is an already existing point.
		hasEndExistingPoint = 16,
	};

private:
	/// @brief The search radius to attach to a point on a existing line.
	uint32_t maxSnapRadiusInPixels = 25;

	/// @brief Indicates that the startPoint has been set.
	Drawing drawState;

	/// @brief Indicates if a line start or end will snap to the nearest star.
	bool snapToStar;

	/// @brief The current pending point.
	StarLine currentLine;

	/// @brief The fixed points.
	std::vector<StarLine> drawnLines;

public:
	/// @brief The frame that is used for calculation and is drawn on.
	const StelCore::FrameType drawFrame = StelCore::FrameJ2000;

	ScmDraw();
	~ScmDraw();

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
};

}  // namespace scm

// Opt In for Drawing to use bitops & and |
template <> struct generic_enum_bitops::allow_bitops<scm::ScmDraw::Drawing>
{
	static constexpr bool value = true;
};

#endif	// SCMDRAW_H