/**************************************************************************/
/*  bone_expander_3d.cpp                                                  */
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

#include "bone_expander_3d.h"

bool BoneExpander3D::_set(const StringName &p_path, const Variant &p_value) {
	String path = p_path;

	if (path.begins_with("settings/")) {
		int which = path.get_slicec('/', 1).to_int();
		String what = path.get_slicec('/', 2);
		ERR_FAIL_INDEX_V(which, settings.size(), false);

		if (what == "bone") {
			set_bone(which, p_value);
		} else if (what == "bone_scale") {
			set_bone_scale(which, p_value);
		} else {
			return false;
		}
	}
	return true;
}

bool BoneExpander3D::_get(const StringName &p_path, Variant &r_ret) const {
	String path = p_path;

	if (path.begins_with("settings/")) {
		int which = path.get_slicec('/', 1).to_int();
		String what = path.get_slicec('/', 2);
		ERR_FAIL_INDEX_V(which, settings.size(), false);

		if (what == "bone") {
			r_ret = get_bone(which);
		} else if (what == "bone_scale") {
			r_ret = get_bone_scale(which);
		} else {
			return false;
		}
	}
	return true;
}

void BoneExpander3D::_get_property_list(List<PropertyInfo> *p_list) const {
	String enum_hint;
	Skeleton3D *skeleton = get_skeleton();
	if (skeleton) {
		enum_hint = skeleton->get_concatenated_bone_names();
	}

	for (int i = 0; i < settings.size(); i++) {
		String path = "settings/" + itos(i) + "/";
		p_list->push_back(PropertyInfo(Variant::INT, path + "bone", PROPERTY_HINT_ENUM, enum_hint));
		p_list->push_back(PropertyInfo(Variant::VECTOR3, path + "bone_scale"));
	}
}

int BoneExpander3D::get_setting_size() {
	return settings.size();
}

void BoneExpander3D::set_setting_size(int p_size) {
	ERR_FAIL_COND(p_size < 0);

	int delta = p_size - settings.size() + 1;
	settings.resize(p_size);

	if (delta > 1) {
		for (int i = 1; i < delta; i++) {
			validate_scale(p_size - i);
		}
	}

	notify_property_list_changed();
}

void BoneExpander3D::validate_scale(int p_index) {
	Vector3 scl = settings[p_index].second;
	if (Math::is_zero_approx(scl.x * scl.y * scl.z)) {
		settings.write[p_index].second = DEFAULT_SCALE;
	}
}

int BoneExpander3D::add_setting(int p_bone, Vector3 p_scale) {
	int pos = settings.size();
	settings.push_back(BoneExpander3DSetting(p_bone, p_scale));
	return pos;
}

void BoneExpander3D::remove_setting(int p_index) {
	settings.remove_at(p_index);
}

void BoneExpander3D::clear_settings() {
	settings.clear();
}

void BoneExpander3D::set_bone(int p_index, int p_bone) {
	ERR_FAIL_INDEX(p_index, settings.size());
	settings.write[p_index].first = p_bone;
}

int BoneExpander3D::get_bone(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, settings.size(), -1);
	return settings[p_index].first;
}

void BoneExpander3D::set_bone_scale(int p_index, Vector3 p_scale) {
	ERR_FAIL_INDEX(p_index, settings.size());
	ERR_FAIL_COND_MSG(Math::is_zero_approx(p_scale.x * p_scale.y * p_scale.z), "Scale must not be zero.");
	settings.write[p_index].second = p_scale;
}

Vector3 BoneExpander3D::get_bone_scale(int p_index) const {
	ERR_FAIL_INDEX_V(p_index, settings.size(), Vector3(1, 1, 1));
	return settings[p_index].second;
}

void BoneExpander3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_setting_size", "size"), &BoneExpander3D::set_setting_size);
	ClassDB::bind_method(D_METHOD("get_setting_size"), &BoneExpander3D::get_setting_size);
	ClassDB::bind_method(D_METHOD("add_setting", "bone", "scale"), &BoneExpander3D::add_setting, DEFVAL(Vector3(1, 1, 1)));
	ClassDB::bind_method(D_METHOD("remove_setting", "index"), &BoneExpander3D::remove_setting);
	ClassDB::bind_method(D_METHOD("clear_setting"), &BoneExpander3D::clear_settings);

	ClassDB::bind_method(D_METHOD("set_bone", "bone"), &BoneExpander3D::set_bone);
	ClassDB::bind_method(D_METHOD("get_bone"), &BoneExpander3D::get_bone);

	ClassDB::bind_method(D_METHOD("set_bone_scale", "scale"), &BoneExpander3D::set_bone_scale);
	ClassDB::bind_method(D_METHOD("get_bone_scale"), &BoneExpander3D::get_bone_scale);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "setting_size", PROPERTY_HINT_RANGE, "0,1000,1", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_ARRAY, "Settings,settings/"), "set_setting_size", "get_setting_size");
}

void BoneExpander3D::_process_modification() {
	Skeleton3D *skeleton = get_skeleton();
	if (!skeleton) {
		return;
	}

	for (const BoneExpander3DSetting &setting : settings) {
		int bone = setting.first;
		if (bone < 0) {
			continue;
		}

		Vector3 src = skeleton->get_bone_pose_scale(bone);
		Vector3 dst = setting.second;
		skeleton->set_bone_pose_scale(bone, src * dst);
		dst = DEFAULT_SCALE / dst;

		Vector<int> children = skeleton->get_bone_children(bone);
		for (const int &child : children) {
			Transform3D pose = skeleton->get_bone_pose(child);
			pose.scale(dst);
			skeleton->set_bone_pose_unsafe(child, pose);
		}
	}
}
