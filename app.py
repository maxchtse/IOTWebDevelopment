from flask import Flask, render_template, make_response, json
from connection import get_sensor_data
from datetime import datetime


app = Flask(__name__)

@app.route("/")
def index():
    analysis_results = get_sensor_data()
    print(analysis_results)
    return render_template("index.html", analysis_results=analysis_results)


@app.route("/view_analysis", methods=["GET", "POST"])
def view_analysis():
    return render_template("view_analysis.html")


@app.route("/data", methods=["GET", "POST"])
def data():
    # Fetch the sensor data
    data_str = get_sensor_data()  ##IF YOU WANT TO GET THE DATA FROM FIREBASE
    import json
    data_str = ''
# Open and read the JSON file

    #the code below is for testing
    # with open('/Users/hamzausmani/IOT Project/IOTWebDevelopment/generated_testdata/generated_data.json', 'r') as file:
    #     # Load JSON data into a Python dictionary
    #     data_str = json.load(file)
    
    # Extract the entry with the latest timestamp
    latest_entry = max(
        data_str.values(),
        key=lambda x: datetime.strptime(x["timestamp"], "%Y-%m-%dT%H:%M:%S%z"),
    )
    # Extract TVOC, eCO2, humidity and temperature for Line Chart
    latest_TVOC = latest_entry["TVOC"]
    latest_eCO2 = latest_entry["eCO2"]
    latest_humidity = latest_entry["Humidity"]
    latest_temperature = latest_entry["Temperature"]
    #
    # Extract CH4, NO2, NO, CO
    latest_CH4 = latest_entry["CH4"]
    latest_NO2 = latest_entry["NO2"]
    latest_NO = latest_entry["NO"]
    latest_CO = latest_entry["CO"]
    #
    
    #Ranking Severity Levels
    latest_CH4_data = {}
    latest_NO2_data = {}
    latest_NO_data = {}
    latest_CO_data = {}

    #IF YOU WANT THE EXTENT OF THE SEVERITY then change the values over here
    if latest_CH4 <= 100:
        latest_CH4_data["value"] = latest_CH4
        latest_CH4_data["severity"] = "Low"
    elif latest_CH4 <= 1000:
        latest_CH4_data["value"] = latest_CH4
        latest_CH4_data["severity"] = "Moderate"
    else:
        latest_CH4_data["value"] = latest_CH4
        latest_CH4_data["severity"] = "High"
        
    
    

    if latest_NO2 <= 100:
        latest_NO2_data["value"] = latest_NO2
        latest_NO2_data["severity"] = "Low"
    elif latest_NO2 <= 1000:
        latest_NO2_data["value"] = latest_NO2
        latest_NO2_data["severity"] = "Moderate"
    else:
        latest_NO2_data["value"] = latest_NO2
        latest_NO2_data["severity"] = "High"

    

    

    if latest_NO <= 100:
        latest_NO_data["value"] = latest_NO
        latest_NO_data["severity"] = "Low"
    elif latest_NO <= 1000:
        latest_NO_data["value"] = latest_NO
        latest_NO_data["severity"] = "Moderate"
    else:
        latest_NO_data["value"] = latest_NO
        latest_NO_data["severity"] = "High"

    

    if latest_CO <= 100:
        latest_CO_data["value"] = latest_CO
        latest_CO_data["severity"] = "Low"
    elif latest_CO <= 1000:
        latest_CO_data["value"] = latest_CO
        latest_CO_data["severity"] = "Moderate"
    else:
        latest_CO_data["value"] = latest_CO
        latest_CO_data["severity"] = "High"

    #Ranking Severity Levels    


    # Prepare data in the required format: [Timestamp, TVOC, eCO2]
    # Convert the timestamp string to a datetime object
    timestamp_dt = datetime.strptime(latest_entry["timestamp"], "%Y-%m-%dT%H:%M:%S%z")
    
    # Convert the datetime object to a timestamp (in milliseconds)
    timestamp_ms = timestamp_dt.timestamp() * 1000
    print(timestamp_ms)
    data = [timestamp_ms, latest_humidity, latest_temperature, latest_TVOC, latest_eCO2, latest_CH4_data, latest_NO2_data, latest_NO_data, latest_CO_data]
    print(data)
    

    response = make_response(json.dumps(data))
    response.content_type = "application/json"
    
    return response


if __name__ == "__main__":
    app.run(port=8000,debug=True)
