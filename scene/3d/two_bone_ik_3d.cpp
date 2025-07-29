/**************************************************************************/
/*  two_bone_ik_3d.cpp                                                    */
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

#include "two_bone_ik_3d.h"

bool TwoBoneIK3D::_set(const StringName &p_path, const Variant &p_value) {
	String path = p_path;

	if (path.begins_with("settings/")) {
		int which = path.get_slicec('/', 1).to_int();
		String what = path.get_slicec('/', 2);
		ERR_FAIL_INDEX_V(which, settings.size(), false);

		if (what == "root_bone_name") {
			set_root_bone_name(which, p_value);
		} else if (what == "root_bone") {
			set_root_bone(which, p_value);
		} else if (what == "mid_bone_name") {
			set_root_bone_name(which, p_value);
		} else if (what == "mid_bone") {
			set_root_bone(which, p_value);
		} else if (what == "end_bone_name") {
			set_end_bone_name(which, p_value);
		} else if (what == "end_bone") {
			String opt = path.get_slicec('/', 3);
			if (opt.is_empty()) {
				set_end_bone(which, p_value);
			} else if (opt == "direction") {
				set_end_bone_direction(which, static_cast<BoneDirection>((int)p_value));
			} else if (opt == "length") {
				set_end_bone_length(which, p_value);
			} else {
				return false;
			}
		} else if (what == "extend_end_bone") {
			set_extend_end_bone(which, p_value);
		} else if (what == "target_node") {
			set_target_node(which, p_value);
		} else if (what == "pole_node") {
			set_pole_node(which, p_value);
		} else {
			return false;
		}
	}
	return true;
}

bool TwoBoneIK3D::_get(const StringName &p_path, Variant &r_ret) const {
	String path = p_path;

	if (path.begins_with("settings/")) {
		int which = path.get_slicec('/', 1).to_int();
		String what = path.get_slicec('/', 2);
		ERR_FAIL_INDEX_V(which, settings.size(), false);

		if (what == "root_bone_name") {
			r_ret = get_root_bone_name(which);
		} else if (what == "root_bone") {
			r_ret = get_root_bone(which);
		} else if (what == "mid_bone_name") {
			r_ret = get_root_bone_name(which);
		} else if (what == "mid_bone") {
			r_ret = get_root_bone(which);
		} else if (what == "end_bone_name") {
			r_ret = get_end_bone_name(which);
		} else if (what == "end_bone") {
			String opt = path.get_slicec('/', 3);
			if (opt.is_empty()) {
				r_ret = get_end_bone(which);
			} else if (opt == "direction") {
				r_ret = (int)get_end_bone_direction(which);
			} else if (opt == "length") {
				r_ret = get_end_bone_length(which);
			} else {
				return false;
			}
		} else if (what == "extend_end_bone") {
			r_ret = is_end_bone_extended(which);
		} else if (what == "target_node") {
			r_ret = get_target_node(which);
		} else if (what == "pole_node") {
			r_ret = get_pole_node(which);
		} else {
			return false;
		}
	}
	return true;
}

void TwoBoneIK3D::_get_property_list(List<PropertyInfo> *p_list) const {
	String enum_hint;
	Skeleton3D *skeleton = get_skeleton();
	if (skeleton) {
		enum_hint = skeleton->get_concatenated_bone_names();
	}

	LocalVector<PropertyInfo> props;

	for (int i = 0; i < settings.size(); i++) {
		String path = "settings/" + itos(i) + "/";
		props.push_back(PropertyInfo(Variant::STRING, path + "root_bone_name", PROPERTY_HINT_ENUM_SUGGESTION, enum_hint));
		props.push_back(PropertyInfo(Variant::INT, path + "root_bone", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NO_EDITOR));
		props.push_back(PropertyInfo(Variant::STRING, path + "end_bone_name", PROPERTY_HINT_ENUM_SUGGESTION, enum_hint));
		props.push_back(PropertyInfo(Variant::INT, path + "end_bone", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NO_EDITOR));
		props.push_back(PropertyInfo(Variant::BOOL, path + "extend_end_bone"));
		props.push_back(PropertyInfo(Variant::INT, path + "end_bone/direction", PROPERTY_HINT_ENUM, "+X,-X,+Y,-Y,+Z,-Z,FromParent"));
		props.push_back(PropertyInfo(Variant::FLOAT, path + "end_bone/length", PROPERTY_HINT_RANGE, "0,1,0.001,or_greater,suffix:m"));

		props.push_back(PropertyInfo(Variant::NODE_PATH, path + "target_node"));
		props.push_back(PropertyInfo(Variant::NODE_PATH, path + "pole_node"));
	}

	for (PropertyInfo &p : props) {
		_validate_dynamic_prop(p);
		p_list->push_back(p);
	}
}

void TwoBoneIK3D::_validate_dynamic_prop(PropertyInfo &p_property) const {
	PackedStringArray split = p_property.name.split("/");
	if (split.size() > 2 && split[0] == "settings") {
		int which = split[1].to_int();

		// Extended end bone option.
		if (split[2] == "end_bone" && !is_end_bone_extended(which) && split.size() > 3) {
			p_property.usage = PROPERTY_USAGE_NONE;
		}
	}
}

void TwoBoneIK3D::set_max_iterations(int p_max_iterations) {
	max_iterations = p_max_iterations;
}

int TwoBoneIK3D::get_max_iterations() const {
	return max_iterations;
}

void TwoBoneIK3D::set_min_distance(real_t p_min_distance) {
	min_distance = p_min_distance;
}

real_t TwoBoneIK3D::get_min_distance() const {
	return min_distance;
}

void TwoBoneIK3D::set_angular_delta_limit(real_t p_angular_delta_limit) {
	angular_delta_limit = p_angular_delta_limit;
}

real_t TwoBoneIK3D::get_angular_delta_limit() const {
	return angular_delta_limit;
}

// Setting.

void TwoBoneIK3D::set_root_bone_name(int p_index, const String &p_bone_name) {
	ERR_FAIL_INDEX(p_index, settings.size());
	settings[p_index]->root_bone_name = p_bone_name;
	Skeleton3D *sk = get_skeleton();
	if (sk) {
		set_root_bone(p_index, sk->find_bone(settings[p_index]->root_bone_name));
	}
}

String TwoBoneIK3D::get_root_bone_name(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, settings.size(), String());
	return settings[p_index]->root_bone_name;
}

void TwoBoneIK3D::set_root_bone(int p_index, int p_bone) {
	ERR_FAIL_INDEX(p_index, settings.size());
	bool changed = settings[p_index]->root_bone != p_bone;
	settings[p_index]->root_bone = p_bone;
	Skeleton3D *sk = get_skeleton();
	if (sk) {
		if (settings[p_index]->root_bone <= -1 || settings[p_index]->root_bone >= sk->get_bone_count()) {
			WARN_PRINT("Root bone index out of range!");
			settings[p_index]->root_bone = -1;
		} else {
			settings[p_index]->root_bone_name = sk->get_bone_name(settings[p_index]->root_bone);
		}
	}
	if (changed) {
		_update_joints(p_index);
	}
}

int TwoBoneIK3D::get_root_bone(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, settings.size(), -1);
	return settings[p_index]->root_bone;
}

void TwoBoneIK3D::set_mid_bone_name(int p_index, const String &p_bone_name) {
	ERR_FAIL_INDEX(p_index, settings.size());
	settings[p_index]->mid_bone_name = p_bone_name;
	Skeleton3D *sk = get_skeleton();
	if (sk) {
		set_mid_bone(p_index, sk->find_bone(settings[p_index]->mid_bone_name));
	}
}

String TwoBoneIK3D::get_mid_bone_name(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, settings.size(), String());
	return settings[p_index]->mid_bone_name;
}

void TwoBoneIK3D::set_mid_bone(int p_index, int p_bone) {
	ERR_FAIL_INDEX(p_index, settings.size());
	bool changed = settings[p_index]->mid_bone != p_bone;
	settings[p_index]->mid_bone = p_bone;
	Skeleton3D *sk = get_skeleton();
	if (sk) {
		if (settings[p_index]->mid_bone <= -1 || settings[p_index]->mid_bone >= sk->get_bone_count()) {
			WARN_PRINT("Mid bone index out of range!");
			settings[p_index]->mid_bone = -1;
		} else {
			settings[p_index]->mid_bone_name = sk->get_bone_name(settings[p_index]->mid_bone);
		}
	}
	if (changed) {
		_update_joints(p_index);
	}
}

int TwoBoneIK3D::get_mid_bone(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, settings.size(), -1);
	return settings[p_index]->mid_bone;
}

void TwoBoneIK3D::set_end_bone_name(int p_index, const String &p_bone_name) {
	ERR_FAIL_INDEX(p_index, settings.size());
	settings[p_index]->end_bone_name = p_bone_name;
	Skeleton3D *sk = get_skeleton();
	if (sk) {
		set_end_bone(p_index, sk->find_bone(settings[p_index]->end_bone_name));
	}
}

String TwoBoneIK3D::get_end_bone_name(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, settings.size(), String());
	return settings[p_index]->end_bone_name;
}

void TwoBoneIK3D::set_end_bone(int p_index, int p_bone) {
	ERR_FAIL_INDEX(p_index, settings.size());
	bool changed = settings[p_index]->end_bone != p_bone;
	settings[p_index]->end_bone = p_bone;
	Skeleton3D *sk = get_skeleton();
	if (sk) {
		if (settings[p_index]->end_bone <= -1 || settings[p_index]->end_bone >= sk->get_bone_count()) {
			WARN_PRINT("End bone index out of range!");
			settings[p_index]->end_bone = -1;
		} else {
			settings[p_index]->end_bone_name = sk->get_bone_name(settings[p_index]->end_bone);
		}
	}
	if (changed) {
		_update_joints(p_index);
	}
}

int TwoBoneIK3D::get_end_bone(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, settings.size(), -1);
	return settings[p_index]->end_bone;
}

void TwoBoneIK3D::set_extend_end_bone(int p_index, bool p_enabled) {
	ERR_FAIL_INDEX(p_index, settings.size());
	settings[p_index]->extend_end_bone = p_enabled;
	settings[p_index]->simulation_dirty = true;
	notify_property_list_changed();
}

bool TwoBoneIK3D::is_end_bone_extended(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, settings.size(), false);
	return settings[p_index]->extend_end_bone;
}

void TwoBoneIK3D::set_end_bone_direction(int p_index, BoneDirection p_bone_direction) {
	ERR_FAIL_INDEX(p_index, settings.size());
	settings[p_index]->end_bone_direction = p_bone_direction;
	settings[p_index]->simulation_dirty = true;
}

TwoBoneIK3D::BoneDirection TwoBoneIK3D::get_end_bone_direction(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, settings.size(), BONE_DIRECTION_FROM_PARENT);
	return settings[p_index]->end_bone_direction;
}

void TwoBoneIK3D::set_end_bone_length(int p_index, float p_length) {
	ERR_FAIL_INDEX(p_index, settings.size());
	settings[p_index]->end_bone_length = p_length;
	settings[p_index]->simulation_dirty = true;
}

float TwoBoneIK3D::get_end_bone_length(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, settings.size(), 0);
	return settings[p_index]->end_bone_length;
}

Vector3 TwoBoneIK3D::get_end_bone_axis(int p_end_bone, BoneDirection p_direction) const {
	Vector3 axis;
	if (p_direction == BONE_DIRECTION_FROM_PARENT) {
		Skeleton3D *sk = get_skeleton();
		if (sk) {
			axis = sk->get_bone_rest(p_end_bone).basis.xform_inv(sk->get_bone_rest(p_end_bone).origin);
			axis.normalize();
		}
	} else {
		axis = get_vector_from_bone_axis(static_cast<BoneAxis>((int)p_direction));
	}
	return axis;
}

void TwoBoneIK3D::set_target_node(int p_index, const NodePath &p_node_path) {
	ERR_FAIL_INDEX(p_index, settings.size());
	settings[p_index]->target_node = p_node_path;
}

NodePath TwoBoneIK3D::get_target_node(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, settings.size(), NodePath());
	return settings[p_index]->target_node;
}

void TwoBoneIK3D::set_pole_node(int p_index, const NodePath &p_node_path) {
	ERR_FAIL_INDEX(p_index, settings.size());
	settings[p_index]->pole_node = p_node_path;
}

NodePath TwoBoneIK3D::get_pole_node(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, settings.size(), NodePath());
	return settings[p_index]->pole_node;
}

void TwoBoneIK3D::set_setting_count(int p_count) {
	ERR_FAIL_COND(p_count < 0);

	int delta = p_count - settings.size();
	if (delta < 0) {
		for (int i = delta; i < 0; i++) {
			memdelete(settings[settings.size() + i]);
		}
	}
	settings.resize(p_count);
	delta++;
	if (delta > 1) {
		for (int i = 1; i < delta; i++) {
			settings.write[p_count - i] = memnew(TwoBoneIK3DSetting);
		}
	}
	notify_property_list_changed();
}

int TwoBoneIK3D::get_setting_count() const {
	return settings.size();
}

void TwoBoneIK3D::clear_settings() {
	set_setting_count(0);
}

void TwoBoneIK3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_max_iterations", "max_iterations"), &TwoBoneIK3D::set_max_iterations);
	ClassDB::bind_method(D_METHOD("get_max_iterations"), &TwoBoneIK3D::get_max_iterations);
	ClassDB::bind_method(D_METHOD("set_min_distance", "min_distance"), &TwoBoneIK3D::set_min_distance);
	ClassDB::bind_method(D_METHOD("get_min_distance"), &TwoBoneIK3D::get_min_distance);
	ClassDB::bind_method(D_METHOD("set_angular_delta_limit", "angular_delta_limit"), &TwoBoneIK3D::set_angular_delta_limit);
	ClassDB::bind_method(D_METHOD("get_angular_delta_limit"), &TwoBoneIK3D::get_angular_delta_limit);

	// Setting.
	ClassDB::bind_method(D_METHOD("set_root_bone_name", "index", "bone_name"), &TwoBoneIK3D::set_root_bone_name);
	ClassDB::bind_method(D_METHOD("get_root_bone_name", "index"), &TwoBoneIK3D::get_root_bone_name);
	ClassDB::bind_method(D_METHOD("set_root_bone", "index", "bone"), &TwoBoneIK3D::set_root_bone);
	ClassDB::bind_method(D_METHOD("get_root_bone", "index"), &TwoBoneIK3D::get_root_bone);

	ClassDB::bind_method(D_METHOD("set_mid_bone_name", "index", "bone_name"), &TwoBoneIK3D::set_mid_bone_name);
	ClassDB::bind_method(D_METHOD("get_mid_bone_name", "index"), &TwoBoneIK3D::get_mid_bone_name);
	ClassDB::bind_method(D_METHOD("set_mid_bone", "index", "bone"), &TwoBoneIK3D::set_mid_bone);
	ClassDB::bind_method(D_METHOD("get_mid_bone", "index"), &TwoBoneIK3D::get_mid_bone);

	ClassDB::bind_method(D_METHOD("set_end_bone_name", "index", "bone_name"), &TwoBoneIK3D::set_end_bone_name);
	ClassDB::bind_method(D_METHOD("get_end_bone_name", "index"), &TwoBoneIK3D::get_end_bone_name);
	ClassDB::bind_method(D_METHOD("set_end_bone", "index", "bone"), &TwoBoneIK3D::set_end_bone);
	ClassDB::bind_method(D_METHOD("get_end_bone", "index"), &TwoBoneIK3D::get_end_bone);

	ClassDB::bind_method(D_METHOD("set_extend_end_bone", "index", "enabled"), &TwoBoneIK3D::set_extend_end_bone);
	ClassDB::bind_method(D_METHOD("is_end_bone_extended", "index"), &TwoBoneIK3D::is_end_bone_extended);
	ClassDB::bind_method(D_METHOD("set_end_bone_direction", "index", "bone_direction"), &TwoBoneIK3D::set_end_bone_direction);
	ClassDB::bind_method(D_METHOD("get_end_bone_direction", "index"), &TwoBoneIK3D::get_end_bone_direction);
	ClassDB::bind_method(D_METHOD("set_end_bone_length", "index", "length"), &TwoBoneIK3D::set_end_bone_length);
	ClassDB::bind_method(D_METHOD("get_end_bone_length", "index"), &TwoBoneIK3D::get_end_bone_length);

	ClassDB::bind_method(D_METHOD("set_target_node", "index", "target_node"), &TwoBoneIK3D::set_target_node);
	ClassDB::bind_method(D_METHOD("get_target_node", "index"), &TwoBoneIK3D::get_target_node);

	ClassDB::bind_method(D_METHOD("set_pole_node", "index", "pole_node"), &TwoBoneIK3D::set_pole_node);
	ClassDB::bind_method(D_METHOD("get_pole_node", "index"), &TwoBoneIK3D::get_pole_node);

	ClassDB::bind_method(D_METHOD("set_setting_count", "count"), &TwoBoneIK3D::set_setting_count);
	ClassDB::bind_method(D_METHOD("get_setting_count"), &TwoBoneIK3D::get_setting_count);
	ClassDB::bind_method(D_METHOD("clear_settings"), &TwoBoneIK3D::clear_settings);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_iterations", PROPERTY_HINT_RANGE, "0,100,or_greater"), "set_max_iterations", "get_max_iterations");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "min_distance", PROPERTY_HINT_RANGE, "0,1,0.01,or_greater"), "set_min_distance", "get_min_distance");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "angular_delta_limit", PROPERTY_HINT_RANGE, "0,180,0.01,radians_as_degrees"), "set_angular_delta_limit", "get_angular_delta_limit");
	ADD_ARRAY_COUNT("Settings", "setting_count", "set_setting_count", "get_setting_count", "settings/");
}

void TwoBoneIK3D::_validate_bone_names() {
	for (int i = 0; i < settings.size(); i++) {
		// Prior bone name.
		if (!settings[i]->root_bone_name.is_empty()) {
			set_root_bone_name(i, settings[i]->root_bone_name);
		} else if (settings[i]->root_bone != -1) {
			set_root_bone(i, settings[i]->root_bone);
		}
		// Prior bone name.
		if (!settings[i]->mid_bone_name.is_empty()) {
			set_mid_bone_name(i, settings[i]->mid_bone_name);
		} else if (settings[i]->mid_bone != -1) {
			set_mid_bone(i, settings[i]->mid_bone);
		}
		// Prior bone name.
		if (!settings[i]->end_bone_name.is_empty()) {
			set_end_bone_name(i, settings[i]->end_bone_name);
		} else if (settings[i]->end_bone != -1) {
			set_end_bone(i, settings[i]->end_bone);
		}
	}
}

void TwoBoneIK3D::_make_all_joints_dirty() {
	//
}

void TwoBoneIK3D::_init_joints(Skeleton3D *p_skeleton, TwoBoneIK3DSetting *setting) {
	//
}

void TwoBoneIK3D::_update_joints(int p_index) {
	//
}

void TwoBoneIK3D::_process_ik(Skeleton3D *p_skeleton, double p_delta) {
	//
}

void TwoBoneIK3D::_process_joints(double p_delta, Skeleton3D *p_skeleton, TwoBoneIK3DSetting *p_setting, Vector<ManyBoneIK3DJointSetting *> &p_joints, Vector<Vector3> &p_chain, const Vector3 &p_destination, const Vector3 &p_pole_destination, bool p_use_pole) {
	//
}

void TwoBoneIK3D::_solve_iteration_with_pole(double p_delta, Skeleton3D *p_skeleton, TwoBoneIK3DSetting *p_setting, Vector<ManyBoneIK3DJointSetting *> &p_joints, Vector<Vector3> &p_chain, const Vector3 &p_destination, int p_joint_size, int p_chain_size, const Vector3 &p_pole_destination, int p_joint_size_half, int p_chain_size_half) {
	//
}

void TwoBoneIK3D::_solve_iteration(double p_delta, Skeleton3D *p_skeleton, TwoBoneIK3DSetting *p_setting, Vector<ManyBoneIK3DJointSetting *> &p_joints, Vector<Vector3> &p_chain, const Vector3 &p_destination, int p_joint_size, int p_chain_size) {
	//
}

void TwoBoneIK3D::reset() {
	Skeleton3D *skeleton = get_skeleton();
	if (!skeleton) {
		return;
	}
	for (int i = 0; i < settings.size(); i++) {
		settings[i]->simulation_dirty = true;
		_init_joints(skeleton, settings[i]);
	}
}

TwoBoneIK3D::~TwoBoneIK3D() {
	clear_settings();
}
