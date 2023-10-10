var chartTVOC;
var chartECO2;
var chartTemperature;
var chartHumidity;

var lastTimestamp = 0; // To keep track of the last fetched data point
/*
          function requestData() {
            // Ajax call to get the Data from Flask
            $.get("/data", function (result) {
              result.forEach((dataPoint) => {
                var timestamp = result[0];
                // Only add data points that are newer than the last timestamp
                if (timestamp > lastTimestamp) {
                  chartTVOC.series[0].addPoint(
                    [timestamp, result[1]],
                    true,
                    false,
                  );
                  chartECO2.series[0].addPoint(
                    [timestamp, result[2]],
                    true,
                    false,
                  );
    
                  // Update the last timestamp
                  lastTimestamp = timestamp;
                }
              });
    
              // Update the displayed sensor values
              $(".point1").text(
                "TVOC : " + Math.round(result[result.length - 1][1]),
              );
              $(".point2").text(
                "eCO2 : " + Math.round(result[result.length - 1][2]),
              );
    
              // call requestData again after two seconds
              setTimeout(requestData, 5000);
            });
          }
    */

// Sample data for gas concentrations and severity levels

//gaugeData method is only for gauge chart currently CH4

function gaugeData() {
  fetch("/data")
    .then((response) => response.json())
    .then((result) => {
      var CH4 = result[5];
      var NO2 = result[6];
      var NO = result[7];
      var CO = result[8];

      // CH4
      Highcharts.chart("ch4-gauge-container", {
        chart: {
          type: "solidgauge",
        },
        title: {
          text: "CH4 (Methane)",
        },
        pane: {
          center: ["50%", "85%"],
          size: "140%",
          startAngle: -90,
          endAngle: 90,
          background: {
            backgroundColor:
              Highcharts.defaultOptions.legend.backgroundColor || "#EEE",
            innerRadius: "60%",
            outerRadius: "100%",
            shape: "arc",
          },
        },
        tooltip: {
          enabled: false,
        },
        yAxis: {
          stops: [
            [0.1, "#55BF3B"], // Green (Low)
            [0.5, "#DDDF0D"], // Yellow (Moderate)
            [0.9, "#DF5353"], // Red (High)
          ],
          lineWidth: 0,
          tickWidth: 0,
          minorTickInterval: null,
          tickAmount: 2,
          title: {
            y: -70,
          },
          labels: {
            y: 16,
          },
          min: 0,
          max: 500,
        },
        plotOptions: {
          solidgauge: {
            dataLabels: {
              y: 5,
              borderWidth: 0,
              useHTML: true,
            },
          },
        },
        series: [
          {
            name: "Gas Concentration",
            data: [CH4.value],
            dataLabels: {
              format:
                '<div style="text-align:center">' +
                '<span style="font-size:20px">{y}</span><br/>' +
                `<span style="font-size:12px;opacity:0.4">${CH4.severity}</span>` +
                "</div>",
            },
            tooltip: {
              valueSuffix: " ppm",
            },
          },
        ],
      });
      // CH4

      // NO2
      Highcharts.chart("no2-gauge-container", {
        chart: {
          type: "solidgauge",
        },
        title: {
          text: "NO2 (Nitrous Oxide)",
        },
        pane: {
          center: ["50%", "85%"],
          size: "140%",
          startAngle: -90,
          endAngle: 90,
          background: {
            backgroundColor:
              Highcharts.defaultOptions.legend.backgroundColor || "#EEE",
            innerRadius: "60%",
            outerRadius: "100%",
            shape: "arc",
          },
        },
        tooltip: {
          enabled: false,
        },
        yAxis: {
          stops: [
            [0.1, "#55BF3B"], // Green (Low)
            [0.5, "#DDDF0D"], // Yellow (Moderate)
            [0.9, "#DF5353"], // Red (High)
          ],
          lineWidth: 0,
          tickWidth: 0,
          minorTickInterval: null,
          tickAmount: 2,
          title: {
            y: -70,
          },
          labels: {
            y: 16,
          },
          min: 0,
          max: 500,
        },
        plotOptions: {
          solidgauge: {
            dataLabels: {
              y: 5,
              borderWidth: 0,
              useHTML: true,
            },
          },
        },
        series: [
          {
            name: "Gas Concentration",
            data: [NO2.value],
            dataLabels: {
              format:
                '<div style="text-align:center">' +
                '<span style="font-size:20px">{y}</span><br/>' +
                `<span style="font-size:12px;opacity:0.4">${NO2.severity}</span>` +
                "</div>",
            },
            tooltip: {
              valueSuffix: " ppm",
            },
          },
        ],
      });
      // NO2

      // NO
      Highcharts.chart("no-gauge-container", {
        chart: {
          type: "solidgauge",
        },
        title: {
          text: "NO (Nitrogen Monoxide)",
        },
        pane: {
          center: ["50%", "85%"],
          size: "140%",
          startAngle: -90,
          endAngle: 90,
          background: {
            backgroundColor:
              Highcharts.defaultOptions.legend.backgroundColor || "#EEE",
            innerRadius: "60%",
            outerRadius: "100%",
            shape: "arc",
          },
        },
        tooltip: {
          enabled: false,
        },
        yAxis: {
          stops: [
            [0.1, "#55BF3B"], // Green (Low)
            [0.5, "#DDDF0D"], // Yellow (Moderate)
            [0.9, "#DF5353"], // Red (High)
          ],
          lineWidth: 0,
          tickWidth: 0,
          minorTickInterval: null,
          tickAmount: 2,
          title: {
            y: -70,
          },
          labels: {
            y: 16,
          },
          min: 0,
          max: 500,
        },
        plotOptions: {
          solidgauge: {
            dataLabels: {
              y: 5,
              borderWidth: 0,
              useHTML: true,
            },
          },
        },
        series: [
          {
            name: "Gas Concentration",
            data: [NO.value],
            dataLabels: {
              format:
                '<div style="text-align:center">' +
                '<span style="font-size:20px">{y}</span><br/>' +
                `<span style="font-size:12px;opacity:0.4">${NO.severity}</span>` +
                "</div>",
            },
            tooltip: {
              valueSuffix: " ppm",
            },
          },
        ],
      });
      // NO

      //CO
      Highcharts.chart("co-gauge-container", {
        chart: {
          type: "solidgauge",
        },
        title: {
          text: "CO (Carbon Monoxide)",
        },
        pane: {
          center: ["50%", "85%"],
          size: "140%",
          startAngle: -90,
          endAngle: 90,
          background: {
            backgroundColor:
              Highcharts.defaultOptions.legend.backgroundColor || "#EEE",
            innerRadius: "60%",
            outerRadius: "100%",
            shape: "arc",
          },
        },
        tooltip: {
          enabled: false,
        },
        yAxis: {
          stops: [
            [0.1, "#55BF3B"], // Green (Low)
            [0.5, "#DDDF0D"], // Yellow (Moderate)
            [0.9, "#DF5353"], // Red (High)
          ],
          lineWidth: 0,
          tickWidth: 0,
          minorTickInterval: null,
          tickAmount: 2,
          title: {
            y: -70,
          },
          labels: {
            y: 16,
          },
          min: 0,
          max: 500,
        },
        plotOptions: {
          solidgauge: {
            dataLabels: {
              y: 5,
              borderWidth: 0,
              useHTML: true,
            },
          },
        },
        series: [
          {
            name: "Gas Concentration",
            data: [CO.value],
            dataLabels: {
              format:
                '<div style="text-align:center">' +
                '<span style="font-size:20px">{y}</span><br/>' +
                `<span style="font-size:12px;opacity:0.4">${CO.severity}</span>` +
                "</div>",
            },
            tooltip: {
              valueSuffix: " ppm",
            },
          },
        ],
      });
      //CO
      // Get the gas level values (replace these with actual values)

      // Define severity levels and corresponding actions

      // Determine severity levels for each gas
      // Determine severity levels for each gas
      // Determine severity levels for each gas
const ch4Severity = CH4.severity;
const no2Severity = NO2.severity;
const noSeverity = NO.severity;
const coSeverity = CO.severity;

// Check the severities and generate the text
let action = "";

if (["High"].some(severity => [ch4Severity, no2Severity, noSeverity, coSeverity].includes(severity))) {
    action = "The gas intensity has reached an extremely high level. Evacuation is strongly advised for immediate safety";
} else if (["Moderate"].some(severity => [ch4Severity, no2Severity, noSeverity, coSeverity].includes(severity))) {
    action = "The gas levels are currently moderate. It's advisable to locate the nearest ventilation point to ensure a safer environment";
} else if (["Low"].some(severity => [ch4Severity, no2Severity, noSeverity, coSeverity].includes(severity))) {
    action = "The gas intensity is low, but please remain cautious for your safety. Ensure to wear appropriate safety gear, including masks, to protect yourself from potential exposure.";
}

// Display the generative text in the <div> element
const generativeTextElement = document.getElementById("generative-text");
generativeTextElement.textContent = `${action}`;


      console.log(action);
    });
}
var TVOCDataPoints = [];
var ECO2DataPoints = [];
var TemperaturePoints = [];
var HumidityPoints = [];
//lineChart method is only for Line chart currently TVOC, ECO2, temperature & humidity
function lineChart() {
  fetch("/data")
    .then((response) => response.json())
    .then((result) => {
      var timestamp = result[0];
      var humidity = result[1];
      var temperature = result[2];
      var TVOC = result[3];
      var ECO2 = result[4];

      console.log(TVOC);
      console.log(ECO2);
      console.log(temperature);
      console.log(humidity);
      // Only add data points that are newer than the last timestamp
      if (timestamp > lastTimestamp) {
        TVOCDataPoints.push({ x: timestamp, y: TVOC });
        ECO2DataPoints.push({ x: timestamp, y: ECO2 });
        TemperaturePoints.push({ x: timestamp, y: temperature });
        HumidityPoints.push({ x: timestamp, y: humidity });

        if (TVOCDataPoints.length > 20) {
          TVOCDataPoints.shift(); // Remove the first element (oldest data point)
        }

        if (ECO2DataPoints.length > 20) {
          ECO2DataPoints.shift(); // Remove the first element (oldest data point)
        }

        if (TemperaturePoints.length > 20) {
          TemperaturePoints.shift(); // Remove the first element (oldest data point)
        }

        if (HumidityPoints.length > 20) {
          HumidityPoints.shift(); // Remove the first element (oldest data point)
        }

        // Update the chart with the last 20 data points
        chartTVOC.xAxis[0].setExtremes(timestamp - 20000, timestamp);
        chartECO2.xAxis[0].setExtremes(timestamp - 20000, timestamp);
        chartTemperature.xAxis[0].setExtremes(timestamp - 20000, timestamp);
        chartHumidity.xAxis[0].setExtremes(timestamp - 20000, timestamp);

        // Update series data
        chartTVOC.series[0].setData(TVOCDataPoints);
        chartECO2.series[0].setData(ECO2DataPoints);
        chartTemperature.series[0].setData(TemperaturePoints);
        chartHumidity.series[0].setData(HumidityPoints);

        // Update the last timestamp
        lastTimestamp = timestamp;
      }
      // Update the displayed sensor values
      $(".point1").text("TVOC : " + Math.round(TVOC));
      $(".point2").text("eCO2 : " + Math.round(ECO2));
      $(".point3").text("Temperature : " + Math.round(temperature));
      $(".point4").text("Humidity : " + Math.round(humidity));
    })
    .catch((error) => {
      console.error("Error fetching data:", error);
    });
}

// Poll every second
setInterval(lineChart, 1000);
setInterval(gaugeData, 3000);

$(document).ready(function () {
  // --------------Chart TVOC ----------------------------
  chartTVOC = new Highcharts.Chart({
    chart: {
      renderTo: "data-TVOC",
      defaultSeriesType: "area",
      events: {
        load: lineChart,
      },
    },
    title: {
      text: "TVOC",
    },
    xAxis: {
      type: "datetime",
      tickPixelInterval: 150,
      maxZoom: 20 * 1000,
    },
    yAxis: {
      minPadding: 0.2,
      maxPadding: 0.2,
      title: {
        text: "Value",
        margin: 80,
      },
    },
    series: [
      {
        color: "#c23d23",
        lineColor: "#303030",
        name: "TVOC",
        data: [],
      },
    ],
  });
  // --------------Chart TVOC Ends - -----------------

  // --------------Chart ECO2 Start ------------------
  chartECO2 = new Highcharts.Chart({
    chart: {
      renderTo: "data-ECO2",
      defaultSeriesType: "area",
      events: {
        load: lineChart,
      },
    },
    title: {
      text: "eCO2",
    },
    xAxis: {
      type: "datetime",
      tickPixelInterval: 150,
      maxZoom: 20 * 1000,
    },
    yAxis: {
      minPadding: 0.2,
      maxPadding: 0.2,
      title: {
        text: "Value",
        margin: 80,
      },
    },
    series: [
      {
        lineColor: "#1d82b8",
        name: "eCO2",
        data: [],
      },
    ],
  });
  // ---------------Chart ECO2 ENDs

  // ---------------Chart Temperature Starts
  chartTemperature = new Highcharts.Chart({
    chart: {
      renderTo: "data-Temperature",
      defaultSeriesType: "area",
      events: {
        load: lineChart,
      },
    },
    title: {
      text: "Temperature",
    },
    xAxis: {
      type: "datetime",
      tickPixelInterval: 150,
      maxZoom: 20 * 1000,
    },
    yAxis: {
      minPadding: 0.2,
      maxPadding: 0.2,
      title: {
        text: "Value",
        margin: 80,
      },
    },
    series: [
      {
        lineColor: "#1d82b8",
        name: "Temperature",
        data: [],
      },
    ],
  });
  // ---------------Chart Temperature Ends

  // ---------------Chart Humidity Starts
  chartHumidity = new Highcharts.Chart({
    chart: {
      renderTo: "data-Humidity",
      defaultSeriesType: "area",
      events: {
        load: lineChart,
      },
    },
    title: {
      text: "Humidity",
    },
    xAxis: {
      type: "datetime",
      tickPixelInterval: 150,
      maxZoom: 20 * 1000,
    },
    yAxis: {
      minPadding: 0.2,
      maxPadding: 0.2,
      title: {
        text: "Value",
        margin: 80,
      },
    },
    series: [
      {
        lineColor: "#1d82b8",
        name: "Humidity",
        data: [],
      },
    ],
  });
  // ---------------Chart Humidity Ends
});
lineChart();

class Image {
  constructor(imgUrl, size) {
    this.imgUrl = imgUrl;
    this.size = size;
  }

  backgroundImage() {
    console.log("inside function ");
    // Select the Image
    var img = document.querySelector(".jumbotron");

    // create Css Text
    var text =
      "margin:auto;" +
      "background-image: url(" +
      this.imgUrl +
      ");" +
      "background-size:cover;" +
      "opacity:1;" +
      "background-blend-mode: darken;" +
      "height: " +
      16 +
      "vh;";

    img.style.cssText = text;
  }

  centerTitle() {
    /*
                Center the Title
             */
    var t1 = document.querySelector("#title");
    t1.classList.add("text-white");
    t1.classList.add("text-center");
    t1.classList.add("display-3");
  }
}
const imgUrl = "https://media3.giphy.com/media/26tn33aiTi1jkl6H6/giphy.gif";
const size = "50";
var obj = new Image(imgUrl, size);
obj.backgroundImage();
obj.centerTitle();
