/**************************************************************************/
/*  two_bone_ik_3d.h                                                      */
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

#pragma once

#include "scene/3d/many_bone_ik_3d.h"

class TwoBoneIK3D : public ManyBoneIK3D {
	GDCLASS(TwoBoneIK3D, ManyBoneIK3D);

public:
	struct TwoBoneIK3DSetting {
		String root_bone_name;
		int root_bone = -1;

		String mid_bone_name;
		int mid_bone = -1;

		String end_bone_name;
		int end_bone = -1;

		// To make virtual end joint.
		bool extend_end_bone = false;
		BoneDirection end_bone_direction = BONE_DIRECTION_FROM_PARENT;
		float end_bone_length = 0.0;

		NodePath target_node;
		NodePath pole_node;

		Vector<ManyBoneIK3DJointSetting *> joints;
		Vector<Vector3> chain;

		// To process.
		bool simulation_dirty = false;
		Transform3D cached_space;

		~TwoBoneIK3DSetting() {
			for (int i = 0; i < joints.size(); i++) {
				memdelete(joints[i]);
			}
			joints.clear();
			chain.clear();
		}
	};

protected:
	int max_iterations = 4;
	real_t min_distance = 0.01; // If distance between end joint and target is less than min_distance, finish iteration.
	real_t min_distance_squared = min_distance * min_distance; // For cache.
	real_t angular_delta_limit = Math::deg_to_rad(2.0); // If the delta is too large, the results before and after iterating can change significantly, and divergence of calculations can easily occur.

	Vector<TwoBoneIK3DSetting *> settings;

	bool _get(const StringName &p_path, Variant &r_ret) const;
	bool _set(const StringName &p_path, const Variant &p_value);
	void _get_property_list(List<PropertyInfo> *p_list) const;
	void _validate_dynamic_prop(PropertyInfo &p_property) const;

	static void _bind_methods();

	virtual void _validate_bone_names() override;
	void _validate_rotation_axes(Skeleton3D *p_skeleton) const;

	void _make_all_joints_dirty() override;
	void _init_joints(Skeleton3D *p_skeleton, TwoBoneIK3DSetting *p_setting);
	void _update_joints(int p_index);

	virtual void _process_ik(Skeleton3D *p_skeleton, double p_delta) override;
	void _process_joints(double p_delta, Skeleton3D *p_skeleton, TwoBoneIK3DSetting *p_setting, Vector<ManyBoneIK3DJointSetting *> &p_joints, Vector<Vector3> &p_chain, const Vector3 &p_target_destination, const Vector3 &p_pole_destination, bool p_use_pole);
	virtual void _solve_iteration_with_pole(double p_delta, Skeleton3D *p_skeleton, TwoBoneIK3DSetting *p_setting, Vector<ManyBoneIK3DJointSetting *> &p_joints, Vector<Vector3> &p_chain, const Vector3 &p_destination, int p_joint_size, int p_chain_size, const Vector3 &p_pole_destination, int p_joint_size_half, int p_chain_size_half);
	virtual void _solve_iteration(double p_delta, Skeleton3D *p_skeleton, TwoBoneIK3DSetting *p_setting, Vector<ManyBoneIK3DJointSetting *> &p_joints, Vector<Vector3> &p_chain, const Vector3 &p_destination, int p_joint_size, int p_chain_size);

public:
	void set_max_iterations(int p_max_iterations);
	int get_max_iterations() const;
	void set_min_distance(real_t p_min_distance);
	real_t get_min_distance() const;
	void set_angular_delta_limit(real_t p_angular_delta_limit);
	real_t get_angular_delta_limit() const;

	// Setting.
	void set_root_bone_name(int p_index, const String &p_bone_name);
	String get_root_bone_name(int p_index) const;
	void set_root_bone(int p_index, int p_bone);
	int get_root_bone(int p_index) const;

	void set_mid_bone_name(int p_index, const String &p_bone_name);
	String get_mid_bone_name(int p_index) const;
	void set_mid_bone(int p_index, int p_bone);
	int get_mid_bone(int p_index) const;

	void set_end_bone_name(int p_index, const String &p_bone_name);
	String get_end_bone_name(int p_index) const;
	void set_end_bone(int p_index, int p_bone);
	int get_end_bone(int p_index) const;

	void set_extend_end_bone(int p_index, bool p_enabled);
	bool is_end_bone_extended(int p_index) const;
	void set_end_bone_direction(int p_index, BoneDirection p_bone_direction);
	BoneDirection get_end_bone_direction(int p_index) const;
	void set_end_bone_length(int p_index, float p_length);
	float get_end_bone_length(int p_index) const;
	Vector3 get_end_bone_axis(int p_end_bone, BoneDirection p_direction) const; // Helper.

	void set_target_node(int p_index, const NodePath &p_target_node);
	NodePath get_target_node(int p_index) const;

	void set_pole_node(int p_index, const NodePath &p_pole_node);
	NodePath get_pole_node(int p_index) const;

	void set_setting_count(int p_count);
	int get_setting_count() const;
	void clear_settings();

	// To process manually.
	virtual void reset() override;

	~TwoBoneIK3D();
};
