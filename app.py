from flask import Flask, render_template
from connection import get_sensor_data  # Import the function from Demo.py
from firebase_admin import initialize_app

# Initialize Firebase app (only once when the application starts)
from flask import Flask,render_template,url_for,request,redirect, make_response
import random
import json
from time import time
from random import random
from flask import Flask, render_template, make_response
from datetime import datetime

app = Flask(__name__)

@app.route('/')
def index():
    # Use the get_sensor_data() function to retrieve data
    analysis_results = get_sensor_data()  #We imported data 
    print(analysis_results)
    return render_template('index.html', analysis_results=analysis_results)

@app.route('/view_data')    
def view_data():
    # A page showing anlysis of the data
    return render_template('view_data.html')

######################################################################################
@app.route('/view_analysis', methods=["GET", "POST"])
def main():
    return render_template('view_analysis.html')


@app.route('/data', methods=["GET", "POST"])
def data():
    # Data Format
    # [TIME, Temperature, Humidity]
    data_str = get_sensor_data()

    # Find the entry with the latest timestamp
    latest_entry = max(data_str.values(), key=lambda x: datetime.strptime(x['timestamp'], '%Y-%m-%d %H:%M:%S'))

    # Find the entry with the latest timestamp

    # Get TVOC and eCO2 values of the latest entry
    latest_TVOC = latest_entry['TVOC']
    latest_eCO2 = latest_entry['eCO2']

    Temperature = random() * 100
    Humidity = random() * 55

    data = [time() * 1000, latest_TVOC, latest_eCO2]
    





    response = make_response(json.dumps(data))

    response.content_type = 'application/json'

    return response

if __name__ == '__main__':
    app.run(debug=True)
