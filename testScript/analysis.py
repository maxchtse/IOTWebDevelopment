import pandas as pd
import matplotlib.pyplot as plt
from connection import get_sensor_data

# Assuming 'data' contains your sensor data as shown above
# Convert data to a Pandas DataFrame
df = pd.DataFrame.from_dict(data["sensorData"], orient="index")
df["timestamp"] = pd.to_datetime(df["timestamp"])

# Set the timestamp as the DataFrame index
df.set_index("timestamp", inplace=True)

# Calculate the average Dust levels over time for each sensor
dust_means = df.groupby(df.index.date)["Dust"].mean()

# Create a time series plot
plt.figure(figsize=(10, 6))
plt.plot(dust_means.index, dust_means.values, marker="o", linestyle="-")
plt.xlabel("Date")
plt.ylabel("Average Dust Level")
plt.title("Average Dust Levels Over Time")
plt.grid(True)

# Save the plot as an image or display it
plt.savefig("dust_time_series.png")  # Optionally, save the plot as an image
# plt.show()  # Optionally, display the plot

# You can also return the calculated data to your Flask route handler
dust_time_series_data = dust_means.to_dict()
