import os
import random
import string
from datetime import datetime, timedelta, timezone
import time
import json

# Function to generate a random timestamp in the given date range
def random_timestamp(start_date, end_date):
    delta = end_date - start_date
    random_days = random.randint(0, delta.days)
    random_time = timedelta(seconds=random.randint(0, 86400))  # 86400 seconds in a day
    return start_date + timedelta(days=random_days) + random_time

# Function to generate a random string for unique keys
def generate_random_key(length):
    return ''.join(random.choices(string.ascii_uppercase + string.digits, k=length))

# Check if the folder exists, if not, create it
folder_path = 'generated_testdata'
if not os.path.exists(folder_path):
    os.makedirs(folder_path)

# File path for the JSON file inside the folder
file_path = os.path.join(folder_path, 'generated_data.json')

while True:
    # Generate data for a specific time range (2 seconds)
    start_date = datetime.now(timezone.utc)
    end_date = start_date + timedelta(seconds=2)

    data = {}  # Dictionary to store generated data

    while start_date < end_date:
        key = generate_random_key(20)  # Generate a unique key
        timestamp = start_date.strftime('%Y-%m-%dT%H:%M:%S%z')  # Format the timestamp with timezone offset        
        TVOC = random.randint(0, 1000)  # Random TVOC value between 0 and 1000
        eCO2 = random.randint(300, 1000)  # Random eCO2 value between 300 and 1000
        Temperature = random.randint(0, 100)  # Random temperature between 0 and 100
        Humidity = random.randint(0, 100)  # Random humidity between 0 and 100
        CH4 = random.randint(0, 2000)  # Random CH4 value between 300 and 1000
        NO2 = random.randint(0, 2000)  # Random NO2 value between 300 and 1000
        NO = random.randint(0, 2000)  # Random NO value between 300 and 1000
        CO = random.randint(0, 2000)  # Random CO value between 300 and 1000

        data[key] = {
            "timestamp": timestamp,            
            "TVOC": TVOC,
            "eCO2": eCO2,
            "Temperature": Temperature,
            "Humidity": Humidity,
            "CH4": CH4,
            "NO2": NO2,
            "NO": NO,
            "CO": CO
        }

        start_date += timedelta(seconds=1)  # Increment timestamp by 1 second

        # Check if the file already exists
        if os.path.exists(file_path):
            # Load the existing data from the file
            with open(file_path, 'r') as json_file:
                existing_data = json.load(json_file)
            # Update the existing data with the new data
            existing_data.update(data)
            data = existing_data

        # Write the updated data to the JSON file inside the folder
        with open(file_path, 'w') as json_file:
            json.dump(data, json_file)

        print("Data written to", file_path)  # Print a message indicating the data has been saved

        time.sleep(2)  # Wait for 2 seconds before generating the next set of data
