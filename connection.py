import firebase_admin
from firebase_admin import credentials
from firebase_admin import db

# Initialize Firebase Admin SDK
def get_sensor_data():
    cred = credentials.Certificate("C:\\Users\\tsech\\OneDrive\\Document\\GitHub\\IOTWebDevelopment\\aqms_\\credentials.json")  # Use double backslashes or raw string
    firebase_admin.initialize_app(cred, {
        'databaseURL': 'https://aqms-8227c-default-rtdb.asia-southeast1.firebasedatabase.app/'  # Replace with your Firebase Realtime Database URL
    })

    # Reference to your database root
    ref = db.reference('/sensorData')

    # Retrieve data from the database
    data = ref.get()

    return data
