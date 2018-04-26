#ifndef JOYSTICK_H

    #define JOYSTICK_H
     
    void init_joystick();
    void joystick_movement();
    void joystick_debug();
    void joystick_debug_from_angles();
    void joystick_debug_from_xyz();
    void joystick_move_angles();
    void joysitck_change_mode(int* change_joystick_mode);
    void joystick2_move_gripper();
    void joystick_move_xyz();
    void joystick_move_xy();
    void joystick_move_z();
    void buttons_move_xyz();
     
#endif /* JOYSTICK_H */