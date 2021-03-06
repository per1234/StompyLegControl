
#include <math.h>
#include "geometry.h"
#include "kinematics.h"

Kinematics::Kinematics(LEG_NUMBER leg_number) {
  set_leg_number(leg_number);
}

void Kinematics::set_leg_number(LEG_NUMBER leg_number) {
  _leg_number = leg_number;
  if ((leg_number == LEG_NUMBER::ML) || 
      (leg_number == LEG_NUMBER::MR)) {
    _hip_min = HIP_MIN_ANGLE_MIDDLE;
    _hip_max = HIP_MAX_ANGLE_MIDDLE;
  } else {
    _hip_min = HIP_MIN_ANGLE;
    _hip_max = HIP_MAX_ANGLE;
  };
}

bool Kinematics::angles_in_limits(float hip, float thigh, float knee) {
  if (hip < _hip_min) return false;
  if (hip > _hip_max) return false;
  if (thigh < THIGH_MIN_ANGLE) return false;
  if (thigh > THIGH_MAX_ANGLE) return false;
  if (knee < KNEE_MIN_ANGLE) return false;
  if (knee > KNEE_MAX_ANGLE) return false;
  return true;
}

bool Kinematics::angles_to_xyz(float hip, float thigh, float knee, float* x, float* y, float* z) {
  // range check
  if (!angles_in_limits(hip, thigh, knee)) return false;
  *x = HIP_LENGTH;
  *y = 0;
  *z = 0;

  float a = THIGH_REST_ANGLE - thigh;
  *x += THIGH_LENGTH * cos(a);
  *z += THIGH_LENGTH * sin(a);

  a = KNEE_REST_ANGLE - knee - thigh;
  *x += KNEE_LENGTH * cos(a);
  *z += KNEE_LENGTH * sin(a);

  *y = *x * sin(hip);
  *x *= cos(hip);
  return true;
};

bool Kinematics::angles_to_xyz(JointAngle3D angles, Point3D* point) {
  return angles_to_xyz(angles.hip, angles.thigh, angles.knee, &(point->x), &(point->y), &(point->z));
};

bool Kinematics::xyz_to_angles(float x, float y, float z, float* hip, float* thigh, float* knee) {
  if (x <= HIP_LENGTH) return false;
  float l = sqrt(x * x + y * y);

  *hip = atan2(y, x);

  float L = sqrt(z * z + (l - HIP_LENGTH) * (l - HIP_LENGTH));

  float a1 = acos(-z / L);
  //float a2 = acos(
  //    (KNEE_LENGTH * KNEE_LENGTH - THIGH_LENGTH * THIGH_LENGTH - L * L) /
  //    (-2 * THIGH_LENGTH * L));
  float a2 = acos(
      (KNEE_LENGTH_SQUARED - THIGH_LENGTH_SQUARED - L * L) /
      (-2 * THIGH_LENGTH * L));

  float alpha = (a1 + a2);

  //float beta = acos(
  //    (L * L - KNEE_LENGTH_SQUARED - THIGH_LENGTH_SQUARED) /
  //    (-2 * KNEE_LENGTH * THIGH_LENGTH));
  float beta = acos(
      (L * L - KNEE_LENGTH_SQUARED - THIGH_LENGTH_SQUARED) /
      (KNEE_THIGH_MIN_2));

  *thigh = THIGH_REST_ANGLE - (alpha - PI / 2.);
  //float base_beta = PI - THIGH_REST_ANGLE + KNEE_REST_ANGLE;
  *knee = BASE_BETA - beta;
  // check ranges, return false if out-of-bounds
  return angles_in_limits(*hip, *thigh, *knee);
};

bool Kinematics::xyz_to_angles(Point3D point, JointAngle3D* angles) {
  return xyz_to_angles(point.x, point.y, point.z, &(angles->hip), &(angles->thigh), &(angles->knee));
};
