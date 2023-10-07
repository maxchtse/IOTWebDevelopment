import requests
import json


def get_sensor_data():
    databaseURL = "https://cits5506-default-rtdb.firebaseio.com/"
    endpoint = "/sensorData.json"
    response = requests.get(databaseURL + endpoint)
    data = response.json()

    # Get the last 10 items based on insertion order
    if isinstance(data, dict):
        # Get the last 10 items based on insertion order
        last_10_items = list(data.items())[-10:]

    # return data
    return dict(last_10_items)


def get_test_sensor_data():
    with open("testDataset/test.json", "r") as file:
        data = json.load(file)

    return data
