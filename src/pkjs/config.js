module.exports = [
  {
    "type": "heading",
    "defaultValue": "Status"
  },
  {
    "type": "text",
    "defaultValue": "Configuration"
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "OpenWeatherMap"
      },
      {
        "type": "input",
        "messageKey": "OWM_API",
        "defaultValue": "",
        "label": "API key"
      },
      {
        "type": "input",
        "messageKey": "OWM_CITY",
        "defaultValue": "",
        "label": "City code"
      }
    ]
  },
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];