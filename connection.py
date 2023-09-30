import requests
import json


def get_sensor_data():
    databaseURL = "https://cits5506-default-rtdb.firebaseio.com/"
    endpoint = "/sensorData.json"
    response = requests.get(databaseURL + endpoint)
    data = response.json()

    # Save to testDataset/dataFromFirebase.json
    with open("testDataset/dataFromFirebase.json", "w") as file:
        json.dump(data, file)

    return get_test_sensor_data()  # for testing only
    # return data


def get_test_sensor_data():
    with open("testDataset/test.json", "r") as file:
        data = json.load(file)

    return data
