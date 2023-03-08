import psycopg2
from datetime import datetime

conn = psycopg2.connect(database = "parameters", user = "postgres", password = "postgresql", host = "127.0.0.1", port = "5432")
print ("Opened database successfully")

# Creating cur object to connect API
cur = conn.cursor()
# datetime object containing current date and time
now = datetime.now()

#Microcontroller parameters sensor table creation
cur.execute('''CREATE TABLE Mcu_parameters
      (CLOCKFREQUENCY_Mhz      INT,
      MODE                      BOOLEAN,
      SPI1                      BOOLEAN,
      I2C0                      BOOLEAN,
      ADC                       BOOLEAN,
      ETHERNET                  BOOLEAN,
      VOLTAGE_V                 FLOAT,
      POWER_mW                  FLOAT,
      CURRENT_mA                FLOAT,
      TEMPERATURE_°C            FLOAT,
      EXACT_TIME            TIMESTAMP);''')

#LC709203F sensor table creation
cur.execute('''CREATE TABLE Battery_Information
      (BATTERY_VOLTAGE_V           FLOAT,
      BATTERY_PERCENTAGE         INT,
      BATTERY_TEMPERATURE_°C       FLOAT);''')

#MPU6050 sensor table creation
cur.execute('''CREATE TABLE MPU6050_Information
      (Acceleration_X               FLOAT,
      Acceleration_Y                FLOAT,
      Acceleration_Z                FLOAT,
      Rotation_X                    FLOAT,
      Rotation_Y                    FLOAT,
      Rotation_Z                    FLOAT,
      TEMPERATURE_°C              FLOAT);''')

#Microphone sensor table creation
cur.execute('''CREATE TABLE LM393_Information
            (SOUND             FLOAT); ''')
print ("Table created successfully")
# dd/mm/YY H:M:S
dt_string = now.strftime(" Date:%d/%m/%Y,Time:%H:%M:%S")
print(dt_string)

conn.commit()
conn.close()
