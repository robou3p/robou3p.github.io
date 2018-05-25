/**
 * @license Licensed under the Apache License, Version 2.0 (the "License"):
 *          http://www.apache.org/licenses/LICENSE-2.0
 */

/**
 * @fileoverview Block for the Arduino map functionality.
 *     The Arduino built in functions syntax can be found at:
 *     http://arduino.cc/en/Reference/HomePage
 *
 * TODO: This block can be improved to set the new range properly.
 */
'use strict';

goog.provide('Blockly.Blocks.linefollow');

goog.require('Blockly.Blocks');
goog.require('Blockly.Types');


/** Common HSV hue for all blocks in this category. */
Blockly.Blocks.linefollow.HUE = 230;

Blockly.Blocks['Calibrate'] = {
  init: function() {
      this.setColour(230);
      this.setInputsInline(false);
      this.appendDummyInput()
      .appendField(Blockly.Msg.CALIB);
      this.setInputsInline(true);
      this.setPreviousStatement(true, null);
      this.setNextStatement(false, null);
      this.setTooltip(Blockly.Msg.CALIBRATE);
  }
};
/**Blockly.Blocks['LinePosition'] = {
  init: function() {
    this.setColour(Blockly.Blocks.robot.HUE);
    this.appendDummyInput()
    .appendField("Line:");
    this.setInputsInline(true);
    this.setOutput(true);
    this.setTooltip(Blockly.Msg.ARD_MAP_TIP);
  },
  getBlockType: function() {
    return Blockly.Types.NUMBER;
  }
};
Blockly.Blocks['Right'] = {
  init: function() {
    this.setColour(Blockly.Blocks.robot.HUE);
    this.appendDummyInput()
    .appendField("right");
    this.setInputsInline(true);
    this.setOutput(true);
    this.setTooltip(Blockly.Msg.ARD_MAP_TIP);
  },
  getBlockType: function() {
    return Blockly.Types.NUMBER;
  }
};**/
