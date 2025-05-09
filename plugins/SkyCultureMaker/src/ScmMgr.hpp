/**
 * @file ScmMgr.hpp
 * @author
 * @brief Mangager for SkyCultureMaker plugin.
 * @version 0.1
 * @date 2025-05-09
 * 
 * Managing the creation process of a new sky culture.
 * 1. Navigate in stellarium (UI) to the location of interest (from where the culture should be created)
 * 2. Starting creation process (click in UI)
 * 3. Draw lines from start to star
 *   a) Only stars should be selectable
 *   b) Add functionality to draw separated/unconnected lines (e.g. cross constelation)
 *   c) Add functionality to delete a line
 *     I)  Deleting a inner line of a stick figure should split the figure into two stick figures
 *     II) Connecting two stick figures should merge them into one stick figure
 * 4. Add button to save sky culture
 * 5. Click save button opens dialog to name: sky culture, lines, aliass, ...
 * 6. Completing the dialog (check that all needed arguments are existing and valid) converts intern c++ object to json
 */