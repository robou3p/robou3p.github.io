/**
 * @license Licensed under the Apache License, Version 2.0 (the "License"):
 *          http://www.apache.org/licenses/LICENSE-2.0
 */

/**
 * @fileoverview Code generator for the Arduino map functionality.
 *     Arduino built-in function docs: http://arduino.cc/en/Reference/HomePage
 */
'use strict';

goog.provide('Blockly.Arduino.robot');

goog.require('Blockly.Arduino');

Blockly.Arduino['ButtonPressed'] = function(block) {
    var pressed = Blockly.Arduino.valueToCode(
        block, 'NUM', Blockly.Arduino.ORDER_NONE) || '';  
        Blockly.Arduino.addInclude('robot', '#include <Robot.h>');
    var code = 'robot.buttonPressed('+ pressed +')';
    return [code, Blockly.Arduino.ORDER_NONE];
};

Blockly.Arduino['Notes'] = function(block) {
    var TRAJ = Blockly.Arduino.valueToCode(
        block, 'asn', Blockly.Arduino.ORDER_ATOMIC) || '0';
        Blockly.Arduino.addInclude('robot', '#include <Robot.h>');
   switch (block.getFieldValue('BOOL')) {
        case 'Ccase':
          return 'robot.beep(261,'+ TRAJ*1000 + ');\n';
        case 'Dcase':
          return 'robot.beep(293,'+ TRAJ*1000 + ');\n';
        case 'Ecase':
          return 'robot.beep(329,'+ TRAJ*1000 + ');\n';
        case 'Fcase':
          return 'robot.beep(349,'+ TRAJ*1000 + ');\n';
        case 'Gcase':
          return 'robot.beep(392,'+ TRAJ*1000 + ');\n';
        case 'Acase':
          return 'robot.beep(440,'+ TRAJ*1000 + ');\n';
        case 'C1case':
          return 'robot.beep(523,'+ TRAJ*1000 + ');\n';
      }
};

Blockly.Arduino['Beep'] = function(block) {
    var frequency = Blockly.Arduino.valueToCode(
        block, 'numf', Blockly.Arduino.ORDER_ATOMIC) || '110';
    var time = Blockly.Arduino.valueToCode(
        block, 'numt', Blockly.Arduino.ORDER_ATOMIC) || '1000';
    Blockly.Arduino.addInclude('robot', '#include <Robot.h>');
    var code = 'robot.beep('+ frequency +', '+ time*1000 + ');\n'; /**time in seconds*/
    return code;
};

Blockly.Arduino['Go_forward'] = function(block) {
    var distance = Blockly.Arduino.valueToCode(
        block, 'dist', Blockly.Arduino.ORDER_NONE) || '10';  
        Blockly.Arduino.addInclude('robot', '#include <Robot.h>');
    var code = 'robot.go('+ distance/100 +');\n';
    return code;
};


Blockly.Arduino['Turn'] = function(block) {
    var angle = Blockly.Arduino.valueToCode(
        block, 'an', Blockly.Arduino.ORDER_ATOMIC) || '0';
        Blockly.Arduino.addInclude('robot', '#include <Robot.h>');
    var code ='robot.turn('+ angle +');\n';
    return code;
};

   
/**Blockly.Arduino['Pelji'] = function(block) {
    var speed = Blockly.Arduino.valueToCode(
        block, 'ROBOT_SPEED', Blockly.Arduino.ORDER_ATOMIC) || '100';
    var rotation = Blockly.Arduino.valueToCode(
        block, 'ROBOT_ROTATION', Blockly.Arduino.ORDER_ATOMIC) || '0';
    Blockly.Arduino.addInclude('robot','#include <Robot.h>');
    var code = 'robot.pelji('+ speed +', '+ rotation + ');\n';
    return code;
};**/