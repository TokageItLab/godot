/**************************************************************************/
/*  fabr_ik_3d.cpp                                                        */
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

#include "fabr_ik_3d.h"

void FABRIK3D::_process_joints(double p_delta, Skeleton3D *p_skeleton, Vector<ManyBoneIK3DJointSetting *> &p_joints, Vector<Vector3> &p_chain, const Transform3D &p_space, const Vector3 &p_destination, int p_max_iterations, real_t p_min_distance_squared) {
	real_t distance_to_target_sq = INFINITY;
	int iteration_count = 0;

	while (distance_to_target_sq > p_min_distance_squared && iteration_count < p_max_iterations) {
		iteration_count++;

		// Backwards.
		bool first = true;
		Quaternion last_rot_inv;
		for (int i = p_joints.size() - 1; i >= 0; i--) {
			ManyBoneIK3DSolverInfo *solver_info = p_joints[i]->solver_info;
			if (!solver_info || Math::is_zero_approx(solver_info->length)) {
				continue;
			}
			Quaternion current_space = (p_space * p_skeleton->get_bone_global_pose(p_joints[i]->bone)).basis.get_rotation_quaternion();

			const int HEAD = i;
			const int TAIL = i + 1;

			if (first) {
				p_chain.write[TAIL] = p_destination;
				first = false;
			}

			Vector3 new_origin = p_chain[TAIL];
			Vector3 new_destination = limit_length(p_chain[TAIL], p_chain[HEAD], solver_info->length);

			if (p_joints[i]->rotation_axis != ROTATION_AXIS_ALL) {
				new_destination = new_origin + current_space.xform(snap_vector_to_plane(p_joints[i]->get_rotation_axis_vector(), current_space.xform_inv(new_destination - new_origin)));
				new_destination = limit_length(new_origin, new_destination, solver_info->length);
			}

			if (p_joints[i]->limitation.is_valid()) {
				new_destination = p_joints[i]->limitation->solve(
					new_origin,
					new_destination,
					last_rot_inv,
					-solver_info->forward_vector, // Tail to Head, so forward must be flipped.
					solver_info->length);
				new_destination = limit_length(new_origin, new_destination, solver_info->length);
			}
			p_chain.write[HEAD] = new_destination;
			last_rot_inv = get_from_to_rotation(solver_info->forward_vector, (p_chain[HEAD] - p_chain[TAIL]).normalized(), Quaternion()).inverse();
		}

		// Forwards.
		first = true;
		last_rot_inv = Quaternion();
		for (int i = 0; i < p_joints.size(); i++) {
			ManyBoneIK3DSolverInfo *solver_info = p_joints[i]->solver_info;
			if (!solver_info || Math::is_zero_approx(solver_info->length)) {
				continue;
			}
			Quaternion current_space = (p_space * p_skeleton->get_bone_global_pose(p_joints[i]->bone)).basis.get_rotation_quaternion();

			const int HEAD = i;
			const int TAIL = i + 1;

			if (first) {
				p_chain.write[HEAD] = p_skeleton->get_bone_global_pose(p_joints[i]->bone).origin;
				first = false;
			}

			Vector3 new_origin = p_chain[HEAD];
			Vector3 new_destination = limit_length(p_chain[HEAD], p_chain[TAIL], solver_info->length);

			if (p_joints[i]->rotation_axis != ROTATION_AXIS_ALL) {
				new_destination = new_origin + current_space.xform(snap_vector_to_plane(p_joints[i]->get_rotation_axis_vector(), current_space.xform_inv(new_destination - new_origin)));
				new_destination = limit_length(new_origin, new_destination, solver_info->length);
			}

			if (p_joints[i]->limitation.is_valid()) {
				new_destination = p_joints[i]->limitation->solve(
					new_origin,
					new_destination,
					last_rot_inv,
					solver_info->forward_vector,
					solver_info->length);
				new_destination = limit_length(new_origin, new_destination, solver_info->length);
			}
			p_chain.write[TAIL] = new_destination;
			last_rot_inv = get_from_to_rotation(solver_info->forward_vector, (p_chain[HEAD] - p_chain[TAIL]).normalized(), Quaternion()).inverse();
		}

		distance_to_target_sq = p_chain[p_chain.size() - 1].distance_squared_to(p_destination);
	}

	// Apply the rotation to the bones.
	for (int i = 0; i < p_joints.size(); i++) {
		ManyBoneIK3DSolverInfo *solver_info = p_joints[i]->solver_info;
		if (!solver_info || Math::is_zero_approx(solver_info->length)) {
			continue;
		}
		Vector3 current_head_to_tail = (p_chain[i + 1] - p_chain[i]).normalized();
		current_head_to_tail = p_skeleton->get_bone_global_pose(p_joints[i]->bone).basis.get_rotation_quaternion().xform_inv(current_head_to_tail);
		solver_info->current_rot = get_from_to_rotation(solver_info->forward_vector, current_head_to_tail, solver_info->current_rot);
		p_skeleton->set_bone_pose_rotation(p_joints[i]->bone,
			get_local_pose_rotation(
				p_skeleton,
				p_joints[i]->bone,
				p_skeleton->get_bone_global_pose(p_joints[i]->bone).basis.get_rotation_quaternion() * solver_info->current_rot));
	}
}
