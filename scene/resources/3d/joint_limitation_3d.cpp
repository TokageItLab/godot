/**************************************************************************/
/*  joint_limitation_3d.cpp                                               */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "joint_limitation_3d.h"
#include "scene/3d/skeleton_modifier_3d.h"

Quaternion JointLimitation3D::_make_space(const Vector3 &p_local_forward_vector) const {
	// The default is to interpret the forward vector as the +Y axis.
	Vector3 axis_y = p_local_forward_vector.normalized();
	Vector3 axis_x = axis_y.get_any_perpendicular();
	Vector3 axis_z = axis_y.cross(axis_x);
	return Basis(axis_x, axis_y, axis_z).get_rotation_quaternion();
}

Vector3 JointLimitation3D::_solve(const Vector3 &p_direction) const {
	return p_direction;
}

Vector3 JointLimitation3D::solve(
			const Vector3 &p_global_origin,
			const Vector3 &p_global_destination,
			const Quaternion &p_rest_on_parent_global_pose,
			const Vector3 &p_local_forward_vector,
			const double p_length
		) const {
	Quaternion space = _make_space(p_local_forward_vector) * p_rest_on_parent_global_pose;
	Vector3 dir = space.xform((p_global_destination - p_global_origin).normalized());
	return p_global_origin + space.xform_inv(_solve(dir)) * p_length;
}
