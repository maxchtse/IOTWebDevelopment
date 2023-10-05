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

while True:
    # Generate data for a specific time range (2 seconds)
    start_date = datetime.now(timezone.utc)
    end_date = start_date + timedelta(seconds=2)

    data = {}  # Dictionary to store generated data

    while start_date < end_date:
        key = generate_random_key(20)  # Generate a unique key
        timestamp = start_date.strftime('%Y-%m-%dT%H:%M:%S%z')  # Format the timestamp with timezone offset
        dust = random.uniform(0, 1)  # Random dust value between 0 and 1
        TVOC = random.randint(0, 1000)  # Random TVOC value between 0 and 1000
        eCO2 = random.randint(300, 1000)  # Random eCO2 value between 300 and 1000

        data[key] = {
            "timestamp": timestamp,
            "Dust": dust,
            "TVOC": TVOC,
            "eCO2": eCO2
        }

        start_date += timedelta(seconds=1)  # Increment timestamp by 1 second

    # Write the generated data to a JSON file
    with open('generated_data.json', 'w') as json_file:
        json.dump(data, json_file)

    print("Data written to generated_data.json")  # Print a message indicating the data has been saved

    time.sleep(2)  # Wait for 2 seconds before generating the next set of data
