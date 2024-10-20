/**************************************************************************/
/*  look_at_modifier_3d.cpp                                               */
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

#include "look_at_modifier_3d.h"

void LookAtModifier3D::_validate_property(PropertyInfo &p_property) const {
	SkeletonModifier3D::_validate_property(p_property);

	if (p_property.name == "bone") {
		Skeleton3D *skeleton = get_skeleton();
		if (skeleton) {
			p_property.hint = PROPERTY_HINT_ENUM;
			p_property.hint_string = skeleton->get_concatenated_bone_names();
		} else {
			p_property.hint = PROPERTY_HINT_NONE;
			p_property.hint_string = "";
		}
	}
}

void LookAtModifier3D::set_bone(int p_bone) {
	bone = p_bone;
}

int LookAtModifier3D::get_bone() const {
	return bone;
}

void LookAtModifier3D::set_forward_axis(LookAtModifier3D::BoneAxis p_axis) {
	forward_axis = p_axis;
}

LookAtModifier3D::BoneAxis LookAtModifier3D::get_forward_axis() const {
	return forward_axis;
}

void LookAtModifier3D::set_primary_rotation(Vector3::Axis p_axis) {
	primary_rotation = p_axis;
}

Vector3::Axis LookAtModifier3D::get_primary_rotation() const {
	return primary_rotation;
}

void LookAtModifier3D::set_use_secondary_rotation(bool p_enabled) {
	use_secondary_rotation = p_enabled;
}

bool LookAtModifier3D::is_using_secondary_rotation() const {
	return use_secondary_rotation;
}

void LookAtModifier3D::set_target_node(NodePath p_target_node) {
	target_node = p_target_node;
}

NodePath LookAtModifier3D::get_target_node() const {
	return target_node;
}

// For time-based interpolation.

void LookAtModifier3D::set_duration(float p_duration) {
	duration = p_duration;
}

float LookAtModifier3D::get_duration() const {
	return duration;
}

void LookAtModifier3D::set_transition_type(Tween::TransitionType p_transition_type) {
	transition_type = p_transition_type;
}

Tween::TransitionType LookAtModifier3D::get_transition_type() const {
	return transition_type;
}

void LookAtModifier3D::set_ease_type(Tween::EaseType p_ease_type) {
	ease_type = p_ease_type;
}

Tween::EaseType LookAtModifier3D::get_ease_type() const {
	return ease_type;
}

// For angle limitation.

void LookAtModifier3D::set_use_angle_limitation(bool p_enabled) {
	use_angle_limitation = p_enabled;
	notify_property_list_changed();
}

bool LookAtModifier3D::is_using_angle_limitation() const {
	return use_angle_limitation;
}

void LookAtModifier3D::set_limit_angle(float p_angle) {
	limit_angle = p_angle;
}

float LookAtModifier3D::get_limit_angle() const {
	return limit_angle;
}

void LookAtModifier3D::set_dump_power(float p_power) {
	dump_power = p_power;
}

float LookAtModifier3D::get_dump_power() const {
	return dump_power;
}

// General API.

void LookAtModifier3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_target_node", "target_node"), &LookAtModifier3D::set_target_node);
	ClassDB::bind_method(D_METHOD("get_target_node"), &LookAtModifier3D::get_target_node);

	ClassDB::bind_method(D_METHOD("set_bone", "bone"), &LookAtModifier3D::set_bone);
	ClassDB::bind_method(D_METHOD("get_bone"), &LookAtModifier3D::get_bone);
	ClassDB::bind_method(D_METHOD("set_forward_axis", "forward_axis"), &LookAtModifier3D::set_forward_axis);
	ClassDB::bind_method(D_METHOD("get_forward_axis"), &LookAtModifier3D::get_forward_axis);
	ClassDB::bind_method(D_METHOD("set_primary_rotation", "axis"), &LookAtModifier3D::set_primary_rotation);
	ClassDB::bind_method(D_METHOD("get_primary_rotation"), &LookAtModifier3D::get_primary_rotation);
	ClassDB::bind_method(D_METHOD("set_use_secondary_rotation", "enabled"), &LookAtModifier3D::set_use_secondary_rotation);
	ClassDB::bind_method(D_METHOD("is_using_secondary_rotation"), &LookAtModifier3D::is_using_secondary_rotation);

	ClassDB::bind_method(D_METHOD("set_duration", "duration"), &LookAtModifier3D::set_duration);
	ClassDB::bind_method(D_METHOD("get_duration"), &LookAtModifier3D::get_duration);
	ClassDB::bind_method(D_METHOD("set_transition_type", "transition_type"), &LookAtModifier3D::set_transition_type);
	ClassDB::bind_method(D_METHOD("get_transition_type"), &LookAtModifier3D::get_transition_type);
	ClassDB::bind_method(D_METHOD("set_ease_type", "ease_type"), &LookAtModifier3D::set_ease_type);
	ClassDB::bind_method(D_METHOD("get_ease_type"), &LookAtModifier3D::get_ease_type);

	ClassDB::bind_method(D_METHOD("set_use_angle_limitation", "enabled"), &LookAtModifier3D::set_use_angle_limitation);
	ClassDB::bind_method(D_METHOD("is_using_angle_limitation"), &LookAtModifier3D::is_using_angle_limitation);
	ClassDB::bind_method(D_METHOD("set_limit_angle", "angle"), &LookAtModifier3D::set_limit_angle);
	ClassDB::bind_method(D_METHOD("get_limit_angle"), &LookAtModifier3D::get_limit_angle);
	ClassDB::bind_method(D_METHOD("set_dump_power", "power"), &LookAtModifier3D::set_dump_power);
	ClassDB::bind_method(D_METHOD("get_dump_power"), &LookAtModifier3D::get_dump_power);

	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "target_node", PROPERTY_HINT_NODE_TYPE, "Node3D"), "set_target_node", "get_target_node");

	ADD_PROPERTY(PropertyInfo(Variant::INT, "bone", PROPERTY_HINT_ENUM, ""), "set_bone", "get_bone");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "forward_axis", PROPERTY_HINT_ENUM, "+X,-X,+Y,-Y,+Z,-Z"), "set_forward_axis", "get_forward_axis");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "primary_rotation", PROPERTY_HINT_ENUM, "X,Y,Z"), "set_primary_rotation", "get_primary_rotation");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_secondary_rotation"), "set_use_secondary_rotation", "is_using_secondary_rotation");

	ADD_GROUP("Time Based Interpolation", "");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "duration", PROPERTY_HINT_RANGE, "0,10,0.001,or_greater,suffix:s"), "set_duration", "get_duration");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "transition_type", PROPERTY_HINT_ENUM, "Linear,Sine,Quint,Quart,Quad,Expo,Elastic,Cubic,Circ,Bounce,Back,Spring"), "set_transition_type", "get_transition_type");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "ease_type", PROPERTY_HINT_ENUM, "In,Out,InOut,OutIn"), "set_ease_type", "get_ease_type");

	ADD_GROUP("Angle Limitation", "");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_angle_limitation"), "set_use_angle_limitation", "is_using_angle_limitation");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "limit_angle", PROPERTY_HINT_RANGE, "0,360,0.01,radians_as_degrees"), "set_limit_angle", "get_limit_angle");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "dump_power", PROPERTY_HINT_RANGE, "0,1000,0.01,or_greater"), "set_dump_power", "get_dump_power");

	BIND_ENUM_CONSTANT(BONE_AXIS_PLUS_X);
	BIND_ENUM_CONSTANT(BONE_AXIS_MINUS_X);
	BIND_ENUM_CONSTANT(BONE_AXIS_PLUS_Y);
	BIND_ENUM_CONSTANT(BONE_AXIS_MINUS_Y);
	BIND_ENUM_CONSTANT(BONE_AXIS_PLUS_Z);
	BIND_ENUM_CONSTANT(BONE_AXIS_MINUS_Z);
}

void LookAtModifier3D::_process_modification() {
	Skeleton3D *skeleton = get_skeleton();
	if (!skeleton) {
		return;
	}

	Node3D *target = cast_to<Node3D>(get_node_or_null(target_node));
	if (!target) {
		return;
	}
	
	Transform3D destination = look_at_with_axes(skeleton->get_bone_pose(bone), target->get_transform().get_origin());
	skeleton->set_bone_pose(bone, destination);
}

Vector3 LookAtModifier3D::get_basis_vector_from_bone_axis(Basis p_basis, LookAtModifier3D::BoneAxis p_axis) {
	Vector3 ret;
	switch (p_axis) {
		case BONE_AXIS_PLUS_X: {
			ret = p_basis.get_column(0).normalized();
		} break;
		case BONE_AXIS_MINUS_X: {
			ret = -p_basis.get_column(0).normalized();
		} break;
		case BONE_AXIS_PLUS_Y: {
			ret = p_basis.get_column(1).normalized();
		} break;
		case BONE_AXIS_MINUS_Y: {
			ret = -p_basis.get_column(1).normalized();
		} break;
		case BONE_AXIS_PLUS_Z: {
			ret = p_basis.get_column(2).normalized();
		} break;
		case BONE_AXIS_MINUS_Z: {
			ret = -p_basis.get_column(2).normalized();
		} break;
	}
	return ret;
}

Vector3 LookAtModifier3D::get_vector_from_bone_axis(LookAtModifier3D::BoneAxis p_axis) {
	Vector3 ret;
	switch (p_axis) {
		case BONE_AXIS_PLUS_X: {
			ret = Vector3(1, 0, 0);
		} break;
		case BONE_AXIS_MINUS_X: {
			ret = Vector3(-1, 0, 0);
		} break;
		case BONE_AXIS_PLUS_Y: {
			ret = Vector3(0, 1, 0);
		} break;
		case BONE_AXIS_MINUS_Y: {
			ret = Vector3(0, -1, 0);
		} break;
		case BONE_AXIS_PLUS_Z: {
			ret = Vector3(0, 0, 1);
		} break;
		case BONE_AXIS_MINUS_Z: {
			ret = Vector3(0, 0, -1);
		} break;
	}
	return ret;
}

Vector3 LookAtModifier3D::get_vector_from_axis(Vector3::Axis p_axis) {
	Vector3 ret;
	switch (p_axis) {
		case Vector3::AXIS_X: {
			ret = Vector3(1, 0, 0);
		} break;
		case Vector3::AXIS_Y: {
			ret = Vector3(0, 1, 0);
		} break;
		case Vector3::AXIS_Z: {
			ret = Vector3(0, 0, 1);
		} break;
	}
	return ret.normalized();
}

Vector2 LookAtModifier3D::get_projection_vector(Vector3 p_vector, Vector3::Axis p_axis) {
	// NOTE: axis is swapped between 2D and 3D.
	Vector2 ret;
	switch (p_axis) {
		case Vector3::AXIS_X: {
			ret = Vector2(p_vector.z, p_vector.y);
		} break;
		case Vector3::AXIS_Y: {
			ret = Vector2(p_vector.x, p_vector.z);
		} break;
		case Vector3::AXIS_Z: {
			ret = Vector2(p_vector.y, p_vector.x);
		} break;
	}
	return ret.normalized();
}

Transform3D LookAtModifier3D::look_at_with_axes(Transform3D p_from, Vector3 p_target) {
	Vector3 forward_vector = (p_target - p_from.origin).normalized();

	// Primary rotation by projection to 2D plane by xform_inv and picking elements.
	Vector3 current_vector = get_basis_vector_from_bone_axis(p_from.basis, forward_axis);
	Vector2 src_vec2 = get_projection_vector(p_from.basis.xform_inv(forward_vector), primary_rotation);
	Vector2 dst_vec2 = get_projection_vector(p_from.basis.xform_inv(current_vector), primary_rotation);
	real_t calculated_angle = src_vec2.angle_to(dst_vec2);

	Transform3D primary_result = p_from.rotated_local(get_vector_from_axis(primary_rotation), calculated_angle);

	if (!use_secondary_rotation) {
		return primary_result;
	}

	Vector3 secondary_axis = get_vector_from_bone_axis(forward_axis) + get_vector_from_axis(primary_rotation);
	Vector3::Axis secondary_rotation = Math::is_zero_approx(secondary_axis.x) ? Vector3::AXIS_X : (Math::is_zero_approx(secondary_axis.y) ? Vector3::AXIS_Y : Vector3::AXIS_Z);

	// Secondary rotation by projection to 2D plane by xform_inv and picking elements.
	current_vector = get_basis_vector_from_bone_axis(primary_result.basis, forward_axis);
	src_vec2 = get_projection_vector(primary_result.basis.xform_inv(forward_vector), secondary_rotation);
	dst_vec2 = get_projection_vector(primary_result.basis.xform_inv(current_vector), secondary_rotation);
	calculated_angle = src_vec2.angle_to(dst_vec2);

	Transform3D secondary_result = primary_result.rotated_local(get_vector_from_axis(secondary_rotation), calculated_angle);

	return secondary_result;
}
