/*!The Graphic Box Library
 * 
 * GBox is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 * 
 * GBox is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with GBox; 
 * If not, see <a href="http://www.gnu.org/licenses/"> http://www.gnu.org/licenses/</a>
 * 
 * Copyright (C) 2014 - 2015, ruki All rights reserved.
 *
 * @author      ruki
 * @file        tessellator.h
 * @ingroup     utils
 *
 */
#ifndef GB_UTILS_TESSELLATOR_H
#define GB_UTILS_TESSELLATOR_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * algorithm
 */

/*! the tessellate polygon algorithm
 *
 * the algorithm is based on libtess2 here and
 * we optimizated some implementation and fixed some bugs.
 *
 * the differents between our algorithm and libtess2's algorithm:
 *
 *     - we change the coordinate system and the sweep direction (sweep line by horizontal here).
 *
 *     - we need not project the vertices because our graphic engine is 2d,
 *       so our algorithm will be faster.
 *
 *     - we processed more cases of the intersection with some numerical errors, 
 *       so our algorithm will be more stable. 
 *
 *     - we change the algorithm of comparing the active edge and make it more stable for numerical errors.
 *
 *     - we optimizate the algorithm of merging into the convex polygon from the triangulated mesh.
 *       we have not counted the vertices for each region, so it will be faster than libtess2.
 *
 * (you can see libtess2/alg_outline.md if want to known more details of algorithm.)
 *
 * there are four stages to the algorithm:
 *
 *     1. build a mesh (DCEL, be similar to quad-edge) from polygon.
 *
 *     2. tessellate the mesh into the monotone regions if the polygon is concave.
 *
 *     3. triangulate the monotone regions.
 *
 *     4. merge the triangulated regions into the convex regions.
 *
 * there are seven stages to the tessellation algorithm:
 *
 *     1. simplify the mesh and process some degenerate cases.
 *
 *     2. build a vertex event queue and sort it (uses the priority queue with min-heap).
 *
 *     3. build an active edge region list and sort it (uses the partial insertion sort).
 *
 *     4. sweep all events from the event queue using the Bentley-Ottman line-sweep algorithm
 *        and calculate the intersection and winding number.
 *
 *     5. we need fix it if the intersection with numerical errors violate 
 *        the mesh topology or active edge list ordering.
 *
 *     6. process some degenerate cases for the mesh faces which were generated when we fixed some cases.
 *
 *     7. get the monotone regions with the left face marked "inside"
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/*! the polygon tessellator mode enum
 *
 * monotone > convex > triangulation
 */
typedef enum __gb_tessellator_mode_e
{
    GB_TESSELLATOR_MODE_CONVEX          = 0     //!< make convex polygon
,   GB_TESSELLATOR_MODE_MONOTONE        = 1     //!< make monotone polygon
,   GB_TESSELLATOR_MODE_TRIANGULATION   = 2     //!< make triangle 

}gb_tessellator_mode_e;

/// the polygon tessellator rule enum
typedef enum __gb_tessellator_rule_e
{
    GB_TESSELLATOR_RULE_ODD             = GB_PAINT_FILL_RULE_ODD     //!< the odd rule 
,   GB_TESSELLATOR_RULE_NONZERO         = GB_PAINT_FILL_RULE_NONZERO //!< the non-zero rule 

}gb_tessellator_rule_e;

/// the polygon tessellator ref type 
typedef struct{}*       gb_tessellator_ref_t;

/*! the polygon tessellator func type
 *
 * @param points        the points of the contour
 * @param count         the points count of the contour
 * @param priv          the user private data
 */
typedef tb_void_t       (*gb_tessellator_func_t)(gb_point_ref_t points, tb_uint16_t count, tb_cpointer_t priv);

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! init the tessellator
 *
 * @return              the tessellator
 */
gb_tessellator_ref_t    gb_tessellator_init(tb_noarg_t);

/*! exit the tessellator
 *
 * @param tessellator   the tessellator
 */
tb_void_t               gb_tessellator_exit(gb_tessellator_ref_t tessellator);

/*! set the tessellator mode
 *
 * @param tessellator   the tessellator
 * @param mode          the mode
 */
tb_void_t               gb_tessellator_mode_set(gb_tessellator_ref_t tessellator, tb_size_t mode);

/*! set the tessellator rule
 *
 * @param tessellator   the tessellator
 * @param rule          the rule
 */
tb_void_t               gb_tessellator_rule_set(gb_tessellator_ref_t tessellator, tb_size_t rule);

/*! set the tessellator func
 *
 * @param tessellator   the tessellator
 * @param func          the tessellator func
 * @param priv          the user private data
 */
tb_void_t               gb_tessellator_func_set(gb_tessellator_ref_t tessellator, gb_tessellator_func_t func, tb_cpointer_t priv);

/*! done the tessellator
 *
 * @param tessellator   the tessellator
 * @param polygon       the polygon
 * @param bounds        the polygon bounds
 */
tb_void_t               gb_tessellator_done(gb_tessellator_ref_t tessellator, gb_polygon_ref_t polygon, gb_rect_ref_t bounds);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


