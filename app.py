from flask import Flask, render_template
from connection import get_sensor_data  # Import the function from Demo.py

app = Flask(__name__)

@app.route('/')
def index():
    # Use the get_sensor_data() function to retrieve data
    analysis_results = get_sensor_data()
    return render_template('index.html', analysis_results=analysis_results)
    



if __name__ == '__main__':
    app.run(debug=True)
