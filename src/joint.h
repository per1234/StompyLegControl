/*
	joint.h - Leg joint class.
	Created by Brett Graham, June 11, 2017.
	Released into the public domain -- so help you God.
*/

#ifndef JOINT_H
#define JOINT_H

#include "pid.h"
#include "Arduino.h"
#include "valve.h"
#include "sensors.h"
#include "transforms.h"


class Joint {
  public:
    Joint(Valve* valve, StringPot* pot, JointAngleTransform* angle_transform, PID* pid);

    void set_pwm_limits(int pwm_min, int pwm_max);

    bool set_target_angle(float angle);
    bool set_target_length(float length);
    bool set_target_adc_value(unsigned int);

    float get_target_angle();
    float get_target_length();
    unsigned int get_target_adc_value();

    float get_current_length();
    float get_current_angle();

    float get_pid_output();

    void update();
  private:
    Valve* _valve;
    StringPot* _pot;
    JointAngleTransform* _angle_transform;
    PID* _pid;

    float _target_angle;
    float _target_length;
    float _target_adc_value;

    float _min_pid_output;
    float _pid_output;
    int _pwm_min;
    int _pwm_max;
};

#endif
