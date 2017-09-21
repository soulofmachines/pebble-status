// Import the Clay package
var Clay = require('pebble-clay');
// Load our Clay configuration file
var clayConfig = require('./config');
// Initialize Clay
var clay = new Clay(clayConfig);
// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    console.log('PebbleKit JS ready!');
       getWeather();
      //console.log(Clay.getSettings());
      console.log(localStorage.getItem("clay-settings"));
  }
);


    var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function getWeather() {
      var now = Math.floor(Date.now() / 1000);
    var store = localStorage.getItem("ts");
    if(Math.abs(now - store) > 900) {
      console.log(now);
    var url = "http://api.openweathermap.org/data/2.5/forecast?id=519690&APPID=ec55b9901c4b0cdc16f723cb72bce0b5";
     xhrRequest(url, 'GET', 
    function(responseText) {
      // responseText contains a JSON object with weather info
      console.log(responseText);
      var json = JSON.parse(responseText);

      // Temperature in Kelvin requires adjustment
      var temperature = Math.round(json.list[0].main.temp - 273.15);
      console.log('Temperature is ' + temperature);

      // Conditions
      var conditions = json.list[0].weather[0].main;      
      console.log('Conditions are ' + conditions);
        
        var weather = temperature + "C, " + conditions;
        localStorage.setItem("ts", now);
        localStorage.setItem("weather", weather);
        
        
        var dictionary = {
            'WEATHER': weather
};

// Send to Pebble
Pebble.sendAppMessage(dictionary,
  function(e) {
    console.log('Weather info sent to Pebble successfully!');
  },
  function(e) {
    console.log('Error sending weather info to Pebble!');
  }
);
        
    }      
  );
    } else {
        var weather = localStorage.getItem("weather");
        var dictionary = {
            'WEATHER': weather
};

// Send to Pebble
Pebble.sendAppMessage(dictionary,
  function(e) {
    console.log('Weather info sent to Pebble successfully!');
  },
  function(e) {
    console.log('Error sending weather info to Pebble!');
  }
);
    }
}

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log('AppMessage received!');
      getWeather();
  }                     
);