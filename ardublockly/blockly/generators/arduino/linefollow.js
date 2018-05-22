/**
 * @license Licensed under the Apache License, Version 2.0 (the "License"):
 *          http://www.apache.org/licenses/LICENSE-2.0
 */

/**
 * @fileoverview Code generator for the Arduino map functionality.
 *     Arduino built-in function docs: http://arduino.cc/en/Reference/HomePage
 */
'use strict';

goog.provide('Blockly.Arduino.linefollow');

goog.require('Blockly.Arduino');


/**
 * Code generator for the map block.
 * Arduino code: loop { map(x, 0, 1024, 0, y) }
 * @param {!Blockly.Block} block Block to generate the code from.
 * @return {array} Completed code with order of operation.
 */
Blockly.Arduino['Calibrate'] = function(block) {
    Blockly.Arduino.addInclude('linefollow', '#include <Robot.h>');
    var code ='while(!robot.buttonPressed());\n'+
    'robot.beep(440,100);\n'+
    'delay(500);\n'+
    'robot.motor[LEFT].setVoltage(2);\n'+
    'robot.motor[RIGHT].setVoltage(-2);\n'+
    'robot.line.calibrate(5000);\n'+
    'robot.motor[LEFT].setVoltage(0);\n'+
    'robot.motor[RIGHT].setVoltage(0);\n'+
    'while(!robot.buttonPressed());\n'+
    'robot.beep(440,100);\n'+
    'delay(1000);'
    return code;
};
/** 
Blockly.Arduino['LinePosition'] = function(block) {

    var code = 'robot.line.getPosition()'
    return code;
};

Blockly.Arduino['Right'] = function(block) {
    var code = '< 0';
    return code;
};**/





