/**************************************************************************/
/*  look_at_modifier_3d.h                                                 */
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

#ifndef LOOK_AT_MODIFIER_3D_H
#define LOOK_AT_MODIFIER_3D_H

#include "scene/3d/skeleton_modifier_3d.h"
#include "scene/animation/tween.h"

class LookAtModifier3D : public SkeletonModifier3D {
	GDCLASS(LookAtModifier3D, SkeletonModifier3D);

public:
	enum BoneAxis {
		BONE_AXIS_PLUS_X,
		BONE_AXIS_MINUS_X,
		BONE_AXIS_PLUS_Y,
		BONE_AXIS_MINUS_Y,
		BONE_AXIS_PLUS_Z,
		BONE_AXIS_MINUS_Z,
	};

private:
	int bone = 0;

	BoneAxis forward_axis = BONE_AXIS_PLUS_Z;
	Vector3::Axis primary_rotation = Vector3::AXIS_Y;
	bool use_secondary_rotation = true;

	NodePath target_node;

	float duration = 0;
	Tween::TransitionType transition_type = Tween::TRANS_LINEAR;
	Tween::EaseType ease_type = Tween::EASE_IN;

	bool use_angle_limitation = false;
	float limit_angle = 360;
	float dump_power = 2.0;

	Quaternion from_q; // For time-based interpolation.
	Quaternion prev_q; // For detecting flipping by angle limit.
	float duration_left = 0;

	Vector3 get_basis_vector_from_bone_axis(Basis p_basis, BoneAxis p_axis);
	Vector3 get_vector_from_bone_axis(BoneAxis p_axis);
	Vector3 get_vector_from_axis(Vector3::Axis p_axis);
	Vector2 get_projection_vector(Vector3 p_vector, Vector3::Axis p_axis);
	Transform3D look_at_with_axes(Transform3D p_from, Vector3 p_target);

protected:
	void _validate_property(PropertyInfo &p_property) const;

	static void _bind_methods();

	virtual void _process_modification() override;

public:
	void set_bone(int p_bone);
	int get_bone() const;

	void set_forward_axis(BoneAxis p_axis);
	BoneAxis get_forward_axis() const;
	void set_primary_rotation(Vector3::Axis p_axis);
	Vector3::Axis get_primary_rotation() const;
	void set_use_secondary_rotation(bool p_enabled);
	bool is_using_secondary_rotation() const;

	void set_target_node(NodePath p_target_node);
	NodePath get_target_node() const;

	void set_duration(float p_duration);
	float get_duration() const;
	void set_transition_type(Tween::TransitionType p_transition_type);
	Tween::TransitionType get_transition_type() const;
	void set_ease_type(Tween::EaseType p_ease_type);
	Tween::EaseType get_ease_type() const;

	void set_use_angle_limitation(bool p_enabled);
	bool is_using_angle_limitation() const;
	void set_limit_angle(float p_angle);
	float get_limit_angle() const;
	void set_dump_power(float p_power);
	float get_dump_power() const;
};

VARIANT_ENUM_CAST(LookAtModifier3D::BoneAxis);

#endif // LOOK_AT_MODIFIER_3D_H
