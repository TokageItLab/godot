/**************************************************************************/
/*  spring_bone_simulator_3d.compat.inc                                   */
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

#ifndef DISABLE_DEPRECATED

SpringBoneSimulator3D::BoneDirection SpringBoneSimulator3D::_get_end_bone_direction_bind_compat_XXXXX(int p_index) {
	return get_end_bone_direction(p_index);
}

void SpringBoneSimulator3D::_set_end_bone_direction_bind_compat_XXXXX(int p_index, SpringBoneSimulator3D::BoneDirection p_bone_direction) {
	set_end_bone_direction(p_index, p_bone_direction);
}

SpringBoneSimulator3D::RotationAxis SpringBoneSimulator3D::_get_rotation_axis_bind_compat_XXXXX(int p_index) const {
	return get_rotation_axis(p_index);
}

void SpringBoneSimulator3D::_set_rotation_axis_bind_compat_XXXXX(int p_index, SpringBoneSimulator3D::RotationAxis p_axis) {
	set_rotation_axis(p_index, p_axis);
}

SpringBoneSimulator3D::RotationAxis SpringBoneSimulator3D::_get_joint_rotation_axis_bind_compat_XXXXX(int p_index, int p_joint) const {
	return get_joint_rotation_axis(p_index, p_joint);
}

void SpringBoneSimulator3D::_set_joint_rotation_axis_bind_compat_XXXXX(int p_index, int p_joint, SpringBoneSimulator3D::RotationAxis p_axis) {
	set_joint_rotation_axis(p_index, p_joint, p_axis);
}

void SpringBoneSimulator3D::_bind_compatibility_methods() {
	ClassDB::bind_compatibility_method(D_METHOD("get_end_bone_direction", "index"), &SpringBoneSimulator3D::_get_end_bone_direction_bind_compat_XXXXX);
	ClassDB::bind_compatibility_method(D_METHOD("set_end_bone_direction", "index", "bone_direction"), &SpringBoneSimulator3D::_set_end_bone_direction_bind_compat_XXXXX);
	ClassDB::bind_compatibility_method(D_METHOD("get_rotation_axis", "index"), &SpringBoneSimulator3D::_get_rotation_axis_bind_compat_XXXXX);
	ClassDB::bind_compatibility_method(D_METHOD("set_rotation_axis", "index", "axis"), &SpringBoneSimulator3D::_set_rotation_axis_bind_compat_XXXXX);
	ClassDB::bind_compatibility_method(D_METHOD("get_joint_rotation_axis", "index", "joint"), &SpringBoneSimulator3D::_get_joint_rotation_axis_bind_compat_XXXXX);
	ClassDB::bind_compatibility_method(D_METHOD("set_joint_rotation_axis", "index", "joint", "axis"), &SpringBoneSimulator3D::_set_joint_rotation_axis_bind_compat_XXXXX);
}

#endif // DISABLE_DEPRECATED
