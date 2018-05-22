var Ardublockly = Ardublockly || {};
Ardublockly.LOCALISED_TEXT = {
  translationLanguage: "Slovenian",
  title: "Ardublockly",
  blocks: "Blocks",
  /* Menu */
  open: "Odpri",
  save: "Save",
  deleteAll: "Zbriši vse",
  settings: "Nastavitve",
  documentation: "Dokumentacija",
  reportBug: "Prijavi napako",
  examples: "Primeri",
  /* Settings */
  compilerLocation: "Lokacija kompilerja",
  compilerLocationDefault: "Lokacija kompilerja neznana",
  sketchFolder: "Mapa skice",
  sketchFolderDefault: "Mapa skice neznana",
  arduinoBoard: "Arduino plošča",
  arduinoBoardDefault: "Arduino plošča neznana",
  comPort: "COM vrata",
  comPortDefault: "COM vrata neznana",
  defaultIdeButton: "Privzet IDE tipka",
  defaultIdeButtonDefault: "IDE opcije neznane",
  language: "Slovenščina",
  languageDefault: "Slovenščina",
  sketchName: "Ime skice",
  /* Arduino console output */
  arduinoOpMainTitle: "Arduino IDE izhod",
  arduinoOpWaiting: "Čakam na IDE izhod...",
  arduinoOpUploadedTitle: "Uspešno naložena skica",
  arduinoOpVerifiedTitle: "Uspešno preverjena skica",
  arduinoOpOpenedTitle: "Skica odprta v IDE",
  arduinoOpOpenedBody: "Skica bi morala biti naložena na Arduino IDE.",
  arduinoOpErrorUpVerTitle: "Zgradba ali nalaganje ni bila uspešna",
  arduinoOpErrorSketchTitle: "Skica ni bila najdena",
  arduinoOpErrorFlagTitle: "Neveljaven argument v ukazni vrstica",
  arduinoOpErrorFlagPrefTitle: "Preference passed to 'get-pref' flag does not exist",
  arduinoOpErrorIdeDirTitle: "Arduino IDE ni bil najden",
  arduinoOpErrorIdeDirBody: "The compiler directory has not been set.<br>" +
                            "Please set it in the Settings.",
  arduinoOpErrorIdeOptionTitle: "What should we do with the Sketch?",
  arduinoOpErrorIdeOptionBody: "The launch IDE option has not been set.<br>" +
                               "Please select an IDE option in the Settings.",
  arduinoOpErrorIdePortTitle: "Serial Port unavailable",
  arduinoOpErrorIdePortBody: "The Serial Port is not accessible.<br>" +
                             "Please check if the Arduino is correctly connected to the PC and select the Serial Port in the Settings.",
  arduinoOpErrorIdeBoardTitle: "Unknown Arduino Board",
  arduinoOpErrorIdeBoardBody: "The Arduino Board has not been set.<br>" +
                              "Please select the appropriate Arduino Board from the settings.",
  /* Modals */
  noServerTitle: "Ardublockly app not running",
  noServerTitleBody: "<p>For all the Ardublockly features to be enabled, the Ardublockly desktop application must be running locally on your computer.</p>" +
                     "<p>If you are using an online version you will not be able to configure the settings nor load the blocks code into an Arduino.</p>" +
                     "<p>Installation instruction can be found in the <a href=\"https://github.com/carlosperate/ardublockly\">Ardublockly repository</a>.</p>" +
                     "<p>If you have Ardublockly already installed, make sure the application is running correctly.</p>",
  noServerNoLangBody: "If the Ardublockly application is not running the language cannot be fully changed.",
  addBlocksTitle: "Additional Blocks",
  /* Alerts */
  loadNewBlocksTitle: "Naložim nove bloke?",
  loadNewBlocksBody: "Loading a new XML file will replace the current blocks from the workspace.<br>" +
                     "Are you sure you want to proceed?",
  discardBlocksTitle: "Zbrišem bloke?",
  discardBlocksBody: "There are %1 blocks on the workspace.<br>" +
                     "Ali ste prepričani da jih želite zbrisati?",
  invalidXmlTitle: "Invalid XML",
  invalidXmlBody: "The XML file was not successfully parsed into blocks. Please review the XML code and try again.",
  /* Tooltips */
  uploadingSketch: "Nalaganje skice na Arduino...",
  uploadSketch: "Naloži skico na Arduino",
  verifyingSketch: "Preverjanje skice...",
  verifySketch: "Preveri skico",
  openingSketch: "Odpiranje skice v Arduino IDE...",
  openSketch: "Odpri skico v IDE",
  notImplemented: "Function not yet implemented",
  /* Prompts */
  ok: "OK",
  okay: "Okay",
  cancel: "Prekliči",
  return: "Vrni",
  /* Cards */
  arduinoSourceCode: "Arduino Source Code",
  blocksXml: "Blocks XML",
  /* Toolbox Categories*/
  catLogic: "Logika",
  catLoops: "Zanke",
  catMath: "Matematika",
  catText: "Besedilo",
  catVariables: "Spremenljivke",
  catFunctions: "Funkcije",
  catInputOutput: "Vhod/Izhod",
  catTime: "Čas",
  catAudio: "Audio",
  catMotors: "Motorji",
  catComms: "Komunikacija",
  catRobot: "Robot",
  catLineFollow: "Sledilec",
};
