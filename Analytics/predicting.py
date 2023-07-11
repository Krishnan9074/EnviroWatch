# Importing required libraries
from sklearn.ensemble import RandomForestRegressor
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.metrics import mean_squared_error
import numpy as np
import gradio as gr

# Load your dataset with pandas (replace "your_dataset.csv" with the filename of your real dataset)
df = pd.read_csv("DailyDelhiClimateTrain.csv")

# Preprocess the data
# No need for scaling in Random Forests; but just check the missing values
if df.isnull().sum().sum() > 0:
  df = df.dropna()  # consider appropriate imputing method (mean, mode, median) instead of dropping NaN values.

# Split the data into independent (X) and dependent variables (y)
X = df[['meantemp','humidity']]
y = df[['wind_speed', 'meanpressure']]

# Split the dataset into training and testing datasets
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2)

# Train the model
forest = RandomForestRegressor(n_estimators=100)
forest.fit(X_train, y_train)

# Validate the model
y_pred = forest.predict(X_test)
print('Root Mean Squared Error:', np.sqrt(mean_squared_error(y_test, y_pred)))

# Define the prediction function
def predict():
    # Use your input data
    meantemp = 25  # replace with your actual data
    humidity = 60  # replace with your actual data
    # Reshape input data to match the model input shape
    input_data = np.array([meantemp, humidity]).reshape(1, -1)
    # Predict output
    prediction = forest.predict(input_data)
    # Return the prediction
    return prediction[0][0], prediction[0][1]

# Define the Gradio interface
iface = gr.Interface(
    fn=predict, 
    inputs=[], 
    outputs=["number", "number"],
    interpretation="default"
)

# Launch the interface
iface.launch()
