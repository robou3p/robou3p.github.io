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

goog.provide('Blockly.Blocks.robot');

goog.require('Blockly.Blocks');
goog.require('Blockly.Types');


/** Common HSV hue for all blocks in this category. */
Blockly.Blocks.robot.HUE = 360;


Blockly.Blocks['ButtonPressed'] = {
  init: function() {
    this.setColour(Blockly.Blocks.robot.HUE);
    this.appendDummyInput()
    .appendField(Blockly.Msg.BUTTON_PRESS);
    this.setInputsInline(true);
    this.setOutput(true);
    this.setTooltip(Blockly.Msg.BUTTON_PRESSED);
  }
};

Blockly.Blocks['Notes'] = {
  init: function() {
    this.setColour(Blockly.Blocks.robot.HUE);
    this.appendDummyInput()
    .appendField(Blockly.Msg.BEEPING);
    this.appendDummyInput()
    .appendField(Blockly.Msg.NOTE);   
    this.jsonInit({
      "message0": "%1",
      "args0": [
        {
          "type": "field_dropdown",
          "name": "BOOL",
          "options": [
            ["C", "Ccase"],
            ["D", "Dcase"],
            ["E", "Ecase"],
            ["F", "Fcase"],
            ["G", "Gcase"],
            ["A", "Acase"],
            ["C1", "C1case"]
          ]
        }
      ],
    });
    this.appendDummyInput()
    .appendField(Blockly.Msg.DURATION);
    this.appendValueInput('asn')
    .setCheck(Blockly.Types.NUMBER.checkList)
    this.appendDummyInput()
    .appendField("s");   
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setTooltip(Blockly.Msg.NOTES);
  }
};

Blockly.Blocks['Beep'] = {
  init: function() {
      this.setColour(Blockly.Blocks.robot.HUE);
      this.appendDummyInput()
      .appendField(Blockly.Msg.BEEPING);
      this.appendDummyInput()
         .appendField(Blockly.Msg.FREQUENCYY);
      this.appendValueInput('numf')
         .setCheck(Blockly.Types.NUMBER.checkList)
      this.appendDummyInput()
         .appendField("Hz");   
      this.appendDummyInput()
        .appendField(Blockly.Msg.DURATION);
      this.appendValueInput('numt')
        .setCheck(Blockly.Types.NUMBER.checkList)
      this.appendDummyInput()
        .appendField("s");   
      this.setInputsInline(true);
      this.setPreviousStatement(true, null);
      this.setNextStatement(true, null);
      this.setTooltip(Blockly.Msg.BEEP);
  }
};

Blockly.Blocks['Go_forward'] = {
  init: function() {
    this.setColour(Blockly.Blocks.robot.HUE);
    this.appendDummyInput()
    .appendField(Blockly.Msg.GO_FORWARD_FOR);
    this.appendValueInput('dist')
      .setCheck(Blockly.Types.NUMBER.checkList);
    this.appendDummyInput()
      .appendField("cm");    
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setTooltip(Blockly.Msg.GO_FORWARD);
}
};

Blockly.Blocks['Turn'] = {
  init: function() {
    this.setColour(Blockly.Blocks.robot.HUE);
    this.appendDummyInput()
    .appendField(Blockly.Msg.TURN_FOR);
    this.appendValueInput('an')
      .setCheck(Blockly.Types.NUMBER.checkList);
    this.appendDummyInput()
      .appendField("°");    
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setTooltip(Blockly.Msg.TURN);
}
};


/**Blockly.Blocks['Drive'] = {
  init: function() {
      this.setColour(Blockly.Blocks.robot.HUE);
      this.setTooltip('');
      this.setHelpUrl('');
      this.setInputsInline(false);
      this.appendDummyInput()
         .appendField("Hitrost:");
      this.appendValueInput('ROBOT_SPEED')
         .setCheck(Blockly.Types.NUMBER.checkList)
         // .appendField("ROBOT_SPEED");
      this.appendDummyInput()
        .appendField("Rotacija:");
      this.appendValueInput('ROBOT_ROTATION')
        .setCheck(Blockly.Types.NUMBER.checkList)
        // .apendField("ROBOT_ROTATION");
      this.setInputsInline(true);
      this.setPreviousStatement(true, null);
      this.setNextStatement(true, null);
  }}**/