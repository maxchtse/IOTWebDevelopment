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
    # data_str = get_sensor_data()
    import json
    data_str = ''
# Open and read the JSON file
    with open('/Users/hamzausmani/IOT Project/IOTWebDevelopment/testDataset/test.json', 'r') as file:
        # Load JSON data into a Python dictionary
        data_str = json.load(file)
    
    # Extract the entry with the latest timestamp
    latest_entry = max(
        data_str.values(),
        key=lambda x: datetime.strptime(x["timestamp"], "%Y-%m-%dT%H:%M:%S%z"),
    )
    # Extract TVOC and eCO2 values from the latest entry
    latest_TVOC = latest_entry["TVOC"]
    latest_eCO2 = latest_entry["eCO2"]

    # Prepare data in the required format: [Timestamp, TVOC, eCO2]
    # Convert the timestamp string to a datetime object
    timestamp_dt = datetime.strptime(latest_entry["timestamp"], "%Y-%m-%dT%H:%M:%S%z")

    # Convert the datetime object to a timestamp (in milliseconds)
    timestamp_ms = timestamp_dt.timestamp() * 1000
    print(timestamp_ms)
    data = [timestamp_ms, latest_TVOC, latest_eCO2]

    response = make_response(json.dumps(data))
    response.content_type = "application/json"
    
    return response


if __name__ == "__main__":
    app.run(debug=True)
